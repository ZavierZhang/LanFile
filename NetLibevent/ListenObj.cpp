#include "stdafx.h"
#include "ListenObj.h"
#include "ConnectQueue.h"
#include "ConnObj.h"

CListenObj::CListenObj()
{
	m_port = 0;
}


CListenObj::~CListenObj()
{
}
void CListenObj::Reg(Util::ListenEvent::CListenEve* pSink)
{
	m_pSink = pSink;
}
void CListenObj::Run_Thread(void* param)
{
	//listen ��������
	m_ListenEvent.SetMultiThreadSupport(true);  //���ö��߳�֧��  ��߹رն��߳�֧�� ����ᵼ���ڴ�й© Ŀǰδ��λ��ԭ��
	m_ListenEvent.CreateEventBase();
	evconnlistener* listen= m_ListenEvent.AttachListen(listener_cb, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, m_port);  //����
	bool bOK = false;
	if (NULL != listen)
		bOK = true;
	//֪ͨ�����������ɹ�
	if (NULL != m_pSink)
		m_pSink->OnListen(bOK, m_ListenEvent.GetListenPort());
	m_ListenEvent.Loop();   //�¼�����ѭ��
}
void CListenObj::OnFinish_Thread()
{
	m_ListenEvent.Release();
	delete this;
}
void CListenObj::listener_cb(struct evconnlistener * listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
	CListenObj* pListen = (CListenObj*)user_data;
	if (NULL == pListen)
		return;
	pListen->OnAccept(fd);
}
void CListenObj::OnAccept(evutil_socket_t fd)
{
	if (NULL != m_pSink)
	{
		m_pSink->OnAccept(fd);
	}
}
int CListenObj::Start(int Port)
{
	m_port = Port;
	CreateThread(this);
	return 0;
}
void CListenObj::Stop()
{
	m_ListenEvent.QuitLoop();  //�˳��¼�ѭ��
}
