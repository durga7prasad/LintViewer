
// LintViewerDlg.h : header file
//

#pragma once


// CLintViewerDlg dialog
class CLintViewerDlg : public CDialogEx
{
// Construction
public:
	CLintViewerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINTVIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// input lint output file
	CMFCEditBrowseCtrl f_input;
	afx_msg void OnUpdateBrowse();
	// analyze
	CButton analyze;
	afx_msg void OnAnalyze();
	// input file value
	CString FInputVal;
	// Error count value
	int ErrorCntVal;
	// warning count value
	int WarningCntVal;
	// info count value
	int InfoCntVal;
	// Note count value
	int NoteCntVal;
	// Reset window control
	CButton ResetCtrl;
	afx_msg void OnBnClickedReset();
	// process the file and update the report data like error, warning, info and note.
	int Process_File(CString FileName);
	unsigned int fileCnt;
};
