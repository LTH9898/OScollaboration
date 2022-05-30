#include "myframe.h"


MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, _T("Scheduling Simulator")), _m_clntDC(this), blockSize(0), lowerWindowX(0), wqX(0)
{
    // Initialize MyFrame
    SetMinSize(wxSize(512, 560));
    SetBackgroundColour(*wxWHITE);
    CreateStatusBar();
    wxInitAllImageHandlers();
    InitColorTable();


    // Menu bar
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Open, _T("&Open\tCtrl-O"));
    menuFile->Append(ID_Save, _T("&Save\tCtrl-S"));
    menuFile->Append(ID_SaveAs, _T("&Save As\tCtrl-Shift-S"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _T("&File"));
    SetMenuBar(menuBar);


    // Upeer window
    // Process button
    wxSize btnSize = wxSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new wxButton(this, BUTTON_CREATE, _T("Create"), wxPoint(20, 5), btnSize);
    new wxButton(this, BUTTON_DELETE, _T("Delete"), wxPoint(30 + BUTTON_WIDTH, 5), btnSize);
    new wxButton(this, BUTTON_CLEAR, _T("Clear"), wxPoint(40 + 2 * BUTTON_WIDTH, 5), btnSize);

    // Process input
    wxSize textSize = wxSize(TEXT_WIDTH, TEXT_HEIGHT);
    long style = wxALIGN_RIGHT | wxBORDER_SIMPLE;
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Time Quantum "), wxPoint(10, 45), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Process ID "), wxPoint(10, 90), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Arrival Time "), wxPoint(10, 90 + TEXT_HEIGHT), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Burst Time "), wxPoint(10, 90 + 2 * TEXT_HEIGHT), textSize, style));
    texts.emplace_back(new wxStaticText(this, wxID_ANY, _T("Priority "), wxPoint(10, 90 + 3 * TEXT_HEIGHT), textSize, style));
    wxSize ctrlSize = wxSize(TEXTCTRL_WIDTH, TEXT_HEIGHT);
    textctrlTQ = new wxTextCtrl(this, wxID_ANY, "", wxPoint(TEXT_WIDTH + 10, 45), ctrlSize, wxBORDER_SIMPLE);
    // Create Scrollbar for upper window
    upperScroll = new wxScrollBar(this, SCROLL_UPPER, wxPoint(0, 200));
   


    // Lower window
    lowerWindowY = upperScroll->GetPosition().y + upperScroll->GetSize().GetHeight();
    wqY = lowerWindowY + 70 + CHART_HEIGHT + 35;

    // Schedular choice
    wxString algorithms[SIZEOF_ALGORITHMS] =
    {
        _T("FCFS"),
        _T("SJF"),
        _T("SRTF"),
        _T("Round Robin"),
        _T("Non-preemptive Priority"),
        _T("Preemptive Priority"),
        _T("Preemptive Priority with RR")
    };

    choiceAlgorithms = new wxChoice(this, wxID_ANY, wxPoint(5, lowerWindowY + 6),
        wxSize(180, 30), SIZEOF_ALGORITHMS, algorithms);
    choiceAlgorithms->SetSelection(0);
    
    auto path = wxStandardPaths::Get().GetDataDir();
    wxSize bitmapBtnSize(BUTTON_HEIGHT, BUTTON_HEIGHT);
    wxImage imgPlay(path + "\\img\\play.png");
    wxImage imgNext(path + "\\img\\next.png");
    wxImage imgStop(path + "\\img\\stop.png");
    imgPlay.Rescale(BUTTON_HEIGHT - 4, BUTTON_HEIGHT - 4, wxIMAGE_QUALITY_HIGH);
    imgNext.Rescale(BUTTON_HEIGHT - 4, BUTTON_HEIGHT - 4, wxIMAGE_QUALITY_HIGH);
    imgStop.Rescale(BUTTON_HEIGHT - 4, BUTTON_HEIGHT - 4, wxIMAGE_QUALITY_HIGH);
    new wxBitmapButton(this, BITMAPBTN_RUN, imgPlay, wxPoint(200, lowerWindowY + 5), bitmapBtnSize);
    new wxBitmapButton(this, BITMAPBTN_STEP, imgNext, wxPoint(200 + BUTTON_HEIGHT + 10, lowerWindowY + 5), bitmapBtnSize);
    new wxBitmapButton(this, BITMAPBTN_RESET, imgStop, wxPoint(200 + (BUTTON_HEIGHT + 10) * 2, lowerWindowY + 5), bitmapBtnSize);


    // Event
    // File events
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, ID_Open);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, ID_Save);
    Bind(wxEVT_MENU, &MyFrame::OnSaveAs, this, ID_SaveAs);
    // Upper window events
    Bind(wxEVT_BUTTON, &MyFrame::CreateProcessBlock, this, BUTTON_CREATE);
    Bind(wxEVT_BUTTON, &MyFrame::DeleteProcessBlock, this, BUTTON_DELETE);
    Bind(wxEVT_BUTTON, &MyFrame::ClearProcessBlock, this, BUTTON_CLEAR);
    Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::OnUpperScroll, this, SCROLL_UPPER);
    Bind(wxEVT_SCROLL_PAGEUP, &MyFrame::OnUpperScroll, this, SCROLL_UPPER);
    Bind(wxEVT_SCROLL_PAGEDOWN, &MyFrame::OnUpperScroll, this, SCROLL_UPPER);
    // Lower window events
    Bind(wxEVT_BUTTON, &MyFrame::RunScheduler, this, BITMAPBTN_RUN);
    Bind(wxEVT_BUTTON, &MyFrame::StepScheduler, this, BITMAPBTN_STEP);
    Bind(wxEVT_BUTTON, &MyFrame::ResetScheduler, this, BITMAPBTN_RESET);

    // Main window event
    Bind(wxEVT_PAINT, &MyFrame::OnPaint, this);
    Bind(wxEVT_SIZE, &MyFrame::OnWindowSize, this);
    Bind(wxEVT_LEFT_DOWN, &MyFrame::OnLeftDown, this);
    Bind(wxEVT_MOTION, &MyFrame::OnMotion, this);
}




