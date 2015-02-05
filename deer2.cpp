// DEER2.CPP - Advanced Compiler

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

#define MAX_INSERTS 10

#define CHUNK_IA "<a>"
#define CHUNK_D1 "<1>"
#define CHUNK_D2 "<2>"
#define COUNT_DELTA 2

class CBlockChunker : public CSimpleChunker
{
public:
	CBlockChunker() : m_nNumDynamics( 0 ) { }

	int GetDynamics() { return m_nNumDynamics; }

protected:
	int m_nNumDynamics;

	void AddDynamic( enumSyn eSyn ) 
	{ 
		Add( ( GetSynMinSyllables( eSyn ) == 1 ) ? CHUNK_D1 : CHUNK_D2 );  
		m_nNumDynamics++; 
	}

	virtual BOOL IsValidChar( char c )
	{
		return ( ( c >= 'a' ) && ( c <= 'z' ) ) || ( c == '-' );
	}

	virtual void InterruptPoint( LPCSTR szBuffer, int & nLoopIndex )
	{
		if( szBuffer[ nLoopIndex ] == '<' )
		{
			char c1 = szBuffer[ nLoopIndex + 1 ];
			if( c1 == 'i' )
				Add( "<a>" ); 
			else
				AddDynamic( DetectSyn( c1, szBuffer[ nLoopIndex + 2 ] ) );
			while( szBuffer[ nLoopIndex ] != '>' )
				nLoopIndex++;
			nLoopIndex++;
		}
	}

};

void CDeerCompiler::GetSyllables( int & nMin, int & nMax, CStringArray & sa, int nDepth )
{
	nMin = 0;
	nMax = 0;
	int nUntil = sa.GetSize();
	if ( ( nDepth != -1 ) && ( nDepth < nUntil ) )
		nUntil = nDepth;
	for( int i = 0; i < nUntil; i++ )
	{
		CString str = sa.GetAt( i );
		if( str.GetAt( 0 ) == '<' )
		{
			switch(  str.GetAt( 1 ) )
			{
			case 'a':
				nMin++;
				nMax++;
				break;
			case '1':
				nMin++;
				nMax += ( 1 + COUNT_DELTA );
				break;
			case '2':
				nMin += 2;
				nMax += ( 2 + COUNT_DELTA );
				break;
			default:
				ASSERT( FALSE );
			}
		}
		else
		{
			int nCount = m_IPA.SyllableCount( str );
			nMax += nCount;
			nMin += nCount;
		}
	}
} 

int CDeerCompiler::GetSeparationPoint( CStringArray & sa, CString & strTrial, int nWord )
{
	int nBreak = 0;
	for( int i = 1; i < nWord; i++ )
	{
		CString strWord( sa.GetAt( i ) );
		if( strWord.GetAt( 0 ) == '<' )
		{
			nBreak = strTrial.Find( '<', nBreak );
			if( i < nWord - 1 )
				nBreak = strTrial.Find( '>', nBreak );
		}
		else
			nBreak = strTrial.Find(  strWord, nBreak );
	}
	return nBreak;
}

