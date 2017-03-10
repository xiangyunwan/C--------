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

#include "eccPropertyHandler.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
/*
	TPropertyHandler v1.0
	=====================
	Author: Erik Dienske

	TPropertyHandler allows control of the properties and values of
	descendants of TComponent (VCL components).

	Every function has a description, take a look at the header file too.
	------
	Example 1:
		TPropertyHandler ph(f_Main);
		Caption = ph.GetValue("BorderIcons");
	Example 2:
		TPropertyHandler ph;
		Caption = ph.GetNameValue(Form1, "WindowState");
	Example 3:
		TPropertyHandler* ph = new TPropertyHandler(CheckBox1);
		Caption = ph.SetValue("Checked","true");
		delete ph;
	------
	If you #define PROPERTYHANDLER_SHOW_WARNINGS warnings will be displayed
	(using ShowMessage()) if a property does not exist or if the
	default Component is not defined - for debugging purposes.
	------
	If a returned string contains several lines it does not display nice in
	components like a Memo if you use: Memo->Lines->Add ..
	but is does look right if you use: Memo->Lines->Text = ..
*/
//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

TPropertyHandler::TPropertyHandler()
{
	FDefaultComponent = NULL;
}
//---------------------------------------------------------------------------

TPropertyHandler::TPropertyHandler(TComponent* comp)
{
	FDefaultComponent = comp;
}
//---------------------------------------------------------------------------

//===========================================================================
//== Private functions: (For internal use by TPropertyHandler)
//===========================================================================

bool TPropertyHandler::DefaultComponentExists()
{
	#ifdef PROPERTYHANDLER_SHOW_WARNINGS
	if (FDefaultComponent == NULL)
	{
		ShowMessage(
			"TPropertyHandler warning:\n\n"
			"Default Component is not defined." );
	}
	#endif

	return (FDefaultComponent != NULL);
}
//---------------------------------------------------------------------------

