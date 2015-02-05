#if !defined(AFX_POSDLG_H__0E401C20_2961_11DA_B4C8_444553540000__INCLUDED_)
#define AFX_POSDLG_H__0E401C20_2961_11DA_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// POSDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPOSDlg dialog

class CPOSDlg : public CDialog
{
// Construction
public:
	CPOSDlg
		( 
		BOOL bWordless,
		CString & strRet, 
		CString & strWord,
		CString & strSeg,
		CStringArray * psaPlugs = NULL,
		CWnd* pParent = NULL
		);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPOSDlg)
	enum { IDD = IDD_POS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPOSDlg)
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bWordless;
	CString & m_strRet;
	CString m_strWord;
	CString m_strSeg;
	CStringArray  * m_psaPlugs;

	// Generated message map functions
	//{{AFX_MSG(CPOSDlg)
	afx_msg void OnSynN();
	afx_msg void OnSynNp();
	afx_msg void OnSynV();
	afx_msg void OnSynVs();
	afx_msg void OnSynVg();
	afx_msg void OnSynVa();
	afx_msg void OnSynVp();
	afx_msg void OnSynA();
	afx_msg void OnSynAc();
	afx_msg void OnSynAs();
	afx_msg void OnSynR();
	afx_msg void OnIgnore();
	afx_msg void OnNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSDLG_H__0E401C20_2961_11DA_B4C8_444553540000__INCLUDED_)
