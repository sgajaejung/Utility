#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxpropertygridctrl.h"

// CLightPanel ��ȭ �����Դϴ�.

class CLightPanel : public CDialogEx
{
public:
	CLightPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLightPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LIGHT };

protected:
	CMFCPropertyGridCtrl m_modelProperty;
	CMFCPropertyGridCtrl m_lightProperty;

	void UpdateModelProperty(const graphic::cMaterial &mtrl);
	void UpdateLightProperty(const graphic::cLight &light);

	void AddPropertyColor4(CMFCPropertyGridProperty *group, CString name, Vector4 value);
	void AddPropertyVector3(CMFCPropertyGridProperty *group, CString name, Vector3 value);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_lightCombo;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLight();
	LRESULT OnPropertyChanged (WPARAM wparam, LPARAM lparam); 
};