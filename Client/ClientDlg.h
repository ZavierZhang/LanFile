
// ClientDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "UtilLanFileTrans.h"
#include "afxwin.h"

// CClientDlg 对话框
class CClientDlg : public CDialogEx,public Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent,public Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	 void OnPorgress(const char* szSessionID, UINT64 uPos, UINT64 uTotal);  // 进度回调接口
	 void OnResult(const char* szSessionID, UINT32 uCode);   //结果数据回调
	
	 virtual void OnCanLanFile(uint32_t uKey, const char* ip, bool bCan);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IP;
	CEdit m_recive;
	afx_msg void OnBnClickedButtonCheckNet();
	afx_msg void OnBnClickedButtonRecive();
	CEdit m_SessionID;
	CEdit m_port;
	CProgressCtrl m_prog;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnDestroy();
};
