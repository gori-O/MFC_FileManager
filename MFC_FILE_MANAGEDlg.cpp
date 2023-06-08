
// MFC_FILE_MANAGEDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_FILE_MANAGE.h"
#include "MFC_FILE_MANAGEDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCFILEMANAGEDlg 대화 상자



CMFCFILEMANAGEDlg::CMFCFILEMANAGEDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_FILE_MANAGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCFILEMANAGEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEFT_LIST, m_left_list);
	DDX_Control(pDX, IDC_RIGHT_LIST, m_right_list);
}

BEGIN_MESSAGE_MAP(CMFCFILEMANAGEDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LEFT_LIST, &CMFCFILEMANAGEDlg::OnLbnDblclkLeftList)
	ON_LBN_DBLCLK(IDC_RIGHT_LIST, &CMFCFILEMANAGEDlg::OnLbnDblclkRightList)
	ON_BN_CLICKED(IDC_R_CREATE_DIR_BTN, &CMFCFILEMANAGEDlg::OnBnClickedRCreateDirBtn)
	ON_BN_CLICKED(IDC_LtoR_COPY_BTN, &CMFCFILEMANAGEDlg::OnBnClickedLtorCopyBtn)
	ON_BN_CLICKED(IDC_R_OPEN_DIR_BTN, &CMFCFILEMANAGEDlg::OnBnClickedROpenDirBtn)
	ON_BN_CLICKED(IDC_R_DEL_BTN, &CMFCFILEMANAGEDlg::OnBnClickedRDelBtn)
END_MESSAGE_MAP()


// CMFCFILEMANAGEDlg 메시지 처리기

BOOL CMFCFILEMANAGEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//m_left_list.Dir(DDL_ARCHIVE | DDL_HIDDEN | DDL_DIRECTORY, L"*.*");

	wchar_t tmp_path[MAX_PATH];
	int len = GetCurrentDirectory(MAX_PATH, tmp_path);
	tmp_path[len++] = '\\';
	tmp_path[len] = 0;
	SetDlgItemText(IDC_L_PATH_EDIT, tmp_path);
	SetDlgItemText(IDC_R_PATH_EDIT, tmp_path);

	DirToList(&m_left_list, tmp_path);
	DirToList(&m_right_list, tmp_path);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCFILEMANAGEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCFILEMANAGEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCFILEMANAGEDlg::DirToList(CListBox* ap_list_box, CString a_path)
{
	ap_list_box->ResetContent();

	CString name;
	WIN32_FIND_DATA file_data;
	HANDLE h_item_list = FindFirstFile(a_path + L"*.*", &file_data);
	if (h_item_list != INVALID_HANDLE_VALUE) {
		do {
			if (file_data.cFileName[0] != '.' || file_data.cFileName[1]) {
				name = file_data.cFileName;
				if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) name = L"[" + name + L"]";
				ap_list_box->InsertString(-1, name);
			}
		} while (FindNextFile(h_item_list, &file_data));
	}
}


void CMFCFILEMANAGEDlg::OnLbnDblclkLeftList()
{
	ChangeDir(&m_left_list, IDC_L_PATH_EDIT);
}


void CMFCFILEMANAGEDlg::OnLbnDblclkRightList()
{
	ChangeDir(&m_right_list, IDC_R_PATH_EDIT);
}


void CMFCFILEMANAGEDlg::ChangeDir(CListBox* ap_list_box, int a_path_ctrl_id)
{
	CString str, path;
	int idx = ap_list_box->GetCurSel();
	ap_list_box->GetText(idx, str);
	if (str[0] == '[') { // its directory
		GetDlgItemText(a_path_ctrl_id, path);
		str.TrimLeft('[');
		str.TrimRight(']');

		if (str == L"..") {
			path.TrimRight('\\');
			int pos = path.ReverseFind('\\');
			path.Delete(pos + 1, path.GetLength() - pos);

		}
		else {
			path += str;
			path += L"\\";
		}
		SetDlgItemText(a_path_ctrl_id, path);
		DirToList(ap_list_box, path);
	}
}

#include "InputNameDlg.h"

void CMFCFILEMANAGEDlg::OnBnClickedRCreateDirBtn()
{
	InputNameDlg ins_dlg;
	if (ins_dlg.DoModal() == IDOK) {
		CString path;
		GetDlgItemText(IDC_R_PATH_EDIT, path);
		CreateDirectory(path + ins_dlg.GetName(), NULL);
		DirToList(&m_right_list, path);
	}
	
}


void CMFCFILEMANAGEDlg::OnBnClickedLtorCopyBtn()
{
	int idx = m_left_list.GetCurSel();
	if (idx != LB_ERR) {
		CString name, src_path, dst_path;
		m_left_list.GetText(idx, name);
		if (name[0] == '[') {
			MessageBox(L"디렉토리는 복사할 수 없습니다!", L"복사 실패", MB_ICONSTOP | MB_OK);
		}
		else {
			GetDlgItemText(IDC_L_PATH_EDIT, src_path);
			GetDlgItemText(IDC_R_PATH_EDIT, dst_path);
			CopyFile(src_path + name, dst_path + name, FALSE);
			DirToList(&m_right_list, dst_path);
		}
	}
}


void CMFCFILEMANAGEDlg::OnBnClickedROpenDirBtn()
{
	CString path;
	GetDlgItemText(IDC_R_PATH_EDIT, path);
	ShellExecute(NULL, L"open", L"explorer.exe", path, path, SW_SHOW);
}


void CMFCFILEMANAGEDlg::OnBnClickedRDelBtn()
{
	int idx = m_right_list.GetCurSel();
	if (idx != LB_ERR) {
		CString name;
		m_right_list.GetText(idx, name);
		if (name[0] == '[') {
			MessageBox(L"디렉토리는 삭제할 수 없습니다.", L"삭제 실패", MB_ICONSTOP | MB_OK);
		}
		else {
			if (MessageBox(L"정말 삭제 하시겠습니까?",name, MB_ICONQUESTION | MB_OKCANCEL) == IDOK) {
				CString path;
				GetDlgItemText(IDC_R_PATH_EDIT, path);
				DeleteFile(path + name);
				DirToList(&m_right_list, path);
			}
		}
	}
}