void MyFrame::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open Test case file"), "", "",
        "Process blocks (*.pcb)|*.pcb", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wxFileInputStream input_stream(openFileDialog.GetPath());

    if (!input_stream.IsOk()) {

        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    wxMessageBox("Save : nop");
}

void MyFrame::OnSaveAs(wxCommandEvent& event)
{
    wxMessageBox("Save As : nop");
}




void MyFrame::CreateProcessBlock(wxCommandEvent& event)
{
    if (blockSize > MAX_PROCESS) {

        wxMessageBox(_T("Exceeding the maximum number of processes"));
        return;
    }

    wxSize ctrlSize = wxSize(TEXTCTRL_WIDTH, TEXT_HEIGHT);

    textctrls.emplace_back(new wxTextCtrl(this, wxID_ANY, "P" + std::to_string(blockSize + 1),
        CreateBlockPos(0, blockSize), ctrlSize, wxBORDER_SIMPLE));
    for (int i = 1; i != 4; i++)
        textctrls.emplace_back(new wxTextCtrl(this, wxID_ANY, "0",
            CreateBlockPos(i, blockSize), ctrlSize, wxBORDER_SIMPLE));
    ++blockSize;

    SetUpperScroll();
}

void MyFrame::DeleteProcessBlock(wxCommandEvent& event)
{
    if (blockSize <= 0)
        return;

    for (int i = 0; i != 4; i++) {

        delete* (textctrls.cend() - 1);
        textctrls.pop_back();
    }
    --blockSize;

    SetUpperScroll();
}

void MyFrame::ClearProcessBlock(wxCommandEvent& event)
{
    for (auto elem : textctrls)
        delete elem;
    textctrls.clear();
    blockSize = 0;

    SetUpperScroll();
}




void MyFrame::RunScheduler(wxCommandEvent& event)
{
    if (!scheduler.IsRunning())
        if (InitScheduler())
            scheduler.StepForward();

    while (scheduler.IsRunning())
        scheduler.StepForward();

    SetChartArea();
    Refresh();
    Update();
}

void MyFrame::StepScheduler(wxCommandEvent& event)
{
    if (!scheduler.IsRunning())
        if (!InitScheduler())
            return;
    scheduler.StepForward();

    SetChartArea();
    Refresh();
    Update();
}

void MyFrame::OnPaint(wxPaintEvent& event)

