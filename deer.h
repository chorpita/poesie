// deer.h : header file
//

#ifndef __DEER_H
#define __DEER_H

#define MAX_VERSES 6
#define MAX_STANZAS 4
#define MAX_RHYMERS 10
#define MAX_RHYMES  24

typedef enum _enumPS
{
	ps_New,
	ps_Expand,
	ps_Gather,
	ps_Balance,
	ps_Title,
	ps_Clean,
	ps_Finish
} enumPS;

typedef enum _enumRC
{
	rc_NoRhyme,
	rc_CrossRhyme,
	rc_PairRhyme,
	rc_BlockRhyme,
	rc_SonnettRhyme
} enumRC;

class CRhymeData
{
public:
	char m_cCode;
	int m_nTotal;
	CIPAElement * m_pIPAE[ MAX_RHYMES ];

	void Add( CIPAElement * pIPAE  ) { m_pIPAE[ m_nTotal++ ] = pIPAE; }
	int GetCollectiveRhyminess( CIPAElement * pIPAE );
};

class CRhymeManager
{
public:
	int m_nTotal;	
	CRhymeData m_r[ MAX_RHYMERS ];
	
	CRhymeManager() : m_nTotal( 0 ) { }

	void Add( char cCode, CIPAElement * pIPAE )
	{
		int nIndex = FindIndex( cCode );
		if( nIndex == -1 )
		{		
			m_r[ m_nTotal ].m_cCode	 = cCode;
			m_r[ m_nTotal ].m_nTotal	= 0;
			m_r[ m_nTotal ].Add( pIPAE ); 
			m_nTotal++;
		}
		else
			m_r[ nIndex ].Add( pIPAE );
	}

	int FindIndex( char cCode )
	{
		for( long i = 0; i <  m_nTotal; i++ )
			if( m_r[ i ].m_cCode == cCode )
				return i;
		return -1;
	}
};

class CVerseData
{
public:
	int  m_nTargetSyllables;
	int m_nSyllablesToSpare;
	int m_nNumDynamics;
	CRhymeData m_r;
	CString m_strWorkVerse;
};

class CStanzaData
{
public:
	int m_nVerses;	
	CString m_strBlock;
	CVerseData m_vd[ MAX_VERSES ];
};

class CPoemData
{
public:
	enumPS m_ePS;
	BOOL m_bTitle;
	BOOL m_bLower;
	BOOL m_bAllowSplit;
	BOOL m_bFixedStanzaLength;
	BOOL m_bFixedSyllableCount;
	enumRC  m_eRC;
	int m_nPoemType;
	enumALGO m_eAlgo;
	int m_nStanzas;
	int m_nWorkBlock;
	int m_nWorkVerse;
	CStanzaData m_sd[ MAX_STANZAS ];
	CString m_strPlug;

	CStanzaData & GetWorkingStanza() { return m_sd[ m_nWorkBlock ]; }
	CVerseData & GetWorkingVerse() { return m_sd[ m_nWorkBlock ].m_vd[ m_nWorkVerse ]; }
	CVerseData & GetVerse( int nIndex ) {  return m_sd[ m_nWorkBlock ].m_vd[ nIndex ]; };
};

class CDeerCompiler : public CWordnetCompiler
{
public:
	CDeerCompiler
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

	virtual ~CDeerCompiler() { }

	virtual void Reset() { }
	virtual BOOL Run( CString & strPoem );

protected:
	CPoemData pd;
	CRhymeManager rm;
	int m_nDebugFlag;

	// block bulding	
	virtual BOOL GatherBlock();
	int GetSeparationPoint( CStringArray & sa, CString & strTrial, int nWord );
	void GetSyllables( int & nMin, int & nMax, CStringArray & sa, int nDepth = -1 );

	// rhyming
	void InsertRhyme();

	// advancement of poem state
	void BlockDone( enumPS epsNextState ) {	StateAdvancer( epsNextState, TRUE ); }
	void VerseDone( enumPS epsNextState );
	void StateAdvancer( enumPS epsNextState, BOOL bBlockDone ); 

	// word insertion
	virtual CIPAElement * GetInsertionWord(	enumSyn eSyn, char cCode, CString & strDef, int nSought, int nMax );
	CIPAElement * GetRandomBrainIPAE( enumSyn eSyn );
	CIPAElement * GetRandomBrainIPAE( enumSyn eSyn, int nSought );

	// initialization
	virtual void InitCompiler();	

	// plugging for display
	void Display( CString & strPoem );
	void WorkingDisplay( CString & strPoem );

	// stages
	void Stage_New( CString & strPoem );
	void Stage_Expand( CString & strPoem );
	void Stage_Gather( CString & strPoem );
	void Stage_Balance( CString & strPoem );	
	void Stage_Title( CString & strPoem );
	void Stage_Clean( CString & strPoem );
	void Stage_Finish( CString & strPoem );
};

#endif // DEER.H


