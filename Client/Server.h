#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


// CServerApp:
// 이 클래스의 구현에 대해서는 Server.cpp을 참조하십시오.
//

class CListenSock;
class clientsock;

struct Packet
	{
		char header;
		char IDlen;
		CString ID;
		CString data;
	};

class CServerApp : public CWinApp
{
public:
	CServerApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
public:
	void InitServer(void);
	void Accept(void);
	void SendData(char& header, CString& id, CString& str);
	void ReceiveData(clientsock* ClientAddress);
	void CloseChild(void);
	void CleanUp(void);
	CListenSock* m_pServer;
	clientsock* m_pChild;
	CTypedPtrList<CObList,clientsock*> m_pClient;
	CString QueC;
	CString AnsC;
	void SendLine(char& header, CString& id, CPoint& ptStart, CPoint& ptEnd);
	void SendThickness(char& header, CString& id, int& thickness);
	int ThicknessC;
	void SendColor(char& header, CString& id, int& Red, int& Green, int& Blue);
	COLORREF ColorC;
	Packet packet;
	int cnum;
	int CReadyflag;
	int snum;
};

// clientsock 명령 대상입니다.

class clientsock : public CAsyncSocket
{
public:
	clientsock();
	virtual ~clientsock();
	virtual void OnClose(int nErrorCode);
	//	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

// CListenSock 명령 대상입니다.

class CListenSock : public CAsyncSocket
{
public:
	CListenSock();
	virtual ~CListenSock();
	virtual void OnAccept(int nErrorCode);
};


extern CServerApp theApp;