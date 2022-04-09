// Defines the unsupported and supported dialog
#pragma once
#include "afxwin.h"
#include "afxcoll.h"


// CUnsupDlg dialog

class CUnsupDlg : public CDialog
{
	DECLARE_DYNAMIC(CUnsupDlg)

public:
	CUnsupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnsupDlg();

// Dialog Data
	enum { IDD = IDD_UNSUP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox mLBox;
	virtual BOOL OnInitDialog();
	CStringArray mCstrings;
	BOOL unsup;    //True to present unsupported dialog, false to present supported
};
