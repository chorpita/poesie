// poesieDlg.h : header file
//

#ifndef __COMPILER_H
#define __COMPILER_H

typedef enum _enumALGO
{
	algo_Worm,
	algo_Chicken,
	algo_Deer
} enumALGO;

class CCompiler
{
public:
	CCompiler( CUserOptions & rUO ) : 
	m_UO( rUO )
	{
	}

	virtual ~CCompiler() { }

	virtual void Reset() = 0;
	virtual BOOL Run( CString & strPoem ) = 0;

protected:
	CUserOptions & m_UO;
};

#define MAX_WORDCHARTS 20

class CWormCompiler : public CCompiler
{
public:
	CWormCompiler
		(
		CUserOptions & rUO, 
		CLexicon & rLex 
		) : 
	  CCompiler( rUO ), 
		m_Lex( rLex ), 
		m_nNumCharts( 0 ) 
	  {
	  }

	~CWormCompiler() { DeleteLists(); }

	virtual void Reset() { DeleteLists(); };
	virtual BOOL Run( CString & strPoem );

protected:
	void InsertWord( int nFind, CString & str );
	CWord * GetRandomWord();
	CWord * GetRandomMatchWord( char c, int nMatch );
	void DeleteLists();
	CMatchChart * FindMatchChart( char c );

	CLexicon & m_Lex;
	int m_nNumCharts;
	CMatchChart  *  m_pCharts[ MAX_WORDCHARTS ];
};

#endif 
