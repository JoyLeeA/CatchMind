
// ServerDlg.h : 헤더 파일
//

#pragma once
#include "atltypes.h"
#include "afxcolorbutton.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "atltime.h"
#include "ScoreBoardDlg.h"


// CServerDlg 대화 상자
class CServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	clientsock* m_pChild;
	CIPAddressCtrl Serv_Addr;
	afx_msg void OnClickedButtonSend();
	CString ServIP;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	CPoint m_cpDraw;
	BOOL pen_flag;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	COLORREF m_color;
	int m_Thickness;
	CSpinButtonCtrl m_ctrSpin;
	CMFCColorButton m_ColorButton;
	afx_msg void OnClickedLinecolor();
	int m_LineThickness;
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_Mode;
	CString m_Question;
	CString m_Solution;
	afx_msg void OnBnClickedQuestionmode();
	afx_msg void OnBnClickedSolutionmode();
	afx_msg void OnBnClickedButtonQuestion();
	afx_msg void OnBnClickedButtonAnswer();
	int m_Score;
	CTime timer;
	CTime StartTime;
	CTime EndTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_Text;
	afx_msg void OnBnClickedButtonClear();
	int a;
	void StopQuestion(void);
	CListBox m_list;
	void AllClear(void);
	int Start_X;
	int Start_Y;
	int End_X;
	int End_Y;
	int Draw_X;
	int Draw_Y;
	void DrawPicture(CPoint& ptStart, CPoint& ptEnd);
	CString Clear;
	void ScreenClear(void);
	char h;
	int n;
	afx_msg void OnBnClickedStartgame();
	afx_msg void OnBnClickedOpenroom();
	CString m_SID;
	CString ScoreS;
	int gameflag;
	CProgressCtrl m_GameTimebar;
	int myturn, gameturn, circle;
	CString RecieveID;
	CString UserID[5];
	CString Score_STR[5];
	int Score[5];
	void FinishGame();
};
