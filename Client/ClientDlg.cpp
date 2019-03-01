
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include<string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_EDIT2, m_recive);
	DDX_Control(pDX, IDC_EDIT1, m_SessionID);
	DDX_Control(pDX, IDC_EDIT3, m_port);
	DDX_Control(pDX, IDC_PROGRESS1, m_prog);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHECK_NET, &CClientDlg::OnBnClickedButtonCheckNet)
	ON_BN_CLICKED(IDC_BUTTON_RECIVE, &CClientDlg::OnBnClickedButtonRecive)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BUTTON_RECIVE)->EnableWindow(FALSE);
	Util::LAN_FILE::Client::RegisterCheckNetSink(this);
	Util::LAN_FILE::Client::RegisterClientSink(this);

	//m_IP.SetAddress(172, 20, 13, 34);
	//m_recive.SetWindowText(L"E:\\innosetup_plugin\\局域网文件传输\\LAN_FILE\\OutPut\\Debug");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


std::string CvtW2A(const std::wstring &strValue, UINT cp)
{
	char szData[1024] = { 0 };
	std::string strBufer;

	if (strValue.empty()) return "";
	int nLen = (int)strValue.size();
	int nRet = ::WideCharToMultiByte(cp, 0, strValue.data(), nLen, szData, 1024, NULL, NULL);
	if (nRet > 0) return strBufer.assign(szData, nRet);

	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		int nRet = ::WideCharToMultiByte(cp, 0, strValue.data(), nLen, NULL, 0, NULL, NULL);
		if (nRet > 0)
		{
			char *pBuf = new char[nRet];
			::WideCharToMultiByte(cp, 0, strValue.data(), nLen, pBuf, nRet, NULL, NULL);
			strBufer.assign(pBuf, nRet);
			delete[]pBuf;
			return strBufer;
		}
	}
	return "";
}
std::wstring CvtA2W(const std::string & strValue, UINT cp)
{
	wchar_t szData[1024] = { 0 };
	std::wstring strBufer;

	if (strValue.empty()) return L"";
	int nLen = (int)strValue.size();
	int nRet = ::MultiByteToWideChar(cp, 0, strValue.data(), nLen, szData, 1024);
	if (nRet > 0) return strBufer.assign(szData, nRet);

	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		int nRet = ::MultiByteToWideChar(cp, 0, strValue.data(), nLen, NULL, 0);
		if (nRet > 0)
		{
			wchar_t *pBuf = new wchar_t[nRet];
			::MultiByteToWideChar(cp, 0, strValue.data(), nLen, pBuf, nRet);
			strBufer.assign(pBuf, nRet);
			delete[]pBuf;
			return strBufer;
		}
	}
	return L"";
}

void CClientDlg::OnPorgress(const char* szSessionID, UINT64 uPos, UINT64 uTotal)
{
	int rate = (uPos * 100) / uTotal;
//	m_prog.SetPos(rate);
}
void CClientDlg::OnResult(const char* szSessionID, UINT32 uCode)
{
	if (0 == uCode)
	{
	//	m_prog.SetPos(100);
		GetDlgItem(IDC_BUTTON_RECIVE)->EnableWindow(FALSE);
	}
	else
	{
		//m_prog.SetPos(0);
		GetDlgItem(IDC_BUTTON_RECIVE)->EnableWindow(FALSE);
		AfxMessageBox(L"接收失败");
	}
		
}
void CClientDlg::OnCanLanFile(uint32_t uKey, const char* ip, bool bCan)
{
	if (bCan)
	{
		GetDlgItem(IDC_BUTTON_RECIVE)->EnableWindow(TRUE);
	}
}



void CClientDlg::OnBnClickedButtonCheckNet()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString str1;
	m_SessionID.GetWindowText(str1);
	std::string sessionid = CvtW2A(str1.GetString(), CP_UTF8);
	if (m_IP.IsBlank())
	{
		//CWnd* pWnd=GetDlgItem(IDC_BUTTON_CHECK);
		AfxMessageBox(_T("请输入对端IP，在Check"));
		return;
	}
	BYTE nf1, nf2, nf3, nf4;
	m_IP.GetAddress(nf1, nf2, nf3, nf4);
	CString ip;
	ip.Format(L"%d.%d.%d.%d", nf1, nf2, nf3, nf4);//这里的nf得到的值是IP值了.

	std::string szip = CvtW2A(ip.GetString(), CP_UTF8);

	CString strPort;
	m_port.GetWindowText(strPort);
	std::string szport = CvtW2A(strPort.GetString(), CP_UTF8);
	int port = atoi(szport.c_str());

	int nCode=Util::LAN_FILE::Client::CanLanFile(szip.c_str(), port);
	if (1 == nCode)
	{
		GetDlgItem(IDC_BUTTON_RECIVE)->EnableWindow(TRUE);
	}
	else if (0 == nCode)
	{
		AfxMessageBox(_T("检测网络失败，不能进行局域网传输"));
	}

}


void CClientDlg::OnBnClickedButtonRecive()
{
	CString strPath;
	m_recive.GetWindowText(strPath);
	if (strPath.IsEmpty())
	{
		AfxMessageBox(_T("请输入接受路径"));
		return;
	}
	CString str1;
	m_SessionID.GetWindowText(str1);
	std::string sessionid = CvtW2A(str1.GetString(), CP_UTF8);

	std::string  recvpath = CvtW2A(strPath.GetString(), CP_UTF8);
	//Util::LAN_FILE::Client::ReciveFile(sessionid.c_str(), recvpath.c_str());
}


void CClientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str1;
	m_SessionID.GetWindowText(str1);
	std::string sessionid = CvtW2A(str1.GetString(), CP_UTF8);
	//Util::LAN_FILE::Client::CancelRecive(sessionid.c_str());
}


void CClientDlg::OnDestroy()
{
	__super::OnDestroy();

	Util::LAN_FILE::Client::DoClientExit();
	// TODO: 在此处添加消息处理程序代码
}
