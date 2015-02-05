// READER.CPP - for evaluation details...

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "syntax.h"

BOOL IsValidReadChar( char c )
{
   return ( ( c >= 'A' ) && ( c <= 'Z' ) ) || ( ( c >= 'a' ) && ( c <= 'z' ) ) || (c == '\'' );
}

class CReadElement : public CPowerElement
{
public:
	BOOL m_bInCOR;
	BOOL m_bHasIPA;
	long m_lCount;
};

class CReadVector  : public CPowerVector< CReadElement >
{
public:
	CReadVector( CIPAManager & rIPA, CCoreVector & rCOR ) : 
	  m_IPA( rIPA ),
	  m_COR( rCOR ) 
	  {
	  }

	void RecordToken( LPCSTR szToken )
	{
		CString str( szToken );
		str.MakeUpper();
		CReadElement * pR = Find( str );
		if( pR )
			pR->m_lCount++;
		else
		{
			pR = new CReadElement;
			pR->m_lCount = 1;
			pR->m_bInCOR = ( m_COR.SortedFind( str ) != NULL );
			pR->m_bHasIPA =  ( m_IPA.GetData().SortedFind( str ) != NULL );
			Add( pR, str );
		}
	}

	CIPAManager & m_IPA;
	CCoreVector & m_COR;
};

void PrintLoop( CReadVector & thing, CArchive & w1ar, long nLow, long nHigh )
{
	int n = thing.size();
	for( int i = 0; i < n; i++ )
	{
		CReadElement * pR =	thing.at( i ).get();
		if( ! pR->m_bInCOR && ( pR->m_lCount >= nLow ) && ( pR->m_lCount  < nHigh ) )
		{
		char s[ 100 ];
		 wsprintf
			   ( 
			   s, 
			   "Form(''%s''), HasIPA(%d), Usage(%d)\n", 
			   pR->m_strKey,
			   pR->m_bHasIPA,
			   pR->m_lCount
			   );
		 w1ar.WriteString( s );
		}
	}
	w1ar.WriteString( "------------------ \n" );
}

void RunReader( CWnd * pDlg, CIPAManager & rIPA, CCoreVector & rCOR ) 
{
	CReadVector thing( rIPA, rCOR );
	long linecount = 0;
  	CStdioFile rfile( "learn.txt",  CFile::modeRead | CFile::typeText );
	CArchive rar( & rfile, CArchive::load );
	BOOL bDone = FALSE;
	while( !bDone ) 
	{
		char sz[ 2000 ];
		char szToken[ 50 ];
		rar.ReadString( sz, 1999 );	
		linecount++;
		if( linecount % 13 == 0 )
		{
			char szMessage[ 100 ];
			int nWords = thing.size();
			wsprintf
			   ( 
			   szMessage, 
			   "%ld Zeilen, %ld neue Wortformen", 
			   linecount, 		   
			   nWords
			   );
			pDlg->GetDlgItem( IDC_POEM )->SetWindowText( szMessage );
		}
		CString str( sz );
		int i = 0;
		int j = 0;
		while( sz[ i ] != '\0' )
		{
			if( IsValidReadChar( sz[ i ] ) )
			{ 
				szToken[ j ] = sz[ i ];
				j++;
			}
			else
			{
				szToken[ j ] = '\0';
				if( j != 0 )
					thing.RecordToken( szToken );
				j = 0;			
			}
			i++;
		}
		if( j != 0)
		szToken[ j ] = '\0';
		thing.RecordToken( szToken );
		bDone = (str == "$end" ); 
	}
	rar.Close();	
	rfile.Close();

	pDlg->GetDlgItem( IDC_POEM )->SetWindowText( "Writing Report" );
	CStdioFile w1file( "freq.txt",  CFile::modeWrite | CFile::modeCreate | CFile::typeText );
	CArchive w1ar( & w1file, CArchive::store );
	int nIPA = 0;
	int nCOR = 0;
	int n = thing.size();
	w1ar.WriteString( "---------CORE WORDS FOUND--------- \n" );
	for( int i = 0; i < n; i++ )
	{
		CReadElement * pR =	thing.at( i ).get();
		if( pR->m_bHasIPA )
			nIPA++;
		if( pR->m_bInCOR)
		{
			nCOR++;
			char s[ 100 ];
			 wsprintf
				   ( 
				   s, 
					"Form(''%s''), HasIPA(%d), Usage(%d)\n", 
				   pR->m_strKey,
				   pR->m_bHasIPA,
				   pR->m_lCount
				   );
			 w1ar.WriteString( s );
		}
	}
	int nCoreSize = rCOR.size();
	w1ar.WriteString( "---------CORE WORDS NOT FOUND--------- \n" );
	for( i = 0; i < nCoreSize; i++)
		if( thing.Find( rCOR[ i ]->m_strKey ) == NULL )
		{
			char s[ 100 ];
			 wsprintf
				   ( 
				   s, 
					"Key(''%s'')\n", 
				   rCOR[ i ]->m_strKey
				   );
			 w1ar.WriteString( s );
		}
	w1ar.WriteString( "--------NON-CORE WORDS RANKED BY COUNT---------- \n" );
	PrintLoop( thing, w1ar, 50000, 1000000000 );
	PrintLoop( thing, w1ar, 20000, 50000 );
	PrintLoop( thing, w1ar, 10000, 20000 );
	PrintLoop( thing, w1ar, 5000, 10000 );
	PrintLoop( thing, w1ar, 2000, 5000 );
	PrintLoop( thing, w1ar, 1000, 2000 );
	PrintLoop( thing, w1ar, 500, 1000 );
	PrintLoop( thing, w1ar, 200, 500 );
	PrintLoop( thing, w1ar, 100, 200 );
	PrintLoop( thing, w1ar, 50, 100 );
	PrintLoop( thing, w1ar, 20, 50 );
	PrintLoop( thing, w1ar, 10, 20 );
	PrintLoop( thing, w1ar, 5, 10 );
	PrintLoop( thing, w1ar, 2, 5 );
	PrintLoop( thing, w1ar, 0, 2 );
	w1ar.WriteString( "--------NON-IPA WORDS ---------- \n" );
	for( i = 0; i < n; i++ )
	{
		CReadElement * pR =	thing.at( i ).get();
		if( ! pR->m_bHasIPA )
		{
		char s[ 100 ];
		 wsprintf
			   ( 
			   s, 
			   "Form(''%s''),  InCore(%d), Usage(%d)\n", 
			   pR->m_strKey,
			   pR->m_bInCOR,
			   pR->m_lCount
			   );
		 w1ar.WriteString( s );
		}
	}

	w1ar.WriteString( "------------END OF REPORT------------------\n" );
	char s[ 100 ];
		 wsprintf
			   ( 
			   s, 
			   "COR Bank Size(%d), COR Forms(%d), IPA Forms(%d), Total Forms(%d )\n", 
				nCoreSize,
				nCOR,
			   nIPA,
			   n
			   );
	w1ar.WriteString( s );
	w1ar.Close();	
	w1file.Close();
	pDlg->GetDlgItem( IDC_POEM )->SetWindowText( "<poem>" );
}
