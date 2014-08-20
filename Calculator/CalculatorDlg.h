
// CalculatorDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CCalculatorDlg 대화 상자
class CD3DView;
class CCalculatorDlg : public CDialogEx
{
// 생성입니다.
public:
	CCalculatorDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	virtual ~CCalculatorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CALCULATOR_DIALOG };

	void MainLoop();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	bool m_loop;
	CD3DView *m_view;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_EditSource;
	afx_msg void OnBnClickedButtonApply();
	CButton m_applyButton;
};
