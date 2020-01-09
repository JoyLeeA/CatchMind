// ServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "ScoreBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
DECLARE_MESSAGE_MAP()
public:
	void StopQuestion(void);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg 대화 상자


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD, pParent)
	  , m_pChild(nullptr)
	  , ServIP(_T(""))
	  , m_cpDraw(0)
	  , pen_flag(false)
	  , m_Thickness(0)
	  , m_LineThickness(0)
	  , m_Mode(0)
	  , m_Question(_T(""))
	  , m_Solution(_T(""))
	  , m_Score(0)
	  , timer(0)
	  , StartTime(0)
	  , EndTime(0)
	  , a(0)
	  , Start_X(0)
	  , Start_Y(0)
	  , End_X(0)
	  , End_Y(0)
	  , Draw_X(0)
	  , Draw_Y(0)
	  , Clear(_T(""))
	  , n(0)
	  , m_SID(_T(""))
	  , ScoreS(_T("0"))
	  , gameflag(0)
	  , myturn(0)
	  , gameturn(0)
	  , circle(0)
{
	m_Mode = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_color = RGB(0,0,0);
	m_Thickness = 1;
	a = 0;
	m_Score = 0;
	n = 0;
	UserID[1] = _T("");
	UserID[2] = _T("");
	UserID[3] = _T("");
	UserID[4] = _T("");
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, Serv_Addr);
	DDX_Control(pDX, IDC_SPIN, m_ctrSpin);
	DDX_Control(pDX, IDC_LINECOLOR, m_ColorButton);
	DDX_Text(pDX, IDC_LINETHICKNESS, m_LineThickness);
	DDV_MinMaxInt(pDX, m_LineThickness, 0, 10);
	DDX_Text(pDX, IDC_QUESTION, m_Question);
	DDX_Text(pDX, IDC_ANSWER, m_Solution);
	DDX_Control(pDX, IDC_SCORE, m_Text);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_SERVERID, m_SID);
	DDX_Control(pDX, IDC_TIME_PROGRESS, m_GameTimebar);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		ON_BN_CLICKED(IDC_BUTTON_SEND, &CServerDlg::OnClickedButtonSend)
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_RBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_BN_CLICKED(IDC_LINECOLOR, &CServerDlg::OnClickedLinecolor)
		ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, &CServerDlg::OnDeltaposSpin)
		ON_BN_CLICKED(IDC_QUESTIONMODE, &CServerDlg::OnBnClickedQuestionmode)
		ON_BN_CLICKED(IDC_ANSWERMODE, &CServerDlg::OnBnClickedSolutionmode)
		ON_BN_CLICKED(IDC_BUTTON_QUESTION, &CServerDlg::OnBnClickedButtonQuestion)
		ON_BN_CLICKED(IDC_BUTTON_ANSWER, &CServerDlg::OnBnClickedButtonAnswer)
		ON_WM_TIMER()
		ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CServerDlg::OnBnClickedButtonClear)
		ON_BN_CLICKED(IDC_STARTGAME, &CServerDlg::OnBnClickedStartgame)
		ON_BN_CLICKED(IDC_OPENROOM, &CServerDlg::OnBnClickedOpenroom)
END_MESSAGE_MAP()


// CServerDlg 메시지 처리기

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData();
	m_ctrSpin.SetRange(1, 20); //선두께 범위 설정
	UDACCEL accels[] = {{0, 1}}; //두께 1씩 증가
	m_ctrSpin.SetAccel(1, accels);
	m_ctrSpin.SetPos(1); //스핀 컨트롤의 현재 위치값 지정

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE); // 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE); // 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	char name[255];
	PHOSTENT MainServer;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((MainServer = gethostbyname(name)) != nullptr)
		{
			ServIP = inet_ntoa(*(struct in_addr*)*MainServer->h_addr_list); //서버의 IP주소를 받는다.
		}
	}

	Serv_Addr.SetWindowTextW(ServIP); //서버의 IP주소를 IP컨트롤로 출력한다.
	Serv_Addr.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE); //연결전까지는 보내기 버튼 비활성화
	GetDlgItem(IDC_STARTGAME)->EnableWindow(FALSE); //연결전까지는 게임시작 버튼 비활성화
	m_Text.SetWindowTextW(ScoreS);

	return TRUE; 
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CServerDlg::OnPaint()  //그리기 함수
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON); //아이콘을 가운데 맞춘다.
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그림.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnClickedButtonSend() //채팅보내기 버튼을 눌렀을때
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strData;
	h = 'M';
	GetDlgItemText(IDC_DATA, strData); //채팅창에 있는 내용을 get한다..
	((CServerApp*)AfxGetApp())->SendData(h, m_SID, strData); //get한 내용을 보내준다.
	SetDlgItemText(IDC_DATA,_T("")); //창에 입력된 내용들을 삭제한다.
}


