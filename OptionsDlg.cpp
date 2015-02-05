#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "poetics.h"
#include "OptionsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog used for App About

class CLexOptionsDlg : public CDialog
{
public:
	CLexOptionsDlg( CUserOptions * pOptions );

// Dialog Data
	//{{AFX_DATA(CLexOptionsDlg)
	enum { IDD = IDD_WORMOPTIONS_DIALOG};
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLexOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CUserOptions * m_pOptions;

	//{{AFX_MSG(CLexOptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePoem();
	afx_msg void OnChangeStanza();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CLexOptionsDlg::CLexOptionsDlg( CUserOptions * pOptions ) : 
	CDialog(CLexOptionsDlg::IDD), 
	m_pOptions( pOptions )
{
	//{{AFX_DATA_INIT(CLexOptionsDlg)
	//}}AFX_DATA_INIT
}

BOOL CLexOptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	BOOL b = ( m_pOptions->m_nPoem == POEM_FIXED );
	GetDlgItem( IDC_NUMSTANZAS )->EnableWindow( b );
	GetDlgItem( IDC_ST_STANZAS )->EnableWindow( b );
	b =	( m_pOptions->m_cbStanza == STANZA_FIXEDVERSE );
	GetDlgItem( IDC_NUMVERSE )->EnableWindow( b );
	GetDlgItem( IDC_ST_VERSES )->EnableWindow( b );		
	return TRUE;
}

void CLexOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLexOptionsDlg)
	DDX_Check( pDX, IDC_HASTITLE, m_pOptions->m_bHasTitle);	
	DDX_Text( pDX, IDC_NUMSTANZAS, m_pOptions->m_nStanzas);
	DDV_MinMaxInt( pDX, m_pOptions->m_nStanzas, 1,  4 );	
	DDX_Text( pDX, IDC_NUMVERSE, m_pOptions->m_nVerses);
    DDV_MinMaxInt( pDX, m_pOptions->m_nVerses, 2,  8 );	
	DDX_CBIndex( pDX, IDC_POEMFORM, m_pOptions->m_nPoem);
	DDX_CBIndex( pDX, IDC_STANZALENGTH, m_pOptions->m_cbStanza);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLexOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CLexOptionsDlg)
	ON_CBN_SELCHANGE(IDC_POEMFORM, OnChangePoem)
	ON_CBN_SELCHANGE(IDC_STANZALENGTH, OnChangeStanza)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLexOptionsDlg::OnChangePoem() 
{
	int index = GetDlgItem( IDC_POEMFORM )->SendMessage( CB_GETCURSEL );
	BOOL b =  ( index  == POEM_FIXED );
	GetDlgItem( IDC_NUMSTANZAS )->EnableWindow( b );
	GetDlgItem( IDC_ST_STANZAS )->EnableWindow( b );	
}

void CLexOptionsDlg::OnChangeStanza()
{
	int index = GetDlgItem( IDC_STANZALENGTH )->SendMessage( CB_GETCURSEL );
	 BOOL b = ( index == POEM_FIXED );
	GetDlgItem( IDC_NUMVERSE )->EnableWindow( b );
	GetDlgItem( IDC_ST_VERSES )->EnableWindow( b );
}

COptionsDlg::COptionsDlg( CUserOptions * pOptions ) : 
	CDialog(COptionsDlg::IDD), 
	m_pOptions( pOptions )
{
	//{{AFX_DATA_INIT(COptionsDlg)
	//}}AFX_DATA_INIT
}

BOOL COptionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem( ID_COMPILEROPTS )->EnableWindow( m_pOptions->m_nCompiler == CMP_WORM );
	return TRUE;
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_CBIndex( pDX, IDC_COMPILER, m_pOptions->m_nCompiler);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(ID_COMPILEROPTS, OnCompileropts)
	ON_CBN_SELCHANGE(IDC_COMPILER, OnSelchangeCompiler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void COptionsDlg::OnCompileropts() 
{
	CLexOptionsDlg dlg( m_pOptions );
	dlg.DoModal();	
}

void COptionsDlg::OnSelchangeCompiler() 
{	
	int index = GetDlgItem( IDC_COMPILER )->SendMessage( CB_GETCURSEL );
	GetDlgItem( ID_COMPILEROPTS )->EnableWindow( index == CMP_WORM );	
}
