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

#include "eccShortcutFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

bool TShortcutFile::Create()
//	Returns true if successful.
{
	if (Path.IsEmpty()) return false;
	if (Name.IsEmpty()) return false;
	String path = IncludeTrailingBackslash(Path) + Name + ".lnk";

	CoUninitialize(); // For some reason it should be called before CoInit...??
	if (CoInitialize(NULL) != S_OK) // CoInitialize() MUST be called before calling COM functions.
		return false;

	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface:
	hres = CoCreateInstance(
		(_GUID)CLSID_ShellLink,
		NULL,
		CLSCTX_INPROC_SERVER,
		(_GUID)IID_IShellLink,
		(void**)&psl
		);
	if (SUCCEEDED(hres))
	{
		// Set the shortcut properties:
		psl->SetPath(Target.c_str());
		psl->SetDescription(Description.c_str());
		psl->SetArguments(Arguments.c_str());
		psl->SetWorkingDirectory(WorkingDir.c_str());
//		psl->SetShowCmd(MSWinState()); // Does not work!?

//		/* Set the Hotkey: (See MS-Help: SetHotkey and GetHotkey.) */
//		int CtrlAltShift = HotKey & 0xff00; // hi-byte
//		CtrlAltShift = CtrlAltShift >> 5; // Moet 5 bitjes naar rechts...
//		int VkCode = HotKey & 0x00ff; // lo-byte = Virtual-key code
//		psl->SetHotkey( (Word)(CtrlAltShift | VkCode) );
		//HotKey = VK_BACK | scAlt | scShift | scCtrl; // Deze manier ook in helpfile!
   // Leuk voorbeeld: HotKey = VK_DELETE | scAlt | scCtrl;


		// Query IShellLink for the IPersistFile interface:
		IPersistFile* ppf;
		hres = psl->QueryInterface((_GUID)IID_IPersistFile, (void**)&ppf);
		if (SUCCEEDED(hres))
		{
			WORD wsz[MAX_PATH];
			// Ensure that the string is ANSI:
			MultiByteToWideChar(CP_ACP, 0, path.c_str(), -1, (wchar_t*)wsz, MAX_PATH);
			// Save the link:
			hres = ppf->Save((wchar_t*)wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}

	// CoInitialize() and CoUninitialize() calls should be balanced:
	CoUninitialize();

	return SUCCEEDED(hres);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//int TShortcutFile::MSWinState()
//// Converts TWindowState to Microsofts ShowWindow params.
//{
//	switch (WindowState)
//	{
//		case wsMaximized:
//			return SW_MAXIMIZE;
//
//		case wsMinimized:
//			return SW_MINIMIZE;
//
//		case wsNormal:
//		default:
//			return SW_SHOWNORMAL;
//	}
//}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
