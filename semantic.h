#ifndef __SEMANTIC_H_
#define __SEMANTIC_H_

class CFormHandler : public CPersistent
{
public:
	virtual int GetMaxLevel() = 0;
	virtual void ProcessForm( CString & str, int nLevel ) = 0;
	virtual void ShowIncrement() = 0;
};

class CLexElement : public CPowerElement
{
public:	
	int m_nLevel;
	int m_nWeight;
};

class CLexVectorBase : public CPowerVector< CLexElement >
{
public:
	virtual void ProcessForm( CString & strForm, int nLevel, CFormHandler * pTop ) = 0;
	virtual void AddDisplayLinkText( CString & strAdd ) = 0;

protected:
	void AddElement( CString & strForm, int nLevel, int nWeight )
	{
		CLexElement * p = new CLexElement;
		p->m_nLevel = nLevel;
		p->m_nWeight = nWeight;
		Add( p, strForm );
	}
};

class CLexBrain : public CFormHandler
{
public:	
	CLexVectorBase	*	m_pNouns;
	CLexVectorBase	*	m_pVerbs;
	CLexVectorBase	*	m_pAdjs;
	CLexVectorBase	*	m_pAdvs;

	CLexBrain( CCoreVector & rCOR, CIPAManager & rIPA );
	virtual ~CLexBrain();

	void Build( LPSTR szRoot, int nMaxLevel,  CWnd * pDlgItem );
	void DisplayLinks( LPSTR szRoot );
	CLexVectorBase * GetLexVector( enumSyn eSyn );

	virtual void Serialize( CArchive & ar );

protected:	
	CCoreVector &		   m_COR;
	CIPAManager &		m_IPA;
	int									  m_nMaxLevel;
	CWnd *						   m_pDlgItem;
	int									   m_nWordCount;

	virtual int GetMaxLevel() { return m_nMaxLevel; }	
	virtual void ProcessForm( CString & strForm, int nLevel );
	virtual void ShowIncrement();
};

BOOL WordExistsInWordnet( LPCSTR szWord, int nPOS ); 

#endif // __SEMANTIC_H_