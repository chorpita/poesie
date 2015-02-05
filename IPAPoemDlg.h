#if !defined(AFX_IPAPOEMDLG_H__924EF140_3DAB_11DA_B4C8_444553540000__INCLUDED_)
#define AFX_IPAPOEMDLG_H__924EF140_3DAB_11DA_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPAPoemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPAPoemDlg dialog

class CIPAPoemDlg : public CDialog
{
// Construction
public:
	CIPAPoemDlg( CString & strPoem, CIPAManager & rIPA, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIPAPoemDlg)
	enum { IDD = IDD_IPAPOEM_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPAPoemDlg)
	protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString & m_strPoem;
	CIPAManager & m_IPA;

	// Generated message map functions
	//{{AFX_MSG(CIPAPoemDlg)
		// NOTE: the ClassWizard will add member functions here	
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPAPOEMDLG_H__924EF140_3DAB_11DA_B4C8_444553540000__INCLUDED_)
