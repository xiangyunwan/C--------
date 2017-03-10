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
#ifndef eccPropertyHandlerH
#define eccPropertyHandlerH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <typinfo.hpp>
/*	typinfo.hpp can be found in $(BCB)\Include\Vcl.
	Also see <typinfo.pas> in $(BCB)\Source\Vcl. */
//---------------------------------------------------------------------------

#ifdef _DEBUG
	#define PROPERTYHANDLER_SHOW_WARNINGS
#endif

#ifdef PROPERTYHANDLER_SHOW_WARNINGS
	#include <Dialogs.hpp> // Necessary for ShowMessage().
#endif
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

class TPropertyHandler {

private:

	TComponent* FDefaultComponent;
	/* 	Default Component for easier function-calls. */

	bool DefaultComponentExists();
	/*	Shows a message if FDefaultComponent is not initialized.
		Use this to debug your program.
		To turn the warning off #define PROPERTYHANDLER_SHOW_WARNINGS. */

	String BuildNameValue(TComponent* comp, String prName);
	/*	Returns string: "PropertyName=Value".
		!! Caution: Assumes all passed parameters are valid!
		This function only exists to present clear code in the other functions. */

	String GetClassNamesValues(TComponent* comp, String prName);
	/*	Returns "PropertyName.SubPropertyName=Value" strings
		for property prName of Component comp as a String.
		!! Caution: Assumes prName is an existing property that is a class!
		This function is used by other functions such as GetNameValue(). */

public:

	// == Functions for any Component ==

	TPropertyHandler(); // Any component constructor.
	/*	Constructor that does not set a default TComponent. */

	int PropertyCount(TComponent* comp);
	/*	Returns the number of properties of comp, including the events. */

	bool PropertyExists(TComponent* comp, String prName);
	/*	Checks if property prName exists for TComponent comp. */

	bool IsClass(TComponent* comp, String prName);
	/*	Returns true if prName is a class (like the property Font). */

	String GetName(TComponent* comp);
	/*	Returns value of Name property for any TComponent (comp) as String. */

	Variant GetValue(TComponent* comp, String prName);
	/*	Returns property value for any TComponent (comp) as Variant.
		If property does not exist the return value is NULL.
		If property is a class the return value is NULL. */

	String GetNameValue(TComponent* comp, String prName);
	/*	Returns "PropertyName=Value" for property prName of comp.
		If property does not exist the return value is "".
		If property is a class (ie. TFont) the return value is a string which
		contains the returnstring from GetClassNamesValues(). */

	String GetNamesValues(TComponent* comp, String prNames);
	/*	Returns "PropertyName.Property=Value" strings for all passed prNames
		(including subprops) of comp as a String.
		Note that multiple props should be seperated by ';' and are all in one string.
		The Name of the comp is not a part of the result. (ie. Not f_Main.Left=xx but Left=xx).
		-Example:
		{
			TPropertyHandler ph;
			Memo1->Lines->Text = ph.GetNamesValues(CheckBox1, "Name;Checked;Cursor" );
		} */

	String GetAllNamesValues(TComponent* comp);
	/*	Returns "PropertyName.Property=Value" string for all properties
		(including subprops) of comp, this list is sorted.
		Events are not returned. */

	void SetValue(TComponent* comp, String prName, Variant prValue);
	/*	There seems to be a bug(?) in the SetPropValue function of TypInfo.pas
		when using the Delphi class Variant.
		-Example:
		{
			TPropertyHandler ph(CheckBox1);
			ph.SetValue("Checked", 1);			// > Ok.
			ph.SetValue("Checked", "true");  // > Ok.
			ph.SetValue("Checked", true);    // >> WRONG!
		}
		The last line does not work as expected because true is passed as value -1,
		instead of +1, and SetPropValue handles the negative value incorrect. */

	void SetNameValue(TComponent* comp, String prName_Value);
	/*	prName_Value is assumed to be formatted like:
			"propname.subprop=value" or "propname=value".
		Make sure this is correct, else this func will not work as expected.
		If prName_Value is a multi-line string, SetNamesValues() will be called.
		Returns false if an error occured. */

	void SetNamesValues(TComponent* comp, String prNames_Values);
	/*	prNames_Values is assumed to be a collection of lines
		seperated by '\n' (ie. Text property of TStringList).
		Every line should be formatted like:
			"propname.subprop=value" or "propname=value".
		It calls SetNameValue() for every line in prNames_Values. */

	TComponent* GetSubComponent(TComponent* comp, String prName);
	/*	Returns a TComponent* for a child component of comp with name prName.
		If there is no child-component return value is NULL. */


	// === Functions using FDefaultComponent: ===

	
	TPropertyHandler(TComponent* comp); // Default component constructor.
	/*	Constructor that sets a default TComponent for easier function-calls. */

	int PropertyCount();
	/*	Same as other PropertyCount() function, for use with the default component. */

	bool PropertyExists(String prName);
	/*	Same as other PropertyExists() function, for use with the default component. */

	bool IsClass(String prName);
	/*	Same as other IsClass() function, for use with the default component. */

	String GetName();
	/*	Same as other GetName() function, for use with the default component. */

	Variant GetValue(String prName);
	/*	Same as other GetValue() function, for use with the default component. */

	String GetNameValue(String prName);
	/*	Same as other GetNameValue() function, for use with the default component. */

	String GetNamesValues(String prNames);
	/*	Same as other GetNamesValues() function, for use with the default component. */

	String GetAllNamesValues();
	/*	Same as other GetAllNamesValues() function, for use with the default component. */

	void SetValue(String prName, Variant prValue);
	void SetNameValue(String prName_Value);
	void SetNamesValues(String prNames_Values);

	TComponent* GetSubComponent(String prName);
};

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
