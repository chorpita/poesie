// IPADialog.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "IPADialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CIPADisplayDlg : public CDialog
{
public:
	CIPADisplayDlg( CString & szForm, CString & szTrans );

// Dialog Data
	//{{AFX_DATA(CIPADisplayDlg)
	enum { IDD = IDD_PHONEBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPADisplayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString & m_strForm;
	CString & m_strTrans;

	//{{AFX_MSG(CIPADisplayDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CIPADisplayDlg::CIPADisplayDlg( CString & rstrForm, CString & rstrTrans ) : 
	CDialog(CIPADisplayDlg::IDD),
	m_strForm( rstrForm ),
    m_strTrans( rstrTrans )
{
	//{{AFX_DATA_INIT(CIPADisplayDlg)
	//}}AFX_DATA_INIT
}

BOOL CIPADisplayDlg::OnInitDialog()
{
	CString str( "[" );
	str += m_strTrans;
	str.Insert( 100, "]" );
	str.Replace( "&", "&&" );
	CString strTitle( "IPA-Transkription der Wortform ''<w>''" );
	strTitle.Replace( "<w>", m_strForm );
	SetWindowText( strTitle );
	GetDlgItem( IDC_PHONEDISPLAY )->SetWindowText( str );
	return CDialog::OnInitDialog();
}

void CIPADisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPADisplayDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIPADisplayDlg, CDialog)
	//{{AFX_MSG_MAP(CIPADisplayDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPADialog dialog


CIPADialog::CIPADialog( CIPAManager & rIPA, CWnd* pParent /*=NULL*/)	: 
	CDialog(CIPADialog::IDD, pParent),
    m_IPA( rIPA )
{
	//{{AFX_DATA_INIT(CIPADialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BOOL CIPADialog::OnInitDialog()
{
	GetDlgItem( ID_IPATRANS )->EnableWindow( FALSE );
	return CDialog::OnInitDialog();
}

void CIPADialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPADialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIPADialog, CDialog)
	//{{AFX_MSG_MAP(CIPADialog)
	ON_BN_CLICKED(ID_IPATRANS, OnIpatrans)
	ON_EN_CHANGE(IDC_WORDTEXT, OnChangeWordtext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPADialog message handlers

void CIPADialog::OnIpatrans() 
{
	CString str;
	GetDlgItemText( IDC_WORDTEXT, str );
	str.MakeUpper();
	CIPAElement * pT = m_IPA.GetData().SortedFind( str  );
	if( pT )
	{
		CString strTrans( pT->m_strTrans );
		strTrans.Remove( '0' );
		strTrans.Remove( '1' );
		strTrans.Remove( '2' );
		CIPADisplayDlg dlg( str, strTrans );
		dlg.DoModal();
	}
	else
	{
		CString strMessage( "die Wortform ''<w>'' befindet sich nicht in der IPA-Datenbank.");
		strMessage.Replace( "<w>", str );
		MessageBox( strMessage, "Wortform nicht gefunden", MB_OK | MB_ICONASTERISK );
	}
}

void CIPADialog::OnChangeWordtext() 
{
	CString str;
	GetDlgItemText( IDC_WORDTEXT, str );
	GetDlgItem( ID_IPATRANS )->EnableWindow( str.GetLength() );
}
