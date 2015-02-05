#ifndef __PHONETICS_H_
#define __PHONETICS_H_

#define IPA_FILE "eng-us.ipa"

#define RHYME_IDENTICAL  -2
#define RHYME_NEARIDENTICAL -1
#define RHYME_FAIL 0

#define LOOKUP_NOUN "NOUN"
#define LOOKUP_VERB "VERB"
#define LOOKUP_ADJ "ADJ"
#define LOOKUP_ADV "ADV"

class CIPAElement : public CPowerElement
{
public:
	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		if( ar.IsLoading() )
			ar >> m_strTrans >> m_nSilben;
		else
			ar << m_strTrans << m_nSilben;
	}
	
	CString m_strTrans;
	int m_nSilben;
};

class CIPALookupElement : public CPowerElement
{
public:
	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		m_dwa.Serialize( ar );
	}
	
	CDWordArray m_dwa;
};

class CIPAManager : public CPersistent
{
typedef CPowerVector< CIPAElement > CIPAVector;
typedef CPowerVector< CIPALookupElement > CIPALookupVector;

public:
	CIPAManager() { Load( IPA_FILE, NULL ); }

	// raw data
	CIPAVector & GetData() { return m_Data; }

	// lookup methods
	CIPALookupVector & GetLookups() { return m_Lookups; }
	CDWordArray & GetLookup( LPCSTR szLookupName );
	CDWordArray & GetLookup( int nPOS ); 
	CIPAElement * RandomLookup( LPCSTR szLookupName );
	CIPAElement * IndexLookup( LPCSTR szLookupName, int nIndex );
	CIPAElement * IndexLookup( int nPOS, int nIndex );

	// misc. methods
	void InsertIndefiniteArticles( CString & strWord );
	BOOL IndefiniteArticleAn( CString & strWord );
	int SyllableCount( CString & strWord );
	void BuildGerund( CString & strVerb, CString & strResult );

	void AddElement( CString & strForm, CString & strTrans, int nSilben )
	{
		CIPAElement * p = new CIPAElement;
		p->m_strTrans = strTrans;
		p->m_nSilben = nSilben;
		m_Data.Add( p, strForm );
	}

	void AddLookupElement( LPCSTR szLookupName )
	{
		CIPALookupElement * p = new CIPALookupElement;
		m_Lookups.Add( p, CString( szLookupName ) );
	}

	virtual void Serialize( CArchive & ar )
	{
		m_Data.Serialize( ar );
		m_Lookups.Serialize( ar );
	}	

protected:	
	CIPAVector m_Data;
	CIPALookupVector m_Lookups;
};

int GetRhyminess( CIPAElement * pIPAE1, CIPAElement * CIPAE2 );

#endif // __PHONETICS_H_