/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdint.h>
#include <stdio.h>

#include "ClearKeyDecryptionManager.h"
#include "ClearKeySessionManager.h"
#include "ClearKeyUtils.h"
#include "ClearKeyStorage.h"
#include "ClearKeyPersistence.h"
#include "gmp-task-utils.h"

#include "mozilla/Assertions.h"

using namespace mozilla;
using namespace std;

ClearKeySessionManager::ClearKeySessionManager()
  : mDecryptionManager(ClearKeyDecryptionManager::Get())
{
  CK_LOGD("ClearKeySessionManager ctor");
  AddRef();

  if (GetPlatform()->createthread(&mThread) != GMPNoErr) {
    CK_LOGD("failed to create thread in clearkey cdm");
    mThread = nullptr;
  }
}

ClearKeySessionManager::~ClearKeySessionManager()
{
  CK_LOGD("ClearKeySessionManager dtor");
   MOZ_ASSERT(!mRefCount);
}

void
ClearKeySessionManager::Init(GMPDecryptorCallback* aCallback)
{
  CK_LOGD("ClearKeySessionManager::Init");
  mCallback = aCallback;
  mCallback->SetCapabilities(GMP_EME_CAP_DECRYPT_AUDIO |
                             GMP_EME_CAP_DECRYPT_VIDEO);
  ClearKeyPersistence::EnsureInitialized();
}

void
ClearKeySessionManager::CreateSession(uint32_t aCreateSessionToken,
                                      uint32_t aPromiseId,
                                      const char* aInitDataType,
                                      uint32_t aInitDataTypeSize,
                                      const uint8_t* aInitData,
                                      uint32_t aInitDataSize,
                                      GMPSessionType aSessionType)
{
  CK_LOGD("ClearKeySessionManager::CreateSession type:%s", aInitDataType);

  // initDataType must be "cenc".
  if (strcmp("cenc", aInitDataType)) {
    mCallback->RejectPromise(aPromiseId, kGMPNotSupportedError,
                             nullptr /* message */, 0 /* messageLen */);
    return;
  }

  if (ClearKeyPersistence::DeferCreateSessionIfNotReady(this,
                                                        aCreateSessionToken,
                                                        aPromiseId,
                                                        aInitData,
                                                        aInitDataSize,
                                                        aSessionType)) {
    return;
  }

  string sessionId = ClearKeyPersistence::GetNewSessionId(aSessionType);
  MOZ_ASSERT(mSessions.find(sessionId) == mSessions.end());

  ClearKeySession* session = new ClearKeySession(sessionId, mCallback, aSessionType);
  session->Init(aCreateSessionToken, aPromiseId, aInitData, aInitDataSize);
  mSessions[sessionId] = session;

  const vector<KeyId>& sessionKeys = session->GetKeyIds();
  vector<KeyId> neededKeys;
  for (auto it = sessionKeys.begin(); it != sessionKeys.end(); it++) {
    // Need to request this key ID from the client. We always send a key
    // request, whether or not another session has sent a request with the same
    // key ID. Otherwise a script can end up waiting for another script to
    // respond to the request (which may not necessarily happen).
    neededKeys.push_back(*it);
    mDecryptionManager->ExpectKeyId(*it);
  }

  if (neededKeys.empty()) {
    CK_LOGD("No keys needed from client.");
    return;
  }

  // Send a request for needed key data.
  string request;
  ClearKeyUtils::MakeKeyRequest(neededKeys, request, aSessionType);
  mCallback->SessionMessage(&sessionId[0], sessionId.length(),
                            kGMPLicenseRequest,
                            (uint8_t*)&request[0], request.length());
}

void
ClearKeySessionManager::LoadSession(uint32_t aPromiseId,
                                    const char* aSessionId,
                                    uint32_t aSessionIdLength)
{
  CK_LOGD("ClearKeySessionManager::LoadSession");

  if (!ClearKeyUtils::IsValidSessionId(aSessionId, aSessionIdLength)) {
    mCallback->ResolveLoadSessionPromise(aPromiseId, false);
    return;
  }

  if (ClearKeyPersistence::DeferLoadSessionIfNotReady(this,
                                                      aPromiseId,
                                                      aSessionId,
                                                      aSessionIdLength)) {
    return;
  }

  string sid(aSessionId, aSessionId + aSessionIdLength);
  if (!ClearKeyPersistence::IsPersistentSessionId(sid)) {
    mCallback->ResolveLoadSessionPromise(aPromiseId, false);
    return;
  }

  // Callsback PersistentSessionDataLoaded with results...
  ClearKeyPersistence::LoadSessionData(this, sid, aPromiseId);
}

