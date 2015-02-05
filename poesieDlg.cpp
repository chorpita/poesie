// poesieDlg.cpp : implementation file
//

#include "stdafx.h"
#include "poesie.h"
#include "generic.h"
#include "phonetics.h"
#include "poetics.h"
#include "lexicon.h"
#include "syntax.h"
#include "compile.h"
#include "semantic.h"
#include "chicken.h"
#include "deer.h"
#include "poesieDlg.h"
#include "DictionaryDlg.h"
#include "IPADialog.h"
#include "IPAPoemDlg.h"
#include "ImportDlg.h"
#include "SyntaxDlg.h"
#include "WordNetDlg.h"
#include "OptionsDlg.h"
#include "reader.h"
#include "wn/wn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPoesieDlg dialog

CPoesieDlg::CPoesieDlg( LPCSTR szDataPath, CWnd* pParent /*=NULL*/)	:
	CDialog(CPoesieDlg::IDD, pParent ),
	m_strPoem( "<poem>" ),
	m_pCompiler( NULL ),
	m_LB( m_COR, m_IPA ),
	m_EXC( m_IPA ),
	m_strDataPath( szDataPath )
{
	  wninit();
	  RandInitialize();

	//{{AFX_DATA_INIT(CPoesieDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPoesieDlg::~CPoesieDlg()
{	
	if( m_pCompiler )
		delete m_pCompiler;
}

void CPoesieDlg::CreateNewCompiler()
{
	switch( m_UO.m_nCompiler )
	{
	case CMP_WORM:
		m_pCompiler = new CWormCompiler( m_UO, m_Lex );
		break;
	case CMP_CHICKEN:
		m_pCompiler = new CChickenCompiler( m_UO, m_IPA, m_COR, m_SF, m_LB, m_EXC, 0 );
		break;
	case CMP_DEER:
		m_pCompiler = new CDeerCompiler( m_UO, m_IPA, m_COR, m_SF, m_LB, m_EXC, 1 );
		break;
	default:
		ASSERT( FALSE );
	}
}

void CPoesieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPoesieDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPoesieDlg, CDialog)
	//{{AFX_MSG_MAP(CPoesieDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_NEWPOEM, OnNewpoem)
	ON_BN_CLICKED(ID_OPTIONS, OnOptions)	
	ON_BN_CLICKED(ID_CONTINUE, OnContinue)
	ON_BN_CLICKED(ID_DICTIONARY, OnDictionary)
	ON_BN_CLICKED(ID_ABOUT, OnAbout)
	ON_BN_CLICKED(ID_REPAIR, OnRepair)
	ON_BN_CLICKED(ID_MAKEPOEM, OnMakepoem)
	ON_BN_CLICKED(ID_WORDNET, OnWordnet)
	ON_BN_CLICKED(ID_SYNTAX, OnSyntax)
	ON_BN_CLICKED(ID_IPATRANS, OnIpatrans)
	ON_BN_CLICKED(ID_CMUIMPORT, OnCmuimport)
	ON_BN_CLICKED(ID_READ, OnRead)
	ON_BN_CLICKED(ID_IPAPOEM, OnIpapoem)
	ON_BN_CLICKED(ID_SAVEAS, OnSaveas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPoesieDlg message handlers

BOOL CPoesieDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	CreateNewCompiler();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	EnableControls( FALSE, FALSE );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPoesieDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPoesieDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPoesieDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPoesieDlg::EnableControls( BOOL bPoemDone, BOOL bAllowNew )
{
	GetDlgItem( ID_IPAPOEM )->EnableWindow(  bPoemDone );
	GetDlgItem( ID_SAVEAS )->EnableWindow( bPoemDone );
	GetDlgItem( ID_NEWPOEM )->EnableWindow( bAllowNew );
}

void CPoesieDlg::OnNewpoem() 
{
	m_pCompiler->Reset();
	 m_strPoem = "<poem>";
	 SetDlgItemTextA( IDC_POEM, m_strPoem );	
	EnableControls( FALSE, FALSE );
}

void CPoesieDlg::OnContinue() 
{
	 BOOL bRunning = m_pCompiler->Run( m_strPoem );
	 GetDlgItem( IDC_POEM )->SetWindowText( m_strPoem );
	 EnableControls( ! bRunning, TRUE );
}

void CPoesieDlg::OnOptions() 
{
	COptionsDlg dlg( & m_UO  );
	if( IDOK == dlg.DoModal() )
	{
		delete m_pCompiler;
		CreateNewCompiler();
	}
}

void CPoesieDlg::OnDictionary() 
{
	CDictionaryDlg dlg( m_Lex, TRUE, this );
	dlg.DoModal();
}

void CPoesieDlg::OnAbout() 
{
	CAboutDlg().DoModal();	
}

void Test( CIPAManager & rIPA, CString & strVerb )
{
	CString strResult;

//	rIPA.BuildGerund( strVerb, strResult );

	InflectS( strVerb, strResult );
	
	int nSy = rIPA.SyllableCount( strVerb );

	int y = 0;
}

void Test_WN( LPSTR szWord, int nPOS )
{
//	int nResult  =	findtheinfo_chorpita( szWord, nPOS );
}

class CBlockChunker : public CSimpleChunker
{
public:
	CBlockChunker() : m_nNumDynamics( 0 ) { }

	int GetDynamics() { return m_nNumDynamics; }

protected:
	int m_nNumDynamics;

	void AddDynamic( enumSyn eSyn ) 
	{ 
//		Add( ( GetSynMinSyllables( eSyn ) == 1 ) ? CHUNK_D1 : CHUNK_D2 );  
		m_nNumDynamics++; 
	}

	virtual BOOL IsValidChar( char c )
	{
		return ( ( c >= 'a' ) && ( c <= 'z' ) ) || ( c == '-' );
	}

	virtual void InterruptPoint( LPCSTR szBuffer, int & nLoopIndex )
	{
		if( szBuffer[ nLoopIndex ] == '<' )
		{
			char c1 = szBuffer[ nLoopIndex + 1 ];
			if( c1 == 'i' )
				Add( "<a>" ); 
			else
				AddDynamic( DetectSyn( c1, szBuffer[ nLoopIndex + 2 ] ) );
			while( szBuffer[ nLoopIndex ] != '>' )
				nLoopIndex++;
			nLoopIndex++;
		}
	}

};

#define RR_COMP( x, y ) s1  =  x ;  s2 = y; s1.MakeUpper(); s2.MakeUpper();  \
	 p1 = m_IPA.SortedFind( s1 ); p2 = m_IPA.SortedFind( s2 );  \
	GetRhyminess( p1, p2 ); 

void CPoesieDlg::OnRepair() 
{
	/*
	CString s1, s2;
	CIPAElement  * p1;
	CIPAElement * p2;

	RR_COMP( "big", "small" );
	RR_COMP( "straw", "flaw" );
	RR_COMP ( "rape", "grape" );
	RR_COMP( "conventional", "unconventional" );
	RR_COMP( "awe", "flaw" );
	RR_COMP( "baseball", "softball" );
	RR_COMP( "sleek", "leak" );
	RR_COMP( "sleep", "bleep" );
	RR_COMP( "stew", "too" );
	RR_COMP( "think", "stink" );
	RR_COMP( "black", "lack" );
	RR_COMP( "true", "untrue" );
	RR_COMP( "decker", "cohenour" );
	*/

	CString s2( "man with <ia> ink" );
	m_IPA.InsertIndefiniteArticles( s2 );
	CString s3( "man with <ia> ink farm of ink and <ia> bat." );
	m_IPA.InsertIndefiniteArticles( s3 );

	CString str( "the sweet of child bombs cheaply from their dime's." );
			int nCount = 0;
		CBlockChunker bc;
		bc.Chunk( str );
		for( int i = 0; i < bc.GetSize(); i++)
			nCount += m_IPA.SyllableCount( bc.GetAt( i ) );

	/*
	CString str( m_LB.m_pNouns->RandElement()->m_strKey );
	str.MakeUpper();
	CIPAElement * pBase = m_IPA.GetData().SortedFind( str );

	for( int i = 0; i < m_IPA.GetData().size(); i++ )
	{
		CIPAElement * pTrial = m_IPA.GetData()[ i ].get();		
		int n= GetRhyminess(  pTrial, pBase );	
	}
	*/

	/*
	Test_WN( "hand", NOUN );
	Test_WN( "land", NOUN );
	Test_WN( "hanaddad", NOUN );
	Test_WN( "juices", NOUN );
	Test_WN( "juice", VERB );

	CString str( findtheinfo( "juices", NOUN, OVERVIEW, ALLSENSES ) );
	*/

	/*
	int n = m_IPA.size();
	for( int i = 0; i < n; i++ )
	{
		CString strWord = m_IPA.at( i )->m_strKey;
		CString strEnd = strWord.Right( 2 );
		if( strEnd.GetLength() > 1 )
			if( ( strEnd.GetAt( 1 ) == 'F' ) || ( strEnd == "FE" ) )
			{
				int nLen = strWord.GetLength();
			}
	}
	*/

	/*
	Test( m_IPA, CString( "fly" ) );
	Test( m_IPA, CString( "play" ) );
	Test( m_IPA, CString( "wax" ) );
	Test( m_IPA, CString( "watch" ) );
	Test( m_IPA, CString( "push" ) );
	Test( m_IPA, CString( "undo" ) );
	Test( m_IPA, CString( "suffer" ) );
	Test( m_IPA, CString( "forget" ) );
	Test( m_IPA, CString( "wax" ) );
	Test( m_IPA, CString( "dye" ) );
	Test( m_IPA, CString( "tie" ) );
	Test( m_IPA, CString( "free" ) );
	Test( m_IPA, CString( "refer" ) );
	Test( m_IPA, CString( "wife" ) );
	Test( m_IPA, CString( "eye" ) );
	Test( m_IPA, CString( "sip" ) );	
	Test( m_IPA, CString( "giraffe" ) );
	Test( m_IPA, CString( "staff" ) );
	Test( m_IPA, CString( "eye" ) );
	Test( m_IPA, CString( "sip" ) );

	int n = m_LB.m_pVerbs->size();
	for( int i = 0; i < n; i++ )
		Test( m_IPA, m_LB.m_pVerbs->at( i )->m_strKey );

	n = m_LB.m_pNouns->size();
	for( i = 0; i < n; i++ )
		Test( m_IPA, m_LB.m_pNouns->at( i )->m_strKey );
*/

	/*
	int n = m_Syntax.size();
	for( int i = 0; i < n; i++ )
	{
		CString str( m_Syntax[ i ]->m_strKey );
		if( str.GetAt( 0 ) == ' ' )
		{
			CString str2( str );
			m_Syntax[ i ]->m_strKey.TrimLeft();
		}
	}
	*/
}
 
void CPoesieDlg::OnMakepoem() 
{
	while( m_pCompiler->Run( m_strPoem ) )
		GetDlgItem( IDC_POEM )->SetWindowText( m_strPoem );
	EnableControls( TRUE, TRUE );
}

void CPoesieDlg::OnWordnet() 
{
	CWordNetDlg dlg( m_LB, m_UO, m_COR, this );
	dlg.DoModal();
}


void CPoesieDlg::OnSyntax() 
{
	if( m_UO.m_strSyntaxPath.GetLength() )
		::SetCurrentDirectory( m_UO.m_strSyntaxPath );

	CSyntaxDlg dlg( m_SF, m_COR, this );
	dlg.DoModal();

	char szDir[ 300 ];
	::GetCurrentDirectory( 300, szDir );
	m_UO.m_strSyntaxPath = szDir;

	::SetCurrentDirectory( m_strDataPath );
}

void CPoesieDlg::OnIpatrans() 
{
	CIPADialog dlg( m_IPA, this );
	dlg.DoModal();
}

void CPoesieDlg::OnCmuimport() 
{
	CImportDlg dlg( m_IPA, m_COR, m_EXC, this );
	dlg.DoModal();
}

void CPoesieDlg::OnRead() 
{
	RunReader( this,  m_IPA, m_COR );
}


void CPoesieDlg::OnIpapoem() 
{
	CIPAPoemDlg dlg( m_strPoem, m_IPA, this );
	dlg.DoModal();	
}

void CPoesieDlg::OnSaveas() 
{
	if( m_UO.m_strSavePath.GetLength() )
		::SetCurrentDirectory( m_UO.m_strSavePath );

	int nPos = m_strPoem.Find( '\n', 1 );
	CString strFileName( m_strPoem.Mid( 1,  nPos - 1 ) );
	strFileName += ".txt";

	CFileDialog dlg
		(
		FALSE, 
		"txt", 
		strFileName, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"Textdateien (*.txt)" 
		);
	if( IDOK == dlg.DoModal() )
	{	
 		CStdioFile rfile( dlg.GetPathName(),  CFile::modeWrite | CFile::modeCreate | CFile::typeText );
		CArchive rar( & rfile, CArchive::store );
		rar.WriteString( m_strPoem );
		rar.Close();	
		rfile.Close();
		char szDir[ 300 ];
		::GetCurrentDirectory( 300, szDir );
		m_UO.m_strSavePath = szDir;
	}	
	::SetCurrentDirectory( m_strDataPath );
}
