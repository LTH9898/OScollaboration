#ifndef __MYFRAME_H__
#define __MYFRAME_H__
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wfstream.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <random>
#include <map>
#include <wx/grid.h>
#include <fstream>
#include "cpu_scheduler.h"



enum
{

    ID_New = 1,
    ID_Open,
    ID_Save,
    ID_SaveAs,

    BUTTON_TEST,
    BUTTON_CREATE,
    BUTTON_DELETE,
    BUTTON_CLEAR,
    SCROLL_UPPER,
    SCROLL_LOWER,

    BITMAPBTN_RUN,
    BITMAPBTN_STEP,
    BITMAPBTN_RESET,
};



class MyFrame : public wxFrame
{
public:
    MyFrame();
        

private:
    // Events
    void OnResult(wxCommandEvent& event);
    // File events
    void OnNew(wxCommandEvent& event)
        { currentFilePath = ""; _ClearProcessBlock(); }
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event)
        { _OnSaveAs(); }
        void _OnSaveAs();

    // Upper window events
    void CreateProcessBlock(wxCommandEvent& event);
    void DeleteProcessBlock(wxCommandEvent& event);
    void ClearProcessBlock(wxCommandEvent& event)
        { _ClearProcessBlock(); }
        void _ClearProcessBlock();
    void OnUpperScroll(wxScrollEvent& event)
        { ScrollUpperWindow(); Refresh(); Update(); }
    // Lower window events
    void RunScheduler(wxCommandEvent& event);
    void StepScheduler(wxCommandEvent& event);
    void ResetScheduler(wxCommandEvent& event)
        { scheduler.Reset(); Refresh(); Update(); }
    void OnLowerScroll(wxScrollEvent& event)
    {
        ScrollLowerWindow();
    }

    // Main window event
    void OnPaint(wxPaintEvent& event);
    void OnWindowSize(wxSizeEvent& event);
    void OnLeftDown(wxMouseEvent& event)
        { previousPos = event.GetLogicalPosition(_m_clntDC); event.Skip(); }
    void OnMotion(wxMouseEvent& event);


    // Functions
    // Upper window functions
    wxPoint CreateBlockPos(int row, int column);
    void SetUpperScroll();
    void ScrollUpperWindow();
    void DragUpperWindow(const wxPoint& currentPos, int direction);
    // Lower window functions
    std::unique_ptr<ProcessQueue> MakeProcessQueue();
    void SetLowerScroll();
    void ScrollLowerWindow();
    //void DragLowerWindow();
    bool InitScheduler();
    void InitColorTable();
    void AllocateColor();
    void SetChartArea();
    void SetBaseX(int& baseX, int end);


    // Private variables
    wxString currentFilePath;

    std::vector<wxStaticText*> texts;
    wxTextCtrl* textctrlTQ;
    std::vector<wxTextCtrl*> textctrls;
    wxScrollBar* upperScroll;
    wxScrollBar* lowerScroll;
    wxChoice* choiceAlgorithms;
    wxGrid* wxgrid;
    
    CpuScheduler scheduler;
#define TABLE_NUM 6
#define CODE_BASE 'A'
    char colorTable[TABLE_NUM * TABLE_NUM * TABLE_NUM][3];
    std::vector<std::string> pidList;
    std::map<std::string, std::string> colorList;
    std::vector<int> chartX;
    std::vector<int> chartWidth;
    std::vector<int> timeX;
    std::vector<int> pidX;

    int blockSize;
    int lowerWindowX;
    int lowerWindowY;
    int chartEnd;
    int wqX;
    int wqY;
    int wqEnd;

    wxClientDC _m_clntDC;
    wxPoint previousPos;

    // Constants
    enum
    {
        MAX_PROCESS = 32,
        SIZEOF_ALGORITHMS = 7,

        BAR_SIZE = 35,
        BUTTON_WIDTH = 70,
        BUTTON_HEIGHT = 25,
        TEXT_WIDTH = 95,
        TEXT_HEIGHT = 20,
        TEXTCTRL_WIDTH = 70,

        CHART_HEIGHT = 40,
        UNIT_CHART = 30
    };
};

inline bool IsPosInRange(const wxPoint& pos, const wxPoint& upperLeft, int lower, int right)
{
    return upperLeft.x <= pos.x && pos.x <= right && upperLeft.y <= pos.y && pos.y <= lower;
}

#endif
