// LightPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LightPanel.h"
#include "afxdialogex.h"
#include "Controller.h"

using namespace graphic;

enum {
	ID_PROPERTY_CUBE = 100,
	ID_PROPERTY_LIGHT,
};


// CLightPanel 대화 상자입니다.
CLightPanel::CLightPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLightPanel::IDD, pParent)
{

}

CLightPanel::~CLightPanel()
{
}

void CLightPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LIGHT, m_lightCombo);
}


BEGIN_MESSAGE_MAP(CLightPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLightPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLightPanel::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_LIGHT, &CLightPanel::OnSelchangeComboLight)
	ON_REGISTERED_MESSAGE (AFX_WM_PROPERTY_CHANGED, OnPropertyChanged) 
END_MESSAGE_MAP()


// CLightPanel 메시지 처리기입니다.


void CLightPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CLightPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


BOOL CLightPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_lightCombo.InsertString(0, L"Light 1");
	m_lightCombo.InsertString(1, L"Light 2");
	m_lightCombo.InsertString(2, L"Light 3");
	m_lightCombo.SetCurSel(0);


	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	//m_modelProperty.ModifyStyle(m_modelProperty.GetStyle(), dwViewStyle);
	if (!m_modelProperty.Create(WS_VISIBLE | WS_CHILD, CRect(0,0,400,200), this, ID_PROPERTY_CUBE))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	m_modelProperty.EnableHeaderCtrl(FALSE);
	m_modelProperty.SetFocus();
	m_modelProperty.SetVSDotNetLook();
	m_modelProperty.MarkModifiedProperties();


	if (!m_lightProperty.Create(WS_VISIBLE | WS_CHILD, CRect(0,240,400,550), this, ID_PROPERTY_LIGHT))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	m_lightProperty.EnableHeaderCtrl(FALSE);
	m_lightProperty.SetFocus();
	m_lightProperty.SetVSDotNetLook();
	m_lightProperty.MarkModifiedProperties();


	UpdateModelProperty(cController::Get()->GetCube().GetMaterial());
	UpdateLightProperty(cController::Get()->GetLight(0));


	return TRUE;
}


void CLightPanel::UpdateModelProperty(const cMaterial &mtrl)
{
	m_modelProperty.RemoveAll();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Cube Property"));

	AddPropertyColor4(pGroup1, L"Diffuse", *(Vector4*)&mtrl.m_mtrl.Diffuse);
	AddPropertyColor4(pGroup1, L"Ambient", *(Vector4*)&mtrl.m_mtrl.Ambient);
	AddPropertyColor4(pGroup1, L"Specular", *(Vector4*)&mtrl.m_mtrl.Specular);
	AddPropertyColor4(pGroup1, L"Emissive", *(Vector4*)&mtrl.m_mtrl.Emissive);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Power"), mtrl.m_mtrl.Power, L""));

	m_modelProperty.AddProperty( pGroup1);
}


void CLightPanel::UpdateLightProperty(const cLight &light)
{
	m_lightProperty.RemoveAll();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Light Property"));

	// construct a COleVariant object. 
	//COleVariant var3DLook((short)VARIANT_FALSE, VT_BOOL);
	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D Look"), var3DLook, 
	//	_T("Specifies the dialog's font will be nonbold and controls will have a 3D border")));

	CString types[] = {
		_T("None"),
		_T("Point Light"),
		_T("SpotLight"),
		_T("Directional Light"),
	};
	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Type"),
		types[ light.m_light.Type] );
	pProp->AddOption(types[0]);
	pProp->AddOption(types[1]);
	pProp->AddOption(types[2]);
	pProp->AddOption(types[3]);
	pProp->AllowEdit(FALSE);
	pGroup1->AddSubItem(pProp);

	AddPropertyColor4(pGroup1, L"Diffuse", *(Vector4*)&light.m_light.Diffuse);
	AddPropertyColor4(pGroup1, L"Specular", *(Vector4*)&light.m_light.Specular);
	AddPropertyColor4(pGroup1, L"Ambient", *(Vector4*)&light.m_light.Ambient);
	AddPropertyVector3(pGroup1, L"Position", *(Vector3*)&light.m_light.Position);
	AddPropertyVector3(pGroup1, L"Direction", *(Vector3*)&light.m_light.Direction);

	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Range"), light.m_light.Range, L""));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Falloff"), light.m_light.Falloff, L""));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Attenuation0"), light.m_light.Attenuation0, L""));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Attenuation1"), light.m_light.Attenuation1, L""));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Attenuation2"), light.m_light.Attenuation2, L""));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Theta"), light.m_light.Theta, L""));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Phi"), light.m_light.Phi, L""));

	pGroup1->AdjustButtonRect();
	pGroup1->AllowEdit();
	pGroup1->Enable();
	pGroup1->Show();
	pGroup1->Redraw();

	m_lightProperty.AddProperty( pGroup1);
}


void CLightPanel::AddPropertyColor4(CMFCPropertyGridProperty *group, CString name, 
	Vector4 value)
{
	CMFCPropertyGridProperty *pProp;

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(name, 0, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("r"), (_variant_t)value.x, L"");
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("g"), (_variant_t)value.y, L"");
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("b"), (_variant_t)value.z, L"");
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("a"), (_variant_t)value.w, L"");
	pSize->AddSubItem(pProp);

	group->AddSubItem(pSize);
}


void CLightPanel::AddPropertyVector3(CMFCPropertyGridProperty *group, CString name, 
	Vector3 value)
{
	CMFCPropertyGridProperty *pProp;

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(name, 0, TRUE);
	pProp = new CMFCPropertyGridProperty(_T("x"), (_variant_t)value.x, L"");
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("y"), (_variant_t)value.y, L"");
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("z"), (_variant_t)value.z, L"");
	pSize->AddSubItem(pProp);

	group->AddSubItem(pSize);
}


void CLightPanel::OnSelchangeComboLight()
{
	const int select = m_lightCombo.GetCurSel();
	UpdateLightProperty(cController::Get()->GetLight(select));
}


LRESULT CLightPanel::OnPropertyChanged (WPARAM wparam, LPARAM lparam)
{
	CMFCPropertyGridProperty *pProperty = (CMFCPropertyGridProperty*)lparam;
	
	switch (wparam)
	{
		case ID_PROPERTY_CUBE:
			{
				if (CString(L"Power") == pProperty->GetName())
				{

				}
				else if ((CString(L"r") == pProperty->GetName()) || 
					(CString(L"g") == pProperty->GetName()) ||
					(CString(L"b") == pProperty->GetName()) || 
					(CString(L"a") == pProperty->GetName()) )
				{
					CMFCPropertyGridProperty *parent = pProperty->GetParent();

					if (CString(L"Diffuse") == parent->GetName())
					{

					}

				}
				else if (CString(L"Diffuse") == pProperty->GetName())
				{
					int a = 0;

				}

			}
			break;

		case ID_PROPERTY_LIGHT:
			{

			}
			break;
	}

	return 0;
}
