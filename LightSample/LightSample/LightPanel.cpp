// LightPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LightPanel.h"
#include "afxdialogex.h"
#include "Controller.h"
#include "PropGridSlider.h"

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
	ON_WM_SIZE()
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

	//----------------------------------------------------------------------------------------------
	// Init PropertyGrid Control
	if (!m_modelProperty.Create(WS_VISIBLE | WS_CHILD, CRect(0,0,400,250), this, ID_PROPERTY_CUBE))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	m_modelProperty.EnableHeaderCtrl(FALSE);
	m_modelProperty.SetFocus();
	m_modelProperty.SetVSDotNetLook();
	m_modelProperty.MarkModifiedProperties();


	if (!m_lightProperty.Create(WS_VISIBLE | WS_CHILD, CRect(0,290,400,550), this, ID_PROPERTY_LIGHT))
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
	AddPropertyVector3(pGroup1, L"Direction", *(Vector3*)&light.m_light.Direction,
		-1, 1, 100);

	//pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Range"), light.m_light.Range, L""));
	pGroup1->AddSubItem(new CPropGridSlider(_T("Range"), light.m_light.Range, L"", 0, 500, 1000) );
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Falloff"), light.m_light.Falloff, L""));
	pGroup1->AddSubItem(new CPropGridSlider(_T("Attenuation0"), light.m_light.Attenuation0, L"", 0, 1.f, 1000));
	pGroup1->AddSubItem(new CPropGridSlider(_T("Attenuation1"), light.m_light.Attenuation1, L"", 0, 0.1f, 1000));
	pGroup1->AddSubItem(new CPropGridSlider(_T("Attenuation2"), light.m_light.Attenuation2, L"", 0, 0.1f, 1000));
	pGroup1->AddSubItem(new CPropGridSlider(_T("Theta"), light.m_light.Theta, L"", 0, 3.14f, 100));
	pGroup1->AddSubItem(new CPropGridSlider(_T("Phi"), light.m_light.Phi, L"", 0, 1, 100) );

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
	pProp = new CPropGridSlider(_T("r"), (_variant_t)value.x, L"", 0, 1, 100);
	pSize->AddSubItem(pProp);

	pProp = new CPropGridSlider( _T("g"), (_variant_t)value.y, L"", 0, 1, 100);
	pSize->AddSubItem(pProp);

	pProp = new CPropGridSlider( _T("b"), (_variant_t)value.z, L"", 0, 1, 100);
	pSize->AddSubItem(pProp);

	pProp = new CPropGridSlider( _T("a"), (_variant_t)value.w, L"", 0, 1, 100);
	pSize->AddSubItem(pProp);

	group->AddSubItem(pSize);
}


void CLightPanel::AddPropertyVector3(CMFCPropertyGridProperty *group, CString name, 
	Vector3 value, const float _min, const float _max, const int slice)
{
	CMFCPropertyGridProperty *pProp;

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(name, 0, TRUE);
	pProp = new CPropGridSlider(_T("x"), (_variant_t)value.x, L"", _min, _max, slice);
	pSize->AddSubItem(pProp);

	pProp = new CPropGridSlider( _T("y"), (_variant_t)value.y, L"", _min, _max, slice);
	pSize->AddSubItem(pProp);

	pProp = new CPropGridSlider( _T("z"), (_variant_t)value.z, L"", _min, _max, slice);
	pSize->AddSubItem(pProp);

	group->AddSubItem(pSize);
}


void CLightPanel::OnSelchangeComboLight()
{
	const int select = m_lightCombo.GetCurSel();
	UpdateLightProperty(cController::Get()->GetLight(select));
	cController::Get()->SelectLight(select);
}


LRESULT CLightPanel::OnPropertyChanged (WPARAM wparam, LPARAM lparam)
{
	CMFCPropertyGridProperty *prop = (CMFCPropertyGridProperty*)lparam;
	
	switch (wparam)
	{
		case ID_PROPERTY_CUBE:
			{
				if ((CString(L"r") == prop->GetName()) || 
					(CString(L"g") == prop->GetName()) ||
					(CString(L"b") == prop->GetName()) || 
					(CString(L"a") == prop->GetName()) )
				{
					CMFCPropertyGridProperty *parent = prop->GetParent();
					ChangeMaterialValue(parent);
				}
				else
				{
					ChangeMaterialValue(prop);
				}
			}
			break;

		case ID_PROPERTY_LIGHT:
			{
				if ((CString(L"r") == prop->GetName()) || 
					(CString(L"g") == prop->GetName()) ||
					(CString(L"b") == prop->GetName()) || 
					(CString(L"a") == prop->GetName()) ||
					(CString(L"x") == prop->GetName()) ||
					(CString(L"y") == prop->GetName()) ||
					(CString(L"z") == prop->GetName()) ||
					(CString(L"w") == prop->GetName()) )
				{
					CMFCPropertyGridProperty *parent = prop->GetParent();
					ChangeLightValue(parent);
				}
				else
				{
					ChangeLightValue(prop);
				}
			}
			break;
	}

	return 0;
}