void
ClearKeySessionManager::PersistentSessionDataLoaded(GMPErr aStatus,
                                                    uint32_t aPromiseId,
                                                    const string& aSessionId,
                                                    const uint8_t* aKeyData,
                                                    uint32_t aKeyDataSize)
{
  CK_LOGD("ClearKeySessionManager::PersistentSessionDataLoaded");
  if (GMP_FAILED(aStatus) ||
      Contains(mSessions, aSessionId) ||
      (aKeyDataSize % (2 * CLEARKEY_KEY_LEN)) != 0) {
    mCallback->ResolveLoadSessionPromise(aPromiseId, false);
    return;
  }

  ClearKeySession* session = new ClearKeySession(aSessionId,
                                                 mCallback,
                                                 kGMPPersistentSession);
  mSessions[aSessionId] = session;

  uint32_t numKeys = aKeyDataSize / (2 * CLEARKEY_KEY_LEN);
  for (uint32_t i = 0; i < numKeys; i ++) {
    const uint8_t* base = aKeyData + 2 * CLEARKEY_KEY_LEN * i;

    KeyId keyId(base, base + CLEARKEY_KEY_LEN);
    MOZ_ASSERT(keyId.size() == CLEARKEY_KEY_LEN);

    Key key(base + CLEARKEY_KEY_LEN, base + 2 * CLEARKEY_KEY_LEN);
    MOZ_ASSERT(key.size() == CLEARKEY_KEY_LEN);

    session->AddKeyId(keyId);

    mDecryptionManager->ExpectKeyId(keyId);
    mDecryptionManager->InitKey(keyId, key);
    mKeyIds.insert(key);
    mCallback->KeyIdUsable(&aSessionId[0], aSessionId.size(),
                           &keyId[0], keyId.size());
  }

  mCallback->ResolveLoadSessionPromise(aPromiseId, true);
}

void
ClearKeySessionManager::UpdateSession(uint32_t aPromiseId,
                                      const char* aSessionId,
                                      uint32_t aSessionIdLength,
                                      const uint8_t* aResponse,
                                      uint32_t aResponseSize)
{
  CK_LOGD("ClearKeySessionManager::UpdateSession");
  string sessionId(aSessionId, aSessionId + aSessionIdLength);

  auto itr = mSessions.find(sessionId);
  if (itr == mSessions.end() || !(itr->second)) {
    CK_LOGW("ClearKey CDM couldn't resolve session ID in UpdateSession.");
    mCallback->RejectPromise(aPromiseId, kGMPNotFoundError, nullptr, 0);
    return;
  }
  ClearKeySession* session = itr->second;

  // Parse the response for any (key ID, key) pairs.
  vector<KeyIdPair> keyPairs;
  if (!ClearKeyUtils::ParseJWK(aResponse, aResponseSize, keyPairs, session->Type())) {
    CK_LOGW("ClearKey CDM failed to parse JSON Web Key.");
    mCallback->RejectPromise(aPromiseId, kGMPInvalidAccessError, nullptr, 0);
    return;
  }

  for (auto it = keyPairs.begin(); it != keyPairs.end(); it++) {
    mDecryptionManager->InitKey(it->mKeyId, it->mKey);
    mKeyIds.insert(it->mKeyId);
    mCallback->KeyIdUsable(aSessionId, aSessionIdLength,
                           &it->mKeyId[0], it->mKeyId.size());
  }

  if (session->Type() != kGMPPersistentSession) {
    mCallback->ResolvePromise(aPromiseId);
    return;
  }

  // Store the keys on disk. We store a record whose name is the sessionId,
  // and simply append each keyId followed by its key.
  vector<uint8_t> keydata;
  Serialize(session, keydata);
  GMPTask* resolve = WrapTask(mCallback, &GMPDecryptorCallback::ResolvePromise, aPromiseId);
  static const char* message = "Couldn't store cenc key init data";
  GMPTask* reject = WrapTask(mCallback,
                             &GMPDecryptorCallback::RejectPromise,
                             aPromiseId,
                             kGMPInvalidStateError,
                             message,
                             strlen(message));
  StoreData(sessionId, keydata, resolve, reject);
}

void
ClearKeySessionManager::Serialize(const ClearKeySession* aSession,
                                  std::vector<uint8_t>& aOutKeyData)
{
  const std::vector<KeyId>& keyIds = aSession->GetKeyIds();
  for (size_t i = 0; i < keyIds.size(); i++) {
    const KeyId& keyId = keyIds[i];
    if (!mDecryptionManager->HasKeyForKeyId(keyId)) {
      continue;
    }
    MOZ_ASSERT(keyId.size() == CLEARKEY_KEY_LEN);
    aOutKeyData.insert(aOutKeyData.end(), keyId.begin(), keyId.end());
    const Key& key = mDecryptionManager->GetDecryptionKey(keyId);
    MOZ_ASSERT(key.size() == CLEARKEY_KEY_LEN);
    aOutKeyData.insert(aOutKeyData.end(), key.begin(), key.end());
  }
}

