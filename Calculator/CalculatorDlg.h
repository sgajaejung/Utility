
// CalculatorDlg.h : ��� ����
//

#pragma once


// CCalculatorDlg ��ȭ ����
class CD3DView;
class CCalculatorDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CCalculatorDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	virtual ~CCalculatorDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CALCULATOR_DIALOG };

	void MainLoop();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	bool m_loop;
	CD3DView *m_view;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
