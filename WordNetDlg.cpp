// WordNetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "syntax.h"
#include "semantic.h"
#include "poetics.h"
#include "WordNetDlg.h"
#include "wn/wn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWordNetDlg dialog


CWordNetDlg::CWordNetDlg
	( 
	CLexBrain & rLB,
	CUserOptions & rUO,
	CCoreVector & rCOR, 
	CWnd * pParent 
	) : 
	CDialog( CWordNetDlg::IDD, pParent ),
	m_LB( rLB ),
	m_UO( rUO ),
	m_COR( rCOR )
{
	//{{AFX_DATA_INIT(CWordNetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWordNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWordNetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Text( pDX, IDC_ROOTWORD, m_UO.m_strRootWords );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWordNetDlg, CDialog)
	//{{AFX_MSG_MAP(CWordNetDlg)
	ON_BN_CLICKED(ID_GENERATE, OnGenerate)
	ON_BN_CLICKED(ID_SEM_SHOW, OnDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWordNetDlg message handlers

void CWordNetDlg::OnGenerate() 
{
	char sz[ 100 ];
	GetDlgItem( IDC_ROOTWORD )->GetWindowText( sz, 99 );
	m_LB.Build( sz, 1, GetDlgItem( IDC_WORDNETRESULT )  );
	m_LB.DisplayLinks( sz );	
}

void CWordNetDlg::OnDisplay() 
{
	m_LB.DisplayLinks( "" );	
}
