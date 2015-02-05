#if !defined(AFX_SYNTAXDLG_H__F1CFFEC0_0043_11DA_B4C8_444553540000__INCLUDED_)
#define AFX_SYNTAXDLG_H__F1CFFEC0_0043_11DA_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SyntaxDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSyntaxDlg dialog

class CSyntaxDlg : public CDialog
{
// Construction
public:
	CSyntaxDlg
		( 
		CSyntaxFactory & rSF, 
		CCoreVector & rCOR,
		CWnd* pParent = NULL
		);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSyntaxDlg)
	enum { IDD = IDD_SYNTAX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyntaxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSyntagmaIndex;
	BOOL m_bTitles;
	CString m_strTagPath;
	CSyntaxFactory & m_SF;
	CCoreVector & m_COR;

	int Parameterize( CString & str, int nMinLength );
	void EnableControls( BOOL bValidIndex );
	BOOL ProcessTitles( CArchive & rar );
	BOOL ProcessText( CArchive & rar );
	void WriteReportFile();

	// Generated message map functions
	//{{AFX_MSG(CSyntaxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGenerate();
	afx_msg void OnChange();
	afx_msg void OnIndexChange();
	afx_msg void OnFilefind();
	afx_msg void OnSynTitel();
	afx_msg void OnSynTitles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNTAXDLG_H__F1CFFEC0_0043_11DA_B4C8_444553540000__INCLUDED_)
