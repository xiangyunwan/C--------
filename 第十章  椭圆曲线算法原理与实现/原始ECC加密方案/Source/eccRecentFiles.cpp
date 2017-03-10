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

#include "eccRecentFiles.h"
#include "eccString.h"
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

const int MaxItemsCount_DEFAULT			= 128;
const String MaxVisibleItemsCount_NAME	= "MaxVisibleItemsCount";
const int MaxVisibleItemsCount_DEFAULT	= 20;
const String ShowPath_NAME				= "ShowPath";
const bool ShowPath_DEFAULT				= false;

//---------------------------------------------------------------------------

void __fastcall TRecentFiles::OnRecentFileClick(TObject *Sender)
{
	TMenuItem* mi = dynamic_cast<TMenuItem*>(Sender);
	if (!mi) return;

	//Call external callback:
	if (OnSelect)
		OnSelect(FItems->Strings[mi->Tag]);
}
//---------------------------------------------------------------------------

TRecentFiles::TRecentFiles(TMenuItem* menu_item, CALLBACK_OnSelect* on_select)
{
	FDefaultMenuItem = menu_item;
	OnSelect 		= on_select;
	FItems	 		= new TStringList();
	FPopMenu 		= new TPopupMenu(NULL);
	// Reset:
	FMaxVisibleItemsCount = MaxVisibleItemsCount_DEFAULT;
	FMaxItemsCount	= MaxItemsCount_DEFAULT;
	FShowHotKeys	= true;
	FShowPath		= ShowPath_DEFAULT;
	Clear();
}
//---------------------------------------------------------------------------

TRecentFiles::~TRecentFiles()
{
	delete FItems;
	delete FPopMenu;
}
//---------------------------------------------------------------------------

void TRecentFiles::Clear()
{
	FItems->Clear();
	if (FDefaultMenuItem)
		FDefaultMenuItem->Clear();
	FPopMenu->Items->Clear();
}
//---------------------------------------------------------------------------

void TRecentFiles::ItemsToMenu(TMenuItem* menu_item)
{
	menu_item->Clear();
	TMenuItem* mi;
	int i=0;
	String hot;
	while (1)
	{
		if (i == FMaxVisibleItemsCount) break;
		if (i == FItems->Count) break;

		mi = new TMenuItem(menu_item);
		try
		{
			if (FShowHotKeys)
			{
				if (i<10) hot = IntToStr(i);
				else hot = String( (char)((int)'A' + (i-10)) );
				hot = "&" + hot + " - ";
			}
			if (FShowPath)
				mi->Caption = hot + FItems->Strings[i];
			else
				mi->Caption = hot + ExtractFileName(FItems->Strings[i]);
			mi->OnClick = OnRecentFileClick;
			mi->Tag		= i;	// Use Tag to refer to the actual item.
			menu_item->Add(mi);
		}
		catch (Exception &ex)
		{
			delete mi;
			#ifdef _DEBUG
				ShowMessage("Could not add a TMenuItem. Function: " __FUNC__);
			#endif
			break;
		}
		i++;
	}
}
//---------------------------------------------------------------------------

void TRecentFiles::UpdateDefaultMenu()
{
	if (FDefaultMenuItem)
		ItemsToMenu(FDefaultMenuItem);
}
//---------------------------------------------------------------------------

void TRecentFiles::Add(String item)
{
	if (item.IsEmpty()) return;
	int pos = FItems->IndexOf(item);
	if (pos != -1) FItems->Delete(pos);
	FItems->Insert(0, item);

	// Check FMaxItemsCount:
	while (FItems->Count > FMaxItemsCount)
		FItems->Delete(FItems->Count -1 );

	UpdateDefaultMenu();
}
//---------------------------------------------------------------------------

int TRecentFiles::SetMaxVisibleItemsCount(int count)
{
	// Check boundaries:
	if (count < 0) count = 0;
	if (count > FMaxItemsCount) count = FMaxItemsCount;

	FMaxVisibleItemsCount = count;
	UpdateDefaultMenu();
	return count;
}
//---------------------------------------------------------------------------

int TRecentFiles::GetVisibleItemsCount()
{
	if (FItems->Count < FMaxVisibleItemsCount)
		return FItems->Count;
	else
		return FMaxVisibleItemsCount;
}
//---------------------------------------------------------------------------

void TRecentFiles::setShowPath(bool show)
{
	_showPath = show;
	UpdateDefaultMenu();
}
//---------------------------------------------------------------------------

void TRecentFiles::ToFile(String fpath)
{
	FItems->SaveToFile(fpath);
}
//---------------------------------------------------------------------------

void TRecentFiles::FromFile(String fpath)
{
	FItems->LoadFromFile(fpath);
	UpdateDefaultMenu();
}
//---------------------------------------------------------------------------

bool TRecentFiles::ToIniFile(String ini_fpath, String section_name)
{
	bool error = false;
	TIniFile* ini = new TIniFile(ini_fpath);
	try
	{
		ini->EraseSection(section_name);
		ini->WriteInteger(section_name, MaxVisibleItemsCount_NAME, FMaxVisibleItemsCount);
		ini->WriteBool(section_name, ShowPath_NAME, FShowPath);
		for (int i=0; i<FItems->Count; i++)
			ini->WriteString(section_name, "Item." + IntToStr(i), FItems->Strings[i]);
	}
	catch(...)
	{
		error = true;
	}
	delete ini;
	return !error;
}
//---------------------------------------------------------------------------

bool TRecentFiles::FromIniFile(String ini_fpath, String section_name)
{
	bool error = false;
	TIniFile* ini = new TIniFile(ini_fpath);
	try
	{
		Clear();

		FMaxVisibleItemsCount = ini->ReadInteger(section_name, MaxVisibleItemsCount_NAME, MaxVisibleItemsCount_DEFAULT);
		FShowPath = ini->ReadBool(section_name, ShowPath_NAME, ShowPath_DEFAULT);

		String item;
		int i=0;
		while (1)
		{
			item = ini->ReadString(section_name, "Item." + IntToStr(i), "");
			if (item.IsEmpty()) break;
			if (FItems->IndexOf(item) == -1)	// No duplicates
				FItems->Append(item);
			++i;
		}
		UpdateDefaultMenu();
	}
	catch(...)
	{
		error = true;
	}
	delete ini;
	return !error;
}
//---------------------------------------------------------------------------

void TRecentFiles::PopupAt(int X, int Y)
{
	FPopMenu->Items->Clear();
	ItemsToMenu(FPopMenu->Items);
	FPopMenu->Popup(X, Y);
}
//---------------------------------------------------------------------------

void TRecentFiles::ToMenuItem(TMenuItem* menu_item)
{
	ItemsToMenu(menu_item);
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
