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
#ifndef eccRecentFilesH
#define eccRecentFilesH
//---------------------------------------------------------------------------
#include <Menus.hpp>
//---------------------------------------------------------------------------

/*	Usage:
	TRecentFiles* rf = new ecc::TRecentFiles();
	if (rf)
		rf->OnSelect = OnSelectRecentFile; // Declared as: void OnSelectRecentFile(String fpath)

	rf->Add("C:\filename.ext");
	rf->PopupAt(Form1->Left + 32, Form1->Top + 48);
	// Or:
	rf->ToMenuItem(recent_files); // [recent_files] is a TMenuItem, most likely part of a TMenu.

	delete rf;
*/

//===========================================================================
namespace ecc {
//===========================================================================

class TRecentFiles {

private:

	int FMaxVisibleItemsCount;
	int FMaxItemsCount;
	TPopupMenu* FPopMenu;
	TMenuItem* FDefaultMenuItem;
	TStringList* FItems;
	bool FShowHotKeys;

	bool _showPath;
    /*  If true, the full paths of the recent files will be displayed. */

	void __fastcall OnRecentFileClick(TObject *Sender);
	/*	Internal callback, called when a menu item is selected. */

	void ItemsToMenu(TMenuItem* menu_item);
	/*	Populates menu_item with FMaxVisibleItemsCount FItems. */

	void UpdateDefaultMenu();

	typedef void CALLBACK_OnSelect(String fpath);
	CALLBACK_OnSelect* OnSelect;
	/*	External callback, can be set using the constructor. */

public:

	TRecentFiles(TMenuItem* menu_item, CALLBACK_OnSelect* on_select);
	~TRecentFiles();

	void Add(String item);
	/*	Adds [item] to the top of FItems. The [item] is assumed to be a path to some file.
		If [item] exists, it is moved to the top (duplicates are not allowed).
		Updates FDefaultMenuItem if it exists. */

	void Clear();
	/*	Empties FItems, and sets FMaxVisibleItemsCount and FMaxItemsCount to their default values.
		Updates FDefaultMenuItem if it exists. */

	bool IsEmpty()
	{	return (FItems->Count == 0); }

	int GetCount()
	{	return FItems->Count; }

	int SetMaxVisibleItemsCount(int count);
	/*	Sets FMaxVisibleItemsCount to [count], but will not exceed FMaxItemsCount.
		Returns the value FMaxVisibleItemsCount is set to.
		Updates FDefaultMenuItem if it exists. */

	int GetMaxVisibleItemsCount()
	{	return FMaxVisibleItemsCount; }

	int GetVisibleItemsCount();
	/*	Returns the actual amount of visible items.
		This value is equal to or less than FMaxVisibleItemsCount. */

	void setShowPath(bool show);
    /*  Sets/unsets _showPath. */

	bool getShowPath()
	{	return _showPath; }

	void ToFile(String fpath);
	/*	Saves list of files (as text) to file [fpath].
		Note: Replaces original file. */

	void FromFile(String fpath);
	/*  Reads FItems from file.
		Note: Replaces existing FItems.
		Updates FDefaultMenuItem if it exists. */

	bool ToIniFile(String ini_fpath, String section_name);
	/*	Saves list of files to section [section_name] of ini-file [ini_fpath].
		Saves the value of FMaxVisibleItemsCount as "MaxVisibleItemsCount=[FMaxVisibleItemsCount]".
		Saves the value of FShowPath as "ShowPath=[FShowPath]".
		The list will be stored as a name=value pair (ie. "Item.0=C:\filename.ext").
		Note: Replaces existing section.
		Returns false if an error occurred. */

	bool FromIniFile(String ini_fpath, String section_name);
	/*  Retrieves data from section [section_name] of ini-file [ini_fpath].
		Reads the values of FMaxVisibleItemsCount and FShowPath.
		Reads the name=value pairs for the items.
		Returns false if an error occurred.
		Updates FDefaultMenuItem if it exists. */

	void PopupAt(int X, int Y);
	/* 	Displays a maximum of FMaxVisibleItemsCount items
		as a PopupMenu at screen coordinates X and Y. */

	void ToMenuItem(TMenuItem* menu_item);
	/* 	Adds a maximum of FMaxVisibleItemsCount items to [menu_item].
		Previously existing items are removed before adding any new items. */
};
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
