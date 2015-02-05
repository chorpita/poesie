#ifndef __LEXICON_H_
#define __LEXICON_H_

#define LEX_VERSION 1

#define BASE_LEXFILE "base.lex"
#define RESCUE_LEXFILE "rescue.lex"

#define WORD_MAXCHARS 20
#define WORD_MAXSILBEN  5
#define WORD_NEWWORD -1

#define WORDTYPE_ADJ 0
#define WORDTYPE_ADV  1
#define WORDTYPE_DET 2
#define WORDTYPE_NAM  3
#define WORDTYPE_QUE 4
#define WORDTYPE_INJ 5
#define WORDTYPE_KNJ 6
#define WORDTYPE_PRP 7
#define WORDTYPE_PRO 8
#define WORDTYPE_NOU 9
#define WORDTYPE_VRB 10

#define WORDGENDER_NONE 0
#define WORDGENDER_MALE 1
#define WORDGENDER_FEMALE 2

#define LEXFILTER_NONE 0
#define LEXFILTER_ENG   1
#define LEXFILTER_GER  2

#define WORDMATCH_CLEANREIM 0
#define WORDMATCH_DIRTYREIM 1
#define WORDMATCH_ASSONANZ 2
#define WORDMATCH_ALLITERAT 3
#define WORDMATCH_MAX                4

class CSilbe
{
public:
	CSilbe() : 
	m_strPhoneText( "" ), 
	m_bTone( TRUE ),
	m_bFlexible( TRUE ), 
	m_bWordBreak( 0 ) 
	{  
	}

	void Serialize( CArchive & );

	const BOOL IsCleanReim( CSilbe & silbe );

	CString m_strPhoneText;
	BOOL  m_bTone;
	BOOL m_bFlexible;
	BYTE m_bWordBreak;
};

class CWord 
{
public:
	CWord() :
	m_strText( "" ),
    m_lid( WORD_NEWWORD ),
	m_nWordType( WORDTYPE_NOU ),
	m_bPlural( FALSE ),
	m_bAmbig( FALSE ),
	m_strExtra( "" ),
	m_nGender( WORDGENDER_NONE ),
	m_bHidden( FALSE ),
	m_bTakesAn( FALSE ),
	m_nNumSilben( 1 )	
	{ 
	}

    void Serialize( CArchive & );

	const void LoadDisplayText( CString & strDisplay );
	const LPCSTR GetWordTypeText();
	const BOOL GetMatch( CWord * pWord, int nMatch );

	CSilbe & GetSilbe( int i ) 
	{
		ASSERT( i >= 0 );
		ASSERT( i < WORD_MAXSILBEN );
		return m_Silben[ i ];
	}
	CSilbe & GetLastSilbe() { return GetSilbe( m_nNumSilben - 1 ); }

	CString m_strText;
    long m_lid;
	int m_nWordType;
	BOOL m_bPlural;
	BOOL m_bAmbig;
    CString m_strExtra;
	int m_nGender;
	BOOL m_bHidden;
	BOOL m_bTakesAn;
	int m_nNumSilben;	

protected:
	CSilbe m_Silben[  WORD_MAXSILBEN ];
};

class CWordRef
{
public:
	CWordRef( CWord * pWord ) : m_pWord( pWord ), m_nRefCount( 0 ) {  }
	virtual ~CWordRef() { }

	operator CWord *() const { return m_pWord; }
	int GetRefCount() { return m_nRefCount; }
	void Use() { m_nRefCount++; }

protected:
	CWord * m_pWord;
	int m_nRefCount;
};

class CWordList : public std::vector< CWordRef >
{
public:
	CWordList()  { }	
	CWordList( LPCSTR szName ) : m_strName( szName ) {  }

	void SetName( LPCSTR sz ) { m_strName = sz; }

protected:
	CString  m_strName;
};

class CMatchChart
{
public:
	CMatchChart( char cid, CWord * pBase );

	const GetId() { return m_id; }

	void SetListName( LPCSTR sz, int nMatch ) { m_WordLists[ nMatch ].SetName( sz ); }
	void AddWordRef( CWordRef & wr, int nMatch )  {	 m_WordLists[ nMatch ].push_back( wr ); }
	CWordRef & GetWordRef( int nMatch, int nIndex ) { return m_WordLists[ nMatch ].at( nIndex ); }
	int GetListSize( int nMatch ) { return m_WordLists[ nMatch ].size(); }

protected:
	char m_id;
	CWord * m_pBase;
	CWordList m_WordLists[ WORDMATCH_MAX ];
};

class CLexicon : public CPersistent
{
	typedef std::vector< std::auto_ptr< CWord > > CVector;

public:
	CLexicon( int nDefaultFilter = LEXFILTER_ENG ) : 
	m_nVersion( LEX_VERSION ), 
	m_nDefaultFilter( nDefaultFilter ),
	m_lSeed( 0 ) 
	{
		Load( BASE_LEXFILE, RESCUE_LEXFILE );
	}
	~CLexicon() { Commit( BASE_LEXFILE ); }

	int GetVersion() { return m_nVersion; }
	int GetDefaultFilter() { return m_nDefaultFilter; }

	int GetNumWords() { return m_Entries.size(); }	
	void AddWord( CWord * pWord );
	CWord * FindWord( long lid );
	void DeleteWord( long Iid );
	CWord * GetWord( int nIndex ) { return m_Entries[ nIndex ].get(); }

	CMatchChart * CreateMatchChart( char c, CWord * pBase );

	virtual void Serialize( CArchive & ar );

protected:
     int m_nVersion;
	 int m_nDefaultFilter;
	 long m_lSeed;
    CVector m_Entries;	

	BOOL Compare( int n1, int n2 );
	void Insertion_Sort( int * pInt, int nSize );
};

/*

  class CWordListManager
{
	typedef std::vector< std::auto_ptr< CWordList > > CVector;

public:
	CWordListManager() { }

protected:
    CVector m_Entries;
};

*/


#endif // __LEXICON_H_