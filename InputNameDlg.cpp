// InputNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFC_FILE_MANAGE.h"
#include "afxdialogex.h"
#include "InputNameDlg.h"


// InputNameDlg 대화 상자

IMPLEMENT_DYNAMIC(InputNameDlg, CDialogEx)

InputNameDlg::InputNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INPUT_NAME_DLG, pParent)
{

}

InputNameDlg::~InputNameDlg()
{
}

void InputNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InputNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &InputNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// InputNameDlg 메시지 처리기


void InputNameDlg::OnBnClickedOk()
{
	if(GetDlgItemText(IDC_NAME_EDIT, m_name)>0)
		CDialogEx::OnOK();
}
