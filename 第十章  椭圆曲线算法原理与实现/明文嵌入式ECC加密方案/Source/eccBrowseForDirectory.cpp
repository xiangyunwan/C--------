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

#include "eccBrowseForDirectory.h"
#include <shlobj.h>	// Requires global Conditional Define: NO_WIN32_LEAN_AND_MEAN
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

// === Some Declarations: ===

static int __stdcall eccBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
/*	Internal function - do not call directly.
	Sets initial directory for SHBrowseForFolder (in EccBrowseForDirectory). */

static String FBfdSelectedDirectory;
/*	Required to communicate with EccBrowseCallbackProc(): */

//===========================================================================

bool TBrowseForDirectory::Execute()
{
	// Make directory available for eccBrowseCallbackProc():
	FBfdSelectedDirectory = Directory;

	// Create and clear BROWSEINFO object:
	BROWSEINFO BrowsingInfo;
	memset(&BrowsingInfo, 0, sizeof(BROWSEINFO));
	char dir[MAX_PATH];

	BrowsingInfo.hwndOwner      = Handle;
	BrowsingInfo.pszDisplayName = dir;
	BrowsingInfo.ulFlags        = FFlags;
	BrowsingInfo.lpszTitle      = Title.c_str();
	BrowsingInfo.lpfn           = eccBrowseCallbackProc;

	// Do the browsing, BIF_NEWDIALOGSTYLE (part of BIF_USENEWUI) requires Ole(Un)Initialize:
	if (FFlags & BIF_NEWDIALOGSTYLE)
		OleInitialize(NULL);
	else
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	LPITEMIDLIST pidl = SHBrowseForFolder(&BrowsingInfo);
	if (FFlags & BIF_NEWDIALOGSTYLE)
		OleUninitialize();
	else
		CoUninitialize();

	// Retrieve selected folder:
	if (pidl)
	{
		char path[MAX_PATH] = "";
		SHGetPathFromIDList(pidl, path);
		Directory = IncludeTrailingBackslash(path);
		return true;
	}
	else // Cancel is pressed:
	{
		Directory = "";
		return false;
	}
}
//---------------------------------------------------------------------------

void TBrowseForDirectory::SetUseNewUI(bool use_new_ui)
{
//	From shlobj.h:	BIF_USENEWUI = (BIF_NEWDIALOGSTYLE | BIF_EDITBOX)

	if (use_new_ui)
	{
		FFlags |= BIF_USENEWUI;
		FFlags &= ~BIF_STATUSTEXT;
	}
	else
	{
		FFlags &= ~BIF_USENEWUI;
		FFlags |= BIF_STATUSTEXT;
	}
}
//---------------------------------------------------------------------------

int __stdcall eccBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
			if (!FBfdSelectedDirectory.IsEmpty())
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)FBfdSelectedDirectory.c_str() );
			break;

		case BFFM_SELCHANGED:
			// Set the status window to the currently selected path.
			// Does not work with BIF_NEWDIALOGSTYLE.
			char szDir[MAX_PATH];
			if (SHGetPathFromIDList((LPITEMIDLIST) lParam , szDir))
				SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
			break;

/*	Following only occurs when FFlags contains BIF_VALIDATE.
		case BFFM_VALIDATEFAILED:
			ShowMessage( String(szDir) +" does not exist.");
			break; */

		default:
			break;
	}

	return 0;
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
