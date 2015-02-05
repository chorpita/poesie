// Semantic.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "syntax.h"
#include "semantic.h"
#include "wn/wn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define BASE_SEMFILE "base.sem"
#define RESCUE_SEMFILE "rescue.sem"

class CLexChunker : public CSimpleChunker
{
	virtual BOOL IsValidChar( char c )
	{
		return ( ( c >= 'a' ) && ( c <= 'z' ) ) || ( c == '-' );
	}
};

class CLexVector : public CLexVectorBase
{
public:
	CLexVector( int nPOS, int nOp1 ) : m_nPOS( nPOS ), m_nOp1( nOp1 )  {  } 

protected:
	int m_nPOS;
	int m_nOp1;

	virtual void ProcessForm( CString & strForm, int nLevel, CFormHandler * pTop )
	{
		if( ! ContainsAtLevel( strForm, nLevel ) )
		{
			CString strInfo( FindInfo( strForm, m_nOp1 ) );	
			if( strInfo.GetLength() )
			{
				if( Find( strForm ) == NULL )
				{
					AddElement( strForm, nLevel, 1 );
					pTop->ShowIncrement();
				}
				if( nLevel <= pTop->GetMaxLevel() )
				{
					ProcessInfo( strInfo, nLevel, pTop );
					ProcessRelations( strForm, nLevel, pTop );
				}
			}
		}	
	}	

	virtual void AddDisplayLinkText( CString & strAdd ) 
	{
		int n = size();
		for( int i = 0; i < n; i++ )
		{
			strAdd +=	at( i )->m_strKey;
			strAdd += " ";
		}
	}
	
	virtual void ProcessRelations( CString & strForm, int nLevel, CFormHandler * pTop ) = 0;

	BOOL ContainsAtLevel( CString & str, int nLevel )
	{
		long n = size();
		for( long i = 0; i < n; i++ )
			if( ( str == at( i )->m_strKey ) && ( nLevel == at( i )->m_nLevel  )  )
				return TRUE;
		return FALSE;
	}

	char * FindInfo( LPCSTR szForm, int nPtrType )
	{
		return findtheinfo( (LPSTR) szForm, m_nPOS, nPtrType, ALLSENSES ); 
	}

	void ProcessInfo( LPCSTR szInfo,  int nLevel, CFormHandler * pTop )
	{
		CString strInfo( szInfo );
		strInfo.MakeLower();
		CLexChunker  par;
		par.Chunk( strInfo );
		int n = par.GetSize();
		for( int i = 0; i < n; i ++ )
			pTop->ProcessForm( par.GetAt( i ), nLevel );
	}

	void ProcessRelation( CString & strForm, int nPtrType, int nLevel, CFormHandler * pTop )
	{
		ProcessInfo( FindInfo( strForm, nPtrType ), nLevel, pTop );
	}
};

class CNounVector : public CLexVector 
{
public:
	CNounVector() : CLexVector( NOUN, HYPERPTR ) { }

	void ProcessRelations( CString & strForm, int nLevel, CFormHandler * pTop )
	{
		ProcessRelation( strForm, ANTPTR, nLevel, pTop );
	//	ProcessRelation( strForm, COORDS, nLevel, pTop );
		ProcessRelation( strForm, HYPOPTR, nLevel, pTop );
	//	ProcessRelation( strForm, -HMERONYM, nLevel, pTop );
		ProcessRelation( strForm, DERIVATION, nLevel, pTop );
	}
};

class CVerbVector : public CLexVector 
{
public:
	CVerbVector() : CLexVector( VERB, HYPERPTR ) { }
	
	void ProcessRelations( CString & strForm, int nLevel, CFormHandler * pTop )
	{
		ProcessRelation( strForm, ANTPTR, nLevel, pTop );
		ProcessRelation( strForm, COORDS, nLevel, pTop );
		ProcessRelation( strForm, HYPOPTR, nLevel, pTop );
	}
};

class CAdjVector : public CLexVector 
{
public:
	CAdjVector() : CLexVector( ADJ, SIMPTR ) { }

