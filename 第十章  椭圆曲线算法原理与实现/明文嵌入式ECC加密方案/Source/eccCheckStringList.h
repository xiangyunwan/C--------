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
#ifndef eccCheckStringListH
#define eccCheckStringListH
//---------------------------------------------------------------------------

/*	Provides a simple struct (TCheckStringListItem) that can be used to
	create an STL vector container class that handles a list of this structs.

	Usage:
	{
		#include "eccCheckStringList.h"
		std::vector<ecc::TCheckStringListItem> MyCheckStringList;
	}
*/
//---------------------------------------------------------------------------

#include <vector>
#include <system.hpp>	// AnsiString

//===========================================================================
namespace ecc {
//===========================================================================

//---------------------------------------------------------------------------

struct TCheckStringListItem {
	bool checked;
	AnsiString str;
};
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
