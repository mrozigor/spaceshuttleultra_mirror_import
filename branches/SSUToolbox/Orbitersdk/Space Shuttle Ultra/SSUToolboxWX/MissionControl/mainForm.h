#ifndef MAINFORM_H
#define MAINFORM_H

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyFrame;

class MyApp : public wxApp
{
public:
	MyFrame *myFrame;

	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxPoint(0,0),
				const wxSize& size = wxSize(300, 200), long style = 0 | wxTAB_TRAVERSAL);

private:
	//void OnHello(wxCommandEvent& event);
	//void OnExit(wxCommandEvent& event);
	//void OnAbout(wxCommandEvent& event);

	//wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_Hello = 1
};

/*
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
*/

#endif // MAINFORM_H
