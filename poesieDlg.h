// poesieDlg.h : header file
//

#if !defined(AFX_POESIEDLG_H__1CA5D186_C73A_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_POESIEDLG_H__1CA5D186_C73A_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPoesieDlg dialog

class CPoesieDlg : public CDialog
{
// Construction
public:
	CPoesieDlg( LPCSTR szDataPath, CWnd* pParent = NULL);	// standard constructor
	~CPoesieDlg();

// Dialog Data
	//{{AFX_DATA(CPoesieDlg)
	enum { IDD = IDD_POESIE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPoesieDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CLexicon m_Lex;
	CIPAManager m_IPA;
	CCoreVector m_COR;	
	CSyntaxFactory m_SF;
	CUserOptions m_UO;
	CLexBrain m_LB;	
	CFormExc m_EXC;
	CString m_strPoem;
	CCompiler * m_pCompiler;
	CString m_strDataPath;
	
	void CreateNewCompiler();
	void EnableControls( BOOL bPoemDone, BOOL bAllowNew );

	// Generated message map functions
	//{{AFX_MSG(CPoesieDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNewpoem();
	afx_msg void OnOptions();
    afx_msg void OnContinue();
	afx_msg void OnDictionary();
	afx_msg void OnAbout();
	afx_msg void OnRepair();
	afx_msg void OnMakepoem();
	afx_msg void OnWordnet();
	afx_msg void OnSyntax();
	afx_msg void OnIpatrans();
	afx_msg void OnCmuimport();
	afx_msg void OnRead();
	afx_msg void OnIpapoem();
	afx_msg void OnSaveas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POESIEDLG_H__1CA5D186_C73A_11D9_B4C8_444553540000__INCLUDED_)