BOOL CDeerCompiler::GatherBlock() 
{
	int nVerseIndex = 0;
	int nTotalVerses = pd.GetWorkingStanza().m_nVerses;

	int nAlarmCount = 0; // for avoiding infinite loops...

	CString strKeep;
	int nMinSyllables;
	int nMaxSyllables;

	while( nVerseIndex <  nTotalVerses )
	{
		BOOL bVerseDone = FALSE;
		while( ! bVerseDone )
		{
			nAlarmCount++;
			if( nAlarmCount == 50 )
				return FALSE;

			int nTargetSyllables = pd.GetVerse( nVerseIndex ).m_nTargetSyllables;

			CString strTrial( strKeep );

			if( strTrial.GetLength() )	
				strTrial += ' ' + m_SF.m_Text.RandElement()->m_strKey;
			else
				strTrial = m_SF.m_Text.RandElement()->m_strKey;

			CBlockChunker bc;
			bc.Chunk( strTrial );

			GetSyllables( nMinSyllables, nMaxSyllables, bc );
		
			if( ( nTargetSyllables >= nMinSyllables ) && 
				(nTargetSyllables <= nMaxSyllables ) ) // perfect match...take segment
			{
				pd.GetVerse( nVerseIndex++ ).m_strWorkVerse = strTrial;
				strKeep.Empty();
				bVerseDone = TRUE;
			}

			if( pd.m_bAllowSplit &&
				( nVerseIndex + 1 < nTotalVerses ) &&
				( nTargetSyllables < nMinSyllables ) && 
				( ( ( nTargetSyllables * 3 )  / 2 ) >= nMinSyllables ) ) // split segment....
			{
				// tricky, split the segment....
				int nWord = bc.GetSize();	
				BOOL bCanSplit = FALSE;
				BOOL bSplitDone = FALSE;
				while( ! bSplitDone )
				{
					nWord--;
					GetSyllables( nMinSyllables, nMaxSyllables, bc, nWord - 1 );
					bSplitDone = ( nTargetSyllables >= nMinSyllables );
					bCanSplit = TRUE; // not always...
				}
				if( bCanSplit )
				{
					int nBreak = GetSeparationPoint( bc, strTrial, nWord );
					pd.GetVerse( nVerseIndex++ ).m_strWorkVerse = strTrial.Left( nBreak - 1 );
					strKeep = strTrial.Mid( nBreak );
					bVerseDone = TRUE;
				}
			}
	
			if( nTargetSyllables > nMaxSyllables ) // too small, keep but continue...
					strKeep = strTrial;
		}
	}

	bool bOK = TRUE;
	for( int i = 0; bOK && ( i < nTotalVerses ); i++ )
	{
		CVerseData & vd = pd.GetVerse( i );
		CBlockChunker bc;
		bc.Chunk( vd.m_strWorkVerse );
		vd.m_nNumDynamics = bc.GetDynamics();
		bOK &= ( ( bc.GetAt( bc.GetSize() - 1 ) == "<1>" ) ||  ( bc.GetAt( bc.GetSize() - 1 ) == "<2>" ) ); 
		// make sure verses end with plug

//		CString str[ 12 ];  // unrem for debugging 
//		for( int k = 0; k < bc.GetSize(); k++)
//			str[ k ] = bc.GetAt( k );

		if( bOK )
		{
			int nTargetSyllables = vd.m_nTargetSyllables;
			GetSyllables( nMinSyllables, nMaxSyllables, bc );
			vd.m_nSyllablesToSpare = nTargetSyllables - nMinSyllables;
			bOK &= ( ( nTargetSyllables >= nMinSyllables ) &&
				( nTargetSyllables <= nMaxSyllables ) ); // make sure syllable count is correct
		}

		for( int j = 0; bOK && ( j < nTotalVerses ); j++ ) 
			if( i < j ) // (2,3) but not (3, 2)
				bOK &= ( vd.m_strWorkVerse != pd.GetVerse( j ).m_strWorkVerse ); 
			// make sure all verses are unique.
	}
	return bOK;
}

void CDeerCompiler::WorkingDisplay( CString & strPoem )
{	
	CString strTemp( pd.GetVerse( 0 ).m_strWorkVerse );
	for( int i = 1; i < pd.GetWorkingStanza().m_nVerses; i++ )
	strTemp += "\n" + pd.GetVerse( i ).m_strWorkVerse;
	pd.GetWorkingStanza().m_strBlock = strTemp;
	strPoem = pd.m_strPlug;
	Display( strPoem );
}

void CDeerCompiler::Display( CString & strPoem )
{
	for( int i = 0; i < pd.m_nStanzas; i++ )
	{
		char s[ 200 ];
		wsprintf( s, "<block%ld>", i );
		strPoem.Replace( s, pd.m_sd[ i ].m_strBlock ); 
	}
}

void CDeerCompiler::StateAdvancer( enumPS epsNextState, BOOL bBlockDone )
{
	if( bBlockDone )
	{	
		pd.m_nWorkVerse = 0;
		if( pd.m_nWorkBlock == pd.m_nStanzas - 1 )
		{
			pd.m_nWorkBlock = 0;
			pd.m_ePS = epsNextState;
		}
		else
			pd.m_nWorkBlock++;
	}
}

void CDeerCompiler::VerseDone( enumPS epsNextState )
{
	StateAdvancer( epsNextState,  pd.m_nWorkVerse == pd.GetWorkingStanza().m_nVerses );
}

