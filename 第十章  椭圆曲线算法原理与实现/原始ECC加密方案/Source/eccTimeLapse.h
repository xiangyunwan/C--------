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
#ifndef TEccTimeLapseH
#define TEccTimeLapseH
//---------------------------------------------------------------------------

//===========================================================================
namespace ecc {
//===========================================================================

/*	Example:

	TTimeLapse tl;
	tl.Start();
	CallLengthyFunction();
	tl.Pause();
	ShowMessage( "Elapsed time:" + tl.AsString() );
	tl.Resume();
	CallAnotherLengthyFunction();
	tl.Stop();
	ShowMessage( "Operation took " + tl.AsString() );
*/

/*	Note: For specialized datetime strings use FormatDateTime(),
	documented in the Borland C++Builder Help files.
*/

//---------------------------------------------------------------------------
class TTimeLapse {

private:
	TDateTime FdtStart;
	TDateTime FdtEnd;
	TDateTime FdtElapsed;
	TDateTime FdtPausedAt;
	bool FRunning;

	// Helper props:
	unsigned short FHour;
	unsigned short FMin;
	unsigned short FSec;
	unsigned short FMSec;

public:
	TTimeLapse();

	void Start();
	void Stop();
	void Pause();
	void Resume();
	TDateTime Elapsed();

	bool IsRunning()
	{	return FRunning; }

	String AsString(const bool ms = true, TDateTime dt = 0);
	/* 	Returns [dt] as a string like "4 Hours, 5 Minutes, 6.789 Seconds".
		If [ms] is false, no millisecond information will be returned.
		If [dt] is omitted (or 0), the output of Elapsed()) will be used as [dt].
		No hour information will be returned if hours==0. */

	String AsStr(TDateTime dt = 0);
	/* 	Returns [dt] as a string like "[H:[M]]M:SS", where H is hours, M is minutes and S is Seconds.
		If [dt] is omitted (or 0), the output of Elapsed()) will be used as [dt].
		No hour information will be returned if hours==0.
		If minutes<10, minute information will be returned as '0', ie: "0:59". */

};
//---------------------------------------------------------------------------

//===========================================================================
} // namespace ecc;
//===========================================================================
#endif
