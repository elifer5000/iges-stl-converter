// SamplingDlg.cpp : implementation file
// Defines the sampling selection dialog

#include "stdafx.h"
#include "Iges.h"
#include "SamplingDlg.h"


// CSamplingDlg dialog

IMPLEMENT_DYNAMIC(CSamplingDlg, CDialog)

CSamplingDlg::CSamplingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSamplingDlg::IDD, pParent)
	, mSampling(0)
	, mMethod(0)
	, mStep(0)
	, mTrimTol(0)
{
	m_pToolTip = NULL;
}

CSamplingDlg::~CSamplingDlg()
{
	delete m_pToolTip;
}

void CSamplingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSAMPLING, mSampling);
	DDX_Radio(pDX, IDC_MIXED, mMethod);
	DDX_Text(pDX, IDC_EDITSTEP, mStep);
	DDX_Text(pDX, IDC_EDITTRIMMING, mTrimTol);
	DDX_Control(pDX, IDC_MIXED, m_btMixed);
	DDX_Control(pDX, IDC_EDITSAMPLING, m_cSampling);
	DDX_Control(pDX, IDC_EDITSTEP, m_cStep);
	DDX_Control(pDX, IDC_EDITTRIMMING, m_cTrim);
}


BEGIN_MESSAGE_MAP(CSamplingDlg, CDialog)
END_MESSAGE_MAP()


// CSamplingDlg message handlers


BOOL CSamplingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
            m_pToolTip->RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSamplingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Set up the tooltip
	m_pToolTip = new CToolTipCtrl;
	
	if(!m_pToolTip->Create(this))
	{
	   TRACE("Unable To create ToolTip\n");
	   return TRUE;
	}

	if (!m_pToolTip->AddTool(&m_btMixed, "Uses parametric error for surfaces with degree 1,\nand path length for higher degree surfaces."))
	{
	   TRACE("Unable to add mixed button to the tooltip\n");
	}
	
	if (!m_pToolTip->AddTool(&m_cSampling, "Tolerance to be used in sampling surfaces.\nAffects Mixed, Parametric Error and Path Length methods.\nLower numbers give higher quality."))
	{
	   TRACE("Unable to add mixed button to the tooltip\n");
	}
	
	if (!m_pToolTip->AddTool(&m_cStep, "Step of each division in Domain Distance method.\nHigher numbers give higher quality."))
	{
	   TRACE("Unable to add mixed button to the tooltip\n");
	}
	if (!m_pToolTip->AddTool(&m_cTrim, "Tolerance to be used when fixing trimming lines problems.\nIf you get errors when opening a file try raising this number."))
	{
	   TRACE("Unable to add mixed button to the tooltip\n");
	}
	
	m_pToolTip->Activate(TRUE);
	m_pToolTip->SetMaxTipWidth(500);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
