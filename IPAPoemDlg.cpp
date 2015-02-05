// IPAPoemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "IPAPoemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CIPAPoemChunker : public CSimpleChunker
{
	virtual BOOL IsValidChar( char c )
	{
		return ( ( c >= 'A' ) && ( c <= 'Z' ) ) || ( c == '-' ) || ( c == '\'' );
	}
};

/////////////////////////////////////////////////////////////////////////////
// CIPAPoemDlg dialog

CIPAPoemDlg::CIPAPoemDlg( CString & strPoem, CIPAManager & rIPA, CWnd* pParent /*=NULL*/) : 
	CDialog(CIPAPoemDlg::IDD, pParent),
	m_strPoem( strPoem ),
	m_IPA( rIPA )
{
	//{{AFX_DATA_INIT(CIPAPoemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BOOL CIPAPoemDlg::OnInitDialog()
{
	CString strLocal( m_strPoem );
	strLocal.MakeUpper();
	strLocal.Remove( '.' );
	strLocal.Remove( '?' );
	strLocal.Remove( '!' );
	strLocal.Remove( ',' );
	CIPAPoemChunker chunk;
	chunk.Chunk( strLocal );
	int nPos = 0;
	for( int i = 0; i < chunk.GetSize(); i++ )
	{
		CString & str = chunk.GetAt( i );
		nPos = strLocal.Find( str, nPos );
		strLocal.Delete( nPos, str.GetLength() );
		CIPAElement * pIPA = m_IPA.GetData().SortedFind( str );
		if( pIPA )
		{
			strLocal.Insert( nPos, pIPA->m_strTrans );
			nPos += ( pIPA->m_strTrans.GetLength() + 1 );
		}
		else
			strLocal.Insert( nPos, "*****" );
	}
	strLocal.Replace( "\n", "\r\n" );
	strLocal.Remove( '0' );
	strLocal.Remove( '1' );
	strLocal.Remove( '2' );
	GetDlgItem( IDC_IPAPOEM )->SetWindowText( strLocal );
	return CDialog::OnInitDialog();
}

void CIPAPoemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPAPoemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIPAPoemDlg, CDialog)
	//{{AFX_MSG_MAP(CIPAPoemDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPAPoemDlg message handlers
