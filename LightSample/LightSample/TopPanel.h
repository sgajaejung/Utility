#pragma once


// CTopPanel �������Դϴ�.
class CLightPanel;
class CTopPanel : public CMiniFrameWnd
{
public:
	CTopPanel();
	virtual ~CTopPanel();

	void Init();


protected:
	CLightPanel *m_lightPanel;

	DECLARE_MESSAGE_MAP()
};

