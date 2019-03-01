
// SeverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Sever.h"
#include "SeverDlg.h"
#include "afxdialogex.h"

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


// CSeverDlg 对话框



CSeverDlg::CSeverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEVER_DIALOG, pParent) 
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m);
	DDX_Control(pDX, IDC_LIST1, m_SendList);
	DDX_Control(pDX, IDC_EDIT1, m_Port);
}

BEGIN_MESSAGE_MAP(CSeverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START_SEVER, &CSeverDlg::OnBnClickedButtonStartSever)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CSeverDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSeverDlg 消息处理程序

bool GetLocalIP(std::string& strIP)
{
	WSADATA WSAData;
	char hostName[256];
	if (!WSAStartup(MAKEWORD(2, 0), &WSAData))
	{
		if (!gethostname(hostName, sizeof(hostName)))
		{
			hostent *host = gethostbyname(hostName);
			if (host != NULL)
			{
				strIP = inet_ntoa(*(struct in_addr*)*host->h_addr_list);
				return true;
			}
		}
	}
	return false;
}
void SplitIP(const char* ip, std::vector<int>& vcIP)
{
	std::string szIP = ip;
	int pos = 0;
	for (int i = 0; i < 4; i++)
	{
		int nIndex = szIP.find('.', pos);
		int nCount = nIndex - pos;
		if (-1 == nIndex)
		{
			nCount = szIP.length() - pos;
		}
		std::string szTemp = szIP.substr(pos, nCount);
		int nIp = atoi(szTemp.c_str());
		vcIP.push_back(nIp);
		pos = nIndex + 1;
	}

}
BOOL CSeverDlg::OnInitDialog()
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
	std::string strIP;
	if (GetLocalIP(strIP))
	{
		std::vector<int> vc;
		SplitIP(strIP.c_str(), vc);
		m.SetAddress(vc[0], vc[1], vc[2], vc[3]);
	}

	m_SendList.InsertColumn(0, _T("sessionid"), 0, 100);
	m_SendList.InsertColumn(1, _T("path"), 0, 200);
	m_SendList.InsertColumn(2, _T("progress"), 0, 100);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSeverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSeverDlg::OnPaint()
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
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSeverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSeverDlg::OnPorgress(const char* szSessionID, UINT64 uPos, UINT64 uTotal)
{
	std::string sessionid = szSessionID;
	int rate = (uPos * 100) / uTotal;
	CString strText;
	strText.Format(L"%d", rate);
	if (sessionid == std::string("11"))
	{
		m_SendList.SetItemText(0, 2, strText);
	}
	else if (sessionid == std::string("22"))
	{
		m_SendList.SetItemText(1, 2, strText);
	}
	else if (sessionid == std::string("33"))
	{
		m_SendList.SetItemText(2, 2, strText);
	}
}
void CSeverDlg::OnBeginSever(bool bOK, int Port)
{
	CString strText;
	strText.Format(L"%d", Port);
	m_Port.SetWindowText(strText);
}


void CSeverDlg::OnResult(const char* szSessionID, UINT32 uCode)
{
	std::string sessionid = szSessionID;
	CString strText(L"100");
	if (1 == uCode)
	{
		strText = L"网络原因，发送失败";
	}
	if (sessionid == std::string("11"))
	{
		m_SendList.SetItemText(0, 2, strText);
	}
	else if (sessionid == std::string("22"))
	{
		m_SendList.SetItemText(1, 2, strText);
	}
	else if (sessionid == std::string("33"))
	{
		m_SendList.SetItemText(2, 2, strText);
	}
}

void CSeverDlg::OnBnClickedButtonStartSever()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPort=Util::LAN_FILE::Sever::Init(this);
	if (-1 == nPort)
	{
		AfxMessageBox(L"服务端已经启动");
		return;
	}
	Util::LAN_FILE::Sever::RegisterSeverSink(this);   //注册事件
	//CString strText;
	//strText.Format(L"%d", nPort);
	//m_Port.SetWindowText(strText);

	CString str(L"F:\\安装切图0724");
	std::string s1=CvtW2A(str.GetString(),CP_UTF8);
	CString str1(L"F:\\高DPI(Duilib)");
	std::string s11 = CvtW2A(str1.GetString(), CP_UTF8);
	// 添加服务端要发送的文件
	Util::LAN_FILE::Sever::SendFile("11", s1.c_str());
	Util::LAN_FILE::Sever::SendFile("22", s11.c_str());
	Util::LAN_FILE::Sever::SendFile("33", "F:\\CodeStorage.rar");
	Util::LAN_FILE::Sever::SendFile("44", "D:\\svc.log");
	
	m_SendList.InsertItem(0, _T("11"));
	m_SendList.SetItemText(0, 1, L"F:\\安装切图0724");

	m_SendList.InsertItem(1, L"22");
	m_SendList.SetItemText(1, 1, L"F:\\高DPI(Duilib)");

	m_SendList.InsertItem(2, L"33");
	m_SendList.SetItemText(2, 1, L"F:\\CodeStorage.rar");

	m_SendList.InsertItem(3, L"44");
	m_SendList.SetItemText(3, 1, L"D:\\svc.log");
}


void CSeverDlg::OnDestroy()
{
	__super::OnDestroy();

	Util::LAN_FILE::Sever::UnInit();
	// TODO: 在此处添加消息处理程序代码
}


void CSeverDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	Util::LAN_FILE::Sever::CancelSend("33");
}
