
// SeverDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "UtilLanFileTrans.h"
#include <vector>

// CSeverDlg 对话框
class CSeverDlg : public CDialogEx, public Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent,public Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve
{
// 构造
public:
	CSeverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void OnPorgress(const char* szSessionID, UINT64 uPos, UINT64 uTotal);
	virtual void OnResult(const char* szSessionID, UINT32 uCode);
	virtual void OnBeginSever(bool bOK, int Port);

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
	CIPAddressCtrl m;
	CListCtrl m_SendList;
	afx_msg void OnBnClickedButtonStartSever();
	CEdit m_Port;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton2();
};