void
ClearKeySessionManager::CloseSession(uint32_t aPromiseId,
                                     const char* aSessionId,
                                     uint32_t aSessionIdLength)
{
  CK_LOGD("ClearKeySessionManager::CloseSession");

  string sessionId(aSessionId, aSessionId + aSessionIdLength);
  auto itr = mSessions.find(sessionId);
  if (itr == mSessions.end()) {
    CK_LOGW("ClearKey CDM couldn't close non-existent session.");
    mCallback->RejectPromise(aPromiseId, kGMPNotFoundError, nullptr, 0);
    return;
  }

  ClearKeySession* session = itr->second;
  MOZ_ASSERT(session);

  ClearInMemorySessionData(session);
  mCallback->ResolvePromise(aPromiseId);
  mCallback->SessionClosed(aSessionId, aSessionIdLength);
}

void
ClearKeySessionManager::ClearInMemorySessionData(ClearKeySession* aSession)
{
  MOZ_ASSERT(aSession);

  const vector<KeyId>& keyIds = aSession->GetKeyIds();
  for (auto it = keyIds.begin(); it != keyIds.end(); it++) {
    MOZ_ASSERT(mDecryptionManager->HasKeyForKeyId(*it));
    mDecryptionManager->ReleaseKeyId(*it);

    const string& sessionId = aSession->Id();
    mCallback->KeyIdNotUsable(&sessionId[0], sessionId.size(),
                              &(*it)[0], it->size());
  }

  mSessions.erase(aSession->Id());
  delete aSession;
}

void
ClearKeySessionManager::RemoveSession(uint32_t aPromiseId,
                                      const char* aSessionId,
                                      uint32_t aSessionIdLength)
{
  CK_LOGD("ClearKeySessionManager::RemoveSession");
  string sessionId(aSessionId, aSessionId + aSessionIdLength);
  auto itr = mSessions.find(sessionId);
  if (itr == mSessions.end()) {
    CK_LOGW("ClearKey CDM couldn't remove non-existent session.");
    mCallback->RejectPromise(aPromiseId, kGMPNotFoundError, nullptr, 0);
    return;
  }

  ClearKeySession* session = itr->second;
  MOZ_ASSERT(session);
  string sid = session->Id();
  bool isPersistent = session->Type() == kGMPPersistentSession;
  ClearInMemorySessionData(session);

  if (!isPersistent) {
    mCallback->ResolvePromise(aPromiseId);
    return;
  }

  ClearKeyPersistence::PersistentSessionRemoved(sid);

  // Overwrite the record storing the sessionId's key data with a zero
  // length record to delete it.
  vector<uint8_t> emptyKeydata;
  GMPTask* resolve = WrapTask(mCallback, &GMPDecryptorCallback::ResolvePromise, aPromiseId);
  static const char* message = "Could not remove session";
  GMPTask* reject = WrapTask(mCallback,
                             &GMPDecryptorCallback::RejectPromise,
                             aPromiseId,
                             kGMPInvalidAccessError,
                             message,
                             strlen(message));
  StoreData(sessionId, emptyKeydata, resolve, reject);
}

void
ClearKeySessionManager::SetServerCertificate(uint32_t aPromiseId,
                                             const uint8_t* aServerCert,
                                             uint32_t aServerCertSize)
{
  // ClearKey CDM doesn't support this method by spec.
  CK_LOGD("ClearKeySessionManager::SetServerCertificate");
  mCallback->RejectPromise(aPromiseId, kGMPNotSupportedError,
                           nullptr /* message */, 0 /* messageLen */);
}

void
ClearKeySessionManager::Decrypt(GMPBuffer* aBuffer,
                                GMPEncryptedBufferMetadata* aMetadata)
{
  CK_LOGD("ClearKeySessionManager::Decrypt");

  if (!mThread) {
    CK_LOGW("No decrypt thread");
    mCallback->Decrypted(aBuffer, GMPGenericErr);
    return;
  }

  mThread->Post(WrapTask(this,
                         &ClearKeySessionManager::DoDecrypt,
                         aBuffer, aMetadata));
}

void
ClearKeySessionManager::DoDecrypt(GMPBuffer* aBuffer,
                                  GMPEncryptedBufferMetadata* aMetadata)
{
  CK_LOGD("ClearKeySessionManager::DoDecrypt");

  GMPErr rv = mDecryptionManager->Decrypt(aBuffer->Data(), aBuffer->Size(),
                                              aMetadata);
  CK_LOGD("DeDecrypt finished with code %x\n", rv);
  mCallback->Decrypted(aBuffer, rv);
}

void
ClearKeySessionManager::Shutdown()
{
  CK_LOGD("ClearKeySessionManager::Shutdown");

  for (auto it = mSessions.begin(); it != mSessions.end(); it++) {
    delete it->second;
  }
  mSessions.clear();
}

void
ClearKeySessionManager::DecryptingComplete()
{
  CK_LOGD("ClearKeySessionManager::DecryptingComplete");

  GMPThread* thread = mThread;
  thread->Join();

  Shutdown();
  mDecryptionManager = nullptr;
  Release();
}
