// poesie.h : main header file for the POESIE application
//

#if !defined(AFX_POESIE_H__1CA5D184_C73A_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_POESIE_H__1CA5D184_C73A_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPoesieApp:
// See poesie.cpp for the implementation of this class
//

class CPoesieApp : public CWinApp
{
public:
	CPoesieApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPoesieApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPoesieApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POESIE_H__1CA5D184_C73A_11D9_B4C8_444553540000__INCLUDED_)
