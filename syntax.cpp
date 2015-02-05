// SYNTAX.CPP 

#include "stdafx.h"
#include "generic.h"
#include "phonetics.h"
#include "syntax.h"
#include "wn/wn.h"

const int SynPOS[ int( s_ERROR ) ] = { NOUN, NOUN,  VERB, VERB, VERB, VERB, VERB, ADJ, ADJ, ADJ, ADV };

const int SynMinSyllables[ int( s_ERROR ) ] =  { 1, 1, 1, 1, 2, 1, 1, 1, 2, 2, 2 };

int GetSynPOS( enumSyn eSyn ) { return SynPOS[ int( eSyn ) ]; }

int GetSynMinSyllables( enumSyn eSyn ) { return SynMinSyllables[ int( eSyn ) ]; }

enumSyn DetectSyn( char c1, char c2 )
{
	switch( c1 )
	{
	case 'a':
		switch ( c2 )
		{
		case ' ': return s_A;
		case 'c': return s_AC ;
		case 's': return s_AS;
		default: ASSERT( FALSE );
		}
		break;
	case 'n':
		switch( c2 )
		{
		case ' ': return s_N;
		case 'p': return s_NP;
		default: ASSERT( FALSE );
		}
		break;
	case 'r': return s_R;
	case 'v':
		switch( c2 )
		{
		case ' ':  return s_V;
		case 'a': return s_VA;
		case 'p': return s_VP;
		case 's': return s_VS;	
		case 'g': return s_VG;
		default: ASSERT( FALSE );
		}
		break;
		default: ASSERT( FALSE );
	}
	return s_ERROR;
}

void CFormExc::Clear()
{
	m_Adjs.clear();
	m_Nouns.clear();
	m_Verbs.clear();		
}

void CFormExc::Serialize( CArchive & ar )
{
	m_Adjs.Serialize( ar );
	m_Nouns.Serialize( ar );
	m_Verbs.Serialize( ar );
}

CIPAElement * CFormExc::InflectWord( CString & strBase, enumSyn eSyn )
{
	CString strWord;
	switch( eSyn )
	{
		case s_N:
		case s_V:
		case s_A:
		case s_R: strWord = strBase; break;				
		case s_NP: NounPlural( strBase, strWord ); break;
		case s_VS: VerbThirdSingular( strBase, strWord ); break;
		case s_VG: VerbGerund( strBase, strWord ); break;
		case s_VA: VerbPast( strBase, strWord ); break;
		case s_VP: VerbPerfect( strBase, strWord ); break;
		case s_AC: AdjCompare( strBase, strWord ); break;
		case s_AS: AdjSuper( strBase, strWord ); break;
		default: ASSERT( FALSE );
	}
	strWord.MakeUpper();
	return m_IPA.GetData().SortedFind( strWord );
}

void CFormExc::AdjCompare( CString & strBase, CString & strResult )
{
	CString str( strBase );
	str.MakeUpper();
	CAdjExcElement * p = m_Adjs.SortedFind( str );
	if( p )
		strResult = p->m_strCompare;
	else
		strResult = "more " + strBase;
	strResult.MakeLower();
}

void CFormExc::AdjSuper( CString & strBase, CString & strResult )
{
	CString str( strBase );
	str.MakeUpper();
	CAdjExcElement * p = m_Adjs.SortedFind( str );
	if( p )
		strResult = p->m_strCompare;
	else
		strResult = "more " + strBase;
	strResult.MakeLower();
}
	
void CFormExc:: NounPlural( CString & strBase, CString & strResult )
{
	CString str( strBase );
	str.MakeUpper();
	CNounExcElement * p = m_Nouns.SortedFind( str );
	if( p )
		strResult = p->m_strPlural;
	else
		InflectS( strBase, strResult );
	strResult.MakeLower();
}

void CFormExc::VerbThirdSingular( CString & strBase, CString & strResult )
{
	InflectS( strBase, strResult );
	strResult.MakeLower();
}

void CFormExc::VerbPast( CString & strBase, CString & strResult )
{
	CString str( strBase );
	str.MakeUpper();
	CVerbExcElement * p = m_Verbs.SortedFind( str );
	if( p )
		strResult = p->m_strPast;
	else
		InflectED( strBase, strResult );
	strResult.MakeLower();
}

void CFormExc::VerbPerfect( CString & strBase, CString & strResult )
{
	CString str( strBase );
	str.MakeUpper();
	CVerbExcElement * p = m_Verbs.SortedFind( str );
	if( p )
		p->GetPerfect( strResult );
	else
		InflectED( strBase, strResult );
	strResult.MakeLower();
}

void CFormExc::VerbGerund( CString & strBase, CString & strResult )
{
	m_IPA.BuildGerund( strBase, strResult ); // sometimes uses phonetic determination
	strResult.MakeLower();
}

// eof - SYNTAX.CPP
