/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

/* Temporary Interface Info related stuff. */

#include "xpcprivate.h"

NS_IMPL_ISUPPORTS(nsInterfaceInfo, NS_IINTERFACEINFO_IID)

nsInterfaceInfo::nsInterfaceInfo(REFNSIID aIID, const char* aName,
                                 nsInterfaceInfo* aParent)
    : mIID(aIID),
      mName(strdup(aName)),
      mParent(aParent)
{
    NS_PRECONDITION(aName, "bad param");
    NS_INIT_REFCNT();
    NS_ADDREF_THIS();

    mMethodBaseIndex = mParent ?
                        mParent->mMethodBaseIndex +
                        mParent->mMethodCount : 0;

    mConstantBaseIndex = mParent ?
                          mParent->mConstantBaseIndex +
                          mParent->mConstantCount : 0;

    // XXX implement for real
    mMethodCount = 0;
    mMethods = NULL;

    mConstantCount = 0;
    mConstants = NULL;

    if(mParent)
        NS_ADDREF(mParent);
}

nsInterfaceInfo::~nsInterfaceInfo()
{
    if(mName)
        free(mName);
    if(mParent)
        NS_RELEASE(mParent);
}

nsresult
nsInterfaceInfo::GetName(const char** name)
{
    NS_PRECONDITION(name, "bad param");
    *name = mName;
    return NS_OK;
}

nsresult
nsInterfaceInfo::GetIID(const nsIID** iid)
{
    NS_PRECONDITION(iid, "bad param");
    *iid = &mIID;
    return NS_OK;
}

nsresult
nsInterfaceInfo::GetParent(nsIInterfaceInfo** parent)
{
    NS_PRECONDITION(parent, "bad param");
    *parent = mParent;
    return NS_OK;
}

nsresult
nsInterfaceInfo::GetMethodCount(int* count)
{
    NS_PRECONDITION(count, "bad param");
    *count = mMethodBaseIndex + mMethodCount;
    return NS_OK;
}

nsresult
nsInterfaceInfo::GetConstantCount(int* count)
{
    NS_PRECONDITION(count, "bad param");
    *count = mConstantBaseIndex + mConstantCount;
    return NS_OK;
}

nsresult
nsInterfaceInfo::GetMethodInfo(unsigned index, const nsXPCMethodInfo** info)
{
    NS_PRECONDITION(info, "bad param");
    if(index < mMethodBaseIndex)
        return mParent->GetMethodInfo(index, info);

    if(index >= mMethodBaseIndex + mMethodCount)
    {
        NS_PRECONDITION(0, "bad param");
        *info = NULL;
        return NS_ERROR_INVALID_ARG;
    }

    // else...
    *info = mMethods[mMethodBaseIndex-index];
    return NS_OK;
}

nsresult
nsInterfaceInfo::GetConstant(unsigned index, const nsXPCConstant** constant)
{
    NS_PRECONDITION(constant, "bad param");
    if(index < mConstantBaseIndex)
        return mParent->GetConstant(index, constant);

    if(index >= mConstantBaseIndex + mConstantCount)
    {
        NS_PRECONDITION(0, "bad param");
        *constant = NULL;
        return NS_ERROR_INVALID_ARG;
    }

    // else...
    *constant = mConstants[mConstantBaseIndex-index];
    return NS_OK;
}

