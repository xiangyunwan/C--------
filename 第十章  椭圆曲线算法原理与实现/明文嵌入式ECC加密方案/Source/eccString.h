/* ==========================================================================

	ecc - Erik's Code Collection
	Copyright (C) 2003-2005 - Erik Dienske

	This file is part of ecc.

	ecc is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	ecc is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ecc; if not, write to the Free Software Foundation, Inc.,
	59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

===========================================================================*/

//---------------------------------------------------------------------------
#ifndef eccStringH
#define eccStringH
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

typedef enum {
	smtDefault, smtDef,
	smtDefaultB, smtDefB,
	smtNotification, smtNotif,
	smtNotificationB, smtNotifB,
	smtWarning,
	smtError,
	smtAppError
} TLBMessageStyle;
/*	Needed by LBMessage() functions. */

/* typedef struct {
	TColor bg_color;
	TColor font_color;
	TFontStyle font_style;
} TLBCustomMessageStyle; */
/* TODO : For future LBMCustomMsg() functions. */

//---------------------------------------------------------------------------

void MemoMessage(TMemo *memo, const String message,
		int lines_before = 0, int lines_after = 0);
/*  Adds message to memo->Lines.
	lines_before = -2 : message replaces the last line of memo.
	lines_before = -1 : message is appended to the last line of memo.
	lines_before > 0  : message is added after <lines_before> empty lines.
	lines_after  > 0  : message is added followed by <lines_after> empty lines. */

void MemoCursorOnLastLine(TMemo* memo);
/* 	Scroll to the last line of memo. */

//---------------------------------------------------------------------------

void LBCursorOnLastLine(TListBox* lbox);
/* 	Scroll to the last line of lbox. */

void LBMessage(TListBox* lbox, String message,
		int lines_before = 0, int lines_after = 0);
/*  Appends message to lbox->Lines,
	without the fancy color/style options that LBCustomMessage() provides.
	- [lines_before]/[lines_after]: See MemoMessage() description. */

void LBMessage(TListBox* lbox, String message, TLBMessageStyle style,
		int lines_before = 0, int lines_after = 0);
/*  Same as LBMessage(),
	but includes TStatusMessageType presets [style].
	To be effective:
	- Requires lbox->Style to be lbOwnerDrawFixed.
	- Requires LBMessage_DrawItem() to be called by lbox's OnDrawItem event. */

void LBMessage_DrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
/*	When using calls to LBMessage(),
	this function should be called by the ListBox's OnDrawItem event.
	Note that ListBox->Style should be lbOwnerDrawFixed. */

/* void LBCustomMessage(TListBox* lbox, String message,
		TColor bg_color, TColor font_color, TFontStyle font_style,
		int lines_before = 0, int lines_after = 0); */
/*  Appends [message] to lbox, with the following properties:
	- [bg_color]: background color of [message].
	- [font_color]: text color of [message].
	- [font_style]: style of [message].
	- [lines_before]/[lines_after]: See MemoMessage() description.
	To be effective:
	- Requires lbox->Style to be lbOwnerDrawFixed.
	- Requires LBCustomMsg_DrawItem() to be called by lbox's OnDrawItem event. */

/* void LBCustomMessage_DrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State); */
/*	When using calls to LBCustomMsg(),
	this function should be called by the ListBox's OnDrawItem event.
	Note that ListBox->Style should be lbOwnerDrawFixed. */

//---------------------------------------------------------------------------

AnsiString AposStr(AnsiString str)
{	return ("'" + str + "'"); }
/*	Returns an (AnsiString | std::string) surrounded with apostrophes ('). */

AnsiString QuoteStr(AnsiString str)
{	return ("\"" + str + "\""); }
/*	Returns an (AnsiString | std::string) surrounded with doublequotes ("). */

String StripNonNumbers(String str);
/*	Transforms a string to a new string that contains only the characters '0' to '9'.
	All other characters are replaced with '', ie. they are removed. */

bool StrToBool(const String bool_str);
/*  Returns true if [bool_str] is "true" (case-insensitive).
	Returns false otherwise. */

bool StrToBoolDef(const String bool_str, const bool def);
/*  Returns true if [bool_str] is "true" (case-insensitive).
	Returns [def] otherwise. */

String BoolToStr(const bool boolie);
/*	Transforms a boolean value to a string that contains the text 'true' or 'false'. */

String StrToXmlQuotStr(String str);
/*	Replaces (") with (&quot;)
	and (') with (&apos;). */

String StrToXmlUtf8Str(String str);
/*	Converts an AnsiString to a UTF-8 encoded AnsiString.
	Also, these characters: & " ' < > are converted to their
	xml-predefined entity references.
	Note that str should not contain any "&..;" substring. */

String ReplaceStr(String str, String from_str, String to_str);
/* 	Calls BCB6's StrUtils::AnsiReplaceStr() */

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
