#include "mainForm.h"

bool MyApp::OnInit()
{
	//myFrame = new MyFrame(NULL);
	//myFrame->Show(true);
	return true;
}

MyFrame::MyFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	wxButton *button = new wxButton(this, wxID_ANY, _T("I am the client frame"), wxDefaultPosition, wxDefaultSize, 0);
}
/*
void MyFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("This is a wxWidgets' Hello world sample",
		"About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}
*/