char GetSonnettCode( int nOption, int nStanza, int nVerse )
{
	switch( nOption )
	{
	case 0:
	default:
		switch( nStanza )
		{
		case 0:
			return ( nVerse % 2 == 0) ? 'a' : 'b';
		case 1:
			return ( nVerse % 2 == 0) ? 'c' : 'd';
		case 2:
			return ( nVerse % 2 == 0) ? 'e' : 'f';
		case 3:
			return 'g';
		default:
			ASSERT( FALSE );
			return 'x';
		}
	}
}

void CDeerCompiler::InitCompiler()
{
	// decide if titled 
	CRandPick kTitle;
	kTitle.Fill( FALSE, 2 );
	kTitle.Fill( TRUE, 7 );
	pd.m_bTitle = kTitle.Pick();

	// allow split
	pd.m_bAllowSplit = TRUE;

	// decide on mixed- vs. lowercase
	CRandPick kLower;
	kLower.Fill( FALSE, 5 );
	kLower.Fill( TRUE, 2 );
	pd.m_bLower = kLower.Pick();

	// decide on poem type
	CRandPick kPoem;
	kPoem.Fill( POEM_RANDOM, 15 );
	kPoem.Fill( POEM_SONNETT, 2 );
	kPoem.Fill( POEM_HAIKU, 70 );
	pd.m_nPoemType = kPoem.Pick(); 

	switch( pd.m_nPoemType )
	{      
		case POEM_RANDOM:
			{
				pd.m_nStanzas = 2 + RandVal( 3 );
				pd.m_eAlgo = algo_Deer;

				CRandPick kRhyme;
				kRhyme.Fill( int( rc_NoRhyme ), 1 );
				kRhyme.Fill( int( rc_CrossRhyme ),  4  );
				kRhyme.Fill( int( rc_BlockRhyme ), 2 );
				kRhyme.Fill( int( rc_PairRhyme ), 3 );
				pd.m_eRC = enumRC( kRhyme.Pick() );


				CRandPick kFixedSL;
				kFixedSL.Fill( FALSE, 7 );
				kFixedSL.Fill( TRUE, 1 );
				pd.m_bFixedStanzaLength = kFixedSL.Pick();

				CRandPick kVersen;		
				kVersen.Fill( 4, 10 );
				kVersen.Fill( 3, 1 );
				kVersen.Fill( 6, 1 );
				kVersen.Fill( 2, 1 );
				int nVerses = kVersen.Pick();

				CRandPick kFixedSC;
			kFixedSC.Fill( FALSE, 1 );
				kFixedSC.Fill( TRUE, 7 );
				pd.m_bFixedSyllableCount = kFixedSC.Pick();

				CRandPick kSilben;
				kSilben.Fill( 6, 46 );
				kSilben.Fill( 8, 6 );
				kSilben.Fill( 10,  4 );
				int nSilb = kSilben.Pick();

				for( int i = 0; i < pd.m_nStanzas; i++ )
				{
					if( pd.m_bFixedStanzaLength )
						pd.m_sd[ i ].m_nVerses = nVerses;
					else
					{
						CRandPick kVersenD;
						kVersenD.Fill( 4, 60 );
						kVersenD.Fill( 3, 3 );
						kVersenD.Fill( 6, 1 );
						kVersenD.Fill( 2, 5 );
						kVersenD.Fill( 1, 5 );
						pd.m_sd[ i ].m_nVerses = nVerses;
					}
		
					for( int j = 0; j < nVerses; j++ )
					{
						if( pd.m_bFixedSyllableCount )
							pd.m_sd[ i ].m_vd[ j ].m_nTargetSyllables = nSilb;
						else
						{
							CRandPick kSilbD;
							kSilbD.Fill( nSilb, 20 );
							kSilbD.Fill( 6, 3 );
							kSilbD.Fill( 8, 1 );
							kSilbD.Fill( 10, 5 );
							pd.m_sd[ i ].m_vd[ j ].m_nTargetSyllables = kSilbD.Pick();
						}

						char c;
						switch( pd.m_eRC )
						{
						case rc_NoRhyme: c = 'x'; break;
						case rc_CrossRhyme: c = char (  2 *  i + ( j % 2 ) + byte(  'a' ) ); break;
						case rc_PairRhyme: c = char( 2 * i + ( j / 2 ) + byte( 'a' ) ); break;
						case rc_BlockRhyme: c = char( 2 * i + ( j && ( j != nVerses - 1) ) + byte( 'a' ) ); break;
						default: ASSERT( FALSE );
						}

						pd.m_sd[ i ].m_vd[ j ].m_r.m_cCode = c; 
					}
				}
			}
		break;
		case POEM_SONNETT:
			{
				pd.m_nStanzas = 4;
				pd.m_bFixedStanzaLength = FALSE;
				pd.m_bFixedSyllableCount = TRUE; // ???
				pd.m_eAlgo                  = algo_Deer; /// ???
				pd.m_eRC		              =  rc_SonnettRhyme;
				pd.m_sd[ 0 ].m_nVerses            = 4;
				pd.m_sd[ 1 ].m_nVerses            = 4;
				pd.m_sd[ 2 ].m_nVerses			= 4;
				pd.m_sd[ 3 ].m_nVerses            = 2;
				for( int i = 0; i < pd.m_nStanzas; i++ )
					for( int j = 0; j < pd.m_sd[ i ].m_nVerses; j++ )
					{
						pd.m_sd[ i ].m_vd[ j ].m_nTargetSyllables = 10;
						pd.m_sd[ i ].m_vd[ j ].m_r.m_cCode = GetSonnettCode( 0, i, j );
					}
			}
			break;
		case POEM_HAIKU:
			{
				pd.m_nStanzas								 = 1;
				pd.m_bFixedStanzaLength			 = TRUE;
				pd.m_bFixedSyllableCount		 = FALSE; 
				pd.m_eAlgo									     = algo_Deer;
				pd.m_eRC								          = rc_NoRhyme;
				pd.m_sd[ 0 ].m_nVerses                  = 3;
				pd.m_sd[ 0 ].m_vd[ 0 ].m_nTargetSyllables = 5;
				pd.m_sd[ 0 ].m_vd[ 1 ].m_nTargetSyllables = 7;
				pd.m_sd[ 0 ].m_vd[ 2 ].m_nTargetSyllables = 5;
				pd.m_sd[ 0 ].m_vd[ 0 ].m_r.m_cCode = 'x';
				pd.m_sd[ 0 ].m_vd[ 1 ].m_r.m_cCode = 'x';
				pd.m_sd[ 0 ].m_vd[ 2 ].m_r.m_cCode = 'x';
			}
			break;
	}

	for( int i = 0; i < pd.m_nStanzas; i++ )
	{
		int nVerses = pd.m_sd[ i ].m_nVerses;

		char s[ 200 ];
		CString strTemp( "<x>" );

		for( int j = 1; j < nVerses; j++ )
		{		
			wsprintf
				( 
				s, 
				"<x>\n<verse%ld.%ld  rhyme_code=%c syllables=%ld>", 
				i, 
				nVerses - j, 
				pd.m_sd[ i ].m_vd[ nVerses - j ].m_r.m_cCode, 
				pd.m_sd[ i ].m_vd[ nVerses - j ].m_nTargetSyllables 
				);
			strTemp.Replace( "<x>", s );
		}
		wsprintf
			( 
			s, 
			"<verse%ld.0 rhyme_code=%c syllables=%ld>",
			i, 
			pd.m_sd[ i ].m_vd[ 0 ].m_r.m_cCode, 
			pd.m_sd[ i ].m_vd[ 0 ].m_nTargetSyllables
			);
		strTemp.Replace( "<x>", s );
		pd.m_sd[ i ].m_strBlock = strTemp;
	}

	rm.m_nTotal = 0;
	pd.m_nWorkBlock = 0;
	pd.m_nWorkVerse = 0;
	pd.m_ePS = ps_Expand;
}