{
    wxSize size = GetClientSize();
    wxPaintDC dc(this);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxColour("#A4A4A4"));
    dc.DrawRectangle(0, 0, size.GetX(), BAR_SIZE);
    dc.DrawRectangle(0, lowerWindowY, size.GetX(), BAR_SIZE);
    
    // If ganttchart is not empty, draw ganttchart
    auto &gantt = scheduler.GetGantthandler();
    if (!gantt.empty()) {

        dc.DrawText("Gantt chart", 20, lowerWindowY + 50);
        int i = 0;
        int x = lowerWindowX + 20;
        int y = lowerWindowY + 70;
        dc.SetPen(*wxBLACK_PEN);

        for (auto elem : gantt) {

            dc.SetBrush(wxColour(colorList[elem.first]));
            dc.DrawRectangle(x + chartX[i], y, chartWidth[i], CHART_HEIGHT);

            dc.DrawText(wxString::FromDouble(elem.second, 1), x + timeX[i], y + CHART_HEIGHT);
            dc.DrawText(elem.first != "" ? elem.first : "IDLE", x + pidX[i], y + 11);
            i++;
        }
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(GetBackgroundColour());
        dc.DrawRectangle(x + chartEnd, y, size.GetX() - (x + chartEnd), CHART_HEIGHT);


        // Draw separator
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.DrawRectangle(0, wqY - 1, size.GetX(), 2);


        // Draw waiting queue
        dc.DrawText("Waiting queue", 20, wqY + 15);
        i = 0;
        x = wqX + 20;
        y = wqY + 35;
        
        for (auto& elem : scheduler.GetWQhandler().GetWaitingQueue()) {

            std::string pid(elem.GetPid());
            dc.SetBrush(wxColour(colorList[pid]));
            dc.DrawRectangle(x + CHART_HEIGHT * i, y, CHART_HEIGHT, CHART_HEIGHT);

            pid = pid != "" ? pid : "IDLE";
            int space = (CHART_HEIGHT - GetTextExtent(pid).GetWidth()) / 2;
            space = space >= 1 ? space : 1;
            dc.DrawText(pid, x + CHART_HEIGHT * i + space, y + 11);
            i++;
        }
        wqEnd = CHART_HEIGHT * i;

        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(GetBackgroundColour());
        dc.DrawRectangle(x + wqEnd, y, size.GetX() - (x + wqEnd), CHART_HEIGHT);
    }
}

void MyFrame::OnWindowSize(wxSizeEvent& event)
{
    wxSize size = GetClientSize();
    upperScroll->SetSize(wxSize(size.GetX(), 15));
    SetUpperScroll();

    Refresh();
    Update();
}

void MyFrame::OnMotion(wxMouseEvent& event) {

    if (event.Dragging()) {

        wxPoint currentPos = event.GetLogicalPosition(_m_clntDC);
        int direction = currentPos.x - previousPos.x;
        previousPos = currentPos;

        DragUpperWindow(currentPos, direction);
    }
}




wxPoint MyFrame::CreateBlockPos(int row, int column)
{
    wxPoint basePos = texts[1]->GetPosition();
    return wxPoint(basePos.x + TEXT_WIDTH + column * TEXTCTRL_WIDTH, basePos.y + row * TEXT_HEIGHT);
}

void MyFrame::SetUpperScroll()
{
    int x = GetClientSize().GetX();
    int virtualSize = 20 + TEXT_WIDTH + blockSize * TEXTCTRL_WIDTH;
    upperScroll->SetScrollbar(upperScroll->GetThumbPosition(), x, virtualSize, x);
    ScrollUpperWindow();
}

void MyFrame::ScrollUpperWindow()
{
    int baseX = 10 - upperScroll->GetThumbPosition();
    for (auto elem : texts)
        elem->SetPosition(wxPoint(baseX, elem->GetPosition().y));
    textctrlTQ->SetPosition(wxPoint(baseX + TEXT_WIDTH, textctrlTQ->GetPosition().y));
    for (int i = 0; i != textctrls.size(); i++)
        textctrls[i]->SetPosition(wxPoint(baseX + TEXT_WIDTH + (i / 4) * TEXTCTRL_WIDTH,
            textctrls[i]->GetPosition().y));

    Refresh();
    Update();
}

void MyFrame::DragUpperWindow(const wxPoint& currentPos, int direction)
{
    wxPoint tqPos = texts[0]->GetPosition();
    wxPoint pbPos = texts[1]->GetPosition();

    if (BAR_SIZE < currentPos.y && currentPos.y < upperScroll->GetPosition().y &&
        !IsPosInRange(currentPos, tqPos, tqPos.y + TEXT_HEIGHT, tqPos.x + TEXT_WIDTH + TEXTCTRL_WIDTH) &&
        !IsPosInRange(currentPos, pbPos, pbPos.y + 4 * TEXT_HEIGHT, pbPos.x + TEXT_WIDTH + blockSize * TEXTCTRL_WIDTH)) {

        upperScroll->SetThumbPosition(upperScroll->GetThumbPosition() - direction);
        ScrollUpperWindow();
    }
}




