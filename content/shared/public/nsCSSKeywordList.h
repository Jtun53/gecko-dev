/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1999 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 */

/******

  This file contains the list of all parsed CSS keywords
  See nsCSSKeywords.h for access to the enum values for keywords

  It is designed to be used as inline input to nsCSSKeywords.cpp *only*
  through the magic of C preprocessing.

  All entires must be enclosed in the macro CSS_KEY which will have cruel
  and unusual things done to it

  It is recommended (but not strictly necessary) to keep all entries
  in alphabetical order

  Requirements:

  Entries are in the form: (name,id). 'id' must always be the same as 'name'
  except that all hyphens ('-') in 'name' are converted to underscores ('_')
  in 'id'. This lets us do nice things with the macros without having to 
  copy/convert strings at runtime. 
  
  'name' entries *must* use only lowercase characters.

  ** Break these invarient and bad things will happen. **    

 ******/

CSS_KEY(-moz-all, _moz_all)
CSS_KEY(-moz-bg-inset, _moz_bg_inset)
CSS_KEY(-moz-bg-outset, _moz_bg_outset)
CSS_KEY(-moz-center, _moz_center)
CSS_KEY(-moz-field, _moz_field)
CSS_KEY(-moz-right, _moz_right)
CSS_KEY(-moz-pre-wrap, _moz_pre_wrap)
CSS_KEY(-moz-scrollbars-none, _moz_scrollbars_none)
CSS_KEY(-moz-scrollbars-horizontal, _moz_scrollbars_horizontal)
CSS_KEY(-moz-scrollbars-vertical, _moz_scrollbars_vertical)
CSS_KEY(above, above)
CSS_KEY(absolute, absolute)
CSS_KEY(activeborder, activeborder)
CSS_KEY(activecaption, activecaption)
CSS_KEY(alias, alias)
CSS_KEY(all, all)
CSS_KEY(always, always)
CSS_KEY(appworkspace, appworkspace)
CSS_KEY(arabic-indic, arabic_indic)
CSS_KEY(armenian, armenian)
CSS_KEY(auto, auto)
CSS_KEY(avoid, avoid)
CSS_KEY(background, background)
CSS_KEY(baseline, baseline)
CSS_KEY(behind, behind)
CSS_KEY(below, below)
CSS_KEY(bengali, bengali)
CSS_KEY(bidi-override, bidi_override)
CSS_KEY(blink, blink)
CSS_KEY(block, block)
CSS_KEY(bold, bold)
CSS_KEY(bolder, bolder)
CSS_KEY(border-box, border_box)
CSS_KEY(both, both)
CSS_KEY(bottom, bottom)
CSS_KEY(button, button)
CSS_KEY(buttonface, buttonface)
CSS_KEY(buttonhighlight, buttonhighlight)
CSS_KEY(buttonshadow, buttonshadow)
CSS_KEY(buttontext, buttontext)
CSS_KEY(capitalize, capitalize)
CSS_KEY(caption, caption)
CSS_KEY(captiontext, captiontext)
CSS_KEY(cell, cell)
CSS_KEY(center, center)
CSS_KEY(center-left, center_left)
CSS_KEY(center-right, center_right)
CSS_KEY(ch, ch)
CSS_KEY(circle, circle)
CSS_KEY(cjk-earthly-branch, cjk_earthly_branch)
CSS_KEY(cjk-heavenly-stem, cjk_heavenly_stem)
CSS_KEY(cjk-ideographic, cjk_ideographic)
CSS_KEY(close-quote, close_quote)
CSS_KEY(cm, cm)
CSS_KEY(code, code)
CSS_KEY(collapse, collapse)
CSS_KEY(compact, compact)
CSS_KEY(condensed, condensed)
CSS_KEY(content-box, content_box)
CSS_KEY(context-menu, context_menu)
CSS_KEY(continuous, continuous)
CSS_KEY(copy, copy)
CSS_KEY(count-down, count_down)
CSS_KEY(count-up, count_up)
CSS_KEY(count-up-down, count_up_down)
CSS_KEY(crop, crop)
CSS_KEY(cross, cross)
CSS_KEY(crosshair, crosshair)
CSS_KEY(dashed, dashed)
CSS_KEY(decimal, decimal)
CSS_KEY(decimal-leading-zero, decimal_leading_zero)
CSS_KEY(default, default)
CSS_KEY(deg, deg)
CSS_KEY(desktop, desktop)
CSS_KEY(devanagari, devanagari)
CSS_KEY(dialog, dialog)
CSS_KEY(digits, digits)
CSS_KEY(disabled, disabled)
CSS_KEY(disc, disc)
CSS_KEY(document, document)
CSS_KEY(dotted, dotted)
CSS_KEY(double, double)
CSS_KEY(e-resize, e_resize)
CSS_KEY(element, element)
CSS_KEY(elements, elements)
CSS_KEY(em, em)
CSS_KEY(embed, embed)
CSS_KEY(enabled, enabled)
CSS_KEY(ex, ex)
CSS_KEY(expanded, expanded)
CSS_KEY(extra-condensed, extra_condensed)
CSS_KEY(extra-expanded, extra_expanded)
CSS_KEY(far-left, far_left)
CSS_KEY(far-right, far_right)
CSS_KEY(fast, fast)
CSS_KEY(faster, faster)
CSS_KEY(field, field)
CSS_KEY(fixed, fixed)
CSS_KEY(georgian, georgian)
CSS_KEY(grab, grab)
CSS_KEY(grabbing, grabbing)
CSS_KEY(grad, grad)
CSS_KEY(graytext, graytext)
CSS_KEY(groove, groove)
CSS_KEY(gujarati, gujarati)
CSS_KEY(gurmukhi, gurmukhi)
CSS_KEY(hebrew, hebrew)
CSS_KEY(help, help)
CSS_KEY(hidden, hidden)
CSS_KEY(hide, hide)
CSS_KEY(high, high)
CSS_KEY(higher, higher)
CSS_KEY(highlight, highlight)
CSS_KEY(highlighttext, highlighttext)
CSS_KEY(hiragana, hiragana)
CSS_KEY(hiragana-iroha, hiragana_iroha)
CSS_KEY(horizontal, horizontal)
CSS_KEY(hz, hz)
CSS_KEY(icon, icon)
CSS_KEY(ignore, ignore)
CSS_KEY(in, in)
CSS_KEY(inactiveborder, inactiveborder)
CSS_KEY(inactivecaption, inactivecaption)
CSS_KEY(inactivecaptiontext, inactivecaptiontext)
CSS_KEY(info, info)
CSS_KEY(infobackground, infobackground)
CSS_KEY(infotext, infotext)
CSS_KEY(inherit, inherit)
CSS_KEY(inline, inline)
CSS_KEY(inline-block, inline_block)
CSS_KEY(inline-table, inline_table)
CSS_KEY(inset, inset)
CSS_KEY(inside, inside)
CSS_KEY(invert, invert)
CSS_KEY(italic, italic)
CSS_KEY(japanese-formal, japanese_formal)
CSS_KEY(japanese-informal, japanese_informal)
CSS_KEY(justify, justify)
CSS_KEY(kannada, kannada)
CSS_KEY(katakana, katakana)
CSS_KEY(katakana-iroha, katakana_iroha)
CSS_KEY(khmer, khmer)
CSS_KEY(khz, khz)
CSS_KEY(landscape, landscape)
CSS_KEY(lao, lao)
CSS_KEY(large, large)
CSS_KEY(larger, larger)
CSS_KEY(left, left)
CSS_KEY(left-side, left_side)
CSS_KEY(leftwards, leftwards)
CSS_KEY(level, level)
CSS_KEY(lighter, lighter)
CSS_KEY(line-through, line_through)
CSS_KEY(list, list)
CSS_KEY(list-item, list_item)
CSS_KEY(loud, loud)
CSS_KEY(low, low)
CSS_KEY(lower, lower)
CSS_KEY(lower-alpha, lower_alpha)
CSS_KEY(lower-greek, lower_greek)
CSS_KEY(lower-latin, lower_latin)
CSS_KEY(lower-roman, lower_roman)
CSS_KEY(lowercase, lowercase)
CSS_KEY(ltr, ltr)
CSS_KEY(malayalam, malayalam)
CSS_KEY(margin-box, margin_box)
CSS_KEY(marker, marker)
CSS_KEY(medium, medium)
CSS_KEY(menu, menu)
CSS_KEY(menutext, menutext)
CSS_KEY(message-box, message_box)
CSS_KEY(middle, middle)
CSS_KEY(mix, mix)
CSS_KEY(mm, mm)
CSS_KEY(move, move)
CSS_KEY(ms, ms)
CSS_KEY(myanmar, myanmar)
CSS_KEY(n-resize, n_resize)
CSS_KEY(narrower, narrower)
CSS_KEY(ne-resize, ne_resize)
CSS_KEY(no-close-quote, no_close_quote)
CSS_KEY(no-open-quote, no_open_quote)
CSS_KEY(no-repeat, no_repeat)
CSS_KEY(none, none)
CSS_KEY(normal, normal)
CSS_KEY(noshade, noshade)
CSS_KEY(nowrap, nowrap)
CSS_KEY(nw-resize, nw_resize)
CSS_KEY(oblique, oblique)
CSS_KEY(once, once)
CSS_KEY(open-quote, open_quote)
CSS_KEY(oriya, oriya)
CSS_KEY(outset, outset)
CSS_KEY(outside, outside)
CSS_KEY(overline, overline)
CSS_KEY(padding-box, padding_box)
CSS_KEY(paragraph, paragraph)
CSS_KEY(pc, pc)
CSS_KEY(persian, persian)
CSS_KEY(pointer, pointer)
CSS_KEY(portrait, portrait)
CSS_KEY(pre, pre)
CSS_KEY(pt, pt)
CSS_KEY(pull-down-menu, pull_down_menu)
CSS_KEY(px, px)
CSS_KEY(rad, rad)
CSS_KEY(read-only, read_only)
CSS_KEY(read-write, read_write)
CSS_KEY(relative, relative)
CSS_KEY(repeat, repeat)
CSS_KEY(repeat-x, repeat_x)
CSS_KEY(repeat-y, repeat_y)
CSS_KEY(ridge, ridge)
CSS_KEY(right, right)
CSS_KEY(right-side, right_side)
CSS_KEY(rightwards, rightwards)
CSS_KEY(rtl, rtl)
CSS_KEY(run-in, run_in)
CSS_KEY(s, s)
CSS_KEY(s-resize, s_resize)
CSS_KEY(scroll, scroll)
CSS_KEY(scrollbar, scrollbar)
CSS_KEY(se-resize, se_resize)
CSS_KEY(select-all, select_all)
CSS_KEY(select-before, select_before)
CSS_KEY(select-after, select_after)
CSS_KEY(select-same, select_same)
CSS_KEY(select-menu, select_menu)
CSS_KEY(semi-condensed, semi_condensed)
CSS_KEY(semi-expanded, semi_expanded)
CSS_KEY(separate, separate)
CSS_KEY(show, show)
CSS_KEY(silent, silent)
CSS_KEY(simp-chinese-formal, simp_chinese_formal)
CSS_KEY(simp-chinese-informal, simp_chinese_informal)
CSS_KEY(slow, slow)
CSS_KEY(slower, slower)
CSS_KEY(small, small)
CSS_KEY(small-caps, small_caps)
CSS_KEY(small-caption, small_caption)
CSS_KEY(smaller, smaller)
CSS_KEY(soft, soft)
CSS_KEY(solid, solid)
CSS_KEY(spell-out, spell_out)
CSS_KEY(spinning, spinning)
CSS_KEY(square, square)
CSS_KEY(start, start)
CSS_KEY(static, static)
CSS_KEY(status-bar, status_bar)
CSS_KEY(sub, sub)
CSS_KEY(super, super)
CSS_KEY(sw-resize, sw_resize)
CSS_KEY(table, table)
CSS_KEY(table-caption, table_caption)
CSS_KEY(table-cell, table_cell)
CSS_KEY(table-column, table_column)
CSS_KEY(table-column-group, table_column_group)
CSS_KEY(table-footer-group, table_footer_group)
CSS_KEY(table-header-group, table_header_group)
CSS_KEY(table-row, table_row)
CSS_KEY(table-row-group, table_row_group)
CSS_KEY(tamil, tamil)
CSS_KEY(telugu, telugu)
CSS_KEY(text, text)
CSS_KEY(text-bottom, text_bottom)
CSS_KEY(text-top, text_top)
CSS_KEY(thai, thai)
CSS_KEY(thick, thick)
CSS_KEY(thin, thin)
CSS_KEY(threeddarkshadow, threeddarkshadow)
CSS_KEY(threedface, threedface)
CSS_KEY(threedhighlight, threedhighlight)
CSS_KEY(threedlightshadow, threedlightshadow)
CSS_KEY(threedshadow, threedshadow)
CSS_KEY(toggle, toggle)
CSS_KEY(top, top)
CSS_KEY(trad-chinese-formal, trad_chinese_formal)
CSS_KEY(trad-chinese-informal, trad_chinese_informal)
CSS_KEY(transparent, transparent)
CSS_KEY(tri-state, tri_state)
CSS_KEY(ultra-condensed, ultra_condensed)
CSS_KEY(ultra-expanded, ultra_expanded)
CSS_KEY(underline, underline)
CSS_KEY(upper-alpha, upper_alpha)
CSS_KEY(upper-latin, upper_latin)
CSS_KEY(upper-roman, upper_roman)
CSS_KEY(uppercase, uppercase)
CSS_KEY(urdu, urdu)
CSS_KEY(vertical, vertical)
CSS_KEY(visible, visible)
CSS_KEY(w-resize, w_resize)
CSS_KEY(wait, wait)
CSS_KEY(wider, wider)
CSS_KEY(window, window)
CSS_KEY(windowframe, windowframe)
CSS_KEY(windowtext, windowtext)
CSS_KEY(workspace, workspace)
CSS_KEY(write-only, write_only)
CSS_KEY(x-fast, x_fast)
CSS_KEY(x-high, x_high)
CSS_KEY(x-large, x_large)
CSS_KEY(x-loud, x_loud)
CSS_KEY(x-low, x_low)
CSS_KEY(x-slow, x_slow)
CSS_KEY(x-small, x_small)
CSS_KEY(x-soft, x_soft)
CSS_KEY(xx-large, xx_large)
CSS_KEY(xx-small, xx_small)
