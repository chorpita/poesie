#if !defined(AFX_WORDNETDLG_H__36265B60_20AF_11DA_B4C8_444553540000__INCLUDED_)
#define AFX_WORDNETDLG_H__36265B60_20AF_11DA_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WordNetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWordNetDlg dialog

class CWordNetDlg : public CDialog
{
// Construction
public:
	CWordNetDlg
		( 
		CLexBrain & rLB,
		CUserOptions & rUO,
		CCoreVector & rCOR, 
		CWnd * pParent = NULL 
		);
	
// Dialog Data
	//{{AFX_DATA(CWordNetDlg)
	enum { IDD = IDD_WORDNET_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordNetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLexBrain & m_LB;
	CUserOptions & m_UO;
	CCoreVector & m_COR;

	// Generated message map functions
	//{{AFX_MSG(CWordNetDlg)
	afx_msg void OnGenerate();
	afx_msg void OnDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORDNETDLG_H__36265B60_20AF_11DA_B4C8_444553540000__INCLUDED_)
