// wxWidgets "Hello World" Program

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/choice.h>
#include <wx/stattext.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

constexpr int SIZEOF_ALGORITHMS = 7;


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnLoad(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
};

enum
{
    ID_Load = 1,
    ID_Save,
    ID_SaveAs,
    ID_ChoiceAlgorithms,
    TEXT_CHOICE,
    BUTTON1
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, _T("Scheduling Simulator"))
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Load, _T("&열기\tCtrl-O"));
    menuFile->Append(ID_Save, _T("&저장\tCtrl-S"));
    menuFile->Append(ID_SaveAs, _T("&다른 이름으로 저장\tCtrl-Shift-S"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&파일");
    SetMenuBar(menuBar);


    wxString algorithms [SIZEOF_ALGORITHMS] =
    {
        _T("FCFS"),
        _T("SJF"),
        _T("SRTF"),
        _T("Round Robin"),
        _T("Non-preemptive Priority"),
        _T("Preemptive Priority"),
        _T("Preemptive Priority with RR")
    };

    new wxStaticText(this, TEXT_CHOICE, _T("스케줄링 알고리즘 선택"), wxPoint(20, 15));
    wxChoice* choiceAlgorithms = new wxChoice(this, ID_ChoiceAlgorithms, wxPoint(15, 35), wxSize(180, 30), SIZEOF_ALGORITHMS, algorithms);


    Bind(wxEVT_MENU, &MyFrame::OnLoad, this, ID_Load);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, ID_Save);
    Bind(wxEVT_MENU, &MyFrame::OnSaveAs, this, ID_SaveAs);
}




void MyFrame::OnLoad(wxCommandEvent& event)
{
    wxMessageBox("Load : nop");
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    wxMessageBox("Save : nop");
}

void MyFrame::OnSaveAs(wxCommandEvent& event)
{
    wxMessageBox("Save As : nop");
}
