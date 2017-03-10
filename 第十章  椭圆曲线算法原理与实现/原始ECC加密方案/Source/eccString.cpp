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
#include <vcl.h>
#pragma hdrstop

#include "eccString.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <StrUtils.hpp>	// AnsiReplaceStr
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------
#ifdef _DEBUG
	static bool MessageLinesParamsValid(int lines_before, int lines_after);
#endif

static int EccPrevMemoLinesBefore = 0;
//---------------------------------------------------------------------------

void MemoMessage(TMemo *memo, const String message,
		int lines_before, int lines_after)
{
	#ifdef _DEBUG
		if (!MessageLinesParamsValid(lines_before, lines_after))
			return;
	#endif

	switch (lines_before)
	{
		case -2:
			if (EccPrevMemoLinesBefore != -2)
				memo->Lines->Append(message);
			else
				memo->Lines->Strings[memo->Lines->Count - 1] = message;
			break;
		case -1:
			memo->Lines->Strings[memo->Lines->Count - 1]
				= memo->Lines->Strings[memo->Lines->Count - 1] + message;
			break;
		default:
			for (lines_before; lines_before > 0; lines_before--)
				memo->Lines->Append("");
			memo->Lines->Append(message);
	}

	for (lines_after; lines_after > 0; lines_after--)
		memo->Lines->Append("");

	MemoCursorOnLastLine(memo);
	EccPrevMemoLinesBefore = lines_before;
}
//---------------------------------------------------------------------------

void MemoCursorOnLastLine(TMemo* memo)
{
	memo->SelStart = memo->Lines->Text.Length() - 1;
}
//===========================================================================

//---------------------------------------------------------------------------

void LBCursorOnLastLine(TListBox* lbox)
{
	lbox->ItemIndex = lbox->Items->Count - 1;
	lbox->ItemIndex = -1;	// Unselect
}
//---------------------------------------------------------------------------

static int EccPrevLBLinesBefore = 0;
//---------------------------------------------------------------------------

void LBMessage(TListBox *lbox, String message,
		int lines_before, int lines_after)
{
	// Tabs etc. don't display very well in ListBoxes:
	/* TODO : Replace  \t, \n and \r with spaces? */
/* 	message = AnsiReplaceStr(message, "\t", " ");
	message = AnsiReplaceStr(message, "\r", " ");
	message = AnsiReplaceStr(message, "\n", " "); */

	#ifdef _DEBUG
		if (!MessageLinesParamsValid(lines_before, lines_after))
			return;
	#endif

	switch (lines_before)
	{
		case -2:
			if (EccPrevLBLinesBefore != -2)
				lbox->Items->Append(message);
			else
				lbox->Items->Strings[lbox->Count - 1] = message;
			break;
		case -1:
			lbox->Items->Strings[lbox->Count - 1]
				= lbox->Items->Strings[lbox->Count - 1] + message;
			break;
		default:
			for (lines_before; lines_before > 0; lines_before--)
				lbox->Items->Append("");
			lbox->Items->Append(message);
	}

	for (lines_after; lines_after > 0; lines_after--)
		lbox->Items->Append("");

	LBCursorOnLastLine(lbox);
	EccPrevLBLinesBefore = lines_before;
}
//---------------------------------------------------------------------------

void LBMessage(TListBox *lbox, String message, TLBMessageStyle style,
		int lines_before, int lines_after)
{
	LBMessage(lbox, message, lines_before, lines_after);

	// (Ab)Use lbox->Items->Objects[] to store TStatusMessageType info:
	lbox->Items->Objects[lbox->Items->Count - 1 - lines_after] = (TObject*)style;
	lbox->Repaint();
}
//---------------------------------------------------------------------------

void LBMessage_DrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox* lbox = dynamic_cast<TListBox*>(Control);
	if (!lbox) return;

	TCanvas* canvas = lbox->Canvas;

	// Set defaults:
	canvas->Brush->Color = lbox->Color;
	canvas->Font->Style = lbox->Font->Style;

	TLBMessageStyle style = (TLBMessageStyle)lbox->Items->Objects[Index];

	switch (style)
	{
		default:
		case smtDefaultB:
		case smtDefB:
			canvas->Font->Style = canvas->Font->Style << fsBold;
		case smtDefault:
		case smtDef:
			canvas->Font->Color = clWindowText;
			break;

		case smtNotificationB:
		case smtNotifB:
			canvas->Font->Color = clNavy;
			canvas->Font->Style = canvas->Font->Style << fsBold;
			canvas->Brush->Color = (TColor)0xFFF0F0;
			break;
		case smtNotification:
		case smtNotif:
			canvas->Font->Color = clBlue;
			break;

		case smtWarning:
			//canvas->Font->Color = clFuchsia;
			canvas->Font->Color = (TColor)0x9900cc;
			break;

		case smtError:
			canvas->Font->Color = clRed;
			canvas->Brush->Color = clCream;
			break;

		case smtAppError:
			canvas->Font->Color = clYellow;
			canvas->Brush->Color = clRed;
			break;
	}

	// Clear the rectangle: (NECESSARY!)
	canvas->FillRect(Rect);

	// Display text:
	canvas->TextOut(Rect.Left + 2, Rect.Top, lbox->Items->Strings[Index]);
}
//---------------------------------------------------------------------------

