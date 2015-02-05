// lexicon.cpp 
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "lexicon.h"

BOOL IsIPAVowel_Lex( TCHAR c )
{
	switch( c )
	{
	case 'a':  case 'A': case 'e': case 'E': case 'i': case 'I': case 'o': case 'O':
	case 'u':  case 'U': case '&': case '3': case '6': case '9': case ';':case '@': 
	case 'M': case 'Q': case 'V': case 'Y': case '{': case 'È': case 'Ë': case 'ø':
		return TRUE;
	default:
		return FALSE;
	}
}

const BOOL CSilbe::IsCleanReim( CSilbe & silbe )
{
	CString strThis( m_strPhoneText );
	CString strThat( silbe.m_strPhoneText );
	while( ! IsIPAVowel_Lex( strThis.GetAt( 0 ) ) )
		strThis.Delete( 0 );
	while( ! IsIPAVowel_Lex( strThat.GetAt( 0 ) ) )
		strThat.Delete( 0 );
	return strThis == strThat;
}

void CSilbe::Serialize( CArchive & ar )
{
	if( ar.IsLoading() )
	{
	   ar >> m_strPhoneText >>  m_bTone >> m_bFlexible >> m_bWordBreak;
	}
	else
	{
		ar << m_strPhoneText <<  m_bTone << m_bFlexible << m_bWordBreak; 
	}
}

void CWord::Serialize( CArchive & ar )
{
	if( ar.IsLoading() )
	{
	   ar >> m_strText >> m_lid >>  m_nWordType;
	   ar >> m_bPlural >>  m_bAmbig >> m_strExtra >> m_nGender >> m_bHidden >> m_bTakesAn;
	   ar >> m_nNumSilben;
	   for( int i = 0; i < m_nNumSilben; i++ )
          m_Silben[ i ].Serialize( ar );
	}
	else
	{
	   ar << m_strText << m_lid <<  m_nWordType;
	   ar << m_bPlural <<  m_bAmbig << m_strExtra << m_nGender << m_bHidden << m_bTakesAn; 
	   ar << m_nNumSilben;
	   for( int i = 0; i < m_nNumSilben; i++ )
          m_Silben[ i ].Serialize( ar );
	}
}

const LPCSTR CWord::GetWordTypeText()
{
	switch( m_nWordType )
	{
	case WORDTYPE_ADJ:
		return "Adjektiv";
	case WORDTYPE_ADV :
		return "Adverb";
	case WORDTYPE_DET:
		return "Determinator";
	case WORDTYPE_NAM:
		return "Eigenname";
	case WORDTYPE_QUE:
		return "Fragefürwort";
	case WORDTYPE_INJ:
		return "Interjektion";
	case WORDTYPE_KNJ:
		return "Konjunktion";
	case WORDTYPE_PRP:
		return "Präposition";
	case WORDTYPE_PRO:
		return "Pronomen";
	case WORDTYPE_NOU:
		return "Substantiv";
	case WORDTYPE_VRB:
		return "Verb";
	default:
		return "Unbekannt";
	}
}

const void CWord::LoadDisplayText( CString & strDisplay )
{
	if( m_strExtra.GetLength() )
		strDisplay = m_strText + " [" + m_strExtra + "]"; 
	else
		strDisplay = m_strText;
}

const BOOL CWord::GetMatch( CWord * pWord, int nMatch )
{
	switch( nMatch )
	{
	case WORDMATCH_CLEANREIM:
		return GetLastSilbe().IsCleanReim( pWord->GetLastSilbe() );
	case WORDMATCH_DIRTYREIM:
		return pWord == this;
	case WORDMATCH_ASSONANZ:
		return pWord == this;
	case WORDMATCH_ALLITERAT:
		{
		char c1  = GetSilbe( 0 ).m_strPhoneText.GetAt( 0 );
		char c2 = pWord->GetSilbe( 0 ).m_strPhoneText.GetAt( 0 );
		return c1 == c2;
		}
	default:
		return TRUE;
	}
}

CMatchChart::CMatchChart( char cid, CWord * pBase ) : 
	m_id( cid ), 
	m_pBase( pBase ) 
	{
		m_WordLists[ WORDMATCH_CLEANREIM ].SetName( "Reine Reime" );
		m_WordLists[  WORDMATCH_DIRTYREIM ].SetName( "Unreime Reime" );
		m_WordLists[ WORDMATCH_ASSONANZ ].SetName( "Assonanz" );
		m_WordLists[ WORDMATCH_ALLITERAT ].SetName( "Alliteration" );
	}

BOOL CLexicon::Compare( int n1, int n2 )
{
	int n = m_Entries[ n1 ]->m_strText.CompareNoCase( m_Entries[ n2 ]->m_strText );
	if( n )
		return ( n > 0 );
	else
		return ( m_Entries[ n1 ]->m_strExtra > m_Entries[ n2 ]->m_strExtra );
}

void CLexicon::Insertion_Sort( int * p, int nSize )
{
	int i;
	for( i = 0; i < nSize; i++ )
		p[ i ] = i;

	for( i = 1; i < nSize; i++ )
	{
		int index = p[ i ];
		int j = i;
		while( (j > 0) && Compare( p[ j - 1 ], index ) )
		{
			p[ j ] = p[ j - 1];
			j--;	
		}
		p[ j ] = index;
	}
}

void CLexicon::Serialize( CArchive & ar )
{
	if( ar.IsLoading() )
	{
       ar >> m_nVersion >>  m_nDefaultFilter >> m_lSeed;
	   int n;
	   ar >> n;
	   for( int i = 0; i < n; i++ )
	   {
	       CWord * pWord = new CWord();	
           pWord->Serialize( ar );
           std::auto_ptr< CWord > sp( pWord );
           m_Entries.push_back( sp ); 
	   }	 
	}
	else
	{
       ar << m_nVersion <<  m_nDefaultFilter << m_lSeed;
	   int n = GetNumWords();

	   ar << n;

	   // implement a sort to serialize words in alphabetical order...
		int * p = new int[ n ];
		Insertion_Sort( p, n );
		for( int i = 0; i < n; i++ )
           m_Entries[ p[ i ] ]->Serialize( ar ); 
		delete p;
	}
}

void CLexicon::AddWord( CWord * pWord )
{
	pWord->m_lid = m_lSeed++;
    std::auto_ptr< CWord > sp( pWord );
    m_Entries.push_back( sp ); 
}

CWord * CLexicon::FindWord( long lid )
{
	int n = GetNumWords();
	while( n > 0 )
		if( lid == m_Entries[ --n ]->m_lid )
			return m_Entries[ n ].get();
	return NULL;
}

void CLexicon::DeleteWord( long lid )
{
	int n = GetNumWords();
	while( n > 0 )
		if( lid == m_Entries[ --n ]->m_lid )
		{
			m_Entries[ n ] = m_Entries.back();
			m_Entries.pop_back();
			return;
		}
}

CMatchChart * CLexicon::CreateMatchChart( char c, CWord * pBase )
{
	ASSERT( pBase );
	CMatchChart * pChart = new CMatchChart( c, pBase );
	int n = GetNumWords();
	for( int i = 0; i < n; i++ )
		for( int j = 0; j < WORDMATCH_MAX; j++ )
			if( pBase->GetMatch( GetWord( i ), j ) )
			{
				CWordRef wr( GetWord( i ) );
				pChart->AddWordRef( wr, j );
			}
	return pChart;
}

// EOF - lexicon.cpp

