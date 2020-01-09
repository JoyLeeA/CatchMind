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


// CServerApp 생성

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
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_pServer=NULL;
	m_pChild=NULL;
}


// 유일한 CServerApp 개체입니다.

CServerApp theApp;


// CServerApp 초기화

BOOL CServerApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다.
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));

	CServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}



void CServerApp::InitServer(void)
{
	m_pServer=new CListenSock;//소켓 생성
	m_pServer->Create(7777);//포트번호 7777로 고정 생성
	m_pServer->Listen();
}


void CServerApp::Accept(void)
{
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();
	if((cnum<10)&&(dlg->gameflag==0))	//클라이언트 10명까지로 제한, 이미 게임이 시작했을 경우에는 참여 불가.
	{
		m_pChild=new clientsock;//클라이언트 소켓생성
		m_pServer->Accept(*m_pChild);//서버와 클라이언트 연결
		m_pMainWnd->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);//연결이 되어야 채팅창의 보내기 버튼 활성화
		cnum++;//클라이언트(유저) 카운팅
		m_pClient.AddTail(m_pChild);	//새로운 클라이언트 접속하도록 추가
	}
	else if((cnum==10)&&(dlg->gameflag==0))//클라이언트 10명이 찼을 때
	{
		m_pChild=new clientsock;//클라이언트 소켓생성
		m_pServer->Accept(*m_pChild);//서버와 클라이언트 연결(accept)
		m_pMainWnd->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);//연결이 되어야 채팅창의 보내기 버튼 활성화
		cnum++;
	}
}

//clientsock의 기본 생성자와 기본 소멸자
clientsock::clientsock()
{
}

clientsock::~clientsock()
{
}

void clientsock::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CServerApp*)AfxGetApp())->CloseChild();
	CAsyncSocket::OnClose(nErrorCode);
}


//void clientsock::OnSend(int nErrorCode)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	CAsyncSocket::OnSend(nErrorCode);
//}


void clientsock::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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


// CListenSock 멤버 함수


