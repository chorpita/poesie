#if !defined(AFX_IMPORTDLG_H__611C47E0_FA3E_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_IMPORTDLG_H__611C47E0_FA3E_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog

class CImportDlg : public CDialog
{
// Construction
public:
	CImportDlg
		( 
		CIPAManager & rIPA, 
		CCoreVector & rCOR,
		CFormExc & rFE,
		CWnd* pParent = NULL
		); 
	
// Dialog Data
	//{{AFX_DATA(CImportDlg)
	enum { IDD = IDD_IMPORT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CIPAManager & m_IPA;
	CCoreVector & m_COR;
	CFormExc & m_EXC;

	void AddIPAElement( CString & strForm, CString & strIPATrans, int nSilben );

	// Generated message map functions
	//{{AFX_MSG(CImportDlg)
	afx_msg void OnCmuimport();
	afx_msg void OnCommonimport();
	afx_msg void OnExcimport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTDLG_H__611C47E0_FA3E_11D9_B4C8_444553540000__INCLUDED_)
