// WordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "lexicon.h"
#include "WordDlg.h"
#include "SilbeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneticDlg dialog 

CPhoneticDlg::CPhoneticDlg( int nNumSilben, CWord & rWord ) : 
	CDialog(CPhoneticDlg::IDD),
    m_nNumSilben( nNumSilben ),
	m_Word( rWord )
{
	//{{AFX_DATA_INIT(CPhoneticDlg)
	//}}AFX_DATA_INIT
}

BOOL CPhoneticDlg::OnInitDialog()
{
	CString str( "[" );
	if( m_nNumSilben != 0 )
		str.Insert( 100, m_Word.GetSilbe( 0 ).m_strPhoneText );
	for( int i = 1; i < m_nNumSilben; i++ )
	{
		str.Insert( 100, "." );
		str.Insert( 100, m_Word.GetSilbe( i ).m_strPhoneText );
	}
	str.Insert( 100, "]" );
	str.Replace( "&", "&&" );
	GetDlgItem( IDC_PHONEDISPLAY )->SetWindowText( str );
	return CDialog::OnInitDialog();
}

void CPhoneticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhoneticDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPhoneticDlg, CDialog)
	//{{AFX_MSG_MAP(CPhoneticDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWordDlg dialog

CWordDlg::CWordDlg( LPCSTR szCaption, CWord & rWord, CLexicon & rLex, CWnd* pParent )
	: CDialog(CWordDlg::IDD, pParent),
	m_strCaption( szCaption ),
	m_Word( rWord ),
	m_Lex( rLex )
{
	//{{AFX_DATA_INIT(CWordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CWordDlg::OnInitDialog()
{
	SetWindowText( m_strCaption );
	EnableSilbenButtons( m_Word.m_nNumSilben );
	EnableWordType( m_Word.m_nWordType );
	return CDialog::OnInitDialog();
}

void CWordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWordDlg)
		DDX_Text( pDX, IDC_WORDTEXT, m_Word.m_strText );
		DDV_MaxChars( pDX, m_Word.m_strText, WORD_MAXCHARS );
		DDX_Text( pDX, IDC_NUMSILBEN, m_Word.m_nNumSilben );
		DDV_MinMaxInt( pDX, m_Word.m_nNumSilben, 1, WORD_MAXSILBEN );
		DDX_CBIndex( pDX, IDC_WORDTYPE, m_Word.m_nWordType );
		DDX_CBIndex( pDX, IDC_GENDER, m_Word.m_nGender);
		DDX_CBIndex( pDX, IDC_TAKESAN, m_Word.m_bTakesAn);
		DDX_Check( pDX, IDC_PLURAL, m_Word.m_bPlural);
		DDX_Check( pDX, IDC_AMBIG, m_Word.m_bAmbig);
		DDX_Check( pDX, IDC_HIDDEN, m_Word.m_bHidden);	
		DDX_Text( pDX, IDC_EXTRATEXT, m_Word.m_strExtra );
		DDV_MaxChars( pDX, m_Word.m_strExtra, 8 );	
		DDX_Text( pDX, IDC_WORDCODE, m_Word.m_lid );
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWordDlg, CDialog)
	//{{AFX_MSG_MAP(CWordDlg)
	ON_CBN_SELCHANGE(IDC_WORDTYPE, OnChangeWordType)
	ON_EN_UPDATE(IDC_NUMSILBEN, OnChangeNumSilben)
	ON_BN_CLICKED(ID_SILBE1, OnSilbe1)
	ON_BN_CLICKED(ID_SILBE2, OnSilbe2)
	ON_BN_CLICKED(ID_SILBE3, OnSilbe3)
	ON_BN_CLICKED(ID_SILBE4, OnSilbe4)
	ON_BN_CLICKED(ID_SILBE5, OnSilbe5)
	ON_BN_CLICKED(ID_PHONEBOX, OnPhoneBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWordDlg::OnSilbeX( int nSilbe, LPCSTR szSilbeNum )
{
	CString strCaption( "<n>. Silbe der Wortform ''<w>'' bearbeiten" );
	strCaption.Replace( "<n>", szSilbeNum );
	char sz[ WORD_MAXCHARS ];
	GetDlgItem( IDC_WORDTEXT )->GetWindowText( sz, WORD_MAXCHARS );
	CString strWord( sz );
	strCaption.Replace( "<w>", strWord );
	CSilbeDlg dlg( strCaption, m_Word, nSilbe, m_Lex.GetDefaultFilter(), this );	
	dlg.DoModal();
}

void CWordDlg::EnableSilbenButtons( int n )
{
	GetDlgItem( ID_SILBE1 )->EnableWindow( n >= 1 );
	GetDlgItem( ID_SILBE2 )->EnableWindow( n >= 2 );
	GetDlgItem( ID_SILBE3 )->EnableWindow( n >= 3 );
	GetDlgItem( ID_SILBE4 )->EnableWindow( n >= 4 );
	GetDlgItem( ID_SILBE5 )->EnableWindow( n >= 5 );
}

void CWordDlg::EnableWordType( int nType )
{
	GetDlgItem( IDC_TAKESAN )->EnableWindow( WORDTYPE_NOU == nType );
	BOOL bGender = ( WORDTYPE_NOU == nType ) || ( WORDTYPE_NAM == nType );
	GetDlgItem( IDC_GENDER )->EnableWindow( bGender );
	GetDlgItem( ST_GENDER )->EnableWindow( bGender );
	BOOL bPlural = bGender || ( WORDTYPE_VRB == nType );
	GetDlgItem( IDC_PLURAL )->EnableWindow( bPlural );
}

/////////////////////////////////////////////////////////////////////////////
// CWordDlg message handlers

void CWordDlg::OnSilbe1()  { OnSilbeX( 0, "1" ); }

void CWordDlg::OnSilbe2()  { OnSilbeX( 1, "2" ); }

void CWordDlg::OnSilbe3()  { OnSilbeX( 2, "3" ); }

void CWordDlg::OnSilbe4()  { OnSilbeX( 3, "4" ); }

void CWordDlg::OnSilbe5()  { OnSilbeX( 4, "5" ); }

void CWordDlg::OnChangeNumSilben() { EnableSilbenButtons( GetDlgItemInt( IDC_NUMSILBEN ) ); }

void CWordDlg::OnPhoneBox() { CPhoneticDlg( GetDlgItemInt( IDC_NUMSILBEN ), m_Word ).DoModal(); }

void CWordDlg::OnChangeWordType()
{
	EnableWordType( GetDlgItem( IDC_WORDTYPE )->SendMessage( CB_GETCURSEL ) );
}
