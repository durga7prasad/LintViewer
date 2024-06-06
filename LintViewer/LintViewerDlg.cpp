
// LintViewerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "LintViewer.h"
#include "LintViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLintViewerDlg dialog



CLintViewerDlg::CLintViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LINTVIEWER_DIALOG, pParent)
	, FInputVal(_T(""))
	, ErrorCntVal()
	, WarningCntVal()
	, InfoCntVal()
	, NoteCntVal()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	fileCnt = 0;
}

void CLintViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, f_input);
	DDX_Control(pDX, IDC_Analyze, analyze);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, FInputVal);
	DDV_MaxChars(pDX, FInputVal, 1024);
	DDX_Text(pDX, IDC_EDIT2, ErrorCntVal);
	DDX_Text(pDX, IDC_EDIT3, WarningCntVal);
	DDX_Text(pDX, IDC_EDIT4, InfoCntVal);
	DDX_Text(pDX, IDC_EDIT5, NoteCntVal);
	DDX_Control(pDX, IDC_RESET, ResetCtrl);
}

BEGIN_MESSAGE_MAP(CLintViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_UPDATE(IDC_MFCEDITBROWSE1, &CLintViewerDlg::OnUpdateBrowse)
	ON_BN_CLICKED(IDC_Analyze, &CLintViewerDlg::OnAnalyze)
	ON_BN_CLICKED(IDC_RESET, &CLintViewerDlg::OnBnClickedReset)
END_MESSAGE_MAP()


// CLintViewerDlg message handlers

BOOL CLintViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLintViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLintViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLintViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLintViewerDlg::OnUpdateBrowse()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.

	UpdateData(true); // to get the value from control to variable
	if (FInputVal.IsEmpty())
	{
		AfxMessageBox(L"No File selected!!");
	}
	else
	{
		OutputDebugStringA("User selected file");
	}
}



void CLintViewerDlg::OnAnalyze()
{
	//HTREEITEM hTree, hErrType;
	int status = 0;

	/* Do File IO */
	status = Process_File(FInputVal);
	if (status)
	{
		// Something went wrong!!
		AfxMessageBox(L"process File failed");
	}
	else
	{
		ResetCtrl.EnableWindow(true);
		CString valueString;
		UpdateData(true);
		valueString.Format(L"\t\tSummary\r\n\n\n\tError\tWarning\tInfo\tNote\n\r\t%u\t%u\t%u\t%u", ErrorCntVal, WarningCntVal, InfoCntVal, NoteCntVal);
		AfxMessageBox(valueString, /*MB_SETFOREGROUND */ 0, NULL);
	}
}



void CLintViewerDlg::OnBnClickedReset()
{
	//int idx;
	int len = 0;

	/* Clear input selection */
	f_input.Clear();
	UpdateData(false);
	/* Clear summary */
	ErrorCntVal = 0;
	WarningCntVal = 0;
	InfoCntVal = 0;
	NoteCntVal = 0;
	UpdateData(false);
}



// process the file and update the report data like error, warning, info and note.
int CLintViewerDlg::Process_File(CString FileName)
{
	CStdioFile file;
	CString str;
	ULONGLONG fileLen = 0, readLen = 0;
	char temp[1024] = { 0 }, * ErrorType = NULL;
	char* token = NULL;

	UpdateData(true);
	if (FInputVal.IsEmpty())
	{
		AfxMessageBox(L"No File selected!!");
		return 1;
	}
	file.Open(FInputVal, CFile::modeRead);
	if (file.m_pStream)
	{
		fileLen = file.GetLength();
		readLen = 0;
		/* Loop till the EOF */
		while(1)
		{
			/* Read a line */
			file.ReadString(str);
			/* Parse the line */
			if (str.IsEmpty())
			{
				/* Got empty line, skip it */
			}
			else
			{
				/* Dump the line to log */
				OutputDebugStringA("read line");
				sprintf(temp, "%S", str);
				switch (temp[0])
				{
					case '"':
						/* This is formatted line */
						// -format="%(%f:%l: %)%t %n: %m"
#if 0
						if (temp[2] == ':')
						{
							/* File name has full path */
							token = strtok(temp, ":");
							while (token != NULL)
							{
								//printf("%s\n", token);
								OutputDebugStringA("------------\n");
								OutputDebugStringA(temp);
								OutputDebugStringA("\n");
								OutputDebugStringA(token);
								OutputDebugStringA("\n");
								OutputDebugStringA("------------\n");
								token = strtok(NULL, ":");
							}
						}
						else
						{
							/* File name has no path */
						}
#endif
						ErrorType = strstr(temp, "Error");
						if (ErrorType)
						{
							/* Its Error */
							ErrorCntVal += 1;
						}
						ErrorType = strstr(temp, "Warning");
						if (ErrorType)
						{
							/* Its Error */
							WarningCntVal += 1;
						}
						ErrorType = strstr(temp, "Info");
						if (ErrorType)
						{
							/* Its Error */
							InfoCntVal += 1;
						}
						ErrorType = strstr(temp, "Note");
						if (ErrorType)
						{
							/* Its Error */
							NoteCntVal += 1;
						}
						break;
					default:
						/* skip lines empty/start with '#' */
						break;
				}
				readLen += str.GetLength();
			}
			/* Update by 1 New line sequence */
			readLen += 1;
			/* Update by 1 for char endcoding */
			readLen += 1;
			if (readLen >= fileLen)
			{
				/* EOF */
				break;
			}
		}
		UpdateData(false);
		file.Close();
	}
	else
	{
		AfxMessageBox(L"Unable to open file!! Please check permissions");
		return 1;
	}

	return 0;
}
