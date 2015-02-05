// DictionaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "lexicon.h"
#include "DictionaryDlg.h"
#include "WordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDictionaryDlg dialog


CDictionaryDlg::CDictionaryDlg( CLexicon & rLexicon, BOOL bSuper, CWnd* pParent ) : 
   CDialog(CDictionaryDlg::IDD, pParent ),
    m_pListBox( NULL ),
	m_Lex( rLexicon ),
	m_bSuper( bSuper )
{
	//{{AFX_DATA_INIT(CDictionaryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BOOL CDictionaryDlg::OnInitDialog()
{
	m_pListBox = ( CListBox * ) GetDlgItem( IDC_WORDLIST );
	ASSERT( m_pListBox != NULL);

	m_pListBox->ResetContent();

	int n = m_Lex.GetNumWords();
	for( int i = 0; i < n; i++ )
	{
		CWord * pWord = m_Lex.GetWord( i );
		CString strDisplay;
		pWord->LoadDisplayText( strDisplay );
		m_pListBox->AddString( strDisplay );
 		m_pListBox->SetItemData( i, pWord->m_lid );
	}

	GetDlgItem( ID_DELETEWORD )->EnableWindow( FALSE );
	GetDlgItem( ID_EDITWORD )->EnableWindow( FALSE );
	GetDlgItem( ID_PHONEBOX )->EnableWindow( FALSE );

	return CDialog::OnInitDialog();
}

void CDictionaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDictionaryDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDictionaryDlg, CDialog)
	//{{AFX_MSG_MAP(CDictionaryDlg)
	ON_LBN_DBLCLK(IDC_WORDLIST, OnDblclkWordlist)
	ON_BN_CLICKED(ID_DELETEWORD, OnDeleteword)
	ON_BN_CLICKED(ID_EDITWORD, OnEditword)
	ON_BN_CLICKED(ID_NEWWORD, OnNewword)
	ON_LBN_SELCHANGE(IDC_WORDLIST, OnSelchangeWordlist)
	ON_BN_CLICKED(ID_LEXBOX, OnLexbox)
	ON_BN_CLICKED(ID_IMPORT, OnImport)
	ON_BN_CLICKED(ID_PHONEBOX, OnPhonebox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDictionaryDlg message handlers

void CDictionaryDlg::OnDblclkWordlist() 
{
	CWord w;
	CWordDlg dlg( "Wortform bearbeiten", w, m_Lex, this );	
	if( IDOK == dlg.DoModal() )
	{
	}
}

void CDictionaryDlg::OnDeleteword() 
{
	int nSel = m_pListBox->GetCurSel();
	long lid = m_pListBox->GetItemData( nSel );
	CWord * pWord = m_Lex.FindWord( lid );
	ASSERT( pWord );	
	CString strMessage( "Möchten Sie die Wortform ''<w>'' löschen?");
	CString strDisplay;
	pWord->LoadDisplayText( strDisplay );
	strMessage.Replace( "<w>", strDisplay );
	if( IDYES == MessageBox( strMessage, "Chorpita Wörterbuch",  MB_YESNO ) )
	{
		m_Lex.DeleteWord( lid );
		m_pListBox->DeleteString( nSel );
		OnSelchangeWordlist();
	}
}

void CDictionaryDlg::OnEditword() 
{
	int nSel = m_pListBox->GetCurSel();
	long lid = m_pListBox->GetItemData( nSel );
	CWord * pWord = m_Lex.FindWord( lid );
	ASSERT( pWord );	
	CWordDlg dlg( "Wortform bearbeiten", * pWord, m_Lex, this );	
	if( IDOK == dlg.DoModal() )
	{
		m_pListBox->DeleteString( nSel );
		CString strDisplay;
		pWord->LoadDisplayText( strDisplay );
		int nPos = m_pListBox->InsertString( nSel, strDisplay ); 
		m_pListBox->SetItemData( nPos, pWord->m_lid );
		OnSelchangeWordlist();
	}
}

void CDictionaryDlg::OnNewword() 
{
	CWord * pWord = new CWord();
	CWordDlg dlg( "Neue Wortform hinzufügen", * pWord, m_Lex, this );
    if( IDOK == dlg.DoModal() )
	{
		m_Lex.AddWord( pWord );
		ASSERT( m_pListBox != NULL);
		CString strDisplay;
		pWord->LoadDisplayText( strDisplay );
		int nPos = m_pListBox->InsertString( -1, strDisplay ); 
		m_pListBox->SetItemData( nPos, pWord->m_lid );
	}
	else
		delete pWord;
}

void CDictionaryDlg::OnSelchangeWordlist() 
{
	int nSel = m_pListBox->GetCurSel();

	// enable/disable buttons
	GetDlgItem( ID_DELETEWORD )->EnableWindow( nSel != -1 );
	GetDlgItem( ID_EDITWORD )->EnableWindow( nSel != -1 );
	GetDlgItem( ID_PHONEBOX )->EnableWindow( nSel != -1 );

	// post data to info controls

	if(  nSel != -1 )
	{	
		CWord * pWord = m_Lex.FindWord( m_pListBox->GetItemData( nSel ) );
		ASSERT( pWord );
		CString strDisplay;
		pWord->LoadDisplayText( strDisplay );
		GetDlgItem( ST_LEXENTRY )->SetWindowText( strDisplay );
		GetDlgItem( ST_WORDBODY )->SetWindowText( pWord->m_strText );
		GetDlgItem( ST_WORDTYPE )->SetWindowText( pWord->GetWordTypeText() );
	}
	else
	{
		GetDlgItem( ST_LEXENTRY )->SetWindowText( "" );
		GetDlgItem( ST_WORDBODY )->SetWindowText( "" );
		GetDlgItem( ST_WORDTYPE )->SetWindowText( "" );
	}
}

void CDictionaryDlg::OnLexbox() 
{

}

void CDictionaryDlg::OnImport() 
{
	MessageBox( "not implemented yet!!" );
}

void CDictionaryDlg::OnPhonebox() 
{
	int nSel = m_pListBox->GetCurSel();
	CWord * pWord = m_Lex.FindWord( m_pListBox->GetItemData( nSel ) );
	ASSERT( pWord );
	CPhoneticDlg( pWord->m_nNumSilben, * pWord ).DoModal();	
}
