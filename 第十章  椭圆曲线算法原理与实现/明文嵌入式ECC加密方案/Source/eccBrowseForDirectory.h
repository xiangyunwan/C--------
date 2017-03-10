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
#ifndef eccBrowseForDirectoryH
#define eccBrowseForDirectoryH

//---------------------------------------------------------------------------
#ifndef NO_WIN32_LEAN_AND_MEAN
	#error You need to define NO_WIN32_LEAN_AND_MEAN globally.
#endif
//---------------------------------------------------------------------------
/*
	Wrapper for WinAPI SHBrowseForFolder.
	-------------------------------------
	Example:
	{
		ecc::TBrowseForDirectory bfd(true);	// Set use_new_ui.
		bfd.Handle 		= f_Main->Handle;	// If not set Handle will be Application->Handle.
		bfd.Title 		= "Choose destination directory:";
		bfd.Directory	= "C:\\Temp"; 		// Initial directory; NULL = Desktop. Also the selected directory.
		String dir;
		if (bfd.Execute()) dir = bfd.Directory;
	}

	Note that MS Help files included with BCB do not provide information for 'versions' higher than 4.0.
	For more information on BROWSEINFO, see:
		http://msdn.microsoft.com/library/default.asp?url=/library/en-us/shellcc/platform/shell/reference/structures/browseinfo.asp
	For more information on 'versions', see:
		http://msdn.microsoft.com/library/default.asp?url=/library/en-us/shellcc/platform/shell/programmersguide/versions.asp
*/
//===========================================================================
namespace ecc {
//===========================================================================

class TBrowseForDirectory {

public:

	TBrowseForDirectory(bool use_new_ui)
	{
		Handle = Application->Handle;
		Directory = "";
		FFlags = BIF_RETURNONLYFSDIRS;// | BIF_SHAREABLE;
		SetUseNewUI(use_new_ui);
	};

	HWND Handle;
	/*	Handle to owner window. */

	String Directory;
	/* 	Initial directory. NULL = Desktop.
		After Execute(), it contains the selected directory, including a trailing backslash. */

	String Title;
	/*	Text that appears on the top of the BrowseForDirectory window. */

	bool Execute();
	/*	Show the BrowseForDirectory window.
		Returns false if cancel was pressed. */

private:

	void SetUseNewUI(bool use_new_ui);

protected:

	UINT FFlags;
};

//===========================================================================
} // namespace ecc
//===========================================================================
#endif
