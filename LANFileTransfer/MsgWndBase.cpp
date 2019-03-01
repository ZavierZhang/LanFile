#include"stdafx.h"
#include "MsgWndBase.h"
const TCHAR szClassName[] = L"CMsgWndBase_Notify_Class";
const TCHAR szWndName[] = L"CMsgWndBase_Notify_Wnd";
CMsgWndBase::CMsgWndBase()
{
	m_hWnd=CreateMsgWnd();
}


CMsgWndBase::~CMsgWndBase()
{
	if (NULL != m_hWnd)
		DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}
LRESULT CALLBACK CMsgWndBase::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{      //过滤基础消息
	case  WM_NCCREATE:
	case WM_CREATE:
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_NCDESTROY:
	{
		LONG_PTR ptr = GetWindowLongPtr(hWnd, GWL_USERDATA);
		CMsgWndBase* pThis = (CMsgWndBase*)ptr;
		if (NULL != pThis)
			 pThis->OnFinalMessage(hWnd);
		break;
	}
	default:
	{
		LONG_PTR ptr = GetWindowLongPtr(hWnd, GWL_USERDATA);
		CMsgWndBase* pThis = (CMsgWndBase*)ptr;
		if (NULL != pThis)
			return pThis->OnMsgCome(message, wParam, lParam);
		break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
HWND  CMsgWndBase::GetSafeWnd()
{
	return m_hWnd;
}
HWND CMsgWndBase::CreateMsgWnd()
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = szClassName;
	RegisterClassEx(&wc);
	HWND hWnd = ::CreateWindowEx(0, szClassName, szWndName, WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), this);
	if (NULL != hWnd)
	{
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)this);  //USER DATA 存储窗口的数据  消息循环的时候获取this 指针
	}
	return hWnd;
}
LRESULT  CMsgWndBase::OnMsgCome(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(NULL!=m_hWnd)
	    return DefWindowProc(m_hWnd, message, wParam, lParam);
	return S_FALSE;
}
void CMsgWndBase::OnFinalMessage(HWND hwnd)
{ 
}