String TPropertyHandler::BuildNameValue(TComponent* comp, String prName)
{
	return prName + "=" + GetPropValue(comp, prName, true);
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetClassNamesValues(TComponent* comp, String prName)
{
	TObject *subObj = (TObject *)GetOrdProp(comp, prName);
	if (subObj == NULL) return ""; // In case the prop-value is empty (like Action).

/*  TObject* subObj is needed to find out if the subprop points to
	another Component (like ActiveControl).
	If it is only the value of that property is returned (ie.: "CheckBox1").
	If it is a class like Font all of the props+values of
	comp->Font are returned. */

	if (dynamic_cast<TComponent*>(subObj))
	{   // Find out if subObj points to another Component by casting.
		return prName + "=" + GetValue((TComponent*)subObj,"Name");
	}

/*  subObj does not point to another Component, but we can convert
	it to a TComponent* subComp so it can be used te get the subProps+values.
	This sounds weird but don't forget that a TFont encapsulated in a TForm
	becomes a descendant of TForm, which is a descendant of TComponent!! */

	TComponent* subComp = (TComponent*)subObj;

	TStringList *tempList = new TStringList;
	String temp;
	try
	{
		PTypeInfo TypeInfo = (PTypeInfo)subComp->ClassInfo();
		PPropList PropList = new TPropList;
		GetPropInfos(TypeInfo, PropList);

		tempList->Sorted = true;
		for (int i=0; i < PropertyCount(subComp); i++)
		{
			String subProp = String(PropList[i]->Name);
			tempList->Add(
				prName + "." + subProp
				+ "=" + GetPropValue(subComp, subProp, true) );
		}

		delete[] PropList;

		temp = tempList->Text;
		// Remove trailing carriage-returns:
		int cr = temp.LastDelimiter("\r");
		if (cr) temp.Delete(cr, 2);
	}
	__finally
	{
		delete tempList;
	}

	return temp;
}

//===========================================================================
//== Published 'information'-functions:
//===========================================================================

int TPropertyHandler::PropertyCount(TComponent* comp)
{
	PTypeInfo TypeInfo = (PTypeInfo)comp->ClassInfo();
	PTypeData TypeData = GetTypeData(TypeInfo);
	return (int)TypeData->PropCount;
}
//---------------------------------------------------------------------------

int TPropertyHandler::PropertyCount()
{
	if (!DefaultComponentExists()) return -1;
	return PropertyCount(FDefaultComponent);
}
//---------------------------------------------------------------------------

bool TPropertyHandler::IsClass(TComponent* comp, String prName)
{
	Typinfo::TTypeKind kind = PropType((TObject*)comp, prName);
	return (kind == tkClass);
}
//---------------------------------------------------------------------------

bool TPropertyHandler::IsClass(String prName)
{
	if (!DefaultComponentExists()) return false;
	return IsClass(FDefaultComponent, prName);
}
//---------------------------------------------------------------------------

bool TPropertyHandler::PropertyExists(TComponent* comp, String prName)
{
	if (prName == "sped_sped_MaxRecentFiles")
		ShowMessage("sped_MaxRecentFiles");
	if (!IsPublishedProp(comp, prName))
	{
//		#ifdef PROPERTYHANDLER_SHOW_WARNINGS
		ShowMessage( "TPropertyHandler warning:\n\n"
				  "Property '" + GetName(comp) + "." + prName + "' does not exist." );
//		#endif
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------

bool TPropertyHandler::PropertyExists(String prName)
{
	if (!DefaultComponentExists()) return false;
	return PropertyExists(FDefaultComponent, prName);
}
//---------------------------------------------------------------------------

//===========================================================================
//== Published Get... -functions:
//===========================================================================

//---------------------------------------------------------------------------

String TPropertyHandler::GetName(TComponent* comp)
{
	return comp->Name;
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetName()
{
	if (!DefaultComponentExists()) return "";
	return GetName(FDefaultComponent);
}
//---------------------------------------------------------------------------

Variant TPropertyHandler::GetValue(TComponent* comp, String prName)
{
	if (!PropertyExists(comp, prName)) return NULL;
	if (IsClass(comp, prName)) return NULL;
	return GetPropValue(comp, prName, true);
	// Parameter true: Strings are preferred returntype (see TypeInfo.pas).
}
//---------------------------------------------------------------------------

Variant TPropertyHandler::GetValue(String prName)
{
	if (!DefaultComponentExists()) return NULL;
	return GetValue(FDefaultComponent, prName);
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetNameValue(TComponent* comp, String prName)
{
	if (!PropertyExists(comp, prName)) return "";
	if (IsClass(comp, prName)) return GetClassNamesValues(comp, prName);
	return BuildNameValue(comp, prName);
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetNameValue(String prName)
{
	if (!DefaultComponentExists()) return "";
	return GetNameValue(FDefaultComponent, prName);
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetNamesValues(TComponent* comp, String prNames)
{
	String list, prop, name1, name2;
	int prop_start = 1, prop_end, sep;
	while (prop_start < prNames.Length())
	{
		// Extract (next) prop from props:
		prop_end = prNames.Pos(";"); // Find end of prop.
		if (!prop_end)
			prop_end = prNames.Length() + 1; // Last prop.
		else
			prNames[prop_end] = ' '; // Remove ';'.
		prop = prNames.SubString(prop_start, prop_end - prop_start);
		prop_start = prop_end + 1;

		// Get the value:
		if (!PropertyExists(comp, prop)) continue;
		if (IsClass(comp, prop))
			list = list + GetClassNamesValues(comp, prop) + "\n";
		else
			list = list + BuildNameValue(comp, prop) + "\n";
	}
	return list;
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetNamesValues(String prNames)
{
	if (!DefaultComponentExists()) return "";
	return GetNamesValues(FDefaultComponent, prNames);
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetAllNamesValues(TComponent* comp)
{
	String list;
	TStringList *tempList = new TStringList;
	try
	{
		tempList->Sorted = true;

		PTypeInfo TypeInfo = (PTypeInfo)comp->ClassInfo();
		PPropList PropList = new TPropList;
		GetPropInfos(TypeInfo, PropList);

		String data;
		for (int i=0; i < PropertyCount(comp); i++)
		{
			String prName = String(PropList[i]->Name);
			if (PropIsType(comp, prName, tkMethod)) continue; // Skip events.
			if (IsClass(comp, prName))
			{	data = GetClassNamesValues(comp, prName); }
			else data = BuildNameValue(comp, prName);
			if (!data.IsEmpty()) tempList->Append( data );
		}

		delete[] PropList;
		list = tempList->Text;
	}
	__finally
	{
		delete tempList;
	}
	return list;
}
//---------------------------------------------------------------------------

String TPropertyHandler::GetAllNamesValues()
{
	if (!DefaultComponentExists()) return "";

	return GetAllNamesValues(FDefaultComponent);
}
//---------------------------------------------------------------------------

void TPropertyHandler::SetValue(TComponent* comp, String prName, Variant prValue)
{
	if ((prValue.Type() == varBoolean) && (prValue == -1)) prValue = 1; // Fixes the bug.

	SetPropValue(comp, prName, prValue);
}
//---------------------------------------------------------------------------

void TPropertyHandler::SetValue(String prName, Variant prValue)
{
	if (!DefaultComponentExists()) return;
	SetValue(FDefaultComponent, prName, prValue);
}
//---------------------------------------------------------------------------

void TPropertyHandler::SetNameValue(TComponent* comp, String prName_Value)
{
	if (prName_Value.LastDelimiter("\r\n"))
	{  // Multi-line string, so call SetNamesValues():
		SetNamesValues(comp, prName_Value);
		return;
	}

	String name2, name1, value; // Reads like this: "name2.name1=value".
	int sep = prName_Value.Pos("=");

	String names = prName_Value.SubString(1, sep - 1);
	value = prName_Value.SubString(sep + 1, prName_Value.Length() - sep + 1);

	sep = names.Pos(".");
	if (sep)
	{	// Format is "propname.subprop=val":
		name1 = names.SubString(sep + 1, names.Length() - sep + 1);
		name2 = names.SubString(1, sep - 1);
		// name2 is the subcomp of comp.
		if (!IsPublishedProp(comp, name2)) // No warning.
		{  /*	name2 is not a component of comp,
				but it might be a component of its own,
				like CheckBox1 is a sub-component of Form1. */
			if ((comp = GetSubComponent(comp, name2)) == NULL) return;
			else
			{
				SetPropValue(comp, name1, value);
				return;
			}
		}
		if (!IsClass(comp, name2)) return;
		// Get pointer to subObj:
		TObject *subObj = (TObject *)GetOrdProp(comp, name2);
		if (subObj == NULL) return;

		SetPropValue(subObj, name1, value);
	}
	else
	{	// Format is "propname=val":
		SetPropValue(comp, names, value);
	}
}
//---------------------------------------------------------------------------

void TPropertyHandler::SetNameValue(String prName_Value)
{
	if (!DefaultComponentExists()) return;
	SetNameValue(FDefaultComponent, prName_Value);
}
//---------------------------------------------------------------------------

void TPropertyHandler::SetNamesValues(TComponent* comp, String prNames_Values)
{
	// Process every line:
	TStringList *list = new TStringList;
	try
	{
		list->Text = prNames_Values;
		int pos = 0;
		while (pos < list->Count)
		{
			SetNameValue(comp, list->Strings[pos]);
			++pos;
		}
	}
	__finally
	{
		delete list;
	}
}
//---------------------------------------------------------------------------

void TPropertyHandler::SetNamesValues(String prNames_Values)
{
	if (!DefaultComponentExists()) return;
	SetNamesValues(FDefaultComponent, prNames_Values);
}
//---------------------------------------------------------------------------

TComponent* TPropertyHandler::GetSubComponent(TComponent* comp, String prName)
{
	for (int i = 0; i < comp->ComponentCount; i++)
	{
		if (comp->Components[i]->Name == prName) return comp->Components[i];
	}
	return NULL;
}
//---------------------------------------------------------------------------

TComponent* TPropertyHandler::GetSubComponent(String prName)
{
	if (!DefaultComponentExists()) return NULL;
	return GetSubComponent(FDefaultComponent, prName);
}
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
