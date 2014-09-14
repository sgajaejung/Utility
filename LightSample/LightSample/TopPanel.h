#pragma once


// CTopPanel 프레임입니다.
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

