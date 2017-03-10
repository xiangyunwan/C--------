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
#ifndef eccErrorH
#define eccErrorH
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

#define ECC_DEBUGINFO	AnsiString("\n\nFunction: " __FUNC__ "\nFile: " __FILE__ "\nLine: " + IntToStr(__LINE__) )

#define ECC_THROW(err)	throw Exception(String(err) + ECC_DEBUGINFO);
/*	Example:
	{	THROW("an error occured"); } */

#define ECC_ERR_MSG(err)	ecc::ShowErrorMessage(String(err) + "\n\nFunc:\t"__FUNC__"\nFile:\t" __FILE__ "\nLine:\t" + IntToStr(__LINE__))

#define ECC_ERR_STR(err)	"Error: '" + String(err) + "' in Function: " __FUNC__ " (Line: " + IntToStr(__LINE__) + "). File: '" __FILE__ "'"
//---------------------------------------------------------------------------

void ShowErrorMessage(const String msg);

String LastErrorCodeToStr(DWORD last_error_code);
/*  Returns a string that describes [last_error_code].
	[last_error_code] is the DWORD returned by WinApi's GetLastError().
	FormatMessage() is used to translates [last_error_code] to a string. */

//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