/* void LBCustomMessage(TListBox *lbox, String message,
		TColor bg_color, TColor font_color, TFontStyle font_style,
		int lines_before, int lines_after)
{
	LBMessage(lbox, message, lines_before, lines_after);

	TLBCustomMessageStyle style;
	style.bg_color		= bg_color;
	style.font_color	= font_color;
	style.font_style	= font_style;

	// (Ab)Use lbox->Items->Objects[] to store TStatusMessageType info:
	lbox->Items->Objects[lbox->Items->Count - 1 - lines_after] = (TObject*)&style;
	lbox->Repaint();
} */
//---------------------------------------------------------------------------

/* void LBCustomMessage_DrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox* lbox = dynamic_cast<TListBox*>(Control);
	if (!lbox) return;

	TCanvas* canvas = lbox->Canvas;

	TLBCustomMessageStyle* style = (TLBCustomMessageStyle*)lbox->Items->Objects[Index];

	// Set style values:
	canvas->Brush->Color = style->bg_color;
	canvas->Font->Color = style->font_color;
	canvas->Font->Style = canvas->Font->Style << style->font_style;

	// Clear the rectangle: (NECESSARY!)
	canvas->FillRect(Rect);

	// Display text:
	canvas->TextOut(Rect.Left + 2, Rect.Top, lbox->Items->Strings[Index]);

	TODO: Should TLBCustomMessageStyle object be freed? 
} */
//---------------------------------------------------------------------------

//===========================================================================
#ifdef _DEBUG
	bool MessageLinesParamsValid(int lines_before, int lines_after)
	{
	// If one of the following errors occurs, you should check the parameters mentioned.
		if (lines_before < -2)
		{
			ShowMessage("ERROR with parameter: "
				"lines_before < -2\n"
				"File: " __FILE__ "\n"
				"Function: " __FUNC__ );
			return false;
		}
		if (lines_after < 0)
		{
			ShowMessage("ERROR with parameter: "
				"lines_after < 0\n"
				"File:" __FILE__ "\n"
				"Function: " __FUNC__ );
			return false;
		}
		return true;
	}
#endif
//===========================================================================

String StripNonNumbers(String str)
{
	for (int i=1; i<=str.Length(); i++)
	{
		if ((str[i] < '0') || (str[i] > '9')) str[i] = ' ';
	}
	return AnsiReplaceStr(str, " ", "");
}
//---------------------------------------------------------------------------

bool StrToBool(String bool_str)
{
	return (LowerCase(bool_str) == "true") ? true : false;
}
//---------------------------------------------------------------------------

bool StrToBoolDef(String bool_str, const bool def)
{
	return (LowerCase(bool_str) == "true") ? true : def;
}
//---------------------------------------------------------------------------

String BoolToStr(const bool boolie)
{
	return boolie ? "true" : "false";
}
//---------------------------------------------------------------------------

String StrToXmlQuotStr(String str)
{
	str = AnsiReplaceStr(str, "\"", "&quot;");
	str = AnsiReplaceStr(str, "'", "&apos;");
	return str;
}
//---------------------------------------------------------------------------

String StrToXmlUtf8Str(String str)
{
	str = AnsiReplaceStr(str, "&", "&amp;"); // Should be the first replacement! (because of the '&').
	str = AnsiReplaceStr(str, "\"", "&quot;");
	str = AnsiReplaceStr(str, "'", "&apos;");
	str = AnsiReplaceStr(str, "<", "&lt;");
	str = AnsiReplaceStr(str, ">", "&gt;");
	return AnsiToUtf8(str);
}
//---------------------------------------------------------------------------

String ReplaceStr(String str, String from_str, String to_str)
{
	return AnsiReplaceStr(str, from_str, to_str);
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================

