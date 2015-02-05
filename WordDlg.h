#if !defined(AFX_WORDDLG_H__23EAD920_DCC2_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_WORDDLG_H__23EAD920_DCC2_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWordDlg dialog

class CWordDlg : public CDialog
{
// Construction
public:
	CWordDlg( LPCSTR szCaption, CWord & rWord, CLexicon & rLex, CWnd* pParent = NULL);   

// Dialog Data
	//{{AFX_DATA(CWordDlg)
	enum { IDD = IDD_WRD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLexicon & m_Lex;
	CWord & m_Word;
    CString m_strCaption;

	void OnSilbeX( int nSilbe, LPCSTR szSilbeNum );
	void EnableSilbenButtons( int nNum );
	void EnableWordType( int nType );

	// Generated message map functions
	//{{AFX_MSG(CWordDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWordType();
	afx_msg void OnChangeNumSilben();
	afx_msg void OnSilbe1();
	afx_msg void OnSilbe2();
	afx_msg void OnSilbe3();
	afx_msg void OnSilbe4();
	afx_msg void OnSilbe5();
	afx_msg void OnPhoneBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CPhoneticDlg dialog 

class CPhoneticDlg : public CDialog
{
public:
	CPhoneticDlg( int nNumSilben, CWord & rWord );

// Dialog Data
	//{{AFX_DATA(CPhoneticDlg)
	enum { IDD = IDD_PHONEBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneticDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nNumSilben;
	CWord & m_Word;

	//{{AFX_MSG(CPhoneticDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORDDLG_H__23EAD920_DCC2_11D9_B4C8_444553540000__INCLUDED_)