void AddInfo( CString & strPoem, LPCSTR szInfo )
{
	char sz[ 100 ];
	wsprintf( sz, "<info>\n<%s>", szInfo );
	strPoem.Replace( "<info>", sz );
}

void CDeerCompiler::Stage_New( CString & strPoem )
{
	InitCompiler();

	if(  pd.m_bTitle )
		strPoem.Replace( "<poem>", "\n<title>\n\n<body> \n\n<info>");
	else
		strPoem.Replace( "<poem>", "\n<body>\n\n<info>" );

	if( pd.m_bAllowSplit )
		AddInfo( strPoem, "allow_enjambement" );

	if( pd.m_bFixedStanzaLength )
		AddInfo( strPoem, "fixed_stanza" );

	if( pd.m_bFixedSyllableCount )
		AddInfo( strPoem, "fixed_syllable_count" );

	if( pd.m_bLower )
		AddInfo( strPoem, "lowercase" );
	
	switch( pd.m_nPoemType )
	{
		case POEM_RANDOM:
		AddInfo( strPoem, "poem_random" );
		break;
		case POEM_SONNETT:
		AddInfo( strPoem, "poem_sonnett" );
		break;
		case POEM_HAIKU:
		AddInfo( strPoem, "poem_haiku" );
		break;
	}
	
	switch( pd.m_eRC )
	{	
	case rc_NoRhyme: AddInfo( strPoem, "rc_no_rhyme" );	break;
	case rc_CrossRhyme: AddInfo( strPoem, "rc_cross_rhyme" ); break;
	case rc_PairRhyme: AddInfo( strPoem, "rc_pair_rhyme" ); break;
	case rc_BlockRhyme: AddInfo( strPoem, "rc_block_rhyme" ); break;
	case rc_SonnettRhyme: break;
	}

	switch( pd.m_eAlgo )
	{
		case algo_Deer:
		AddInfo( strPoem, "algo_deer" );
		break;
		default:
		AddInfo( strPoem, "algo_deer" );
		break;
	}

	for( int i = 1; i < pd.m_nStanzas; i++ )
	{
		char sz[ 100 ];
		wsprintf( sz,  "<body>\n\n<block%ld>", pd.m_nStanzas - i  );
		strPoem.Replace( "<body>", sz );
	}
	strPoem.Replace( "<body>", "<block0>" );

	pd.m_strPlug = strPoem;
}