void CServerDlg::OnMouseMove(UINT nFlags, CPoint point) //마우스가 움직임과 관련된 함수
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_Mode == 1) //출제 모드(1)
	{
		if (!m_Question.IsEmpty()) //문제가 비어있을경우 그림그리기 작동X
		{
			if (pen_flag) //마우스가 클릭된 상태일때
			{
				h = 'P';
				((CServerApp*)AfxGetApp())->SendLine(h, m_SID, m_cpDraw, point);//커서의 위치를 전송
				DrawPicture(m_cpDraw, point); //위치에 따라 그림을 그림
				m_cpDraw = point; //커서의 현위치 저장
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	pen_flag = TRUE; //마우스의 클릭 상태를 나타냄(False == 안눌려져있는 상태)
	SetCapture(); //마우스의 상태를 전부 저장
	m_cpDraw = point; //현재 커서의 위치를 m_cpDraw에 저장한다.
	if (m_Mode == 1) 
	{
		if (!m_Question.IsEmpty()) 
		{
			h = 'P';
			((CServerApp*)AfxGetApp())->SendLine(h, m_SID, point, point);//마우스의 현재 위치를 전송
			DrawPicture(m_cpDraw, point); 
			m_cpDraw = point; 
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CServerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_Mode == 1)
	{
		if (!m_Question.IsEmpty()) 
		{
			Clear = _T("");
			h = 'E';
			((CServerApp*)AfxGetApp())->SendData(h, m_SID, Clear);//화면을 클리어하는 신호 발생
			ScreenClear(); //화면클리어
		}
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CServerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	pen_flag = FALSE;
	ReleaseCapture(); //마우스 상태 저장 중단

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CServerDlg::OnClickedLinecolor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_color = m_ColorButton.GetColor(); //선에 색 값을 적용
	int R, G, B;
	h = 'C'; //선의 색값 R,G,B값을 받아온다.
	R = GetRValue(m_color);
	G = GetGValue(m_color);
	B = GetBValue(m_color);
	((CServerApp*)AfxGetApp())->SendColor(h, m_SID, R, G, B); //R,G,B값을 클라이언트에게 전송
}


void CServerDlg::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) //spin으로 선굵기 선택(spin control 사용)
{
	h = 'T';
	LPNMUPDOWN pnmupdown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pnmupdown->iDelta > 0) //업버튼 클릭시
	{
		if (m_Thickness >= 20)
			return;

		m_Thickness ++; //Thickness 한단계 상승
		((CServerApp*)AfxGetApp())->SendThickness(h, m_SID, m_Thickness);
		
	}
	else //다운버튼 클릭시
	{
		if (m_Thickness <= 1)
			return;

		m_Thickness --; //Thickness 한단계 하락
		((CServerApp*)AfxGetApp())->SendThickness(h, m_SID, m_Thickness);
	}

	*pResult = 0;
}


BOOL CServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->wParam == VK_RETURN) //Enter버튼눌렀을때 프로그램이 뻑나는 현상 방지
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CServerDlg::OnBnClickedQuestionmode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Mode = 1;
	ScreenClear(); //화면클리어
	SetDlgItemText(IDC_ANSWER,_T("")); //정답입력란 초기화(빈칸으로)
	m_Solution.Empty(); //정답변수 초기화
	a = 0; //점수 초기화
	m_Thickness = 1; //선 굵기 초기화
	COLORREF black = (0, 0, 0);
	m_ColorButton.SetColor(black); //색을 검정색으로 초기화
	m_color = m_ColorButton.GetColor(); //설정한 컬러값을 선에 적용
	KillTimer(1); //타이머 비활성화
	StartTime = 0; //시작 시간 0으로 초기화
	EndTime = 0;//종료 시간 0으로 초기화
	m_GameTimebar.SetRange(0, 0); //제한시간 초기화
	AfxMessageBox(_T("<출제 모드>입니다.\r\n[문제]란에 정답이 될 단어를 적어주세요"));
}


