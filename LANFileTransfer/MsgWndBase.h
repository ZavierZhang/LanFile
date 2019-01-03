#pragma once
#include <windows.h>
//���� ������Ϣ�Ĵ��ڻ���
class CMsgWndBase
{
public:
	CMsgWndBase();
	~CMsgWndBase();
	HWND  GetSafeWnd();
	virtual LRESULT  OnMsgCome(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hwnd);
protected:
	HWND CreateMsgWnd();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hWnd;
};

