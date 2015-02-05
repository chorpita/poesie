// DEER.CPP - Advanced Compiler

#include "stdafx.h"
#include "generic.h"
#include "poetics.h"
#include "lexicon.h"
#include "compile.h"
#include "phonetics.h"
#include "syntax.h"
#include "semantic.h"
#include "chicken.h"
#include "deer.h"
#include "wn/wn.h"


int CRhymeData::GetCollectiveRhyminess( CIPAElement * pIPAE )
{
	int nScore = 5;
	for( int i = 0; i < m_nTotal; i++ )
		nScore = min( nScore, GetRhyminess( pIPAE, m_pIPAE[ i ] ) );
	return nScore;
}

/*
BOOL CDeerCompiler::InsertWord( CString & str, LPCSTR szToken, int nSynCode )
{
		int nStart = str.Find(  szToken );
		BOOL bFail =  ( nStart == -1 );
		if( ! bFail )
		{
			int nEnd = str.Find( '>', nStart );
			ASSERT( nEnd > 0 );
			int nOpenQ = str.Find( QUOTE_CHAR, nStart );
			ASSERT( nOpenQ > 0 );
			int nCloseQ = str.Find( QUOTE_CHAR, nOpenQ + 1 );
			ASSERT( nCloseQ > 0 );
			CString strTag( str.Mid( nStart, nEnd - nStart + 1 ) );
			CString strWord;

			CRandPick kPlug;
			kPlug.Fill( WPLUG_POET, 2 );
			kPlug.Fill( WPLUG_BRAIN, 4 );
			kPlug.Fill( WPLUG_PIER, 3 );

			switch( kPlug.Pick() )
			{
			case WPLUG_POET:
				strWord = str.Mid( nOpenQ + 1, nCloseQ - nOpenQ - 1 );
				break;
			case WPLUG_BRAIN:
				InsertBrainWord( strWord, nSynCode );
				break;
			case WPLUG_PIER:
				InsertPierWord( strWord, nSynCode );
				break;
			default:
				ASSERT( FALSE );
			}
			str.Replace( strTag,  strWord );
		}
		return bFail;
}

*/

CIPAElement * CDeerCompiler::GetRandomBrainIPAE( enumSyn eSyn )
{
	CString strLexBrain( m_LB.GetLexVector( eSyn )->RandElement()->m_strKey );
	return m_EXC.InflectWord( strLexBrain, eSyn ); 
}

CIPAElement * CDeerCompiler::GetRandomBrainIPAE( enumSyn eSyn, int nSought )
{
	CIPAElement * pIPAE;
	int nFound = 0;
	while( nFound != nSought )
	{
		pIPAE = NULL;
		while( pIPAE == NULL )
			pIPAE = GetRandomBrainIPAE( eSyn );	
		nFound = pIPAE->m_nSilben;
	}
	return pIPAE; 
}

CIPAElement * CDeerCompiler::GetInsertionWord
	( 
	enumSyn eSyn, 
	char cCode, 
	CString & strDef,
	int nSought, 
	int nMax 
	)
{
	ASSERT( nSought > 0 );
	CIPAElement * pIPAE;
	if( cCode == 'x' )
		pIPAE = GetRandomBrainIPAE( eSyn, nSought );
	else
	{
		int nRhymeIndex = rm.FindIndex( cCode );
		if( nRhymeIndex == -1 )
			pIPAE = GetRandomBrainIPAE( eSyn, nSought );
		else
		{
			CRhymeData & rd = rm.m_r[ nRhymeIndex ];

			int nPOS = GetSynPOS( eSyn );
			int nLookupSize = m_IPA.GetLookup( nPOS ).GetSize();
			long nRandIndex = RandVal( nLookupSize );
			
			int nBestIndex = nRandIndex;
			int nBestScore;
			pIPAE = m_IPA.IndexLookup( nPOS, nRandIndex );
			if( ( pIPAE->m_nSilben != nSought ) || ( pIPAE->m_strKey.GetLength() == 1 ) )
				nBestScore = -5;
			else
				nBestScore = rd.GetCollectiveRhyminess( pIPAE );	
			int i = nRandIndex + 1;	
			if( i == nLookupSize )
				i = 0;	
			while( i != nRandIndex )
			{
				int nScore;
				pIPAE = m_IPA.IndexLookup( nPOS,  i );
				if( pIPAE->m_nSilben != nSought )
					nScore = -5;
				else
					nScore = rd.GetCollectiveRhyminess( pIPAE );	
				if( nScore > nBestScore )
				{
					nBestScore = nScore;
					nBestIndex = i;
				}	
				i++;
				if( i == nLookupSize )
					i = 0;	
			}		
			pIPAE = m_IPA.IndexLookup( nPOS,  nBestIndex );		
		}	
		rm.Add( cCode, pIPAE );
	}
	return pIPAE;
}

/*
void CDeerCompiler::InsertRhyme()
{
	CVerseData & vd = pd.GetWorkingVerse();

	char cCode = vd.m_r.m_cCode; // get verse code
	int nToSpare = vd.m_nSyllablesToSpare;
	int nDynamics = vd.m_nNumDynamics;


	CRhymeData * pRD = rm.Find( cCode );						// find rhyme data


	CIPAElement * pIPAE; // = m_IPA[ nRandIndex ].get(); // get a new word

	if( pRD ) // rhyme with existing word
	{
		long nRandIndex = RandVal( m_IPA.GetData().size() );

		int nBestIndex = nRandIndex;
		int nBestScore = GetRhyminess( m_IPA.GetData()[ nRandIndex ].get(), pRD->m_pIPAE );	
		int i = nRandIndex + 1;	
		if( i == m_IPA.GetData().size() )
			i = 0;	
		while( i != nRandIndex )
		{
			int nScore = GetRhyminess( m_IPA.GetData()[ i ].get(), pRD->m_pIPAE );
			if( nScore > nBestScore )
			{
				nBestScore = nScore;
				nBestIndex = i;
			}	
			i++;
			if( i == m_IPA.GetData().size() )
				i = 0;	
		}		

		pIPAE = m_IPA.GetData()[ nBestIndex ].get();

	}
	else 
	{	
		// new rhyme code, take the new word
		
		pIPAE = GetRandomBrainIPAE( s_N );

		rm.Add( cCode, pIPAE );
	}

	CString strWord; 
	strWord = pIPAE->m_strKey;
	strWord.MakeLower();



	CString str( vd.m_strWorkVerse );
	int nFind = str.ReverseFind(  '<' );
	str.Delete( nFind, str.Find( '>', nFind ) - nFind + 1 );
	str.Insert( nFind, strWord );

	// modify verse data
	vd.m_nNumDynamics--;
	vd.m_nSyllablesToSpare -= ( pIPAE->m_nSilben - 1 );
	vd.m_strWorkVerse = str;
	pd.m_nWorkVerse++;
}

*/

// eof - DEER.CPP

