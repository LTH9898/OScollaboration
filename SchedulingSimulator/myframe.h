#ifndef __MYFRAME_H__
#define __MYFRAME_H__
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wfstream.h>
#include "cpu_scheduler.h"



enum
{
    ID_Open = 1,
    ID_Save,
    ID_SaveAs,

    BUTTON_CREATE,
    BUTTON_DELETE,
    BUTTON_CLEAR,
    BUTTON_CONFIRM,
    BUTTON_RUN,
    BUTTON_STEPRUN,
    BUTTON_GANTTCLEAR,
    SCROLL_UPPER,
};



class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    // Events
    // File events
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    // Upper window events
    void CreateProcessBlock(wxCommandEvent& event);
    void DeleteProcessBlock(wxCommandEvent& event);
    void ClearProcessBlock(wxCommandEvent& event);
    void OnUpperScroll(wxScrollEvent& event)


    {
        ScrollUpperWindow();
    }
    void ConFirmProcessBlock(wxCommandEvent& event);

    // Main window event
    void OnPaint(wxPaintEvent& event);
    void OnWindowSize(wxSizeEvent& event);
    void OnLeftDown(wxMouseEvent& event)
    {
        previousPos = event.GetLogicalPosition(_m_clntDC); event.Skip();
    }
    void OnMotion(wxMouseEvent& event);

    // Functions
    // Upper window functions
    wxPoint CreateBlockPos(int row, int column);
    void SetUpperScroll();
    void ScrollUpperWindow();
    void DragUpperWindow(const wxPoint& currentPos, int direction);

    // Lower window functions
    void CreateGanttChart(wxCommandEvent& event);
    //void DragLowerWindow(wxPoint currentPos, wxPoint direction);


    // Private variables
    wxString currentFilePath;

    std::vector<wxStaticText*> texts;
    wxTextCtrl* textctrlTQ;
    std::vector<wxTextCtrl*> textctrls;
    wxScrollBar* upperScroll;
    std::unique_ptr<ProcessQueue> MakeProcessQueue();
    int blockSize;

    int lowerWindowY;

    wxClientDC _m_clntDC;
    wxPoint previousPos;

    // Constants
    enum
    {
        BAR_SIZE = 35,
        BUTTON_WIDTH = 70,
        BUTTON_HEIGHT = 25,
        TEXT_WIDTH = 95,
        TEXT_HEIGHT = 20,
        TEXTCTRL_WIDTH = 70,

        MAX_PROCESS = 32,
        SIZEOF_ALGORITHMS = 7
    };
};

inline bool IsPosInRange(const wxPoint& pos, const wxPoint& upperLeft, int lower, int right)
{
    return upperLeft.x <= pos.x && pos.x <= right && upperLeft.y <= pos.y && pos.y <= lower;
}

#endif