void CServerDlg::OnBnClickedSolutionmode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CServerApp* App = (CServerApp *)AfxGetApp(); 
	m_Mode = 2;
	ScreenClear(); 
	SetDlgItemText(IDC_QUESTION,_T("")); 	
	m_Question.Empty(); 
	a = 0; 
	App->ThicknessC = 1; 
	App->ColorC = (0, 0, 0); 
	m_ColorButton.SetColor(App->ColorC); 
	KillTimer(1); 
	StartTime = 0; 
	EndTime = 0;
	m_GameTimebar.SetRange(0, 0); 
	AfxMessageBox(_T("<풀이 모드>입니다.\r\n상대방이 그린 그림을 유추하여, 정답란에 적으세요"));
}


void CServerDlg::OnBnClickedButtonQuestion()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Mode == 0) //모드 선택전
	{
		AfxMessageBox(_T("<게임 모드>를 확인 해주세요."));
		SetDlgItemText(IDC_QUESTION,_T("")); 
		m_Question.Empty(); 
	}
	else if (m_Mode == 1) //출제모드
	{
		GetDlgItemText(IDC_QUESTION, m_Question); //문제창에 있는 스트링을 변수로 저장
		if (m_Question.IsEmpty()) //문제란이 비어있을 때
			AfxMessageBox(_T("문제를 출제해주세요."));
		else
		{
			h = 'Q';
			AfxMessageBox(_T("문제가 출제되었습니다.\r\n그림을 그려주세요."));
			((CServerApp*)AfxGetApp())->SendData(h, m_SID, m_Question); //문제데이터 전송
			m_GameTimebar.SetRange(0, 180); //문제를 받아옴과 동시에 시간재기 시작
			m_GameTimebar.SetPos(180); //제한시간 180초=3분
			SetTimer(1, 1000, nullptr); //문제를 출제함과 동시에 순간 타이머 시작
		}
	}
	else //정답맞추기모드
	{
		AfxMessageBox(_T("출제 모드가 아닙니다."));
		SetDlgItemText(IDC_QUESTION,_T("")); //문제란 비우기
		m_Question.Empty(); //변수 초기화
	}
}


void CServerDlg::OnBnClickedButtonAnswer()
{
	if (m_Mode == 0) //모드선택 전에는 문제, 정답 란에 쓰지 못하게 설정
	{
		AfxMessageBox(_T("모드를 설정해주세요."));
		SetDlgItemText(IDC_ANSWER,_T("")); //정답란 비우기
		m_Solution.Empty(); //정답변수 초기화
	}
	else if (m_Mode == 2) //풀이모드
	{
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		h = 'A';
		CServerApp* App = (CServerApp *)AfxGetApp(); //App클래스 선언
		CString m_QuestionC = (App->QueC); //App클래스의 변수QueC를 가져다쓴다.
		GetDlgItemText(IDC_ANSWER, m_Solution); //정답란의 변수를 얻어온다.
		((CServerApp*)AfxGetApp())->SendData(h, m_SID, m_Solution); //정답데이터 전송
		if (!(m_Solution.Compare(m_QuestionC))) //정답을 맞췄을때
			//Compare함수를 써서 두 문자열이 같은지 아닌지 확인한다.(같으면0)
		{
			m_Solution.Empty(); //정답변수 비우기

			if (m_Solution.IsEmpty()) //정답변수 비워지면
			{
				EndTime = CTime::GetCurrentTime(); //정지순간의 시간을 가져옴
				CTimeSpan SubTime = EndTime - StartTime; //시작과 끝 시간의 차를 구한다.
				int SubMinuteS = SubTime.GetMinutes(); //시간차이구함
				if (SubMinuteS < 1) //시간차이에 따른 점수 분배
					//1분이내에 맞추면 10점
				{
					a = 10;
					m_Score += a;
				}
				else if ((1 <= SubMinuteS) & (SubMinuteS < 2)) //1~2분사이 7점
				{
					a = 7;
					m_Score += a;
				}
				else if ((2 <= SubMinuteS) & (SubMinuteS < 3)) //2~3분사이 3점
				{
					a = 3;
					m_Score += a;
				}
				KillTimer(1); //정답 맞추면 타이머 종료
				StartTime = 0; //시작,종료시간 초기화
				EndTime = 0;
				m_GameTimebar.SetRange(0, 0); //타이머 바 초기화
				ScreenClear(); //화면 지우기
				SetDlgItemText(IDC_ANSWER,_T("")); //정답란 비우기
				ScoreS.Format(_T("%d"), m_Score); //int->CString 형변환
				m_Text.SetWindowTextW(ScoreS); //점수창에 점수카운팅
			}
		}
		else //정답을 틀렸을때
		{
			AfxMessageBox(_T("정답이 아닙니다."));
			SetDlgItemText(IDC_ANSWER,_T("")); //정답란 비우기
			m_Solution.Empty(); //정답변수 초기화
		}
	}
	else //출제모드 일 때
	{
		AfxMessageBox(_T("풀이 모드가 아닙니다."));
		SetDlgItemText(IDC_ANSWER,_T("")); //정답란 비우기
		m_Solution.Empty(); //정답변수 초기화
	}
}


void CServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CServerApp* App = (CServerApp*)AfxGetApp(); 
	timer = CTime::GetCurrentTime(); //시간 함수
	if (nIDEvent == 1)
	{
		int pos = m_GameTimebar.GetPos(); // 타임바의 게이지를 가져온다.
		if (pos > 0) 
		{
			pos--; 
			m_GameTimebar.SetPos(pos);
		}
		else //제한시간이 끝나면
		{
			KillTimer(1); //타이머 종료
			++gameturn;
			if (gameturn > App->cnum) //게임의 순서가 유저수를 초과할 경우 0으로 초기화
				gameturn = 0;
			if (m_Mode == 1) 
				AfxMessageBox(_T("제한시간이 끝났습니다"));
			else if (m_Mode == 2) 
				AfxMessageBox(_T("제한시간안에 정답을 맞추지 못했습니다."));
			if (gameturn == myturn)
				OnBnClickedQuestionmode(); //출제 모드
			else if (gameturn == 7);
			else
				OnBnClickedSolutionmode(); //풀이 모드
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CServerDlg::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_list.ResetContent(); //대화목록을 비운다.
}


void CServerDlg::StopQuestion(void)
{
	CServerApp* App = (CServerApp *)AfxGetApp(); 
	GetDlgItemText(IDC_QUESTION, m_Question); //문제란의 스트링을 변수로 저장
	CString m_SolutionC = App->AnsC; //
	if (!(m_SolutionC.Compare(m_Question))) //문자열 상호 검사
	{
		CString notice, Data;
		gameturn++; // 다음 턴으로 넘어가기 위함
		h = 'F';
		Data.Format(_T("%d%s"), gameturn, RecieveID);
		notice.Format(_T("%s님의 득점"), RecieveID); //맞춘 클라이언트의 닉네임과 득점 알림
		AfxMessageBox(notice);
		App->SendData(h, m_SID, Data); //맞춘 유저의 닉네임과 게임순서를 다른 클라이언트들에게 전송
		SetDlgItemText(IDC_QUESTION,_T("")); //문제란 초기화
		m_Question.Empty(); //변수 초기화
		ScreenClear(); //화면 클리어
		m_Thickness = 1; //선 굴기 초기화
		m_color = RGB(0,0,0); //색상 초기화
		m_ctrSpin.SetPos(1); //스핀 컨트롤의 현재 위치값 지정
		OnBnClickedSolutionmode(); 
	}
}


void CServerDlg::AllClear(void) //클라이언트와연결이끊어지면 모든것을 초기화
{
	SetDlgItemText(IDC_ANSWER,_T("")); //정답란 비우기
	m_Solution.Empty(); //정답변수 초기화
	SetDlgItemText(IDC_QUESTION,_T("")); //문제란 비우기
	m_Question.Empty(); //문제변수 초기화
	m_Score = 0; //점수초기화
	m_Thickness = 1; //선두께 초기화
	m_ctrSpin.SetPos(1); //스핀 컨트롤의 현재 위치값 지정
	m_color = RGB(0,0,0); //선의 색상 초기화
	SetDlgItemText(IDC_SCORE,_T("")); //점수표시창 지우기(속성에서 Simple->True)
	m_list.ResetContent(); //채팅방 초기화
	ScreenClear(); //그림 초기화
}


void CServerDlg::DrawPicture(CPoint& ptStart, CPoint& ptEnd)
{
	if (m_Mode == 2) // 
	{
		CServerApp* App = (CServerApp*)AfxGetApp(); 
		m_Thickness = App->ThicknessC; //클라이언트의 선 굵기를 받아옴
		m_color = App->ColorC; //클라이언트의 색상을 받아옴.
	}

	CClientDC dc(this); //DC를 생성한다

	CPen pen; //펜을생성한다.
	pen.CreatePen(PS_SOLID, m_Thickness, m_color); //펜의 속성 설정(굵기, 색상0
	CPen* oldPen = dc.SelectObject(&pen); //
	dc.MoveTo(ptStart); //선그리기 시작점
	dc.LineTo(ptEnd); //선그리기 끝점
	dc.SelectObject(oldPen); //DC의 펜을 복구
}


void CServerDlg::ScreenClear(void)
{
	Invalidate(); 
}

void CServerDlg::OnBnClickedStartgame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_STARTGAME)->EnableWindow(FALSE); //게임시작 버튼 비활성화
	CServerApp* App = (CServerApp *)AfxGetApp(); //App클래스 선언
	gameflag = 1; //게임시작된 것을 알리는 변수
	h = 'U';
	CString UserInfo;
	UserInfo.Format(_T("%d$%s$%s$%s$%s"), App->cnum, UserID[1].Mid(1), UserID[2].Mid(1), UserID[3].Mid(1),
	                UserID[4].Mid(1)); //방장을 제외한 유저의 수와 각각의 닉네임을 보냄
	App->SendData(h, m_SID, UserInfo); //게임이 시작되면 클라이언트들에게 전체 유저의 정보 보내면서 게임시작을 알림
	CString startgame;
	startgame.Format(_T("게임 시작"));
	((CListBox*)GetDlgItem(IDC_LIST1))->InsertString(-1, startgame); //게임이 시작되었음을 채팅창에 공지로 띄움
	m_list.SetTopIndex(m_list.GetCount() - 1);
	OnBnClickedQuestionmode(); //게임의 첫시작은 무조건 서버가 문제자로 시작(방장이므로)
}


