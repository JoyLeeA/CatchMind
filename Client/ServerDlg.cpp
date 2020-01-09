// ServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"
#include "ScoreBoardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV �����Դϴ�.

	// �����Դϴ�.
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


// CServerDlg ��ȭ ����


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


// CServerDlg �޽��� ó����

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData();
	m_ctrSpin.SetRange(1, 20); //���β� ���� ����
	UDACCEL accels[] = {{0, 1}}; //�β� 1�� ����
	m_ctrSpin.SetAccel(1, accels);
	m_ctrSpin.SetPos(1); //���� ��Ʈ���� ���� ��ġ�� ����

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE); // ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE); // ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	char name[255];
	PHOSTENT MainServer;
	if (gethostname(name, sizeof(name)) == 0)
	{
		if ((MainServer = gethostbyname(name)) != nullptr)
		{
			ServIP = inet_ntoa(*(struct in_addr*)*MainServer->h_addr_list); //������ IP�ּҸ� �޴´�.
		}
	}

	Serv_Addr.SetWindowTextW(ServIP); //������ IP�ּҸ� IP��Ʈ�ѷ� ����Ѵ�.
	Serv_Addr.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE); //������������ ������ ��ư ��Ȱ��ȭ
	GetDlgItem(IDC_STARTGAME)->EnableWindow(FALSE); //������������ ���ӽ��� ��ư ��Ȱ��ȭ
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CServerDlg::OnPaint()  //�׸��� �Լ�
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON); //�������� ��� �����.
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnClickedButtonSend() //ä�ú����� ��ư�� ��������
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strData;
	h = 'M';
	GetDlgItemText(IDC_DATA, strData); //ä��â�� �ִ� ������ get�Ѵ�..
	((CServerApp*)AfxGetApp())->SendData(h, m_SID, strData); //get�� ������ �����ش�.
	SetDlgItemText(IDC_DATA,_T("")); //â�� �Էµ� ������� �����Ѵ�.
}


void CServerDlg::OnMouseMove(UINT nFlags, CPoint point) //���콺�� �����Ӱ� ���õ� �Լ�
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_Mode == 1) //���� ���(1)
	{
		if (!m_Question.IsEmpty()) //������ ���������� �׸��׸��� �۵�X
		{
			if (pen_flag) //���콺�� Ŭ���� �����϶�
			{
				h = 'P';
				((CServerApp*)AfxGetApp())->SendLine(h, m_SID, m_cpDraw, point);//Ŀ���� ��ġ�� ����
				DrawPicture(m_cpDraw, point); //��ġ�� ���� �׸��� �׸�
				m_cpDraw = point; //Ŀ���� ����ġ ����
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	pen_flag = TRUE; //���콺�� Ŭ�� ���¸� ��Ÿ��(False == �ȴ������ִ� ����)
	SetCapture(); //���콺�� ���¸� ���� ����
	m_cpDraw = point; //���� Ŀ���� ��ġ�� m_cpDraw�� �����Ѵ�.
	if (m_Mode == 1) 
	{
		if (!m_Question.IsEmpty()) 
		{
			h = 'P';
			((CServerApp*)AfxGetApp())->SendLine(h, m_SID, point, point);//���콺�� ���� ��ġ�� ����
			DrawPicture(m_cpDraw, point); 
			m_cpDraw = point; 
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CServerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_Mode == 1)
	{
		if (!m_Question.IsEmpty()) 
		{
			Clear = _T("");
			h = 'E';
			((CServerApp*)AfxGetApp())->SendData(h, m_SID, Clear);//ȭ���� Ŭ�����ϴ� ��ȣ �߻�
			ScreenClear(); //ȭ��Ŭ����
		}
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CServerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	pen_flag = FALSE;
	ReleaseCapture(); //���콺 ���� ���� �ߴ�

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CServerDlg::OnClickedLinecolor()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_color = m_ColorButton.GetColor(); //���� �� ���� ����
	int R, G, B;
	h = 'C'; //���� ���� R,G,B���� �޾ƿ´�.
	R = GetRValue(m_color);
	G = GetGValue(m_color);
	B = GetBValue(m_color);
	((CServerApp*)AfxGetApp())->SendColor(h, m_SID, R, G, B); //R,G,B���� Ŭ���̾�Ʈ���� ����
}


void CServerDlg::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) //spin���� ������ ����(spin control ���)
{
	h = 'T';
	LPNMUPDOWN pnmupdown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (pnmupdown->iDelta > 0) //����ư Ŭ����
	{
		if (m_Thickness >= 20)
			return;

		m_Thickness ++; //Thickness �Ѵܰ� ���
		((CServerApp*)AfxGetApp())->SendThickness(h, m_SID, m_Thickness);
		
	}
	else //�ٿ��ư Ŭ����
	{
		if (m_Thickness <= 1)
			return;

		m_Thickness --; //Thickness �Ѵܰ� �϶�
		((CServerApp*)AfxGetApp())->SendThickness(h, m_SID, m_Thickness);
	}

	*pResult = 0;
}


BOOL CServerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->wParam == VK_RETURN) //Enter��ư�������� ���α׷��� ������ ���� ����
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CServerDlg::OnBnClickedQuestionmode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_Mode = 1;
	ScreenClear(); //ȭ��Ŭ����
	SetDlgItemText(IDC_ANSWER,_T("")); //�����Է¶� �ʱ�ȭ(��ĭ����)
	m_Solution.Empty(); //���亯�� �ʱ�ȭ
	a = 0; //���� �ʱ�ȭ
	m_Thickness = 1; //�� ���� �ʱ�ȭ
	COLORREF black = (0, 0, 0);
	m_ColorButton.SetColor(black); //���� ���������� �ʱ�ȭ
	m_color = m_ColorButton.GetColor(); //������ �÷����� ���� ����
	KillTimer(1); //Ÿ�̸� ��Ȱ��ȭ
	StartTime = 0; //���� �ð� 0���� �ʱ�ȭ
	EndTime = 0;//���� �ð� 0���� �ʱ�ȭ
	m_GameTimebar.SetRange(0, 0); //���ѽð� �ʱ�ȭ
	AfxMessageBox(_T("<���� ���>�Դϴ�.\r\n[����]���� ������ �� �ܾ �����ּ���"));
}


void CServerDlg::OnBnClickedSolutionmode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	AfxMessageBox(_T("<Ǯ�� ���>�Դϴ�.\r\n������ �׸� �׸��� �����Ͽ�, ������� ��������"));
}


