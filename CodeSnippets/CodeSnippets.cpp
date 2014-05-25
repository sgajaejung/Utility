
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/listctrl.h>
#include <wx/dnd.h>
#include <vector>
#include <fstream>
#include <string>
using std::string;
using std::vector;


class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title);

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSelected(wxListEvent& event);

protected:
	bool ReadCodeSnippetsFile(const string &fileName);

private:
	DECLARE_EVENT_TABLE()
	wxListCtrl *m_listCtrl;
	wxTextCtrl *m_textCtrl;
	vector<string> m_itemStr;
	vector<string> m_itemSource;
};

enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT,

	LIST_CTRL = 1000,
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
	EVT_MENU(Minimal_About, MyFrame::OnAbout)
	EVT_LIST_ITEM_SELECTED(LIST_CTRL, MyFrame::OnSelected)
END_EVENT_TABLE()

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
	if ( !wxApp::OnInit() )
		return false;

	MyFrame *frame = new MyFrame("Code Snippets with wxWidgets");
	frame->Show(true);
	return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition,  wxSize(600, 500))
{
	SetIcon(wxICON(sample));

#if wxUSE_MENUS
	wxMenu *fileMenu = new wxMenu;
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");
	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	SetMenuBar(menuBar);
#endif // wxUSE_MENUS



	wxBoxSizer *row1 = new wxBoxSizer(wxVERTICAL);
	SetSizer(row1);

	wxBoxSizer *column1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *column2 = new wxBoxSizer(wxVERTICAL);
	row1->Add( column1, 1, wxGROW, 10 );
	row1->Add( column2, 1, wxGROW, 10 );

	m_listCtrl = new wxListCtrl(this, LIST_CTRL,
		wxDefaultPosition, wxDefaultSize,
		wxLC_LIST | wxBORDER_THEME | wxLC_EDIT_LABELS);
	column1->Add( m_listCtrl, 1, wxEXPAND, 10 );

	m_textCtrl = new wxTextCtrl( this, wxID_ANY, wxT("souce code"),
		wxDefaultPosition, wxDefaultSize,
		wxTE_READONLY | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_RICH);
	column2->Add( m_textCtrl, 1, wxEXPAND, 10 );

	m_itemStr.reserve(128);
	m_itemStr.reserve(128);

	ReadCodeSnippetsFile("code.txt");
}


void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
		(
		"Welcome to %s!\n"
		"\n"
		"This is the Code Snippets Application\n"
		"running under %s.",
		wxVERSION_STRING,
		wxGetOsDescription()
		),
		"About Code Snippets",
		wxOK | wxICON_INFORMATION,
		this);
}


void MyFrame::OnSelected(wxListEvent& event)
{
	string source;
	if ((int)m_itemSource.size() >= event.m_itemIndex)
	{
		source = m_itemSource[ event.m_itemIndex];
		m_textCtrl->Clear();
		m_textCtrl->WriteText( source );
	}

	wxTextDataObject dragData(source);
	wxDropSource dragSource( this );
	dragSource.SetData( dragData );
	wxDragResult result = dragSource.DoDragDrop( TRUE );
}


bool MyFrame::ReadCodeSnippetsFile(const string &fileName)
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	int selectItem = -1;
	string source;

	while (1)
	{
		string str;
		if (!std::getline(fin, str))
			break;
		const int idx = str.find("@@item");
		if ((string::npos == idx) || (idx > 10))
		{
			source += str;
			source += "\n";
		}
		else
		{
			if (selectItem >= 0)
				m_itemSource.push_back(source);

			source.clear();// 초기화.
			++selectItem;
			m_itemStr.push_back( &str[6] );
		}
	}
	// 마지막 소스 내용 추가.
	if (!source.empty())
		m_itemSource.push_back(source);

	for (int i=0; i < (int)m_itemStr.size(); ++i)
	{
		m_listCtrl->InsertItem(i, m_itemStr[ i]);
	}

	return true;
}