std::unique_ptr<ProcessQueue> MyFrame::MakeProcessQueue()
{
    auto pQ = std::make_unique<ProcessQueue>();
    pidList.clear();
 
    for (auto i = 0; i + 3 < 4 * blockSize; i = i + 4) {
        std::string tempPid = textctrls[i]->GetValue().ToStdString();
        double tempArrivaltime;
        textctrls[i + 1]->GetValue().ToDouble(&tempArrivaltime);
        double tempBursttime;
        textctrls[i + 2]->GetValue().ToDouble(&tempBursttime);
        unsigned tempPriority;
        textctrls[i + 3]->GetValue().ToUInt(&tempPriority);

        tempArrivaltime = tempArrivaltime < 0 ? 0 : tempArrivaltime;
        textctrls[i + 1]->SetValue(wxString::FromDouble(tempArrivaltime));
        tempBursttime = tempBursttime < 0 ? 0 : tempBursttime;
        textctrls[i + 2]->SetValue(wxString::FromDouble(tempBursttime));

        pQ->Emplace(tempPid, tempArrivaltime, tempBursttime, tempPriority);
        pidList.push_back(tempPid);
    }
    return pQ;
}

bool MyFrame::InitScheduler()
{
    // Set queue and parameter for scheduler
    scheduler.SetProcessQueue(MakeProcessQueue());
    double tq;
    textctrlTQ->GetValue().ToDouble(&tq);

    switch (choiceAlgorithms->GetSelection()) {

    case 0://FCFS
        scheduler.SetAlgorithm(Scheduling::FCFS, false, false);
        break;

    case 1://SJF
        scheduler.SetAlgorithm(Scheduling::SJF, false, false);
        break;

    case 2://SRJF
        scheduler.SetAlgorithm(Scheduling::SJF, true, false);
        break;

    case 3://RR
        if (tq <= 0) {

            wxMessageBox("Time quantum must be more than 0");
            return false;
        }
        scheduler.SetAlgorithm(Scheduling::FCFS, false, true);
        scheduler.SetTimeQuantum(tq);
        break;

    case 4://Non-preemptive Priority
        scheduler.SetAlgorithm(Scheduling::Priority, false, false);
        break;

    case 5://Preemptive Priority
        scheduler.SetAlgorithm(Scheduling::Priority, true, false);
        break;

    case 6://Non-preemptive Priority with RR
        if (tq <= 0) {

            wxMessageBox("Time quantum must be more than 0");
            return false;
        }
        scheduler.SetAlgorithm(Scheduling::Priority, false, true);
        scheduler.SetTimeQuantum(tq);
        break;

    default://Not selected
        break;
    }

    AllocateColor();
    return true;
}

void MyFrame::InitColorTable()
{
    for (int i = 0; i != TABLE_NUM; i++) {

        for (int j = 0; j != TABLE_NUM; j++) {
            
            for (int k = 0; k != TABLE_NUM; k++) {
                
                colorTable[i * TABLE_NUM * TABLE_NUM + j * TABLE_NUM + k][0] = CODE_BASE + i; // R
                colorTable[i * TABLE_NUM * TABLE_NUM + j * TABLE_NUM + k][1] = CODE_BASE + j; // G
                colorTable[i * TABLE_NUM * TABLE_NUM + j * TABLE_NUM + k][2] = CODE_BASE + k; // B
            }
        }
    }
}

void MyFrame::AllocateColor()
{
    colorList.clear();

    // generate random index of colorTable
    int idx[TABLE_NUM * TABLE_NUM * TABLE_NUM - 1];
    for (int i = 0; i != TABLE_NUM * TABLE_NUM * TABLE_NUM - 1; i++)
        idx[i] = i;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(idx, idx + TABLE_NUM * TABLE_NUM * TABLE_NUM - 1, g);

    // Allocate color
    char str[] = "#FFFFFF";
    for (int i = 0; i != blockSize; i++) {

        str[1] = colorTable[idx[i]][0]; // R
        str[3] = colorTable[idx[i]][1]; // G
        str[5] = colorTable[idx[i]][2]; // B

        colorList.emplace(pidList[i], str);
    }
    // if IDLE, draw white
    colorList.emplace("", "#FFFFFF");
}

void MyFrame::SetChartArea()
{
    chartX.clear();
    chartWidth.clear();
    timeX.clear();
    pidX.clear();
    int prevX = 0;

    // Set Gantt chart area
    for (auto& elem : scheduler.GetGantthandler()) {

        double length = elem.second * (double)UNIT_CHART;
        int newX = (int)(length + 0.5);
        int width = newX - prevX;

        chartX.push_back(prevX);
        chartWidth.push_back(width);
        timeX.push_back(newX - GetTextExtent(wxString::FromDouble(elem.second, 1)).GetWidth() / 2);
        
        int pidWidth = GetTextExtent(elem.first != "" ? elem.first : "IDLE").GetWidth();
        int space = (width - pidWidth) / 2;
        pidX.push_back(prevX + (space >= 1 ? space : 1));

        prevX = newX;
    }
    chartEnd = prevX;
}
