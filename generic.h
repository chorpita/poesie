#ifndef __GENERIC_H_
#define __GENERIC_H_

#define QUOTE_CHAR '\"'

#define MAX_R_DSTRB 128

class CPersistent
{
public:
	virtual ~CPersistent() { }

	virtual void Serialize( CArchive & ar ) { }

	// must be called by derived class !!!
protected:
	void Load( LPCSTR szMainFile, LPCSTR szRescueFile );
    void Commit( LPCSTR szMainFile );
};

class CPowerElement 
{
public:
	virtual ~CPowerElement() { }

	virtual void Serialize( CArchive & ar ) 
	{
		if( ar.IsLoading() )
			ar >> m_strKey;
		else
			ar << m_strKey;
	}

	CString  m_strKey;
};

template <class _TData >
class CPowerVector : public std::vector< std::auto_ptr< _TData > >, public CPersistent
{
public:
	_TData * Find( CString & str )
	{
		   long n = size();
		   for( long i = 0; i < n; i++ )
		   {
		       _TData * pD = (*(begin() + i )).get();	
			   if( pD->m_strKey == str )
				   return pD;
			}				
		return NULL;
	}

	_TData * SortedFind( CString & str )
	{
		long nLow = 0;
		long nHigh = size();
		BOOL bDone = FALSE;
		while( ! bDone )
		{
			long nTry = (nLow + nHigh) / 2;
		     _TData * pD = (*(begin() + nTry )).get();	
			 if( pD->m_strKey == str )
				return pD;
			bDone = ( nTry == nLow );
				if( pD->m_strKey < str )
					nLow = nTry;
			   else
					nHigh = nTry;
		}
		return NULL;
	}

	BOOL ContainsUS( CString & str )
	{
		CString strUpper( str );
		strUpper.MakeUpper();
		return ( SortedFind( strUpper ) != NULL );
	}

	void Add( _TData * pD, CString & str )
	{
		pD->m_strKey = str;
		std::auto_ptr< _TData > sp( pD );
		push_back( sp );
	}

	void Serialize( CArchive & ar )
	{
		if( ar.IsLoading() )
		{
			long lCount;
			ar >> lCount;
			for( long i = 0; i < lCount; i++ )
			{
			    _TData * pT = new _TData;	
			    pT->Serialize( ar );
			    std::auto_ptr< _TData > sp( pT );
			    push_back( sp ); 
			}	 
		}
		else
		{
			long lCount = size();
			ar << lCount;
			for( long i = 0L; i < lCount; i++ )
				at( i )->Serialize( ar );
		}
	}

	_TData * RandElement()
	{
		return ( * ( begin() + RandVal( size() ) ) ).get();	
	}
};

class CRandPick
{
public:
	CRandPick()  : m_nSize( 0 ) { }

	void Fill( int nVal, int nCount );
	int Pick();

protected:
	int m_nSize;
	int m_nArray[ MAX_R_DSTRB ];
};

class CSimpleChunker : public CStringArray
{
public:
	void Chunk( LPCSTR szBuffer );

protected:
	virtual BOOL IsValidChar( char c ) = 0;
	virtual void InterruptPoint( LPCSTR szBuffer, int & nLoopIndex ) { }
};

// utility functions

void RandInitialize();
int RandVal( int nRange );
void ScrambleVector( int nRange, int * pVector );

void ParseInteger( TCHAR * szText, UINT nIDPrompt, ... );
void ParseNumericControl( CWnd * pCtrl, int * nVal );

// text functions

BOOL IsEnglishVowel( TCHAR c );
void InflectS( CString & strBase, CString & strResult );
void InflectED( CString & strBase, CString & strResult );

#endif // __GENERIC_H_

