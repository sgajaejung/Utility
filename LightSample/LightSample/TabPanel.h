#pragma once

#include "resource.h"
#include "afxcmn.h"


class CTabPanel : public CDialogEx
{
public:
	CTabPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAINTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
};
