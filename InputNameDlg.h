#pragma once
#include "afxdialogex.h"


// InputNameDlg 대화 상자

class InputNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InputNameDlg)
private:
	CString m_name;
public:
	InputNameDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~InputNameDlg();

	const wchar_t* GetName() { return m_name; }
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_NAME_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