	void ProcessRelations( CString & strForm, int nLevel, CFormHandler * pTop )
	{
		ProcessRelation( strForm, ANTPTR, nLevel, pTop );
	}
};

class CAdvVector : public CLexVector 
{
public:
	CAdvVector() : CLexVector( ADV, SYNS ) { }

	void ProcessRelations( CString & strForm, int nLevel, CFormHandler  * pTop )
	{
		ProcessRelation( strForm, ANTPTR, nLevel, pTop );
	}
};

CLexBrain::CLexBrain( CCoreVector & rCOR, CIPAManager & rIPA ) :
	m_pNouns( new CNounVector ),
	m_pVerbs( new CVerbVector ),
	m_pAdjs( new CAdjVector ),
	m_pAdvs( new CAdvVector ),
	m_COR( rCOR ),
	m_IPA( rIPA ),
	m_nMaxLevel( 0 ),
	m_pDlgItem( NULL ),
	m_nWordCount( 0 )
{
		Load( BASE_SEMFILE, RESCUE_SEMFILE ); 
}

CLexBrain::~CLexBrain()
{
	Commit( BASE_SEMFILE ); 
	delete m_pAdvs;
	delete m_pAdjs;
	delete m_pVerbs;
	delete m_pNouns;
}

CLexVectorBase * CLexBrain::GetLexVector( enumSyn eSyn )
{
	switch( GetSynPOS( eSyn ) )
	{
	case VERB: return m_pVerbs;
	case NOUN: return m_pNouns;
	case ADJ: return m_pAdjs;
	case ADV: return m_pAdvs;
	default: ASSERT( FALSE ); return NULL;
	}
}

void CLexBrain::Build( LPSTR szRoot, int nMaxLevel, CWnd * pDlgItem ) 
{ 
	m_nMaxLevel = nMaxLevel;
	m_pDlgItem = pDlgItem;

	m_nWordCount = 0;
	m_pNouns->clear();
	m_pVerbs->clear();
	m_pAdjs->clear();
	m_pAdvs->clear();

	CString strRoot( szRoot );
	strRoot.MakeLower();
	CLexChunker  par;
	par.Chunk( strRoot );
	int n = par.GetSize();
	for( int i = 0; i < n; i ++ )
		ProcessForm( par.GetAt( i ), 0 );
}

void CLexBrain::ProcessForm( CString & strForm, int nLevel )
{
	if( ! m_COR.ContainsUS( strForm ) && m_IPA.GetData().ContainsUS( strForm ) )
	{	
		nLevel++;
		m_pNouns->ProcessForm( strForm, nLevel, this );
		m_pVerbs->ProcessForm( strForm, nLevel, this );
		m_pAdjs->ProcessForm( strForm, nLevel, this );
		m_pAdvs->ProcessForm( strForm, nLevel, this );
		CString strAdv_ly( strForm + "ly" );
		m_pAdvs->ProcessForm( strAdv_ly, nLevel, this );
	}
} 

void CLexBrain::ShowIncrement()
{
	if( m_pDlgItem )
	{
		char szMessage[ 200 ];
		m_nWordCount++;
		wsprintf( szMessage, "%d", m_nWordCount );
		m_pDlgItem->SetWindowText( szMessage );
	}
}

void CLexBrain::DisplayLinks( LPSTR szRoot )
{
	CString str( "NOUNS: ");
	m_pNouns->AddDisplayLinkText( str );
	str += "VERBS: ";
	m_pVerbs->AddDisplayLinkText( str );
	str += "ADJS: ";
	m_pAdjs->AddDisplayLinkText( str );
	str += "ADVS: ";
	m_pAdvs->AddDisplayLinkText( str );
	::MessageBox( NULL, str, szRoot, MB_OK );
}

void CLexBrain::Serialize( CArchive & ar )
{
	m_pNouns->Serialize( ar );
	m_pVerbs->Serialize( ar );
	m_pAdjs->Serialize( ar );
	m_pAdvs->Serialize( ar );
}

BOOL WordExistsInWordnet( LPCSTR szWord, int nPOS )
{
	char sz[ 200 ];
	strcpy( sz, szWord );
	return index_lookup( sz, nPOS ) != NULL;
}