void CListenSock::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	((CServerApp*)AfxGetApp())->Accept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void CServerApp::SendData(char& header, CString& id, CString& str)
	//채팅데이터를 클라이언트에게 보낸다.
{
	POSITION pos = m_pClient.GetHeadPosition();//클라이언트의 위치 지정
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();//Dialog클래스 참조 선언
	packet.header=header;
	packet.ID=id;
	packet.IDlen='0'+packet.ID.GetLength();
	CString sdata;
	packet.data=str;
	sdata.Format(_T("%c%c%s%s"),packet.header,packet.IDlen,packet.ID,packet.data);//1개의 문자열(CString)데이터로 만들어서 보냄
	while(pos != NULL)//클라이언트가 연결된 만큼 데이터를 보냄
	{
		m_pChild = m_pClient.GetNext(pos);	//다음 클라이언트를 연결
		m_pChild->Send((LPCTSTR)sdata, sizeof(TCHAR)*(sdata.GetLength()+1));	//데이터를 클라이언트에 보낸다.
	}
	if(!id.Compare(dlg->m_SID))	//서버가 보내는 데이터에 대해서만 수행
	{
		if(packet.header=='M')		
		{
			CString strText;
			strText.Format(_T("[나]:%s"), str);
			((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1, strText);
			dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		}
		else if(packet.header=='A')	
		{
			CString Solution;
			Solution.Format(_T("[나]:정답:%s"),str);
			((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1, Solution);
			dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		}
	}
}


void CServerApp::ReceiveData(clientsock* ClientAddress)
{
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();//Dialog클래스 참조 선언
	char buf[200];
	ClientAddress->Receive(buf, sizeof(buf));//데이터를 받아서 temp에 저장
	CString data((LPCTSTR) buf);//char -> CString 형변환
	CString strText, strPoint[4], strColor[3], ID;
	int idlen;
	CPoint ptStart, ptEnd;//그림데이터를 받기위해 선언해준다.
	idlen=_ttoi(data.Mid(1,1));//받아온 유저의 ID길이를 변수에 저장
	ID=data.Mid(2,idlen);//받아온 ID길이만큼 ID를 읽어서 변수에 저장
	if(data.Left(1)=='Q')//문제데이터 받아왔을때
	{
		QueC=data.Mid(2+idlen);//Q를 뺀 나머지데이터를 새로저장
		dlg->m_GameTimebar.SetRange(0,180);	//문제를 받아옴과 동시에 시간재기 시작
		dlg->m_GameTimebar.SetPos(180);		//제한시간 180초=3분
		dlg->SetTimer(1,1000,NULL);//문제를 받아옴과 동시에 순간 타이머 시작
		dlg->StartTime=CTime::GetCurrentTime();//시작되는 순간의 시간을 가져옴
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
	}
	else if(data.Left(1)=='A')//정답데이터 받아왔을때
	{
		CString answer;
		answer.Format(_T("[%s]:정답:%s"),ID,data.Mid(2+idlen));
		((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1, answer);//클라이언트의 정답을 채팅창에 표시
		dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
		if((dlg->myturn==dlg->gameturn))//내가 문제내는 차례일 때 StopQuestion함수 실행
		{
			AnsC=data.Mid(2+idlen);//A를 빼고 남은 데이터를 새로저장
			dlg->RecieveID=ID;//정답쓴 유저의 닉네임 변수에 저장
			dlg->StopQuestion();//Dlg클래스의 StopQuestion함수 실행
		}
	}
	else if(data.Left(1)=='S')//점수데이터 받아왔을때
	{
		snum++;//점수데이터 받아온 갯수 체크
		if(snum <= cnum)//cnum(유저의 수)가 되기 전까지 받아와서 변수에 저장
		{
			for(int j=1;j<(cnum+1);j++)//유저의 수만큼 변수에 저장
			{
				if(!(dlg->UserID[j].Mid(1).Compare(ID)))//점수를 받아온 유저의 아이디를 검색
					dlg->Score[j]=_ttoi(data.Mid(2+idlen));//맞는 번호의 변수에 점수데이터를 저장
			}
		}
		if(snum == cnum)//유저의 수만큼 받아오면 다른 유저들에게 전체 점수정보 뿌림
		{
			for(int k=(cnum+1); k<10; k++)//빈칸(유저없음)은 1로 표시
			{
				dlg->Score[k]=1;
			}
			dlg->Score[0]=dlg->m_Score;
			AfxMessageBox(_T("게임이 끝났습니다! \r\n결과를 확인하세요!"));
			CString ScoreData;
			ScoreData.Format(_T("$%d$%d$%d$%d$%d"),dlg->m_Score,dlg->Score[1],dlg->Score[2],dlg->Score[3],dlg->Score[4]);
			//모든 유저의 점수 데이터를 한번에 보냄
			SendData(buf[0],dlg->m_SID,ScoreData);
			CScoreBoardDlg open;//전체 점수판 띄움
			open.DoModal();
		}
	}
	else if(data.Left(1)=='P')//그림데이터를 받아왔을때
	{
		//문자열을 $에 따라서 나눈다.
		for(int j=0;j<4;j++)
		{
			AfxExtractSubString( strPoint[j], data.Mid(2+idlen), (j+1), '$');
		}
		//CString->int로 형변환해주어 좌표값에 넣어준다.
		ptStart.x=_ttoi(strPoint[0]);
		ptStart.y=_ttoi(strPoint[1]);
		ptEnd.x=_ttoi(strPoint[2]);
		ptEnd.y=_ttoi(strPoint[3]);
		dlg->DrawPicture(ptStart, ptEnd);//받은데이터를 화면에 그려준다.
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
	}
	else if(data.Left(1)=='T')//선두께에 대한 데이터를 받았을때
	{
		ThicknessC=_ttoi(data.Mid(2+idlen));	//형변환해서 변수에 저장
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
	}
	else if(data.Left(1)=='C')//선색상에 대한 데이터를 받았을때
	{
		int R,G,B;
		//문자열을 $에 따라서 나눈다.
		for(int k=0;k<3;k++)
		{
			AfxExtractSubString( strColor[k], data.Mid(2+idlen), k+1, '$');
		}
		//CString->int로 형변환해주어 R,G,B에 넣어준다.
		R=_ttoi(strColor[0]);
		G=_ttoi(strColor[1]);
		B=_ttoi(strColor[2]);
		ColorC=RGB(R,G,B);//받아온 색정보를 변수에 대입한다.
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
	}
	else if(data.Left(1)=='E')//화면 지워지는 신호를 받아왔을때
	{
		dlg->ScreenClear();//똑같이 화면을 지운다.
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
	}
	else if(data.Left(1)=='M')//채팅 메세지를 받아왔을때
	{
		strText.Format(_T("[%s]:%s"),ID,data.Mid(2+idlen));//ID와 채팅데이터 1개의 데이터로 묶어줌
		((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1,strText);//채팅창에 표시
		dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
	}
	else if(data.Left(1)=='I')//정보데이터 받아왔을때
	{
		if(cnum<=4)
		{
			strText.Format(_T("[%s]님이 참가했습니다."),ID);//클라이언트가 연결됐다고 채팅창에 공지처럼 알려줌
			((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1,strText);
			dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
			dlg->UserID[cnum].Format(_T("%d%s"),cnum,ID);	//클라이언트의 닉네임과 연결된 순서를 저장
			SendData(buf[0],dlg->m_SID,dlg->UserID[cnum]);//순서정보를 보냄
		}
	}
	else if(data.Left(1)=='R')//준비신호를 받았을때
	{
		CString Ready;
		CReadyflag++;//몇명의 유저가 준비했는지 체크
		Ready.Format(_T("[%s]님이 준비를 완료했습니다"),ID);//준비한 유저를 채팅창에 표시
		((CListBox*)m_pMainWnd->GetDlgItem(IDC_CHATROOM))->InsertString(-1,Ready);
		dlg->m_list.SetTopIndex(dlg->m_list.GetCount()-1);
		if(CReadyflag==cnum)//참가한 유저가 전부 준비가 완료되었을 때
			dlg->GetDlgItem(IDC_STARTGAME)->EnableWindow(TRUE);//게임시작 버튼 활성화
	}
	else if(data.Left(1)=='F')//정답신호를 받아왔을 때
	{
		if(!(data.Mid(3+idlen).Compare(dlg->m_SID)))//정답을 맞춘게 자신일 때
		{
			if(cnum!=1)//클라이언트랑 1:1이 아닐 때
				SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송해주기 위해 보내줌
			AfxMessageBox(_T("정답입니다!"));//자신이 맞췄다는걸 메세지박스로 띄움
		}
		else	
		{
			if(data.Mid(3+idlen).GetLength()>0)//정답을 맞춘 사람이 본인이 아닌 경우
			{
				strText.Format(_T("%s님이 정답을 맞추셨습니다."),data.Mid(3+idlen));//정답을 맞춘 유저의 닉네임을 팝업함
				AfxMessageBox(strText);
				if(cnum!=1)//클라이언트랑 1:1이 아닐 때
					SendData(buf[0],ID,data.Mid(2+idlen));	//다른 클라이언트들에게 전송
			}
		}
		dlg->gameturn=_ttoi(data.Mid(2+idlen,1));//순서를 int형변환
		if(dlg->gameturn==dlg->myturn)// 게임턴이 나의 차례로 왔을 경우	
			dlg->OnBnClickedQuestionmode();	//출제 모드
		else if(dlg->gameturn==7)//게임이 끝났을 때
			dlg->FinishGame();//끝낼때 초기화 함수 실행
		else //내 차례가 아닌 경우
			dlg->OnBnClickedSolutionmode();	//정답자 모드
	}
}


void CServerApp::CloseChild(void)
{
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();
	cnum--;
	if(cnum==0)	//Client가 전부 연결이 끊어졌을 경우
	{
		AfxMessageBox(_T("다른 유저들과의 연결이 끊겼습니다."));
		dlg->AllClear();
		dlg->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		CleanUp();
	}
	else	//1:1이 아닌경우에서 Client 1명이 나갔을 경우
		AfxMessageBox(_T("유저 1명이 나갔습니다."));
}


void CServerApp::CleanUp(void)
{
	delete m_pServer;
	POSITION pos = m_pClient.GetHeadPosition();	
	while(pos != NULL)
	{											//pos 값이 완전히 사라질때까지 반복 수행
		delete m_pClient.GetNext(pos);			// 클라이언트의 수만큼 삭제
	}
}

void CServerApp::SendLine(char& header, CString& id, CPoint& ptStart, CPoint& ptEnd)
{
	CString Line;
	Line.Format(_T("$%d$%d$%d$%d"), ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); //각 포인터의 값을 저장하여 클라이언트에게 전송
	SendData(header,id,Line);
}


void CServerApp::SendThickness(char& header, CString& id, int& thickness)
{
	CString strThick;
	strThick.Format(_T("%d"),thickness);//선 두께정보를 CString으로 형변환
	SendData(header,id,strThick); //선 두께에 대한 데이터를 클라이언트에게 전송
}


void CServerApp::SendColor(char& header, CString& id, int& Red, int& Green, int& Blue)
{
	CString strColor;
	strColor.Format(_T("$%d$%d$%d"),Red, Green, Blue); //받아온 선색상 데이터를 저장하여 클라이언트에게 전송
	SendData(header,id,strColor);
}