void CDeerCompiler::Stage_Expand( CString & strPoem )
{
	Display( strPoem );
	pd.m_ePS = ps_Gather;
}

void CDeerCompiler::Stage_Gather( CString & strPoem )
{
	BOOL bOK = GatherBlock();
	WorkingDisplay( strPoem );
	if( bOK )
		BlockDone( ps_Balance );
}

void CDeerCompiler::Stage_Balance( CString & strPoem )
{	
	CVerseData & vd = pd.GetWorkingVerse();
	CString str( vd.m_strWorkVerse );
	int nDynamics = vd.m_nNumDynamics;
	
	ASSERT( nDynamics <= MAX_INSERTS );

	// set up arrays
	char cCodes[ MAX_INSERTS ];
	enumSyn eSyns[ MAX_INSERTS ];
	CString strDefaults[ MAX_INSERTS ];

	// set up loop
	int nFind = 0;
	for( int i = 0; i < nDynamics; i++ )
	{
		nFind = str.Find( '<', nFind );     
		while ( str.GetAt( nFind + 1 ) == 'i' )
			nFind = str.Find( '<', nFind + 3 );	
		
		cCodes[ i ] = ( i == nDynamics - 1 ) ? vd.m_r.m_cCode : 'x';
		eSyns[ i ] = DetectSyn( str.GetAt( nFind + 1), str.GetAt( nFind + 2 ) );

		nFind = str.Find( QUOTE_CHAR, nFind + 3 );
		int nEnd = str.Find( QUOTE_CHAR, nFind + 1 );
		strDefaults[ i ] = str.Mid( nFind + 1, nEnd - nFind - 1 );
	}

	// scramble array
	int nScramble[ MAX_INSERTS ];
	ScrambleVector( nDynamics, nScramble );

	// result array
	CIPAElement * pResults[ MAX_INSERTS ];
	
	// spare syllables
	int nToSpare = vd.m_nSyllablesToSpare;

	// word gathering loop in scrambled order
	for( i = 0; i < nDynamics; i++ )
	{
		int nSIndex = nScramble[ i ];
		int nMin = GetSynMinSyllables( eSyns[ nSIndex ] );;
		pResults[ nSIndex ] = GetInsertionWord
														( 	
														eSyns[ nSIndex ], 
														cCodes[ nSIndex ],
														strDefaults[ nSIndex ],
														nMin + ( nToSpare / ( nDynamics - i ) ), 		
														nMin + nToSpare	
														);
		nToSpare += ( nMin - pResults[ nSIndex ]->m_nSilben );
	}

	// text insertion loop
	for( i = 0; i < nDynamics; i++ )
	{
		CString strWord( pResults[ i ]->m_strKey );
		strWord.MakeLower();

		int nFind = str.Find( '<' );     // skip over indefinite article markers....
		while ( str.GetAt( nFind + 1 ) == 'i' )
			nFind = str.Find( '<', nFind + 3 );

		str.Delete( nFind, str.Find( '>', nFind ) - nFind + 1 );
		str.Insert( nFind, strWord );
	}

	if( FALSE ) // m_nDebugFlag )
	{
		CString strTemp( str );
		strTemp.Replace( "<ia>", "a" );
		int nCount = 0;
		CBlockChunker bc;
		bc.Chunk( strTemp );
		for( int i = 0; i < bc.GetSize(); i++)
			nCount += m_IPA.SyllableCount( bc.GetAt( i ) );
		char sz[ 20 ];
		wsprintf( sz, " (%ld)", nCount );
		str += sz;	
//		ASSERT( vd.m_nTargetSyllables == nCount );
	}

	// finish up
	pd.GetWorkingVerse().m_strWorkVerse = str;
	pd.m_nWorkVerse++;
	WorkingDisplay( strPoem );
	VerseDone( pd.m_bTitle ? ps_Title : ps_Clean );
}

