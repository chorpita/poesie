#ifndef __SYNTAX_H_
#define __SYNTAX_H_

typedef enum _enumSyn
{
	s_N = 0,
	s_NP = 1,
	s_V = 2,
	s_VS = 3,
	s_VG = 4,
	s_VP = 5,
	s_VA = 6,
	s_A = 7,
	s_AC = 8,
	s_AS = 9,
	s_R = 10,
	s_ERROR = 11
} enumSyn;

#define COR_FILE "eng-us.cor"
#define EXC_FILE "eng-us.afw"
#define BASE_SYNFILE "base.syn"
#define RESCUE_SYNFILE "rescue.syn"

class CCoreElement : public CPowerElement
{
public:	
	int m_nCode;

	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		if( ar.IsLoading() )
			ar >> m_nCode;
		else
			ar << m_nCode;
	}
};

class CCoreVector : public CPowerVector< CCoreElement >
{
public:
	CCoreVector() { Load( COR_FILE, NULL ); }

	void AddElement( CString & strForm, int nCode )
	{
		CCoreElement * p = new CCoreElement;
		p->m_nCode = nCode;
		Add( p, strForm );
	}
};

class CAdjExcElement : public CPowerElement
{
public:
	CString m_strCompare;
	CString m_strSuper;

	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		if( ar.IsLoading() )
			ar >> m_strCompare >> m_strSuper;
		else
			ar << m_strCompare << m_strSuper;
	}
};

class CAdjExcVector : public CPowerVector< CAdjExcElement >
{
public:
	void AddElement( CString & strForm, CString & strCompare, CString & strSuper )
	{
		CAdjExcElement * p = new CAdjExcElement;
		p->m_strCompare = strCompare;
		p->m_strSuper = strSuper;
		Add( p, strForm );
	}
};

class CNounExcElement : public CPowerElement
{
public:
	CString m_strPlural;

	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		if( ar.IsLoading() )
			ar >> m_strPlural;
		else
			ar << m_strPlural;
	}
};

class CNounExcVector : public CPowerVector< CNounExcElement >
{
public:
	void AddElement( CString & strForm, CString & strPlural )
	{
		CNounExcElement * p = new CNounExcElement;
		p->m_strPlural = strPlural;
		Add( p, strForm );
	}
};

class CVerbExcElement : public CPowerElement
{
public:
	CString m_strPast;
	CString m_strVarPerfect;

	void GetPerfect( CString & strPerfect )
	{
		if( m_strVarPerfect.GetLength() )
			strPerfect = m_strVarPerfect;
		else
			strPerfect = m_strPast;
	}

	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		if( ar.IsLoading() )
			ar >> m_strPast >> m_strVarPerfect;
		else
			ar << m_strPast << m_strVarPerfect;
	}

};

class CVerbExcVector : public CPowerVector< CVerbExcElement >
{
public:
	void AddElement( CString & strForm, CString & strPast, CString & strVarPerfect )
	{
		CVerbExcElement * p = new CVerbExcElement;
		p->m_strPast = strPast;
		p->m_strVarPerfect = strVarPerfect;
		Add( p, strForm );
	}
};

class CFormExc : public CPersistent
{
public:
	CIPAManager & m_IPA;
	CAdjExcVector  m_Adjs;
	CNounExcVector m_Nouns;
	CVerbExcVector  m_Verbs;

	CFormExc( CIPAManager & rIPA ) : m_IPA( rIPA ) {  Load( EXC_FILE, NULL ); }
	
	void Clear();
	virtual void Serialize( CArchive & ar );

	CIPAElement * InflectWord( CString & strBase, enumSyn eSyn );

	void AdjCompare( CString & strBase, CString & strResult );
	void AdjSuper( CString & strBase, CString & strResult );
	
	void  NounPlural( CString & strBase, CString & strResult );

	void VerbThirdSingular( CString & strBase, CString & strResult );
	void VerbPast( CString & strBase, CString & strResult );
	void VerbPerfect( CString & strBase, CString & strResult );
	void VerbGerund( CString & strBase, CString & strResult );
};

class CSyntaxElement : public CPowerElement
{
public:	
	int m_nCount;

	virtual void Serialize( CArchive & ar )
	{
		CPowerElement::Serialize( ar );
		if( ar.IsLoading() )
			ar >> m_nCount;
		else
			ar << m_nCount;
	}
};

class CSyntaxVector : public CPowerVector< CSyntaxElement >
{
public:
	void RecordSegment( CString & str )
	{
		CSyntaxElement * p = Find( str );
		if( p )
			p->m_nCount++;
		else
		{
			p = new CSyntaxElement;
			p->m_nCount = 1;
			Add( p, str );
		}
	}
};

class CSyntaxFactory : public CPersistent
{
public:
	CSyntaxVector m_Title;
	CSyntaxVector m_Text;

	CSyntaxFactory() { Load( BASE_SYNFILE, RESCUE_SYNFILE ); }
	virtual ~CSyntaxFactory() { Commit( BASE_SYNFILE ); }

	virtual void Serialize( CArchive & ar )
	{
		m_Title.Serialize( ar );
		m_Text.Serialize( ar );
	}
};


enumSyn DetectSyn( char c1, char c2 );

int GetSynPOS( enumSyn eSyn );
int GetSynMinSyllables( enumSyn eSyn );


#endif // __SYNTAX_H