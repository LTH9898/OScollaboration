#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wfstream.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <random>
#include <map>
#include "cpu_scheduler.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class ResultDialog :
    public wxDialog
{
public:
    ResultDialog(wxWindow* parent, wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxASCII_STR(wxDialogNameStr));



private:
    wxTextCtrl* cityBox = nullptr;
    wxTextCtrl* streetBox = nullptr;
    std::vector<wxStaticText*> texts;
    std::vector<std::string> pidList;

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