void CServerDlg::OnBnClickedOpenroom() //방개설하면 Listen함수 동작
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItemTextW(IDC_SERVERID, m_SID); //Client ID값을 받아옴
	TCHAR* tchr;
	CString str = _T("");
	if (m_SID.IsEmpty()) //닉네임이 비어있다면?
		AfxMessageBox(_T("[나의 닉네임]을 설정해주세요"));
	else //닉네임을 적었을 때
	{
		if (m_SID.GetLength() >= 15) //닉네임이 15자 이상일때
		{
			AfxMessageBox(_T("[나의 닉네임]을 15자 밑으로 설정해주세요"));
			SetDlgItemText(IDC_SERVERID,_T("")); //Edit칸 비우기
			m_SID.Empty(); //닉네임 변수 초기화
		}
		else
		{
			    ((CServerApp*)AfxGetApp())->InitServer(); //listen함수
				GetDlgItem(IDC_OPENROOM)->EnableWindow(FALSE); //버튼 비활성화
				UserID[0] = m_SID;
			
		}
	}
}


void CServerDlg::FinishGame()
{
	ScreenClear(); 
	KillTimer(1); 
	m_GameTimebar.SetRange(0, 0);
	SetDlgItemText(IDC_QUESTION,_T("")); 	
	m_Question.Empty(); 
	Score[0] = m_Score;
}
