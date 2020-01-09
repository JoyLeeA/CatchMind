#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxsock.h"
#include "ScoreBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CServerApp ����

CServerApp::CServerApp()
	: m_pServer(NULL)
	, m_pChild(NULL)
	, QueC(_T(""))
	, AnsC(_T(""))
	, ThicknessC(0)
	, cnum(0)
	, snum(0)
	, CReadyflag(0)
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
	m_pServer=NULL;
	m_pChild=NULL;
}


// ������ CServerApp ��ü�Դϴ�.

CServerApp theApp;


// CServerApp �ʱ�ȭ

BOOL CServerApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�.
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ��ȭ ���ڿ� �� Ʈ�� �� �Ǵ�
	// �� ��� �� ��Ʈ���� ���ԵǾ� �ִ� ��� �� �����ڸ� ����ϴ�.
	CShellManager *pShellManager = new CShellManager;

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	CServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ [Ȯ��]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ [���]�� Ŭ���Ͽ� ��ȭ ���ڰ� ������ �� ó����
		//  �ڵ带 ��ġ�մϴ�.
	}

	// ������ ���� �� �����ڸ� �����մϴ�.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�  ���� ���α׷��� ���� �� �ֵ��� FALSE��
	// ��ȯ�մϴ�.
	return FALSE;
}



void CServerApp::InitServer(void)
{
	m_pServer=new CListenSock;//���� ����
	m_pServer->Create(7777);//��Ʈ��ȣ 7777�� ���� ����
	m_pServer->Listen();
}


void CServerApp::Accept(void)
{
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();
	if((cnum<10)&&(dlg->gameflag==0))	//Ŭ���̾�Ʈ 10������� ����, �̹� ������ �������� ��쿡�� ���� �Ұ�.
	{
		m_pChild=new clientsock;//Ŭ���̾�Ʈ ���ϻ���
		m_pServer->Accept(*m_pChild);//������ Ŭ���̾�Ʈ ����
		m_pMainWnd->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);//������ �Ǿ�� ä��â�� ������ ��ư Ȱ��ȭ
		cnum++;//Ŭ���̾�Ʈ(����) ī����
		m_pClient.AddTail(m_pChild);	//���ο� Ŭ���̾�Ʈ �����ϵ��� �߰�
	}
	else if((cnum==10)&&(dlg->gameflag==0))//Ŭ���̾�Ʈ 10���� á�� ��
	{
		m_pChild=new clientsock;//Ŭ���̾�Ʈ ���ϻ���
		m_pServer->Accept(*m_pChild);//������ Ŭ���̾�Ʈ ����(accept)
		m_pMainWnd->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);//������ �Ǿ�� ä��â�� ������ ��ư Ȱ��ȭ
		cnum++;
	}
}

//clientsock�� �⺻ �����ڿ� �⺻ �Ҹ���
clientsock::clientsock()
{
}

clientsock::~clientsock()
{
}

void clientsock::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CServerApp*)AfxGetApp())->CloseChild();
	CAsyncSocket::OnClose(nErrorCode);
}


//void clientsock::OnSend(int nErrorCode)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//
//	CAsyncSocket::OnSend(nErrorCode);
//}


void clientsock::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CServerApp*)AfxGetApp())->ReceiveData(this);
	CAsyncSocket::OnReceive(nErrorCode);
}

// CListenSock

CListenSock::CListenSock()
{
}

CListenSock::~CListenSock()
{
}


// CListenSock ��� �Լ�


