// chicken.h : header file
//

#ifndef __CHICKEN_H
#define __CHICKEN_H

class CWordnetCompiler : public CCompiler
{
public:
	CWordnetCompiler
		( 
		CUserOptions & rUO, 
		CIPAManager & rIPA,
		CCoreVector & rCOR,	
		CSyntaxFactory & rSF,
		CLexBrain & rLB,
		CFormExc & rEXC
		) : 
	CCompiler( rUO ),
	m_IPA( rIPA ),
	m_COR( rCOR ),
	m_SF( rSF ),
	m_LB( rLB ),
	m_EXC( rEXC )
	{
	}	

protected:
	CIPAManager & m_IPA;
	CCoreVector & m_COR;	
	CSyntaxFactory & m_SF;
	CLexBrain & m_LB;
	CFormExc & m_EXC;
};

class CChickenCompiler : public CWordnetCompiler
{
public:
	CChickenCompiler
		( 
		CUserOptions & rUO, 
		CIPAManager & rIPA,
		CCoreVector & rCOR,	
		CSyntaxFactory & rSF,
		CLexBrain & rLB,
		CFormExc & rEXC,
		int nDebugFlag
		) : 
	CWordnetCompiler( rUO, rIPA, rCOR, rSF, rLB, rEXC ),
	m_nDebugFlag( nDebugFlag )
	{
	}

	virtual ~CChickenCompiler() { }

	virtual void Reset() { }
	virtual BOOL Run( CString & strPoem );

protected:
	int m_nDebugFlag;

	void InsertPierWord( CString & strWord, enumSyn eSyn );
	void InsertBrainWord( CString & strWord, enumSyn eSyn );
	BOOL InsertWord( CString & str, LPCSTR szToken, enumSyn eSyn );	
};

#endif 
