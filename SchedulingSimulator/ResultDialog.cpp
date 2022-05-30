#include "ResultDialog.h"
#include "myframe.h"
#include "cpu_scheduler.h"

ResultDialog::ResultDialog(wxWindow* parent, wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name) :
    wxDialog(parent, id, title, pos, size, style, name) 
{

    
    
    //Create Boxes
    wxSize textSize = wxSize(TEXT_WIDTH, TEXT_HEIGHT);
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Process ID "), wxPoint(10, 0), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Arrival Time "), wxPoint(10, 0 + TEXT_HEIGHT), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Burst Time "), wxPoint(10, 0 + 2 * TEXT_HEIGHT), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Priority "), wxPoint(10, 0 + 3 * TEXT_HEIGHT), textSize, style));
    wxSize ctrlSize = wxSize(TEXTCTRL_WIDTH, TEXT_HEIGHT);
    //textctrlTQ = new wxTextCtrl(this, wxID_ANY, "", wxPoint(TEXT_WIDTH + 10, 45), ctrlSize, wxBORDER_SIMPLE);
    // Create Scrollbar for upper window
    //upperScroll = new wxScrollBar(this, SCROLL_UPPER, wxPoint(0, 200));

    //texts.emplace_back(new wxStaticText(this, wxID_ANY, pidList[0], wxPoint(10, 0 + 3 * TEXT_HEIGHT), textSize, style));
    
    
    //Create button Field
    wxSize btnSize = wxSize(BUTTON_WIDTH, BUTTON_HEIGHT);
   

    SetMinSize(wxSize(512, 200));
    Fit();
}

//std::unique_ptr<ProcessQueue> MyFrame::MakeProcessQueue()
//{
//    auto pQ = CreateProcessQueue();
//    pidList.clear();
//
//    for (auto i = 0; i + 3 < 4 * blockSize; i = i + 4) {
//        std::string tempPid = textctrls[i]->GetValue().ToStdString();
//        double tempArrivaltime;
//        textctrls[i + 1]->GetValue().ToDouble(&tempArrivaltime);
//        double tempBursttime;
//        textctrls[i + 2]->GetValue().ToDouble(&tempBursttime);
//        unsigned tempPriority;
//        textctrls[i + 3]->GetValue().ToUInt(&tempPriority);
//
//        tempArrivaltime = tempArrivaltime < 0 ? 0 : tempArrivaltime;
//        textctrls[i + 1]->SetValue(wxString::FromDouble(tempArrivaltime));
//        tempBursttime = tempBursttime < 0 ? 0 : tempBursttime;
//        textctrls[i + 2]->SetValue(wxString::FromDouble(tempBursttime));
//
//        pQ->emplace(tempPid, tempArrivaltime, tempBursttime, tempPriority);
//        pidList.push_back(tempPid);
//    }
//    return pQ;
//}