void CListenSock::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	((CServerApp*)AfxGetApp())->Accept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void CServerApp::SendData(char& header, CString& id, CString& str)
	//ä�õ����͸� Ŭ���̾�Ʈ���� ������.
{
	POSITION pos = m_pClient.GetHeadPosition();//Ŭ���̾�Ʈ�� ��ġ ����
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();//DialogŬ���� ���� ����
	packet.header=header;
	packet.ID=id;
	packet.IDlen='0'+packet.ID.GetLength();
	CString sdata;
	packet.data=str;
	sdata.Format(_T("%c%c%s%s"),packet.header,packet.IDlen,packet.ID,packet.data);//1���� ���ڿ�(CString)�����ͷ� ���� ����
	while(pos != NULL)//Ŭ���̾�Ʈ�� ����� ��ŭ �����͸� ����
	{
		m_pChild = m_pClient.GetNext(pos);	//���� Ŭ���̾�Ʈ�� ����
		m_pChild->Send((LPCTSTR)sdata, sizeof(TCHAR)*(sdata.GetLength()+1));	//�����͸� Ŭ���̾�Ʈ�� ������.
	}
	if(!id.Compare(dlg->m_SID))	//������ ������ �����Ϳ� ���ؼ��� ����
	{
		if(packet.header=='M')		
		{
			CString strText;
			strText.Format(_T("[��]:%s"), str);
			((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1, strText);
			dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		}
		else if(packet.header=='A')	
		{
			CString Solution;
			Solution.Format(_T("[��]:����:%s"),str);
			((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1, Solution);
			dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		}
	}
}


void CServerApp::ReceiveData(clientsock* ClientAddress)
{
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();//DialogŬ���� ���� ����
	char buf[200];
	ClientAddress->Receive(buf, sizeof(buf));//�����͸� �޾Ƽ� temp�� ����
	CString data((LPCTSTR) buf);//char -> CString ����ȯ
	CString strText, strPoint[4], strColor[3], ID;
	int idlen;
	CPoint ptStart, ptEnd;//�׸������͸� �ޱ����� �������ش�.
	idlen=_ttoi(data.Mid(1,1));//�޾ƿ� ������ ID���̸� ������ ����
	ID=data.Mid(2,idlen);//�޾ƿ� ID���̸�ŭ ID�� �о ������ ����
	if(data.Left(1)=='Q')//���������� �޾ƿ�����
	{
		QueC=data.Mid(2+idlen);//Q�� �� �����������͸� ��������
		dlg->m_GameTimebar.SetRange(0,180);	//������ �޾ƿȰ� ���ÿ� �ð���� ����
		dlg->m_GameTimebar.SetPos(180);		//���ѽð� 180��=3��
		dlg->SetTimer(1,1000,NULL);//������ �޾ƿȰ� ���ÿ� ���� Ÿ�̸� ����
		dlg->StartTime=CTime::GetCurrentTime();//���۵Ǵ� ������ �ð��� ������
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
	}
	else if(data.Left(1)=='A')//���䵥���� �޾ƿ�����
	{
		CString answer;
		answer.Format(_T("[%s]:����:%s"),ID,data.Mid(2+idlen));
		((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1, answer);//Ŭ���̾�Ʈ�� ������ ä��â�� ǥ��
		dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
		if((dlg->myturn==dlg->gameturn))//���� �������� ������ �� StopQuestion�Լ� ����
		{
			AnsC=data.Mid(2+idlen);//A�� ���� ���� �����͸� ��������
			dlg->RecieveID=ID;//���侴 ������ �г��� ������ ����
			dlg->StopQuestion();//DlgŬ������ StopQuestion�Լ� ����
		}
	}
	else if(data.Left(1)=='S')//���������� �޾ƿ�����
	{
		snum++;//���������� �޾ƿ� ���� üũ
		if(snum <= cnum)//cnum(������ ��)�� �Ǳ� ������ �޾ƿͼ� ������ ����
		{
			for(int j=1;j<(cnum+1);j++)//������ ����ŭ ������ ����
			{
				if(!(dlg->UserID[j].Mid(1).Compare(ID)))//������ �޾ƿ� ������ ���̵� �˻�
					dlg->Score[j]=_ttoi(data.Mid(2+idlen));//�´� ��ȣ�� ������ ���������͸� ����
			}
		}
		if(snum == cnum)//������ ����ŭ �޾ƿ��� �ٸ� �����鿡�� ��ü �������� �Ѹ�
		{
			for(int k=(cnum+1); k<10; k++)//��ĭ(��������)�� 1�� ǥ��
			{
				dlg->Score[k]=1;
			}
			dlg->Score[0]=dlg->m_Score;
			AfxMessageBox(_T("������ �������ϴ�! \r\n����� Ȯ���ϼ���!"));
			CString ScoreData;
			ScoreData.Format(_T("$%d$%d$%d$%d$%d"),dlg->m_Score,dlg->Score[1],dlg->Score[2],dlg->Score[3],dlg->Score[4]);
			//��� ������ ���� �����͸� �ѹ��� ����
			SendData(buf[0],dlg->m_SID,ScoreData);
			CScoreBoardDlg open;//��ü ������ ���
			open.DoModal();
		}
	}
	else if(data.Left(1)=='P')//�׸������͸� �޾ƿ�����
	{
		//���ڿ��� $�� ���� ������.
		for(int j=0;j<4;j++)
		{
			AfxExtractSubString( strPoint[j], data.Mid(2+idlen), (j+1), '$');
		}
		//CString->int�� ����ȯ���־� ��ǥ���� �־��ش�.
		ptStart.x=_ttoi(strPoint[0]);
		ptStart.y=_ttoi(strPoint[1]);
		ptEnd.x=_ttoi(strPoint[2]);
		ptEnd.y=_ttoi(strPoint[3]);
		dlg->DrawPicture(ptStart, ptEnd);//���������͸� ȭ�鿡 �׷��ش�.
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
	}
	else if(data.Left(1)=='T')//���β��� ���� �����͸� �޾�����
	{
		ThicknessC=_ttoi(data.Mid(2+idlen));	//����ȯ�ؼ� ������ ����
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
	}
	else if(data.Left(1)=='C')//������ ���� �����͸� �޾�����
	{
		int R,G,B;
		//���ڿ��� $�� ���� ������.
		for(int k=0;k<3;k++)
		{
			AfxExtractSubString( strColor[k], data.Mid(2+idlen), k+1, '$');
		}
		//CString->int�� ����ȯ���־� R,G,B�� �־��ش�.
		R=_ttoi(strColor[0]);
		G=_ttoi(strColor[1]);
		B=_ttoi(strColor[2]);
		ColorC=RGB(R,G,B);//�޾ƿ� �������� ������ �����Ѵ�.
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
	}
	else if(data.Left(1)=='E')//ȭ�� �������� ��ȣ�� �޾ƿ�����
	{
		dlg->ScreenClear();//�Ȱ��� ȭ���� �����.
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
	}
	else if(data.Left(1)=='M')//ä�� �޼����� �޾ƿ�����
	{
		strText.Format(_T("[%s]:%s"),ID,data.Mid(2+idlen));//ID�� ä�õ����� 1���� �����ͷ� ������
		((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1,strText);//ä��â�� ǥ��
		dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
	}
	else if(data.Left(1)=='I')//���������� �޾ƿ�����
	{
		if(cnum<=4)
		{
			strText.Format(_T("[%s]���� �����߽��ϴ�."),ID);//Ŭ���̾�Ʈ�� ����ƴٰ� ä��â�� ����ó�� �˷���
			((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1,strText);
			dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
			dlg->UserID[cnum].Format(_T("%d%s"),cnum,ID);	//Ŭ���̾�Ʈ�� �г��Ӱ� ����� ������ ����
			SendData(buf[0],dlg->m_SID,dlg->UserID[cnum]);//���������� ����
		}
	}
	else if(data.Left(1)=='R')//�غ��ȣ�� �޾�����
	{
		CString Ready;
		CReadyflag++;//����� ������ �غ��ߴ��� üũ
		Ready.Format(_T("[%s]���� �غ� �Ϸ��߽��ϴ�"),ID);//�غ��� ������ ä��â�� ǥ��
		((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1,Ready);
		dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		if(CReadyflag==cnum)//������ ������ ���� �غ� �Ϸ�Ǿ��� ��
			dlg->GetDlgItem(IDC_STARTGAME)->EnableWindow(TRUE);//���ӽ��� ��ư Ȱ��ȭ
	}
	else if(data.Left(1)=='F')//�����ȣ�� �޾ƿ��� ��
	{
		if(!(data.Mid(3+idlen).Compare(dlg->m_SID)))//������ ����� �ڽ��� ��
		{
			if(cnum!=1)//Ŭ���̾�Ʈ�� 1:1�� �ƴ� ��
				SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� �������ֱ� ���� ������
			AfxMessageBox(_T("�����Դϴ�!"));//�ڽ��� ����ٴ°� �޼����ڽ��� ���
		}
		else	
		{
			if(data.Mid(3+idlen).GetLength()>0)//������ ���� ����� ������ �ƴ� ���
			{
				strText.Format(_T("%s���� ������ ���߼̽��ϴ�."),data.Mid(3+idlen));//������ ���� ������ �г����� �˾���
				AfxMessageBox(strText);
				if(cnum!=1)//Ŭ���̾�Ʈ�� 1:1�� �ƴ� ��
					SendData(buf[0],ID,data.Mid(2+idlen));	//�ٸ� Ŭ���̾�Ʈ�鿡�� ����
			}
		}
		dlg->gameturn=_ttoi(data.Mid(2+idlen,1));//������ int����ȯ
		if(dlg->gameturn==dlg->myturn)// �������� ���� ���ʷ� ���� ���	
			dlg->OnBnClickedQuestionmode();	//���� ���
		else if(dlg->gameturn==7)//������ ������ ��
			dlg->FinishGame();//������ �ʱ�ȭ �Լ� ����
		else //�� ���ʰ� �ƴ� ���
			dlg->OnBnClickedSolutionmode();	//������ ���
	}
}


void CServerApp::CloseChild(void)
{
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();
	cnum--;
	if(cnum==0)	//Client�� ���� ������ �������� ���
	{
		AfxMessageBox(_T("�ٸ� ��������� ������ ������ϴ�."));
		dlg->AllClear();
		dlg->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		CleanUp();
	}
	else	//1:1�� �ƴѰ�쿡�� Client 1���� ������ ���
		AfxMessageBox(_T("���� 1���� �������ϴ�."));
}


void CServerApp::CleanUp(void)
{
	delete m_pServer;
	POSITION pos = m_pClient.GetHeadPosition();	
	while(pos != NULL)
	{											//pos ���� ������ ����������� �ݺ� ����
		delete m_pClient.GetNext(pos);			// Ŭ���̾�Ʈ�� ����ŭ ����
	}
}

void CServerApp::SendLine(char& header, CString& id, CPoint& ptStart, CPoint& ptEnd)
{
	CString Line;
	Line.Format(_T("$%d$%d$%d$%d"), ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); //�� �������� ���� �����Ͽ� Ŭ���̾�Ʈ���� ����
	SendData(header,id,Line);
}


void CServerApp::SendThickness(char& header, CString& id, int& thickness)
{
	CString strThick;
	strThick.Format(_T("%d"),thickness);//�� �β������� CString���� ����ȯ
	SendData(header,id,strThick); //�� �β��� ���� �����͸� Ŭ���̾�Ʈ���� ����
}


void CServerApp::SendColor(char& header, CString& id, int& Red, int& Green, int& Blue)
{
	CString strColor;
	strColor.Format(_T("$%d$%d$%d"),Red, Green, Blue); //�޾ƿ� ������ �����͸� �����Ͽ� Ŭ���̾�Ʈ���� ����
	SendData(header,id,strColor);
}
