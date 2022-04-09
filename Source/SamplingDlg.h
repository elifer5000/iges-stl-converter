// Defines the sampling selection dialog

#pragma once
#include "afxwin.h"


// CSamplingDlg dialog

class CSamplingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSamplingDlg)

public:
	CSamplingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamplingDlg();

// Dialog Data
	enum { IDD = IDD_SAMPLING_DLG };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btMixed;
	CToolTipCtrl* m_pToolTip;		//For tooltip
	virtual BOOL OnInitDialog();
	//Data retrieved from program
	float mSampling;
	int mMethod;
	float mStep;
	
	CEdit m_cSampling;
	CEdit m_cStep;
	float mTrimTol;
	CEdit m_cTrim;
};
