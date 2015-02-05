// PHONETICS.CPP

#include "stdafx.h"
#include "generic.h"
#include "phonetics.h"
#include "wn/wn.h"

const LPCSTR POSLookupName[ SATELLITE ] =
	{
	NULL,
	LOOKUP_NOUN,
	LOOKUP_VERB,
	LOOKUP_ADJ,
	LOOKUP_ADV
	};

LPCSTR GetPOSLookupName( int nPOS )
{
	ASSERT( ( nPOS >= NOUN ) && ( nPOS <= ADV ) );
	return POSLookupName[ nPOS ];
}

BOOL IsIPAVowel( char cIPASound )
{
	switch( cIPASound )
	{
		case 'a':
		case '&':
		case 'V':
		case 'O':
		case 'E':
		case '3':
		case 'e':
		case 'I':
		case 'i':
		case '@':
		case 'U':
		case 'u':
		case ';':
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL IsStressMarker( char cIPA )
{
	return ( ( cIPA >= '0' ) && ( cIPA <= '2' ) );
}

BOOL IsPreRCons( char cIPASound )
{
	switch( cIPASound )
	{
		case 'b':
		case 'd':
		case 'f':
		case 'g':
		case 'k':
		case 'p':
		case 'S':
		case 't':
		case 'T':
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL IsPreLCons( char cIPASound )
{
	switch( cIPASound )
	{
		case 'b':
		case 'f':
		case 'g':
		case 'k':
		case 'p':
		case 's':
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL IsPostSCons( char cIPASound )
{
	switch( cIPASound )
	{
		case 'f':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'p':
		case 't':
		case 'w':
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL IsPreJCons( char cIPASound )
{
	switch( cIPASound )
	{
		case 'b':
		case 'f':
		case 'g':
		case 'Z':
		case 'k':
		case 'm':
		case 'p':
		case 'v':
		case 'w':
			return TRUE;
		default:
			return FALSE;
	}
}

BOOL IsPreWCons( char cIPASound )
{
	switch( cIPASound )
	{
		case 'd':
		case 'k':
		case 's':
		case 't':
			return TRUE;
		default:
			return FALSE;
	}
}

CDWordArray & CIPAManager::GetLookup( LPCSTR szLookupName )
{
	CIPALookupElement  * p = m_Lookups.Find( CString( szLookupName ) );
	ASSERT( p );
	return p->m_dwa;
}

CDWordArray & CIPAManager::GetLookup( int nPOS ) 
{ 
	return GetLookup( GetPOSLookupName( nPOS ) ); 
}

CIPAElement * CIPAManager::RandomLookup( LPCSTR szLookupName )
{
	CDWordArray & dwa = GetLookup( szLookupName );
	DWORD  dw = dwa.GetAt( RandVal( dwa.GetSize() ) );
	return m_Data[ dw ].get();
}

CIPAElement * CIPAManager::IndexLookup( LPCSTR szLookupName, int nIndex )
{
	CDWordArray & dwa = GetLookup( szLookupName );
	ASSERT( nIndex >= 0 );
	ASSERT( nIndex < dwa.GetSize() );
	DWORD  dw = dwa.GetAt( nIndex );
	return m_Data[ dw ].get();
}

CIPAElement * CIPAManager::IndexLookup( int nPOS, int nIndex ) 
{ 
	return IndexLookup( GetPOSLookupName( nPOS ), nIndex ); 
}

void CIPAManager::InsertIndefiniteArticles( CString & str )
{
	int nMark = str.Find( "<ia>" );
	while( nMark >= 0 )
	{
		CString strTemp( str.Mid( nMark + 5 ) );
		int nEnd = strTemp.FindOneOf( " .'\"?!,:" );
		CString strWord( nEnd == - 1 ? strTemp : strTemp.Left( nEnd ) );
		str.Delete( nMark, 4 );
		if( IndefiniteArticleAn( strWord ) )
			str.Insert( nMark, "an" );
		else
			str.Insert( nMark, 'a' );
		nMark = str.Find( "<ia>" );
	}
}

BOOL CIPAManager::IndefiniteArticleAn( CString & strWord )
{
	CString str( strWord );
	str.MakeUpper();
	CIPAElement  * p = GetData().SortedFind( str );
	if( p )
		return IsIPAVowel( p->m_strTrans.GetAt( 0 ) );
	else
		return FALSE;
} 

int CIPAManager::SyllableCount( CString & strWord )
{
	CString str( strWord );
	str.MakeUpper();
	CIPAElement  * p = GetData().SortedFind( str );
	if( p )
		return p->m_nSilben;
	else
		return 0;
}

void CIPAManager::BuildGerund( CString & strVerb, CString & strResult )
{
	CString strBase( strVerb );
	int nLen = strBase.GetLength();
	char cLast = strBase.GetAt( nLen - 1 );
	char cNLast = strBase.GetAt( nLen - 2 );
	char cNNLast = ( nLen > 2 ) ? strBase.GetAt( nLen - 3 ) : 'b';
	switch( cLast )
	{
	case 'e':
		switch( cNLast )
		{
		case 'a':
		case 'e':
		case 'o':
		case 'u':
		case 'y':
			break;
		case 'i':
			strBase.Delete( nLen - 2, 2 );
			strBase += 'y';
		default:
			strBase.Delete( nLen - 1 );
			break;
		}
		break;
	case 'b':
	case 'd':
	case 'f':
	case 'g':
	case 'k':
	case 'l':
	case 'm':
	case 'n':
	case 'p':
	case 'r':
	case 's':
	case 't':
	case 'z':
		if( IsEnglishVowel( cNLast ) && ! IsEnglishVowel( cNNLast ) )
		{
			BOOL bAdd = ( SyllableCount( strBase ) == 1 );
			if( ! bAdd )
			{
				CString strFind( strBase );
				strFind.MakeUpper();
				CIPAElement  * pIPA = GetData().SortedFind( strFind );
				if( pIPA )
				{
					CString strIPA( pIPA->m_strTrans );
					int nL = strIPA.GetLength();
					BOOL bGo = TRUE;
					for( int i = nL - 1; bGo; i-- )
					{
						char c = strIPA.GetAt( i );
						if( IsStressMarker( c ) )
						{
							bGo = FALSE;
							bAdd = ( c >= '1' );
						}
					}
				}
			}
			if( bAdd )
				strBase += cLast;
		}
		break;
	default:
		break;
	}
	strResult = strBase + "ing";
}

BOOL TrickyDoubleConsTest( char c1, char c2, LPCSTR szPreConsonants )
{
	BOOL bPreC1		=  ( _tcschr( szPreConsonants, c1 ) != NULL ); 
	BOOL bPreC2	    =  ( _tcschr( szPreConsonants, c2 ) != NULL );
	BOOL bStressStar1 = IsStressMarker( c1 ) || ( c1 == '*');
	BOOL bStressStar2 = IsStressMarker( c2 ) || ( c2 == '*' );
	return ( ( bPreC1 || bPreC2 ) && ( bPreC1 || bStressStar1 ) && ( bPreC2 || bStressStar2 ) );
}

int GetRhyminess( CIPAElement * pIPAE1, CIPAElement * pIPAE2 )
{
	ASSERT( pIPAE1 ); 
	CString & strTrans1 = pIPAE1->m_strTrans;

	ASSERT( pIPAE2 );
	CString & strTrans2 = pIPAE2->m_strTrans;

	if( strTrans1 == strTrans2 )
		return RHYME_IDENTICAL;

	int n1 = strTrans1.GetLength() - 1;	
	int n2 = strTrans2.GetLength() - 1;

	char c1, c2, cPost, cLast;

	// cLast needed possibly later for special cases
	cLast = strTrans1.GetAt( n1 );

	int nDepth = 0;
	BOOL bMatch = TRUE;
	BOOL bStress = FALSE;

	while( bMatch && ( n1 >= 0 ) && ( n2 >= 0 ) )
	{
		cPost = c1;
		c1= strTrans1.GetAt( n1 );
		c2 = strTrans2.GetAt( n2 );
		bMatch = ( c1 == c2 );
		if( bMatch && IsStressMarker( c1 ) )
		{
			bStress = ( c1 == '1' );
			if( bStress )
				nDepth++;		
		}
		n1--;
		n2--;
	}

	if( ( IsIPAVowel( c1 ) || IsIPAVowel( c2 ) ) && bStress ) // did not complete the vowel
		nDepth--;

	if( nDepth > 0 )  // complicated solution to eliminate many false rhymes.
	{	
		// degrade certain "questionable rhymes"
		//
		if( bMatch ) 
		{
			// roll down the scan one more character, as if an empty was equal to a '*'...
			// so that (c1, c2) represent the true conflict point.
			
			cPost = c1; // save matched char

			if( n1< 0 )
			{
				c1 = '*';
				c2 = strTrans2.GetAt( n2 );
			}
			else // both can't be zero...that would be a homonym....handled above already.
			{
				c1 = strTrans1.GetAt( n1 );
				c2 = '*';
			}
		}

		// VALID CASE 1:
		// c1: any consonant  
		// c2: any different consonant
		// cPost: any vowel
		//
		if( ( IsIPAVowel( cPost ) ) && ! IsIPAVowel( c1 )  && ! IsIPAVowel( c2 ) )
			return nDepth;

		// VALID CASE 2:
		// c1: any pre-R consonant, star or number
		// c2: any pre-R consonant, star or number (at least one must be a pre-R consonant!!!)
		// cPost: 'R'
		if( cPost == 'R' )
		{
			if( TrickyDoubleConsTest( c1, c2, "bdfgkpStT" ) )
				return nDepth;
			else
				return RHYME_NEARIDENTICAL; // we expect a failure below, so leave now...
		}

		// VALID CASE 3:
		// c1: any pre-L consonant, star or number
		// c2: any pre-L consonant, star or number   (at least one must be a pre-L consonant !!)
		// cPost: 'l' (i.e. lowercase 'L' )
		if( cPost == 'l' ) 
		{
			if( TrickyDoubleConsTest( c1, c2, "bfgkps" ) ) 
				return nDepth;
			// we must continue to other scenarios...
		}

		// VALID CASE 4:
		// c1: any pre-J consonant, star or number
		// c2: any pre-J consonant, star or number ( at least one must be a pre-J consonant!! )
		// cPost: "j'
		if( cPost == 'j' ) 
		{
			if( TrickyDoubleConsTest( c1, c2, "bfgZkmpvw" ) )
				return nDepth;
			else
				return RHYME_NEARIDENTICAL;  // we expect a failure below, so leave now...
		}
		
		// VALID CASE 5:
		// c1: any pre-W consonant, star or number
		// c2: any pre-W consonant, star or number ( at least one must be a pre-W consonant!! )
		// cPost: 'w'
		if( cPost == 'w' ) 
		{
			if( TrickyDoubleConsTest( c1, c2, "dkst" ) )
				return nDepth;
			else
				return RHYME_NEARIDENTICAL;  // we expect a failure below, so leave now...
		}

		// VALID CASE 6:
		// c1: any pre-W consonant, star or number
		// c2: any pre-W consonant, star or number ( at least one must be a pre-W consonant!! )
		// cPost: 'S'  (i.e. "SH"-sound )
		if( cPost == 'S' ) 
		{
			if( TrickyDoubleConsTest( c1, c2, "t" ) )
				return nDepth;
			else
				return RHYME_NEARIDENTICAL;  // we expect a failure below, so leave now...
		}

		// VALID CASE 7:
		// c1: any 's'
		// c2: any 's' ( at least one must be an 's' !!!)
		// cPost:  any post-S consonant
		if( ( ( c1 == 's' ) || ( c2 == 's' ) )  && IsPostSCons( cPost ) )
			return nDepth;

		if( TRUE )
		{
			BOOL b = FALSE;
		}
	}
	return RHYME_FAIL;
}

// eof - PHONETICS.CPP