void CLightPanel::ChangeMaterialValue(CMFCPropertyGridProperty *prop)
{
	RET(!prop);

	cCube2 &cube = cController::Get()->GetCube();

	if (CString(L"Power") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		cube.GetMaterial().m_mtrl.Power = value;
	}
	else if (CString(L"Diffuse") == prop->GetName())
	{
		const Vector4 value= GetPropertyVector4(prop);
		cube.GetMaterial().m_mtrl.Diffuse = *(D3DXCOLOR*)&value;
	}
	else if (CString(L"Ambient") == prop->GetName())
	{
		const Vector4 value = GetPropertyVector4(prop);
		cube.GetMaterial().m_mtrl.Ambient = *(D3DXCOLOR*)&value;
	}
	else if (CString(L"Specular") == prop->GetName())
	{
		const Vector4 value = GetPropertyVector4(prop);
		cube.GetMaterial().m_mtrl.Specular = *(D3DXCOLOR*)&value;
	}
	else if (CString(L"Emissive") == prop->GetName())
	{
		const Vector4 value = GetPropertyVector4(prop);
		cube.GetMaterial().m_mtrl.Emissive = *(D3DXCOLOR*)&value;
	}
}


void CLightPanel::ChangeLightValue(CMFCPropertyGridProperty *prop)
{
	RET(!prop);

	const int selectLight = m_lightCombo.GetCurSel();
	cLight &light = cController::Get()->GetLight(selectLight);


	if (CString(L"Diffuse") == prop->GetName())
	{
		const Vector4 value= GetPropertyVector4(prop);
		light.m_light.Diffuse = *(D3DXCOLOR*)&value;
	}
	else if (CString(L"Ambient") == prop->GetName())
	{
		const Vector4 value = GetPropertyVector4(prop);
		light.m_light.Ambient = *(D3DXCOLOR*)&value;
	}
	else if (CString(L"Specular") == prop->GetName())
	{
		const Vector4 value = GetPropertyVector4(prop);
		light.m_light.Specular = *(D3DXCOLOR*)&value;
	}
	else if (CString(L"Position") == prop->GetName())
	{
		const Vector3 value = GetPropertyVector3(prop);
		light.m_light.Position = *(D3DXVECTOR3*)&value;
	}
	else if (CString(L"Direction") == prop->GetName())
	{
		const Vector3 value = GetPropertyVector3(prop);
		light.m_light.Direction = *(D3DXVECTOR3*)&value;
	}	
	else if (CString(L"Range") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Range = value;
	}
	else if (CString(L"Falloff") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Falloff = value;
	}
	else if (CString(L"Attenuation0") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Attenuation0 = value;
	}
	else if (CString(L"Attenuation1") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Attenuation1 = value;
	}
	else if (CString(L"Attenuation2") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Attenuation2 = value;
	}
	else if (CString(L"Theta") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Theta = value;
	}
	else if (CString(L"Phi") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		light.m_light.Phi = value;
	}
	else if (CString(L"Type") == prop->GetName())
	{
		const _variant_t value = prop->GetValue();
		CString type = value;
		for (int i=0; i < prop->GetOptionCount(); ++i)
		{
			if (prop->GetOption(i) == type)
			{
				light.m_light.Type = (D3DLIGHTTYPE)i;
				break;
			}
		}
	}

}


Vector4 CLightPanel::GetPropertyVector4(CMFCPropertyGridProperty *group)
{
	CMFCPropertyGridProperty *propX = group->GetSubItem(0);
	CMFCPropertyGridProperty *propY = group->GetSubItem(1);
	CMFCPropertyGridProperty *propZ = group->GetSubItem(2);
	CMFCPropertyGridProperty *propW = group->GetSubItem(3);

	_variant_t x = propX->GetValue();
	_variant_t y = propY->GetValue();
	_variant_t z = propZ->GetValue();
	_variant_t w = propW->GetValue();

	return Vector4(x, y, z, w);
}


Vector3 CLightPanel::GetPropertyVector3(CMFCPropertyGridProperty *group)
{
	CMFCPropertyGridProperty *propX = group->GetSubItem(0);
	CMFCPropertyGridProperty *propY = group->GetSubItem(1);
	CMFCPropertyGridProperty *propZ = group->GetSubItem(2);

	_variant_t x = propX->GetValue();
	_variant_t y = propY->GetValue();
	_variant_t z = propZ->GetValue();

	return Vector3(x, y, z);
}


void CLightPanel::MoveChildCtrlWindow(CWnd &wndCtrl, int cx, int cy)
{
	if (wndCtrl.GetSafeHwnd())
	{
		CRect wr;
		wndCtrl.GetWindowRect(wr);
		ScreenToClient(wr);
		wndCtrl.MoveWindow(wr.left, wr.top, cx, wr.Height());
	}
}


void CLightPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	MoveChildCtrlWindow(m_modelProperty, cx, cy);
	MoveChildCtrlWindow(m_lightProperty, cx, cy);	
}
