
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "UtilLanFileTrans.h"
#include "afxwin.h"

// CClientDlg �Ի���
class CClientDlg : public CDialogEx,public Util::LAN_FILE::FILE_TRANSFER_EVENT::CFileTransferEvent,public Util::LAN_FILE::FILE_TRANSFER_EVENT::CNetCheckEvent
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	 void OnPorgress(const char* szSessionID, UINT64 uPos, UINT64 uTotal);  // ���Ȼص��ӿ�
	 void OnResult(const char* szSessionID, UINT32 uCode);   //������ݻص�
	
	 virtual void OnCanLanFile(uint32_t uKey, const char* ip, bool bCan);
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