void CServerDlg::OnBnClickedButtonQuestion()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_Mode == 0) //��� ������
	{
		AfxMessageBox(_T("<���� ���>�� Ȯ�� ���ּ���."));
		SetDlgItemText(IDC_QUESTION,_T("")); 
		m_Question.Empty(); 
	}
	else if (m_Mode == 1) //�������
	{
		GetDlgItemText(IDC_QUESTION, m_Question); //����â�� �ִ� ��Ʈ���� ������ ����
		if (m_Question.IsEmpty()) //�������� ������� ��
			AfxMessageBox(_T("������ �������ּ���."));
		else
		{
			h = 'Q';
			AfxMessageBox(_T("������ �����Ǿ����ϴ�.\r\n�׸��� �׷��ּ���."));
			((CServerApp*)AfxGetApp())->SendData(h, m_SID, m_Question); //���������� ����
			m_GameTimebar.SetRange(0, 180); //������ �޾ƿȰ� ���ÿ� �ð���� ����
			m_GameTimebar.SetPos(180); //���ѽð� 180��=3��
			SetTimer(1, 1000, nullptr); //������ �����԰� ���ÿ� ���� Ÿ�̸� ����
		}
	}
	else //������߱���
	{
		AfxMessageBox(_T("���� ��尡 �ƴմϴ�."));
		SetDlgItemText(IDC_QUESTION,_T("")); //������ ����
		m_Question.Empty(); //���� �ʱ�ȭ
	}
}


