#if !defined(AFX_SILBEDLG_H__23EAD921_DCC2_11D9_B4C8_444553540000__INCLUDED_)
#define AFX_SILBEDLG_H__23EAD921_DCC2_11D9_B4C8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SilbeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSilbeDlg dialog

class CSilbePhoneDlg : public CDialog
{
// Construction
public:
	CSilbePhoneDlg
		( 
		LPCSTR szCaption, 
		CSilbe & rSilbe,
		int nDefaultFilter, 
		CWnd* pParent = NULL
		);  

// Dialog Data
	//{{AFX_DATA(CSilbePhoneDlg)
	enum { IDD = IDD_SILBE_PHONE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSilbePhoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   
	CString m_strCaption;
    CSilbe & m_Silbe;
    int m_nFilter;

	// Generated message map functions
	//{{AFX_MSG(CSilbePhoneDlg)
	virtual BOOL OnInitDialog();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CSilbeDlg : public CDialog
{
// Construction
public:
	CSilbeDlg
		( 
		LPCSTR szCaption, 
		CWord & rWord,
		int nSilbe,
		int nDefaultFilter, 
		CWnd* pParent = NULL
		);  

// Dialog Data
	//{{AFX_DATA(CSilbeDlg)
	enum { IDD = IDD_SILBE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSilbeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   
	CString m_strCaption;
    CWord & m_Word;
	int m_nSilbe;
    int m_nFilter;

	void SilbeBreakCode();

	// Generated message map functions
	//{{AFX_MSG(CSilbeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSilbePhone();
	afx_msg void OnBreakPlus();	
	afx_msg void OnBreakMinus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SILBEDLG_H__23EAD921_DCC2_11D9_B4C8_444553540000__INCLUDED_)
