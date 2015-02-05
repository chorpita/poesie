#include "stdafx.h"
#include "generic.h"
#include "poetics.h"
#include "lexicon.h"
#include "compile.h"

void CWormCompiler::DeleteLists()
{
	while( m_nNumCharts )
		delete m_pCharts[ --m_nNumCharts ];
}

CWord * CWormCompiler::GetRandomWord()
{
	return m_Lex.GetWord( RandVal( m_Lex.GetNumWords() ) );
}

CWord * CWormCompiler::GetRandomMatchWord( char c, int nMatch )
{
	CMatchChart * pChart = FindMatchChart( c );
	if( pChart )
	{
		CWordRef wr1 = pChart->GetWordRef( nMatch, RandVal( pChart->GetListSize( nMatch ) ) );
		CWordRef wr2 = pChart->GetWordRef( nMatch, RandVal( pChart->GetListSize( nMatch ) ) );
		if( wr1.GetRefCount() )
			return wr2;
		else
			return wr1;
		// handle reference count later...lame code here!!
	}
	else
	{
		CWord * pWord = GetRandomWord();
		m_pCharts[ m_nNumCharts++ ] = m_Lex.CreateMatchChart( c, pWord );
		return pWord;
	}
}

CMatchChart * CWormCompiler::FindMatchChart( char c )
{
	for( int i = 0; i < m_nNumCharts; i++ )
		if( c == m_pCharts[ i ]->GetId() )
			return m_pCharts[ i ];
	return NULL;
}

void CWormCompiler::InsertWord( int nFind, CString & str )
{
	CString strLocal( str );
	int nReim = strLocal.Find( "r=" );
	int nEnd= strLocal.Find( ">", nFind );
	BOOL bReim = ( nReim > 0 ) && (nReim < nEnd );
	char cReim;
	int nMatch = 0;	
	if( bReim )
	{
		cReim = strLocal.GetAt( nReim + 2 );	
		int nMatchSearch = strLocal.Find( "#" );	
		if( nMatchSearch < nEnd )
		{
	    	int nTemp = byte( strLocal.GetAt( nMatchSearch + 1 ) ) - byte( '0' );	
			if( (nTemp >= 0) && (nTemp < 4 ) )
				nMatch = nTemp;
		}
	}
	strLocal.Delete( nFind, nEnd + 1 - nFind );
	 if( bReim )
	 {		
		strLocal.Insert
			(  
			nFind, 
			GetRandomMatchWord( cReim,  nMatch  )->m_strText 
			);
	}
	 else
	 {
			// strLocal.Insert( nFind, "<word=\"" + GetRandomWord()->m_strText + "\">" );
		strLocal.Insert( nFind, GetRandomWord()->m_strText );
	 }
	str = strLocal;
}

BOOL CWormCompiler::Run( CString & strPoem )
{
     CString strLocal( strPoem );

	 BOOL bRunning = strLocal.Find( "<" ) >= 0;

	 int nFind = strLocal.Find( "<s" );

	 if( nFind >= 0 )
		 InsertWord( nFind, strLocal );

	 if( m_UO.m_bHasTitle)
	      strLocal.Replace( "<poem>", "title \n  \n<body>");
	 else
		 strLocal.Replace( "<poem>", "<body>" );

	 strLocal.Replace( "<verse>", "<s r=a#3> <s> <s> <s r=b#0>" );

	 switch( m_UO.m_cbStanza )
	 {
         int i;	 
	 case STANZA_FIXEDVERSE:
         for( i = 1; i < m_UO.m_nVerses; i++ )
		    strLocal.Replace( "<block>", "<block> \n<verse>");
		 strLocal.Replace( "<block>", "<verse>" );
		 break;
	 case STANZA_RANDOMVERSE:
         for( i = 0; i <= RandVal( 8 ); i++ )
		    strLocal.Replace( "<block", "<block> \n<verse>");
		 strLocal.Replace( "<block>", "<verse>" );
		 break;
	 }	 	 
	 switch( m_UO.m_nPoem )
	 {      
		 int i;
	 case POEM_FIXED:
         for( i = 1; i < m_UO.m_nStanzas; i++ )
		    strLocal.Replace( "<body>", "<body> \n  \n<block>");
		 strLocal.Replace( "<body>", "<block>" );
		 break;
	 case POEM_RANDOM:
         for( i = 1; i <= RandVal( 4 ); i++ )
		    strLocal.Replace( "<body>", "<body> \n  \n<block>");
		 strLocal.Replace( "<body>", "<block>" );
		 break;
     case POEM_SONNETT:
		 strLocal.Replace( "<body>", "<4> \n \n<4> \n \n<3>  \n \n<3>" );  
		 strLocal.Replace( "<4>", "<verse> \n<verse> \n<verse> \n<verse>" );
		 strLocal.Replace( "<3>", "<verse> \n<verse> \n<verse>" );
     case POEM_HAIKU:
		 strLocal.Replace( "<body>", "<5> \n<7> \n<5>" );
		 strLocal.Replace( "<5>", "<s> <s> <s> <s> <s>" );
		 strLocal.Replace( "<7>", "<s> <s> <s> <s> <s> <s> <s>" );
		 break;
	 }	 
	 strPoem = strLocal;

	 return bRunning;
}

// end COMPILE.CPP
