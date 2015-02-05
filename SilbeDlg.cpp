// SilbeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "lexicon.h"
#include "SilbeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSilbeDlg dialog

CSilbePhoneDlg::CSilbePhoneDlg
     ( 
	 LPCSTR szCaption, 
	 CSilbe & rSilbe,
	 int nDefaultFilter, 
	 CWnd* pParent 
	 ) : 
    CDialog(CSilbePhoneDlg::IDD, pParent ),
	m_strCaption( szCaption ),
	m_Silbe( rSilbe ),
	m_nFilter( nDefaultFilter )
{
	//{{AFX_DATA_INIT(CSilbeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CSilbePhoneDlg::OnInitDialog()
{
	SetWindowText( m_strCaption );
	return CDialog::OnInitDialog();
}

void CSilbePhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSilbePhoneDlg)
	DDX_Text( pDX, IDC_PHONETEXT, m_Silbe.m_strPhoneText);
	DDX_CBIndex( pDX, IDC_SILBEFILTER, m_nFilter);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSilbePhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CSilbeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSilbeDlg dialog


CSilbeDlg::CSilbeDlg
     ( 
	 LPCSTR szCaption, 
	 CWord & rWord,
	 int nSilbe,
	 int nDefaultFilter, 
	 CWnd * pParent 
	 ) : 
    CDialog(CSilbeDlg::IDD, pParent ),
	m_strCaption( szCaption ),
	m_Word( rWord ),
	m_nSilbe( nSilbe ),
	m_nFilter( nDefaultFilter )
{
	//{{AFX_DATA_INIT(CSilbeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CSilbeDlg::OnInitDialog()
{
	SetWindowText( m_strCaption );
	if( m_nSilbe )
	{	
	GetDlgItem( IDC_BREAKGROUP )->EnableWindow( TRUE );
	SilbeBreakCode();
	}
	return CDialog::OnInitDialog();
}

void CSilbeDlg::SilbeBreakCode()
{
	CString strBreak( m_Word.m_strText );	
	BYTE b = m_Word.GetSilbe( m_nSilbe ).m_bWordBreak;	
	GetDlgItem( IDC_BREAKPLUS )->EnableWindow(  b < strBreak.GetLength() - 1 );
	GetDlgItem( IDC_BREAKMINUS )->EnableWindow( b > 0 );
	if( b > 0 )
	    strBreak.Insert( b, " | " );
	GetDlgItem( IDC_BREAKDISPLAY )->SetWindowText( strBreak );
}

void CSilbeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSilbeDlg)
	DDX_Check( pDX, IDC_TONE, m_Word.GetSilbe( m_nSilbe ).m_bTone);
	DDX_Check( pDX, IDC_FLEXIBLE, m_Word.GetSilbe( m_nSilbe ).m_bFlexible);	
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSilbeDlg, CDialog)
	//{{AFX_MSG_MAP(CSilbeDlg)
		ON_BN_CLICKED(ID_SILBE_PHONE, OnSilbePhone)
		ON_BN_CLICKED(IDC_BREAKPLUS, OnBreakPlus)		
		ON_BN_CLICKED(IDC_BREAKMINUS, OnBreakMinus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSilbeDlg::OnSilbePhone()
{
	CString strCaption( "Phonetische Darstellung der <c>" );
	strCaption.Replace(  "<c>", m_strCaption );
	CSilbePhoneDlg dlg( strCaption, m_Word.GetSilbe( m_nSilbe ), m_nFilter, this );	
	dlg.DoModal();
}

void CSilbeDlg::OnBreakPlus()
{
	m_Word.GetSilbe( m_nSilbe ).m_bWordBreak++;
	SilbeBreakCode();
}

void CSilbeDlg::OnBreakMinus()
{
	m_Word.GetSilbe( m_nSilbe ).m_bWordBreak--;
	SilbeBreakCode();
}

/////////////////////////////////////////////////////////////////////////////
// CSilbeDlg message handlers
