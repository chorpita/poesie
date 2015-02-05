// SyntaxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "syntax.h"
#include "SyntaxDlg.h"
#include "POSDlg.h"
#include "wn/wn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSyntaxDlg dialog


CSyntaxDlg::CSyntaxDlg
	( 
	CSyntaxFactory & rSF, 
	CCoreVector & rCOR,
	CWnd* pParent /*=NULL*/
	) : 
	CDialog(CSyntaxDlg::IDD, pParent ),
	m_nSyntagmaIndex(  rSF.m_Text.size()  ),
	m_bTitles( FALSE ),
	m_SF( rSF ),
	m_COR( rCOR )
{
	//{{AFX_DATA_INIT(CSyntaxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSyntaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyntaxDlg)
	DDX_Text( pDX, IDC_SYNTAGMA_INDEX, m_nSyntagmaIndex);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSyntaxDlg, CDialog)
	//{{AFX_MSG_MAP(CSyntaxDlg)
	ON_BN_CLICKED(ID_GENERATE, OnGenerate)
	ON_BN_CLICKED(IDC_SYNTAGMA_CHG, OnChange)	
	ON_EN_CHANGE(IDC_SYNTAGMA_INDEX, OnIndexChange)
	ON_BN_CLICKED(ID_FILEFIND, OnFilefind)
	ON_BN_CLICKED(IDC_SYN_TITLES, OnSynTitles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL IsValidSegmentChar( char c )
{
   return ( ( c >= 'A' ) && ( c <= 'Z' ) ) || ( ( c >= 'a' ) && ( c <= 'z' ) ) || (c == '\'' ) || (c == ' ');
}

BOOL IsSegmentDone( char c )
{
	return (c == '.') || (c == ',') || (c == '?') || (c == '!');
}

int CSyntaxDlg::Parameterize( CString & str, int nMinLength )
{
	BOOL bNone = TRUE;

	int nLen = str.GetLength();
	if( ( nLen > 120 ) || ( nLen < nMinLength ) ) // too long, too short
		return IDNO;

	if( ! m_bTitles && ( str.FindOneOf( ".!?" ) < nLen - 1  ) ) // intermediate punctuation
		return IDNO;

	CString strWork( str );
	while( strWork.Replace( "  ", " " ) );
	strWork.MakeLower();
	CString strVanish( strWork );

	if( m_bTitles )
		strVanish += '.';  // add for parsing

	CStringArray saPlugs;

	BOOL bSub = TRUE;
	while( bSub )
	{
		int nAt = strVanish.FindOneOf( " .!?,:" );
		if( nAt == 0 )
			strVanish.Delete( 0 );

		if( nAt > 0 )
		{
			CString strWord = strVanish.Left( nAt );
			strVanish.Delete( 0, nAt + 1 );
			
			strWord.Replace( "'s", "" );  // remove apostrophe 's'
			strWord.Replace( "\"", "" ); // remove quotation marks

			int nLen = strWord.GetLength(); 

			if( nLen && ! m_COR.ContainsUS( strWord ) )
			{
				bNone = FALSE;
				CString strRet;
				
				CPOSDlg dlg( FALSE, strRet, strWord, strWork, & saPlugs, this );
				switch ( dlg.DoModal() )
				{
				case IDNO:					// abort this segment
					return IDNO;

				case IDCANCEL:		// abort import completely
					return IDCANCEL;	
				
				case IDIGNORE:		// do not replace
					break;

				default:						// replace	
					
					CString strFinalReplace( "<# def=\"*\">" );  // build formula
					strFinalReplace.Replace( "*", strWord );	
					strFinalReplace.Replace( "#", strRet );	
					saPlugs.Add( strFinalReplace );    // keep for later

					CString strTempReplace( "<#>" );  // create temporary
					char cLetter = char( BYTE( 'A' ) + saPlugs.GetSize()  - 1 );
					strTempReplace.Replace( '#', cLetter );

					int nFind = strWork.Find( strWord );
					strWork.Delete( nFind, strWord.GetLength() );
					strWork.Insert( nFind, strTempReplace );
				}
			}
		}
		bSub = ( nAt >= 0 );
	}

	CString strDlg( strWork );

	strWork.Insert( 0, ' ' );
	strWork.Replace( " an ",  " <ia> " );
	strWork.Replace( " a ", " <ia> " );
	strWork.Replace( " i ", " I " );
	strWork.Replace( " i'", " I'" );
	while( strWork.Replace( "  ", " " ) );
	strWork.Delete( 0 );

	BYTE nSize = saPlugs.GetSize();
	for( BYTE i = 0; i < nSize; i++ )
	{
		CString strTempReplace( "<#>" );  // create temporary
		char cLetter = char( BYTE( 'A' ) + i  );
		strTempReplace.Replace( '#', cLetter );
		strWork.Replace( strTempReplace, saPlugs.GetAt( i ) );
	}
	str = strWork;
	
	if( bNone )
	{
		CPOSDlg dlg( TRUE, CString(), CString( "< keine ersetzbaren Wörter >" ), strDlg, NULL, this );
		return dlg.DoModal();
	}
	return IDYES;
}

/////////////////////////////////////////////////////////////////////////////
// CSyntaxDlg message handlers

BOOL CSyntaxDlg::ProcessTitles( CArchive & rar )
{
	BOOL bSuccess = FALSE;
	BOOL bDone = FALSE;
	while( ! bDone ) 
	{
		char sz[ 200 ];
		rar.ReadString( sz, 199 );	
		CString str( sz );
		str.TrimRight();
		str.TrimLeft();
		bDone = (str == "$end" ); 
		if( !bDone && str.GetLength() )
		{
			switch( Parameterize( str, 5 ) )
			{
			case IDCANCEL:
				return FALSE;
			case IDYES:
				bSuccess = TRUE;
				m_SF.m_Title.RecordSegment( str );
				break;
			}
		}
	}
	if( bSuccess)
		MessageBox( "Das Taggen der Titel-Syntagmen ist gelungen", "Erfolg" );	
	else
		MessageBox( "Keine Titel-Syntagmen wurden gefunden", "Fehler" );	
	return TRUE;
}

BOOL CSyntaxDlg::ProcessText( CArchive & rar )
{
	CString strSegment;
	BOOL bSuccess = FALSE;
	BOOL bDone = FALSE;
	while( ! bDone ) 
	{
		char sz[ 200 ];
		rar.ReadString( sz, 199 );	
		CString str( sz );		
		bDone = (str == "$end" ); 
		if( !bDone && str.GetLength() )
		{
			int nAt = str.FindOneOf( ".!?" );
			if( nAt >= 0 )
			{
				strSegment += str.Left( nAt + 1 );	
				strSegment.TrimLeft();
				switch( Parameterize( strSegment, 20 ) )
				{
				case IDCANCEL:
					return FALSE;
				case IDYES:
					bSuccess = TRUE;
					m_SF.m_Text.RecordSegment( strSegment );
					break;
				}
				if( str.GetLength() > nAt + 2 )
					strSegment = str.Mid( nAt + 2 );
				else
					strSegment = "";
			}
			else
				strSegment += str + ' ';
		}
	}	
	
	if( bSuccess)
		MessageBox( "Das Taggen der Syntagmen ist gelungen", "Erfolg" );	
	else
		MessageBox( "Keine Syntagmen wurden gefunden", "Fehler" );	
	return TRUE;
}

void CSyntaxDlg::OnGenerate() 
{
  	CStdioFile rfile( m_strTagPath,  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );
	if( m_bTitles )
		ProcessTitles( rar ); 
	else
		ProcessText( rar );
	rar.Close();	
	rfile.Close();
	WriteReportFile();
	m_strTagPath = "";
	EnableControls( FALSE );
	GetDlgItem( IDC_SYN_FILENAME )->SetWindowText( "Keine Datei ausgewählt" );
}

void CSyntaxDlg::WriteReportFile()
{
	CStdioFile w1file( "syn_report.txt",  CFile::modeWrite | CFile::modeCreate | CFile::typeText );
	CArchive w1ar( & w1file, CArchive::store );

	int n = m_SF.m_Title.size();
	w1ar.WriteString( "---------BEGIN TITLE REPORT-------- \n" );
	for( int i = 0; i < n; i++ )
	{
		CSyntaxElement * pR =	m_SF.m_Title.at( i ).get();

		char s[ 2000 ];
		wsprintf
				   ( 
				   s, 
					"Form(\"%s\"), Usage(%d)\n", 
				   pR->m_strKey,
				   pR->m_nCount
				   );
		w1ar.WriteString( s );
	}

	w1ar.WriteString( "------------END TITLE REPORT--------------\n" );
	char s[ 100 ];
		 wsprintf
			   ( 
			   s, 
			   "Total Segments(%d)\n", 
			   n
			   );
	w1ar.WriteString( s );

	n = m_SF.m_Text.size();
	w1ar.WriteString( "---------BEGIN TEXT REPORT-------- \n" );
	for( i = 0; i < n; i++ )
	{
		CSyntaxElement * pR =	m_SF.m_Text.at( i ).get();

		char s[ 2000 ];
		wsprintf
				   ( 
				   s, 
					"Form(\"%s\"), Usage(%d)\n", 
				   pR->m_strKey,
				   pR->m_nCount
				   );
		w1ar.WriteString( s );
	}

	w1ar.WriteString( "------------END TEXT REPORT------------\n" );
	wsprintf
			   ( 
			   s, 
			   "Total Segments(%d)\n", 
			   n
			   );
	w1ar.WriteString( s );

	w1ar.Close();	
	w1file.Close();
}

void CSyntaxDlg::EnableControls( BOOL bValidIndex )
{	
	BOOL bFileFound = ( m_strTagPath.GetLength()  > 0 );
	GetDlgItem( IDC_SYNTAGMA_EDIT )->EnableWindow( bValidIndex );
	GetDlgItem( IDC_SYNTAGMA_CHG )->EnableWindow( bValidIndex );
	GetDlgItem( IDC_SYN_FILENAME )->EnableWindow( bFileFound );
	GetDlgItem( ID_GENERATE )->EnableWindow( bFileFound && ! bValidIndex );
}

BOOL CSyntaxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	EnableControls( FALSE );
	return TRUE;
}