void CServerDlg::OnBnClickedButtonAnswer()
{
	if (m_Mode == 0) //��弱�� ������ ����, ���� ���� ���� ���ϰ� ����
	{
		AfxMessageBox(_T("��带 �������ּ���."));
		SetDlgItemText(IDC_ANSWER,_T("")); //����� ����
		m_Solution.Empty(); //���亯�� �ʱ�ȭ
	}
	else if (m_Mode == 2) //Ǯ�̸��
	{
		// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
		h = 'A';
		CServerApp* App = (CServerApp *)AfxGetApp(); //AppŬ���� ����
		CString m_QuestionC = (App->QueC); //AppŬ������ ����QueC�� �����پ���.
		GetDlgItemText(IDC_ANSWER, m_Solution); //������� ������ ���´�.
		((CServerApp*)AfxGetApp())->SendData(h, m_SID, m_Solution); //���䵥���� ����
		if (!(m_Solution.Compare(m_QuestionC))) //������ ��������
			//Compare�Լ��� �Ἥ �� ���ڿ��� ������ �ƴ��� Ȯ���Ѵ�.(������0)
		{
			m_Solution.Empty(); //���亯�� ����

			if (m_Solution.IsEmpty()) //���亯�� �������
			{
				EndTime = CTime::GetCurrentTime(); //���������� �ð��� ������
				CTimeSpan SubTime = EndTime - StartTime; //���۰� �� �ð��� ���� ���Ѵ�.
				int SubMinuteS = SubTime.GetMinutes(); //�ð����̱���
				if (SubMinuteS < 1) //�ð����̿� ���� ���� �й�
					//1���̳��� ���߸� 10��
				{
					a = 10;
					m_Score += a;
				}
				else if ((1 <= SubMinuteS) & (SubMinuteS < 2)) //1~2�л��� 7��
				{
					a = 7;
					m_Score += a;
				}
				else if ((2 <= SubMinuteS) & (SubMinuteS < 3)) //2~3�л��� 3��
				{
					a = 3;
					m_Score += a;
				}
				KillTimer(1); //���� ���߸� Ÿ�̸� ����
				StartTime = 0; //����,����ð� �ʱ�ȭ
				EndTime = 0;
				m_GameTimebar.SetRange(0, 0); //Ÿ�̸� �� �ʱ�ȭ
				ScreenClear(); //ȭ�� �����
				SetDlgItemText(IDC_ANSWER,_T("")); //����� ����
				ScoreS.Format(_T("%d"), m_Score); //int->CString ����ȯ
				m_Text.SetWindowTextW(ScoreS); //����â�� ����ī����
			}
		}
		else //������ Ʋ������
		{
			AfxMessageBox(_T("������ �ƴմϴ�."));
			SetDlgItemText(IDC_ANSWER,_T("")); //����� ����
			m_Solution.Empty(); //���亯�� �ʱ�ȭ
		}
	}
	else //������� �� ��
	{
		AfxMessageBox(_T("Ǯ�� ��尡 �ƴմϴ�."));
		SetDlgItemText(IDC_ANSWER,_T("")); //����� ����
		m_Solution.Empty(); //���亯�� �ʱ�ȭ
	}
}


void CServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CServerApp* App = (CServerApp*)AfxGetApp(); 
	timer = CTime::GetCurrentTime(); //�ð� �Լ�
	if (nIDEvent == 1)
	{
		int pos = m_GameTimebar.GetPos(); // Ÿ�ӹ��� �������� �����´�.
		if (pos > 0) 
		{
			pos--; 
			m_GameTimebar.SetPos(pos);
		}
		else //���ѽð��� ������
		{
			KillTimer(1); //Ÿ�̸� ����
			++gameturn;
			if (gameturn > App->cnum) //������ ������ �������� �ʰ��� ��� 0���� �ʱ�ȭ
				gameturn = 0;
			if (m_Mode == 1) 
				AfxMessageBox(_T("���ѽð��� �������ϴ�"));
			else if (m_Mode == 2) 
				AfxMessageBox(_T("���ѽð��ȿ� ������ ������ ���߽��ϴ�."));
			if (gameturn == myturn)
				OnBnClickedQuestionmode(); //���� ���
			else if (gameturn == 7);
			else
				OnBnClickedSolutionmode(); //Ǯ�� ���
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CServerDlg::OnBnClickedButtonClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_list.ResetContent(); //��ȭ����� ����.
}


void CServerDlg::StopQuestion(void)
{
	CServerApp* App = (CServerApp *)AfxGetApp(); 
	GetDlgItemText(IDC_QUESTION, m_Question); //�������� ��Ʈ���� ������ ����
	CString m_SolutionC = App->AnsC; //
	if (!(m_SolutionC.Compare(m_Question))) //���ڿ� ��ȣ �˻�
	{
		CString notice, Data;
		gameturn++; // ���� ������ �Ѿ�� ����
		h = 'F';
		Data.Format(_T("%d%s"), gameturn, RecieveID);
		notice.Format(_T("%s���� ����"), RecieveID); //���� Ŭ���̾�Ʈ�� �г��Ӱ� ���� �˸�
		AfxMessageBox(notice);
		App->SendData(h, m_SID, Data); //���� ������ �г��Ӱ� ���Ӽ����� �ٸ� Ŭ���̾�Ʈ�鿡�� ����
		SetDlgItemText(IDC_QUESTION,_T("")); //������ �ʱ�ȭ
		m_Question.Empty(); //���� �ʱ�ȭ
		ScreenClear(); //ȭ�� Ŭ����
		m_Thickness = 1; //�� ���� �ʱ�ȭ
		m_color = RGB(0,0,0); //���� �ʱ�ȭ
		m_ctrSpin.SetPos(1); //���� ��Ʈ���� ���� ��ġ�� ����
		OnBnClickedSolutionmode(); 
	}
}


void CServerDlg::AllClear(void) //Ŭ���̾�Ʈ�Ϳ����̲������� ������ �ʱ�ȭ
{
	SetDlgItemText(IDC_ANSWER,_T("")); //����� ����
	m_Solution.Empty(); //���亯�� �ʱ�ȭ
	SetDlgItemText(IDC_QUESTION,_T("")); //������ ����
	m_Question.Empty(); //�������� �ʱ�ȭ
	m_Score = 0; //�����ʱ�ȭ
	m_Thickness = 1; //���β� �ʱ�ȭ
	m_ctrSpin.SetPos(1); //���� ��Ʈ���� ���� ��ġ�� ����
	m_color = RGB(0,0,0); //���� ���� �ʱ�ȭ
	SetDlgItemText(IDC_SCORE,_T("")); //����ǥ��â �����(�Ӽ����� Simple->True)
	m_list.ResetContent(); //ä�ù� �ʱ�ȭ
	ScreenClear(); //�׸� �ʱ�ȭ
}


void CServerDlg::DrawPicture(CPoint& ptStart, CPoint& ptEnd)
{
	if (m_Mode == 2) // 
	{
		CServerApp* App = (CServerApp*)AfxGetApp(); 
		m_Thickness = App->ThicknessC; //Ŭ���̾�Ʈ�� �� ���⸦ �޾ƿ�
		m_color = App->ColorC; //Ŭ���̾�Ʈ�� ������ �޾ƿ�.
	}

	CClientDC dc(this); //DC�� �����Ѵ�

	CPen pen; //���������Ѵ�.
	pen.CreatePen(PS_SOLID, m_Thickness, m_color); //���� �Ӽ� ����(����, ����0
	CPen* oldPen = dc.SelectObject(&pen); //
	dc.MoveTo(ptStart); //���׸��� ������
	dc.LineTo(ptEnd); //���׸��� ����
	dc.SelectObject(oldPen); //DC�� ���� ����
}


void CServerDlg::ScreenClear(void)
{
	Invalidate(); 
}

void CServerDlg::OnBnClickedStartgame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_STARTGAME)->EnableWindow(FALSE); //���ӽ��� ��ư ��Ȱ��ȭ
	CServerApp* App = (CServerApp *)AfxGetApp(); //AppŬ���� ����
	gameflag = 1; //���ӽ��۵� ���� �˸��� ����
	h = 'U';
	CString UserInfo;
	UserInfo.Format(_T("%d$%s$%s$%s$%s"), App->cnum, UserID[1].Mid(1), UserID[2].Mid(1), UserID[3].Mid(1),
	                UserID[4].Mid(1)); //������ ������ ������ ���� ������ �г����� ����
	App->SendData(h, m_SID, UserInfo); //������ ���۵Ǹ� Ŭ���̾�Ʈ�鿡�� ��ü ������ ���� �����鼭 ���ӽ����� �˸�
	CString startgame;
	startgame.Format(_T("���� ����"));
	((CListBox*)GetDlgItem(IDC_LIST1))->InsertString(-1, startgame); //������ ���۵Ǿ����� ä��â�� ������ ���
	m_list.SetTopIndex(m_list.GetCount() - 1);
	OnBnClickedQuestionmode(); //������ ù������ ������ ������ �����ڷ� ����(�����̹Ƿ�)
}


void CServerDlg::OnBnClickedOpenroom() //�氳���ϸ� Listen�Լ� ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItemTextW(IDC_SERVERID, m_SID); //Client ID���� �޾ƿ�
	TCHAR* tchr;
	CString str = _T("");
	if (m_SID.IsEmpty()) //�г����� ����ִٸ�?
		AfxMessageBox(_T("[���� �г���]�� �������ּ���"));
	else //�г����� ������ ��
	{
		if (m_SID.GetLength() >= 15) //�г����� 15�� �̻��϶�
		{
			AfxMessageBox(_T("[���� �г���]�� 15�� ������ �������ּ���"));
			SetDlgItemText(IDC_SERVERID,_T("")); //Editĭ ����
			m_SID.Empty(); //�г��� ���� �ʱ�ȭ
		}
		else
		{
			    ((CServerApp*)AfxGetApp())->InitServer(); //listen�Լ�
				GetDlgItem(IDC_OPENROOM)->EnableWindow(FALSE); //��ư ��Ȱ��ȭ
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
