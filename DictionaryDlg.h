#if !defined(AFX_DICTIONARYDLG_H__8C09B060_DC06_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_DICTIONARYDLG_H__8C09B060_DC06_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DictionaryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDictionaryDlg dialog

class CDictionaryDlg : public CDialog
{
// Construction
public:
	CDictionaryDlg( CLexicon & rLexicon, BOOL bSuper, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDictionaryDlg)
	enum { IDD = IDD_DICTIONARY_DIALOG  };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDictionaryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListBox * m_pListBox;
    CLexicon & m_Lex;
	BOOL m_bSuper;
	// Generated message map functions
	//{{AFX_MSG(CDictionaryDlg)
		virtual BOOL OnInitDialog();
	afx_msg void OnDblclkWordlist();
	afx_msg void OnDeleteword();
	afx_msg void OnEditword();
	afx_msg void OnNewword();
	afx_msg void OnSelchangeWordlist();
	afx_msg void OnLexbox();
	afx_msg void OnImport();
	afx_msg void OnPhonebox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DICTIONARYDLG_H__8C09B060_DC06_11D9_B4C8_444553540000__INCLUDED_)