void CSyntaxDlg::OnIndexChange() 
{
	int n;
	ParseNumericControl( GetDlgItem( IDC_SYNTAGMA_INDEX ), & n );
	int nSize = m_bTitles ? m_SF.m_Title.size() : m_SF.m_Text.size();
	BOOL bEnable = ( ( n >= 0 ) && ( n < nSize ) );
	EnableControls( bEnable );	
	CString strSyntag;
	if( bEnable )
		strSyntag = m_bTitles ? m_SF.m_Title[ n ]->m_strKey : m_SF.m_Text[ n ]->m_strKey;
	GetDlgItem( IDC_SYNTAGMA_EDIT )->SetWindowText( strSyntag );
}

void CSyntaxDlg::OnChange() 
{
	CString strData;
	GetDlgItem( IDC_SYNTAGMA_EDIT )->GetWindowText( strData );
	int n;
	ParseNumericControl( GetDlgItem( IDC_SYNTAGMA_INDEX ), & n );
	if( m_bTitles )
		m_SF.m_Title[ n ]->m_strKey = strData;
	else
		m_SF.m_Text[ n ]->m_strKey = strData;
	EnableControls( FALSE );
}

void CSyntaxDlg::OnFilefind() 
{
	CFileDialog dlg
		(
		TRUE, 
		"txt", 
		"*.txt", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, 
		"Textdateien (*.txt)" 
		);
	if( IDOK == dlg.DoModal() )
	{
		m_strTagPath = dlg.GetPathName();
		GetDlgItem( IDC_SYN_FILENAME )->SetWindowText( dlg.GetFileName() );
		EnableControls( FALSE );
	}
}

void CSyntaxDlg::OnSynTitles() 
{
	m_bTitles = ! m_bTitles;
	GetDlgItem( IDC_SYNTAGMA_INDEX )->SetWindowText( "-1" );
	GetDlgItem( IDC_SYNTAGMA_EDIT )->SetWindowText( "" );
	m_nSyntagmaIndex = -1;
	EnableControls( FALSE );
}
