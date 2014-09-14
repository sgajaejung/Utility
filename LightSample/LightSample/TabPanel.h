#pragma once

#include "resource.h"
#include "afxcmn.h"


class CTabPanel : public CDialogEx
{
public:
	CTabPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAINTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
};
