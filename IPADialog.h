#if !defined(AFX_IPADIALOG_H__DA66D820_F89C_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_IPADIALOG_H__DA66D820_F89C_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPADialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPADialog dialog

class CIPADialog : public CDialog
{
// Construction
public:
	CIPADialog( CIPAManager & rIPA, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIPADialog)
	enum { IDD = IDD_IPA_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPADialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CIPAManager & m_IPA;

	// Generated message map functions
	//{{AFX_MSG(CIPADialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnIpatrans();
	afx_msg void OnChangeWordtext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPADIALOG_H__DA66D820_F89C_11D9_B4C8_444553540000__INCLUDED_)
