// Iges.h : main header file for the Iges application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIgesApp:
// See Iges.cpp for the implementation of this class
//

class CIgesApp : public CWinApp
{
public:
	CIgesApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIgesApp theApp;