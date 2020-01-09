#include "stdafx.h"
#include "Server.h"
#include "ScoreBoardDlg.h"
#include "afxdialogex.h"
#include "ServerDlg.h"

#define MAX 10

// CScoreBoardDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CScoreBoardDlg, CDialog)

CScoreBoardDlg::CScoreBoardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScoreBoardDlg::IDD, pParent)
{

}

CScoreBoardDlg::~CScoreBoardDlg()
{
}

void CScoreBoardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScoreBoardDlg, CDialog)
END_MESSAGE_MAP()


// CScoreBoardDlg �޽��� ó�����Դϴ�.


BOOL CScoreBoardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CServerDlg *dlg = (CServerDlg*)AfxGetMainWnd();//DialogŬ���� ���� ����

	int i, j;
	int ranking[MAX];
	CString rank_str[MAX];

	for(i=0; i<MAX; i++)
	{
		ranking[i]=1;//��� 1������ �ʱ�ȭ
	}

	//������ ���� ������ ������ ��� 1�� ����
	for(i=0; i<MAX; i++)
	{
		for(j=0; j<MAX; j++)
		{
			if(dlg->Score[i] < dlg->Score[j])
			{
				++ranking[i];				
			}
		}
	}

	for(int k=0; k<MAX; k++)
	{
		if(dlg->Score[k]!=1)
		//������ ������ ������ 1�̾ƴϹǷ� ����� ����
		{
			dlg->Score_STR[k].Format(_T("%d��"),dlg->Score[k]);
			rank_str[k].Format(_T("%d�� :"),ranking[k]);
		}
		else//������ ������ ������ 1�̹Ƿ� ���ٰ� ǥ��
		{
			dlg->Score_STR[k]=_T("");
			rank_str[k].Format(_T(" "));
		}
	}
	//�����͸� �� ����ش�.
	SetDlgItemText(IDC_RANK1,rank_str[0]);
	SetDlgItemText(IDC_USER1,dlg->UserID[0]);
	SetDlgItemText(IDC_SCORE1,dlg->Score_STR[0]);
	SetDlgItemText(IDC_RANK2,rank_str[1]);
	SetDlgItemText(IDC_USER2,dlg->UserID[1].Mid(1));
	SetDlgItemText(IDC_SCORE2,dlg->Score_STR[1]);
	SetDlgItemText(IDC_RANK3,rank_str[2]);
	SetDlgItemText(IDC_USER3,dlg->UserID[2].Mid(1));
	SetDlgItemText(IDC_SCORE3,dlg->Score_STR[2]);
	SetDlgItemText(IDC_RANK4,rank_str[3]);
	SetDlgItemText(IDC_USER4,dlg->UserID[3].Mid(1));
	SetDlgItemText(IDC_SCORE4,dlg->Score_STR[3]);
	SetDlgItemText(IDC_RANK5,rank_str[4]);
	SetDlgItemText(IDC_USER5,dlg->UserID[4].Mid(1));
	SetDlgItemText(IDC_SCORE5,dlg->Score_STR[4]);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
