// ImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "syntax.h"
#include "ImportDlg.h"
#include "semantic.h"
#include "wn/wn.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BIG_BUFFER_SIZE 65536

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog


CImportDlg::CImportDlg( CIPAManager & rIPA, CCoreVector & rCOR, CFormExc & rEXC, CWnd* pParent /*=NULL*/) : 
	CDialog(CImportDlg::IDD, pParent ),
	m_IPA( rIPA ),
	m_COR( rCOR ),
	m_EXC( rEXC )
{
	//{{AFX_DATA_INIT(CImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialog)
	//{{AFX_MSG_MAP(CImportDlg)
	ON_BN_CLICKED(ID_CMUIMPORT, OnCmuimport)
	ON_BN_CLICKED(ID_COMMONIMPORT, OnCommonimport)
	ON_BN_CLICKED(ID_EXCIMPORT, OnExcimport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL IsValidFormChar( char c )
{
   return ( ( c >= 'A' ) && ( c <= 'Z' ) ) ||  ( ( c >= '0' ) && ( c <= '9' ) ) ||  (c == '\'' ) || ( c == '-' );
}

BOOL IsValidPhoneChar( char c )
{
   return ( ( c >= 'A' ) && ( c <= 'Z' ) ) || (c == '0' ) || ( c == '1' ) || ( c == '2' );
}


#define CONVERT( a, b ) if( s == a ) { s = b; return; }
#define CONVERT_V( a, b ) if( s.Find( a ) == 0 ) { s.Replace( a, b ); n++; return; }


void CMUtoIPA( CString & s, int & n )
{
	CONVERT_V( "AA", "a" );
	CONVERT_V( "AE", "&" );
	CONVERT_V( "AH", "V" );
	CONVERT_V( "AO", "O;" );
	CONVERT_V( "AW", "aU" );	
	CONVERT_V( "AY", "aI" );
	CONVERT( "B", "b" );
	CONVERT( "CH", "tS" );
	CONVERT( "D", "d" );
	CONVERT( "DH", "D" );

	CONVERT_V( "EH", "E" );
	CONVERT_V( "ER", "3;" );
	CONVERT_V( "EY", "eI" );
	CONVERT( "F", "f" );
	CONVERT( "G", "g" );
	CONVERT( "HH", "h" );
	CONVERT_V( "IH", "I" );
	CONVERT_V( "IY", "i" );
	CONVERT( "JH", "dZ" );
	CONVERT( "K", "k" );
	
	CONVERT( "L", "l" );
	CONVERT( "M", "m" );
	CONVERT( "N", "n" );
	CONVERT( "NG", "N" );
	CONVERT_V( "OW", "@U" );
	CONVERT_V( "OY", "OI" );
	CONVERT( "P", "p" );
	CONVERT( "R", "R" );
	CONVERT( "S", "s" );
	CONVERT( "SH", "S" );

	CONVERT( "T", "t" );
	CONVERT( "TH", "T" );
	CONVERT_V( "UH", "U" );
	CONVERT_V( "UW", "u" );
	CONVERT( "V", "v" );
	CONVERT( "W", "w" );
	CONVERT( "Y", "j" );
	CONVERT( "Z", "z" );
	CONVERT( "ZH", "Z" );
}

/////////////////////////////////////////////////////////////////////////////
// CImportDlg message handlers

void CImportDlg::AddIPAElement( CString & strForm, CString & strIPATrans, int nSilben )
{	
	DWORD dwIndex = m_IPA.GetData().size();
	m_IPA.AddElement( strForm, strIPATrans, nSilben );
	strForm.MakeLower();
	if( WordExistsInWordnet( strForm, NOUN ) )
		m_IPA.GetLookup( LOOKUP_NOUN ).Add( dwIndex );
	if( WordExistsInWordnet( strForm, VERB ) )
		m_IPA.GetLookup( LOOKUP_VERB ).Add( dwIndex );
		if( WordExistsInWordnet( strForm, ADJ ) )
		m_IPA.GetLookup( LOOKUP_ADJ ).Add( dwIndex );
	if( WordExistsInWordnet( strForm, ADV ) )
		m_IPA.GetLookup( LOOKUP_ADV ).Add( dwIndex );
}

void CImportDlg::OnCmuimport() 
{
	GetDlgItem( IDC_CMUMESSAGE )->SetWindowText( "Datei 'CMU.DAT' wird geöffnet" );
	GetDlgItem( IDOK )->EnableWindow( FALSE );

	m_IPA.GetData().clear();
	m_IPA.GetLookups().clear();
	m_IPA.AddLookupElement( LOOKUP_NOUN );
	m_IPA.AddLookupElement( LOOKUP_VERB  );
	m_IPA.AddLookupElement( LOOKUP_ADJ );
	m_IPA.AddLookupElement( LOOKUP_ADV );

   	CStdioFile rfile( "CMU.DAT",  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );
	BOOL bDone = FALSE;
	long lCount = 0;
	while( !bDone ) 
	{
		char sz[ 200 ];
		rar.ReadString( sz, 199 );

		CString str( sz );
		if( IsValidFormChar( sz[ 0 ] ) && ! ( str.Find( "(", 0 ) > 0 ) && ! ( str.Find( ".", 0 ) > 0 ) )
		{
			char szMessage[ 100 ];
			lCount++;
			wsprintf( szMessage, "%ld Wortformen übersetzt", lCount );
			GetDlgItem( IDC_CMUMESSAGE )->SetWindowText( szMessage );

			int i = 0;
			char szForm[ 50 ];
			while( IsValidFormChar( sz[ i ] ) )	
			{ 
				szForm[ i ] = sz[ i ];	
				i++; 
			}
			szForm[ i ] = '\0';		
			CString strForm( szForm );
			int nSilben = 0;
			CString strIPATrans( "" );
			char szToken[ 10 ];
			int j = 0;
			while( sz[ i ] != '\0' )
			{
				if( IsValidPhoneChar( sz[ i ] ) )
				{ 
					szToken[ j ] = sz[ i ];
					j++;
				}
				else
				{
					szToken[ j ] = '\0';
					if( j != 0 )
					{
						CString strIPA( szToken );
						CMUtoIPA( strIPA, nSilben );
						strIPATrans += strIPA;
					}
					j = 0;			
				}
				i++;
			}
			szToken[ j ] = '\0';
			if( j )
			{
				CString strIPA( szToken );
				CMUtoIPA( strIPA, nSilben );
				strIPATrans += strIPA;
			}
			if( strForm.FindOneOf( "0123456789" ) == -1 ) // get rid of "weird" elements with numbers
				AddIPAElement( strForm, strIPATrans, nSilben );
		}
		bDone = (str == "$end" ); 
	}

	rar.Close();	
	rfile.Close();
	GetDlgItem( IDC_CMUMESSAGE )->SetWindowText( "Datei 'ENG-US.IPA' wird geschrieben" );

	if( CFileFind().FindFile( IPA_FILE ) )
			CFile::Remove( IPA_FILE );
	CStdioFile wfile( IPA_FILE, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary );
	CArchive war( &wfile, CArchive::store );
	m_IPA.Serialize( war );
	war.Close();
	wfile.Close();
	GetDlgItem( IDOK )->EnableWindow( TRUE );	
	GetDlgItem( IDC_CMUMESSAGE )->SetWindowText( "Erfolg" );

}

void CImportDlg::OnCommonimport() 
{
	GetDlgItem( IDC_COREMESSAGE )->SetWindowText( "Datei 'COMMON.TXT' wird geöffnet" );
	GetDlgItem( IDOK )->EnableWindow( FALSE );

	m_COR.clear();
	CStdioFile rfile( "CORE.TXT",  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );
	BOOL bDone = FALSE;
	long lCount = 0;
	CString strOld;
	while( !bDone ) 
	{
		char sz[ 200 ];
		rar.ReadString( sz, 199 );
		CString str( sz );
		str.MakeUpper();
		str.Replace( " ", "" );
		if( sz[ 0 ] != '$' )
		{
			if( str < strOld )
			{
				MessageBox( strOld, "Sorting Failure" , MB_OK | MB_ICONHAND );
			}		

		char szMessage[ 100 ];
			lCount++;
			wsprintf( szMessage, "%ld Wortformen eingelesen", lCount );
			GetDlgItem( IDC_COREMESSAGE )->SetWindowText( szMessage );

			m_COR.AddElement( str, 0 );
			strOld = str;
		}
		bDone = (str == "$END" ); 
	}	
	rar.Close();	
	rfile.Close();

	GetDlgItem( IDC_COREMESSAGE )->SetWindowText( "Datei 'ENG-US.COR' wird geschrieben" );
	if( CFileFind().FindFile( COR_FILE ) )
			CFile::Remove(  COR_FILE );
	CStdioFile wfile( COR_FILE, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary );
	CArchive war( &wfile, CArchive::store );
	m_COR.Serialize( war );
	war.Close();
	wfile.Close();

	GetDlgItem( IDOK )->EnableWindow( TRUE );	
	GetDlgItem( IDC_COREMESSAGE )->SetWindowText( "Erfolg" );
}

/*
void ExcParseLine( char * p, UINT & nPos, CString & str1, CString & str2 )
{
	char sz1[ 100 ];
	char sz2[ 100 ];
	int n = 0;
	while( p[ nPos ] != ' ' )
		sz1[ n++ ] = p[ nPos++ ];
	sz1[ n ] = '\0';		
	nPos++;	
	n = 0;
	while( p[ nPos ] != '\n' )
		sz2[ n++ ] = p[ nPos++ ];
	sz2[ n ] = '\0';
	nPos++;
	str1 = sz1;
	str2 = sz2;
}
*/

void ExcParseLine( char * p, CString & str1, CString & str2, CString & str3 )
{
	char sz1[ 100 ];
	char sz2[ 100 ];
	char sz3[ 100 ];
	int n = 0;
	int nPos = 0;
	while( ( p[ nPos ] != ' '  ) && ( p[ nPos ] != '\0' ) )
		sz1[ n++ ] = p[ nPos++ ];
	sz1[ n ] = '\0';

	if( p[ nPos ] == '\n' )
		return;
		
	nPos++;	
	n = 0;
	while( ( p[ nPos ] != ' ' ) && ( p[ nPos ] != '\0' ) )
		sz2[ n++ ] = p[ nPos++ ];
	sz2[ n ] = '\0';

	if( p[ nPos ] == ' ' )
	{
		nPos++;	
		n = 0;
		while( p[ nPos ] != '\0' )
			sz3[ n++ ] = p[ nPos++ ];
		sz3[ n ] = '\0';
		nPos++;
		str3 = sz3;
	}
	str1 = sz1;
	str2 = sz2;
}

void CImportDlg::OnExcimport() 
{
	GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( "Datei 'EXC_A.DAT' wird geöffnet" );
	GetDlgItem( IDOK )->EnableWindow( FALSE );
	m_EXC.Clear();	

	{
   	CStdioFile rfile( "exc_a.dat",  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );

	BOOL bDone = FALSE;
	long lCount = 0;
	CString strOld;
	while( !bDone ) 
	{
		CString strBase, strCompare, strSuper;
		char sz[ 200 ];
		rar.ReadString( sz, 199 );
		ExcParseLine( sz, strBase, strCompare, strSuper );
		strBase.MakeUpper();
		strCompare.MakeUpper();
		strSuper.MakeUpper();
		if( sz[ 0 ] != '$' )
		{
			if( strBase < strOld )
			{
				MessageBox( strOld, "Sorting Failure" , MB_OK | MB_ICONHAND );
			}
			
			char szMessage[ 100 ];
			lCount++;
			wsprintf( szMessage, "%ld Adjektiven eingelesen", lCount );
			GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( szMessage );

			if( m_IPA.GetData().ContainsUS( strBase ) )		
				m_EXC.m_Adjs.AddElement( strBase, strCompare, strSuper );

			strOld = strBase;
		}
		bDone = ( CString( sz ) == "$END" ); 
	}		
	rar.Close();	
	rfile.Close();
	}

	{
 	CStdioFile rfile( "exc_n.dat",  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );

	BOOL bDone = FALSE;
	long lCount = 0;
	CString strOld;
	while( !bDone ) 
	{
		CString strBase, strPlural, strNotUsed;
		char sz[ 200 ];
		rar.ReadString( sz, 199 );
		ExcParseLine( sz, strBase, strPlural, strNotUsed );
		strBase.MakeUpper();
		strPlural.MakeUpper();
		if( sz[ 0 ] != '$' )
		{
			if( strBase < strOld )
			{
				MessageBox( strOld, "Sorting Failure" , MB_OK | MB_ICONHAND );
			}	

			if( strNotUsed.GetLength() )
			{
				MessageBox( strBase, "3 Elements" , MB_OK | MB_ICONHAND );
			}	
			
			char szMessage[ 100 ];
			lCount++;
			wsprintf( szMessage, "%ld Substantive eingelesen", lCount );
			GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( szMessage );

			if( m_IPA.GetData().ContainsUS( strBase ) && m_IPA.GetData().ContainsUS( strPlural ) )
				m_EXC.m_Nouns.AddElement( strBase, strPlural );

			strOld = strBase;
		}
		bDone = ( CString( sz ) == "$END" ); 
	}		
	rar.Close();	
	rfile.Close();
	}

	{
 	CStdioFile rfile( "exc_v.dat",  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );

	BOOL bDone = FALSE;
	long lCount = 0;
	CString strOld;
	while( !bDone ) 
	{
		CString strBase, strPast, strPerfect;
		char sz[ 200 ];
		rar.ReadString( sz, 199 );
		ExcParseLine( sz, strBase, strPast, strPerfect );
		strBase.MakeUpper();
		strPast.MakeUpper();
		strPerfect.MakeUpper();
		if( sz[ 0 ] != '$' )
		{
			if( strBase < strOld )
			{
				MessageBox( strOld, "Sorting Failure" , MB_OK | MB_ICONHAND );
			}	
			
			char szMessage[ 100 ];
			lCount++;
			wsprintf( szMessage, "%ld Verben eingelesen", lCount );
			GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( szMessage );

			if( m_IPA.GetData().ContainsUS( strBase ) && m_IPA.GetData().ContainsUS( strPast ) )
				m_EXC.m_Verbs.AddElement( strBase, strPast, strPerfect );

			strOld = strBase;
		}
		bDone = ( CString( sz ) == "$END" ); 
	}		
	rar.Close();	
	rfile.Close();
	}

	/*
	{
	int nKilled = 0;
	CStdioFile wfile( "exc_v2.dat", CFile::modeWrite | CFile::modeCreate | CFile::typeText );
	CArchive war( &wfile, CArchive::store );
	int n = m_EXC.m_Verbs.size();
	for( int i = 0; i < n; i++ )
	{
		char sz[ 100 ];
		CVerbExcElement * p = m_EXC.m_Verbs[ i ].get();	
		if(  TRUE )
		{
			p->m_strKey.MakeLower();
			p->m_strPast.MakeLower();
			p->m_strVarPerfect.MakeLower();
//			wsprintf( sz, "%s %s %s\n", p->m_strKey, p->m_strCompare, p->m_strSuper );
			wsprintf( sz, "%s %s %s\n", p->m_strKey, p->m_strPast, p->m_strVarPerfect );
			war.WriteString( sz );
		}
		else
			nKilled++;
	}
	war.WriteString( "$END\n" );

	long lCount = 0;

	war.Close();
	wfile.Close();
	}
	*/
	
	GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( "Datei 'ENG-US.AFW' wird geschrieben" );
	if( CFileFind().FindFile( EXC_FILE ) )
			CFile::Remove(  EXC_FILE );
	CStdioFile wfile( EXC_FILE, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary );
	CArchive war( &wfile, CArchive::store );
	m_EXC.Serialize( war );
	war.Close();
	wfile.Close();

	GetDlgItem( IDOK )->EnableWindow( TRUE );	
	GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( "Erfolg" );
}

/*
void CImportDlg::OnExcimport() 
{
	GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( "Datei 'ADJ.EXC' wird geöffnet" );
	GetDlgItem( IDOK )->EnableWindow( FALSE );
	m_EXC.Clear();	
	char * pBuffer = new char[ BIG_BUFFER_SIZE ]; 	

   	CStdioFile rfile( "NOUN.EXC",  CFile::modeRead | CFile::typeBinary );
	CArchive rar( & rfile, CArchive::load );
	
	UINT nDataSize = rar.Read( pBuffer, BIG_BUFFER_SIZE );
	UINT nPos = 0;
	
	CString strLeft, strRight;

	while( nPos < nDataSize )
	{
		ExcParseLine( pBuffer, nPos, strLeft, strRight );
		if( ( strLeft != strRight ) && ! ( m_COR.ContainsUS( strLeft ) ) )
		{
			CAdjExcElement * p = m_EXC.m_Adjs.Find( strRight );
			if( p )
			{
				if( strLeft.Right( 2 ) == "er" )
					p->m_strCompare = strLeft;
				else
					p->m_strSuper = strLeft;
			}
			else
			{
				if( strLeft.Right( 2 ) == "er" )
					m_EXC.m_Adjs.AddElement( strRight, strLeft, CString( "" ) );
				else
					m_EXC.m_Adjs.AddElement( strRight, CString( "" ), strLeft );
			}
		}
	}		
	delete pBuffer;	
	rar.Close();	
	rfile.Close();	
	CStdioFile wfile( "exc_n.dat", CFile::modeWrite | CFile::modeCreate | CFile::typeText );
	CArchive war( &wfile, CArchive::store );
	int n = m_EXC.m_Adjs.size();
	for( int i = 0; i < n; i++ )
	{
		char sz[ 100 ];
		CAdjExcElement * p = m_EXC.m_Adjs[ i ].get();
		wsprintf( sz, "%s %s %s\n", p->m_strKey, p->m_strCompare, p->m_strSuper );
		war.WriteString( sz );
	}
	war.WriteString( "$END\n" );

	long lCount = 0;

	war.Close();
	wfile.Close();
	GetDlgItem( IDOK )->EnableWindow( TRUE );	
	GetDlgItem( IDC_EXCMESSAGE )->SetWindowText( "Erfolg" );
}

*/

