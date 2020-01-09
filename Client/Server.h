#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CServerApp:
// �� Ŭ������ ������ ���ؼ��� Server.cpp�� �����Ͻʽÿ�.
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

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

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

// clientsock ��� ����Դϴ�.

class clientsock : public CAsyncSocket
{
public:
	clientsock();
	virtual ~clientsock();
	virtual void OnClose(int nErrorCode);
	//	virtual void OnSend(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

// CListenSock ��� ����Դϴ�.

class CListenSock : public CAsyncSocket
{
public:
	CListenSock();
	virtual ~CListenSock();
	virtual void OnAccept(int nErrorCode);
};


extern CServerApp theApp;