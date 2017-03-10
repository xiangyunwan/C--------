// AesCodeDlg.h : header file
//

#if !defined(AFX_AESCODEDLG_H__7018599B_05B3_44CE_AB91_D58BD0FF4063__INCLUDED_)
#define AFX_AESCODEDLG_H__7018599B_05B3_44CE_AB91_D58BD0FF4063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAesCodeDlg dialog

class CAesCodeDlg : public CDialog
{
// Construction
public:
	int char2num(char ch);
	CAesCodeDlg(CWnd* pParent = NULL);	// standard constructor
	CString EnDe_filename;
// Dialog Data
	//{{AFX_DATA(CAesCodeDlg)
	enum { IDD = IDD_AESCODE_DIALOG };
	CProgressCtrl	m_prog;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAesCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAesCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBAesEn();
	afx_msg void OnBAesDe();
	afx_msg void OnBFile();
	afx_msg void OnBFileEn();
	afx_msg void OnBFileDe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AESCODEDLG_H__7018599B_05B3_44CE_AB91_D58BD0FF4063__INCLUDED_)
