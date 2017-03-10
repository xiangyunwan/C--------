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

#include "eccError.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

void ShowErrorMessage(const String msg)
{
	MessageDlg(msg, mtError, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

String LastErrorCodeToStr(DWORD last_error_code)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	// DWORD dwFlags : source and processing options
		NULL,	// LPCVOID lpSource : pointer to  message source
		last_error_code,	// DWORD dwMessageId : requested message identifier
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language. // DWORD dwLanguageId : language identifier for requested message
		(LPTSTR) &lpMsgBuf,	// LPTSTR lpBuffer : pointer to message buffer
		0,		// DWORD nSize : maximum size of message buffer
		NULL	// va_list *Arguments : address of array of message inserts
		);
	String err_str = String((char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return err_str;
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================

