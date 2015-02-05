#ifndef __OPTIONSDLG_H
#define __OPTIONSDLG_H

class COptionsDlg : public CDialog
{
public:
	COptionsDlg( CUserOptions * pOptions );

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CUserOptions * m_pOptions;

	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCompileropts();
	afx_msg void OnSelchangeCompiler();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __OPTIONSDLG_H
