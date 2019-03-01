
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include<string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClientDlg �Ի���



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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BUTTON_RECIVE)->EnableWindow(FALSE);
	Util::LAN_FILE::Client::RegisterCheckNetSink(this);
	Util::LAN_FILE::Client::RegisterClientSink(this);

	//m_IP.SetAddress(172, 20, 13, 34);
	//m_recive.SetWindowText(L"E:\\innosetup_plugin\\�������ļ�����\\LAN_FILE\\OutPut\\Debug");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		AfxMessageBox(L"����ʧ��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str1;
	m_SessionID.GetWindowText(str1);
	std::string sessionid = CvtW2A(str1.GetString(), CP_UTF8);
	if (m_IP.IsBlank())
	{
		//CWnd* pWnd=GetDlgItem(IDC_BUTTON_CHECK);
		AfxMessageBox(_T("������Զ�IP����Check"));
		return;
	}
	BYTE nf1, nf2, nf3, nf4;
	m_IP.GetAddress(nf1, nf2, nf3, nf4);
	CString ip;
	ip.Format(L"%d.%d.%d.%d", nf1, nf2, nf3, nf4);//�����nf�õ���ֵ��IPֵ��.

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
		AfxMessageBox(_T("�������ʧ�ܣ����ܽ��о���������"));
	}

}


void CClientDlg::OnBnClickedButtonRecive()
{
	CString strPath;
	m_recive.GetWindowText(strPath);
	if (strPath.IsEmpty())
	{
		AfxMessageBox(_T("���������·��"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str1;
	m_SessionID.GetWindowText(str1);
	std::string sessionid = CvtW2A(str1.GetString(), CP_UTF8);
	//Util::LAN_FILE::Client::CancelRecive(sessionid.c_str());
}


void CClientDlg::OnDestroy()
{
	__super::OnDestroy();

	Util::LAN_FILE::Client::DoClientExit();
	// TODO: �ڴ˴������Ϣ����������
}
