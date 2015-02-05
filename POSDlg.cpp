// POSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "POSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPOSDlg dialog


CPOSDlg::CPOSDlg
	(
	BOOL bWordless,
	CString & strRet,
	CString & strWord,
	CString & strSeg,
	CStringArray * psaPlugs,
	CWnd* pParent /*=NULL*/
	) : 
	CDialog(CPOSDlg::IDD, pParent),
	m_bWordless( bWordless ),
	m_strRet( strRet ),
	m_strWord( strWord ),
	m_strSeg( strSeg ),
	m_psaPlugs( psaPlugs )
{
	//{{AFX_DATA_INIT(CPOSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CPOSDlg::OnInitDialog()
{
	if( m_bWordless )
	{
		GetDlgItem( IDC_SYN_N )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_NP )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_V )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_VS )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_VG )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_VA )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_VP  )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_A )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_AC )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_AS )->EnableWindow( FALSE );
		GetDlgItem( IDC_SYN_R )->EnableWindow( FALSE );	
	}
	else
	{	
		CString str( m_strWord );
		str.MakeUpper();
		int nFind = m_strSeg.Find( m_strWord );
		m_strSeg.Delete( nFind, m_strWord.GetLength() );
		m_strSeg.Insert( nFind, str );
		int nSize = m_psaPlugs->GetSize();
		for( int i = 0; i < nSize; i++ )
		{
			CString str( m_psaPlugs->GetAt( i ) );
			str = str.Mid( str.Find( "\"" ) + 1 );
			str = str.Left( str.Find( "\"" ) );
			CString strPlug( "<#>" );
			char cLetter = char( BYTE( 'A' ) + i  );
			strPlug.Replace( '#', cLetter );
			m_strSeg.Replace( strPlug, str );
		}
	}
	
	GetDlgItem( IDC_POS_WORD )->SetWindowText( m_strWord );
	GetDlgItem( IDC_POS_SYNTAG )->SetWindowText( m_strSeg );
	
	return CDialog::OnInitDialog();
}

void CPOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPOSDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPOSDlg, CDialog)
	//{{AFX_MSG_MAP(CPOSDlg)
	ON_BN_CLICKED(IDC_SYN_N, OnSynN)
	ON_BN_CLICKED(IDC_SYN_NP, OnSynNp)
	ON_BN_CLICKED(IDC_SYN_V, OnSynV)
	ON_BN_CLICKED(IDC_SYN_VS, OnSynVs)
	ON_BN_CLICKED(IDC_SYN_VG, OnSynVg)
	ON_BN_CLICKED(IDC_SYN_VA, OnSynVa)
	ON_BN_CLICKED(IDC_SYN_VP, OnSynVp)
	ON_BN_CLICKED(IDC_SYN_A, OnSynA)
	ON_BN_CLICKED(IDC_SYN_AC, OnSynAc)
	ON_BN_CLICKED(IDC_SYN_AS, OnSynAs)
	ON_BN_CLICKED(IDC_SYN_R, OnSynR)
	ON_BN_CLICKED(IDIGNORE, OnIgnore)
	ON_BN_CLICKED(IDNO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPOSDlg message handlers

void CPOSDlg::OnSynN() 
{
	m_strRet = "n";
	EndDialog( 0 );
}

void CPOSDlg::OnSynNp() 
{
	m_strRet = "np";
	EndDialog( 0 );	
}


void CPOSDlg::OnSynV() 
{
	m_strRet = "v";
	EndDialog( 0 );		
}

void CPOSDlg::OnSynVs() 
{
	m_strRet = "vs";
	EndDialog( 0 );			
}

void CPOSDlg::OnSynVg() 
{
	m_strRet = "vg";
	EndDialog( 0 );			
}


void CPOSDlg::OnSynVa() 
{
	m_strRet = "va";
	EndDialog( 0 );		
}


void CPOSDlg::OnSynVp() 
{
	m_strRet = "vp";
	EndDialog( 0 );			
}

void CPOSDlg::OnSynA() 
{
	m_strRet = "a";
	EndDialog( 0 );			
}

void CPOSDlg::OnSynAc() 
{
	m_strRet = "ac";
	EndDialog( 0 );		
}

void CPOSDlg::OnSynAs() 
{
	m_strRet = "as";
	EndDialog( 0 );			
}


void CPOSDlg::OnSynR() 
{
	m_strRet = "r";
	EndDialog( 0 );			
}

void CPOSDlg::OnIgnore() 
{
	EndDialog( IDIGNORE );	
}

void CPOSDlg::OnNo() 
{
	EndDialog( IDNO );
}

// POSDLG.CPP