void CDeerCompiler::Stage_Title( CString & strPoem )
{
	CString strTitle( m_SF.m_Title.RandElement()->m_strKey );
	int nFind = strTitle.Find( '<' );
	while( nFind != -1 )
	{
		while ( strTitle.GetAt( nFind + 1 ) == 'i' )
			nFind = strTitle.Find( '<', nFind + 3 );
		enumSyn eSyn = DetectSyn( strTitle.GetAt( nFind + 1), strTitle.GetAt( nFind + 2 ) );
		CIPAElement * pIPAE = NULL;
		while( ! pIPAE )
			pIPAE = GetRandomBrainIPAE( eSyn );
		CString strWord( pIPAE->m_strKey );
		strWord.MakeLower();
		strTitle.Delete( nFind, strTitle.Find( '>', nFind ) - nFind + 1 );
		strTitle.Insert( nFind, strWord );	
		nFind = strTitle.Find( '<', nFind );     
	}
	m_IPA.InsertIndefiniteArticles( strTitle );
	pd.m_strPlug.Replace( "<title>", strTitle );
	WorkingDisplay( strPoem );
	pd.m_ePS = ps_Clean;
}

void CDeerCompiler::Stage_Clean( CString & strPoem )
{
	int n = pd.GetWorkingStanza().m_nVerses;
	for( int i = 0; i < n; i++ )
		m_IPA.InsertIndefiniteArticles( pd.GetVerse( i ).m_strWorkVerse );
	WorkingDisplay( strPoem );		
	BlockDone( ps_Finish );
}

void CDeerCompiler::Stage_Finish( CString & strPoem )
{
	int nFind = strPoem.Find( "<info>" );
	if( nFind > 0 )
		strPoem = strPoem.Left( nFind );
}

BOOL CDeerCompiler::Run( CString & strPoem ) 
{
	if( strPoem.Find( "<" ) == -1 ) // if all normal text, poem is done...
		return FALSE;

	if( strPoem ==  "<poem>"  ) // if "<poem>", then we are just starting...
		pd.m_ePS = ps_New;

	switch( pd.m_ePS )
	{
		case ps_New: Stage_New( strPoem ); break;
		case ps_Expand: Stage_Expand( strPoem ); break;
		case ps_Gather: Stage_Gather( strPoem ); break;
		case ps_Balance: Stage_Balance( strPoem ); break;	
		case ps_Title: Stage_Title( strPoem ); break;
		case ps_Clean: Stage_Clean( strPoem ); break;
		case ps_Finish: Stage_Finish( strPoem ); break;
	}

	return TRUE;
}

// eof - DEER2.CPP

