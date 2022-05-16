#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wfstream.h>


constexpr int SIZEOF_ALGORITHMS = 7;

enum
{
    ID_Open = 1,
    ID_Save,
    ID_SaveAs,

    BUTTON_CREATE,
    BUTTON_DELETE,
    BUTTON_CLEAR,
    SCROLL_UPPER,
};



class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    // Events
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);

    void CreateProcessBlock(wxCommandEvent& event);
    void DeleteProcessBlock(wxCommandEvent& event);
    void ClearProcessBlock(wxCommandEvent& event);
    void OnUpperScroll(wxScrollEvent& event);

    void OnPaint(wxPaintEvent& event);
    void OnWindowSize(wxSizeEvent& event);

    // Functions
    wxPoint CreateBlockPos(int row, int column);
    void SetUpperScroll();
    void ScrollUpperWindow();

    // Private variables
    wxString currentFilePath;

    std::vector<wxStaticText*> texts;
    wxTextCtrl* textctrlTQ;
    std::vector<wxTextCtrl*> textctrls;
    wxScrollBar* upperScroll;
    int blockSize;

    int lowerWindowY;

    // Constants
    enum
    {
        BUTTON_WIDTH = 70,
        BUTTON_HEIGHT = 25,
        TEXT_WIDTH = 95,
        TEXT_HEIGHT = 20,
        TEXTCTRL_WIDTH = 70,
        
        MAX_PROCESS = 32,
        SIZEOF_ALGORITHMS = 7
    };
};