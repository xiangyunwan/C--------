// AesCode.h : main header file for the AESCODE application
//

#if !defined(AFX_AESCODE_H__E3DA199C_BD02_4442_9676_7E0A15A7CC55__INCLUDED_)
#define AFX_AESCODE_H__E3DA199C_BD02_4442_9676_7E0A15A7CC55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAesCodeApp:
// See AesCode.cpp for the implementation of this class
//

class CAesCodeApp : public CWinApp
{
public:
	CAesCodeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAesCodeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAesCodeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AESCODE_H__E3DA199C_BD02_4442_9676_7E0A15A7CC55__INCLUDED_)
