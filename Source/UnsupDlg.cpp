// UnsupDlg.cpp : implementation file
// Defines the unsupported and supported dialog

#include "stdafx.h"
#include "Iges.h"
#include "UnsupDlg.h"


// CUnsupDlg dialog

IMPLEMENT_DYNAMIC(CUnsupDlg, CDialog)

CUnsupDlg::CUnsupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnsupDlg::IDD, pParent)
{
	unsup=TRUE;
}

CUnsupDlg::~CUnsupDlg()
{
}

void CUnsupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UNSUP_LIST, mLBox);
}


BEGIN_MESSAGE_MAP(CUnsupDlg, CDialog)
END_MESSAGE_MAP()


// CUnsupDlg message handlers

BOOL CUnsupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	int i=0;
	while(i<mCstrings.GetCount())
		mLBox.AddString(mCstrings.ElementAt(i++));
	
	if(unsup) SetWindowText("Unsupported entities");
	if(!unsup) SetWindowText("Entities in this model");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
