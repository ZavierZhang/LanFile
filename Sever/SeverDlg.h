
// SeverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "UtilLanFileTrans.h"
#include <vector>

// CSeverDlg �Ի���
class CSeverDlg : public CDialogEx, public Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent,public Util::LAN_FILE::Sever::BeginSeverEve::CBeginSeverEve
{
// ����
public:
	CSeverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual void OnPorgress(const char* szSessionID, UINT64 uPos, UINT64 uTotal);
	virtual void OnResult(const char* szSessionID, UINT32 uCode);
	virtual void OnBeginSever(bool bOK, int Port);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
