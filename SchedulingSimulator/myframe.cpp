﻿#include "myframe.h"
#include "set"
#include "algorithm"

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, _T("Scheduling Simulator")), _m_clntDC(this), blockSize(0), lowerWindowX(20), wqX(0), currentFilePath("")
{
    // Initialize MyFrame
    SetMinSize(wxSize(512, 560));
    SetBackgroundColour(*wxWHITE);
    CreateStatusBar();
    wxInitAllImageHandlers();
    InitColorTable();


    // Menu bar
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_New, _T("&New\tCtrl-N"));
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


    // schedular 
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


    // File events
    Bind(wxEVT_MENU, &MyFrame::OnNew, this, ID_New);
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
        "Test case (*.tc)|*.tc", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::ifstream ifs(openFileDialog.GetPath().utf8_string());
    if (!ifs.is_open()) {
    
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

    // read string from ifstream
    auto read = [&](std::ifstream& ifs) {

        std::string input;
        ifs >> input;
        input.pop_back();
        return input;
    };

    // Clear textctrls
    _ClearProcessBlock();
    // Time quantum
    textctrlTQ->SetValue(read(ifs));
    // Number of process
    blockSize = std::stoul(read(ifs));

    // Create test case
    for (int i = 0; i != blockSize; i++) {

        wxSize ctrlSize = wxSize(TEXTCTRL_WIDTH, TEXT_HEIGHT);
        for (int j = 0; j != 4; j++) {

            textctrls.emplace_back(new wxTextCtrl(this, wxID_ANY, read(ifs),
                CreateBlockPos(j, i), ctrlSize, wxBORDER_SIMPLE));
        }
    }
    
    currentFilePath = openFileDialog.GetPath();
    ifs.close();
    SetUpperScroll();
    Refresh();
    Update();
}

void MyFrame::OnSave(wxCommandEvent& event)
{
    // If currentFilePath is empty, save as
    if (currentFilePath == "") {

        _OnSaveAs();
        return;
    }

    std::ofstream ofs(currentFilePath.utf8_string());
    if (!ofs.is_open()) {

        wxLogError("Cannot save file '%s'.", currentFilePath);
        return;
    }


    ofs << textctrlTQ->GetValue().utf8_string() + ";" << std::endl;
    ofs << blockSize << ";" << std::endl;
    for (auto elem : textctrls)
        ofs << elem->GetValue().utf8_string() + ";" << std::endl;

    ofs.close();
}

void MyFrame::_OnSaveAs()
{
    wxFileDialog saveFileDialog(this, _("Save Test case file"), "", "",
        "Test case (*.tc)|*.tc", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::ofstream ofs(saveFileDialog.GetPath().utf8_string());
    if (!ofs.is_open()) {

        wxLogError("Cannot save file '%s'.", saveFileDialog.GetPath());
        return;
    }


    ofs << textctrlTQ->GetValue().utf8_string() + ";" << std::endl;
    ofs << blockSize << ";" << std::endl;
    for (auto elem : textctrls)
        ofs << elem->GetValue().utf8_string() + ";" << std::endl;

    currentFilePath = saveFileDialog.GetPath();
    ofs.close();
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
    Refresh();
    Update();
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
    Refresh();
    Update();
}

void MyFrame::_ClearProcessBlock()
{
    for (auto elem : textctrls)
        delete elem;
    textctrls.clear();
    blockSize = 0;
    textctrlTQ->SetValue("");

    SetUpperScroll();
    Refresh();
    Update();
}




void MyFrame::RunScheduler(wxCommandEvent& event)
{
    if (!scheduler.IsRunning())
        if (InitScheduler())
            scheduler.StepForward();

    while (scheduler.IsRunning())
        scheduler.StepForward();

    SetChartArea();
    SetBaseX(lowerWindowX, chartEnd);
    SetBaseX(wqX, wqEnd);
    Refresh();
    Update();

    ShowResult();
}   

void MyFrame::StepScheduler(wxCommandEvent& event)
{
    if (!scheduler.IsRunning())
        if (!InitScheduler())
            return;

    scheduler.StepForward();

    SetChartArea();
    lowerWindowX += GetClientSize().GetWidth() - chartEnd - 40;
    SetBaseX(lowerWindowX, chartEnd);
    SetBaseX(wqX, wqEnd);
    Refresh();
    Update();

    if (!scheduler.IsRunning())
        ShowResult();
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
    auto& gantt = scheduler.GetGantthandler();
    if (!gantt.empty()) {

        dc.DrawText("Gantt chart", 20, lowerWindowY + 50);
        int i = 0;
        int x = lowerWindowX;
        int y = lowerWindowY + 70;
        dc.SetPen(*wxBLACK_PEN);

        for (auto& elem : gantt) {

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
    SetBaseX(lowerWindowX, chartEnd);
    SetBaseX(wqX, wqEnd);

    Refresh();
    Update();
}

void MyFrame::OnMotion(wxMouseEvent& event) {

    if (event.Dragging()) {

        wxPoint currentPos = event.GetLogicalPosition(_m_clntDC);
        int direction = currentPos.x - previousPos.x;
        previousPos = currentPos;

        // Gantt chart dragging event
        if (lowerWindowY + 35 < currentPos.y && currentPos.y < wqY) {

            lowerWindowX += direction;
            SetBaseX(lowerWindowX, chartEnd);
            Refresh();
            Update();
        }
        else if (wqY < currentPos.y) {

            wqX += direction;
            SetBaseX(wqX, wqEnd);
            Refresh();
            Update();
        }
        else
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

        Refresh();
        Update();
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


        arrivalList.push_back(make_pair(tempPid, tempArrivaltime));
        burstTimeList.push_back(make_pair(tempPid, tempBursttime));
    }
    return pQ;
}

bool MyFrame::InitScheduler()
{
    // Set queue and parameter for scheduler
    scheduler.SetProcessQueue(MakeProcessQueue());

    // Check duplicate PID
    for (int i = 0; i < pidList.size(); i++) {

        if (pidList[i] == "") {

            wxMessageBox("No empty Process ID allowed", "Test case error", wxICON_INFORMATION);
            return false;
        }
        for (int j = i + 1; j < pidList.size(); j++) {

            if (pidList[i] == pidList[j]) {

                wxMessageBox("No duplicate Process ID allowed", "Test case error", wxICON_INFORMATION);
                return false;
            }
        }
    }

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

            wxMessageBox("Time quantum must be more than 0", "Test case error", wxICON_INFORMATION);
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

            wxMessageBox("Time quantum must be more than 0", "Test case error", wxICON_INFORMATION);
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

void MyFrame::SetBaseX(int& baseX, int end)
{
    int width = GetClientSize().GetWidth();
    if (end + 40 <= width)
        baseX = 20;
    else {

        if (baseX > 20)
            baseX = 20;
        else if (baseX < width - end - 20)
            baseX = width - end - 20;
    }
}




////////////////////////////////////////////////////////////////////////
void MyFrame::ShowResult()
{

    wxSize textSize = wxSize(TEXT_WIDTH, TEXT_HEIGHT);
    long style = wxALIGN_RIGHT | wxBORDER_SIMPLE;
    wxDialog* dialog = new wxDialog;
    dialog->Create(NULL, wxID_ANY,
        "Result",
        wxDefaultPosition,
        wxSize(700, 500),
        wxDEFAULT_DIALOG_STYLE,
        wxASCII_STR(wxDialogNameStr));

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxGrid* grid = new wxGrid(dialog, -1, wxPoint(0, 0), wxSize(500, 300));

    grid->CreateGrid(pidList.size() + 1, 3);

    grid->SetColLabelValue(0, "Waiting Time");
    grid->SetColLabelValue(1, "Response Time");
    grid->SetColLabelValue(2, "Turnaround Time");
    //Average Row name Change
    grid->SetRowLabelValue(pidList.size(), "Average");
    grid->SetRowSize(pidList.size(), 40);
    
    GanttChart TAList = GetWaitingTime();
    GanttChart::const_iterator it;
    int i;
    for (i=0, it = TAList.begin(); it!=TAList.end(); i++, it++)
    {
        grid->SetRowLabelValue(i, (*it).first + " : " + std::to_string((*it).second));
        grid->SetRowSize(i, 40);
    }

    grid->SetCellValue(0, 0, std::to_string(timeX[0]));

    for (int i = 0; i < 4; i++)
    {
        grid->SetColSize(i, 200);
    }

    //grid->SetCellValue(0, 0, "wxGrid is good");
    //grid->SetCellValue(0, 3, "This is read->only");
    //grid->SetReadOnly(0, 3);
    //// Colours can be specified for grid cell contents
    //grid->SetCellValue(3, 3, "green on grey");
    //grid->SetCellTextColour(3, 3, *wxGREEN);
    //grid->SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);
    //// We can specify the some cells will store numeric
    //// values rather than strings. Here we set grid column 5
    //// to hold floating point values displayed with width of 6
    //// and precision of 2
    //grid->SetColFormatFloat(5, 6, 2);
    //grid->SetCellValue(0, 6, "3.1415");

    SetSizer(mainSizer);
    SetMinSize(wxSize(700, 100));

    dialog->ShowModal();
}


GanttChart MyFrame::GetTurnAroundTime() const
{
    GanttChart TAList;
    GanttChart ganttChart = scheduler.GetGantthandler();
    std::set<std::string> s;
    GanttChart::const_reverse_iterator rit;
    int cnt;
    for (rit = ganttChart.rbegin(), cnt = 0; rit != ganttChart.rend(); rit++, cnt< pidList.size()) {
        if (s.find(rit->first) == s.end()) {
            s.insert(rit->first);

            double temp = rit->second;
            for (int i = 0; i < arrivalList.size(); i++) {
                if (arrivalList[i].first == rit->first) {
                    temp -= arrivalList[i].second;
                    break;
                }
            }
            TAList.push_back(make_pair(rit->first, temp));
            cnt++;
        }
    }

    return TAList;
}




GanttChart  MyFrame::GetWaitingTime() const
{

    GanttChart WTList;
    GanttChart ganttChart = scheduler.GetGantthandler();
    std::set<std::string> s;
    GanttChart::const_reverse_iterator rit;
    int cnt;
    for (rit = ganttChart.rbegin(), cnt = 0; rit != ganttChart.rend(); rit++, cnt < pidList.size()) {
        if (s.find(rit->first) == s.end()) {
            s.insert(rit->first);

            double temp = rit->second;
            for (int i = 0; i < burstTimeList.size(); i++) {
                if (burstTimeList[i].first == rit->first) {
                    temp -= burstTimeList[i].second;
                    temp -= arrivalList[i].second; // only if arrivalList and burstTime list sorted in same order!!!!!
                    break;
                }
            }

            WTList.push_back(make_pair(rit->first, temp));
            cnt++;
        }
    }

    return WTList;
}


GanttChart  MyFrame::GetResponseTime() const
{

    GanttChart RTList;
    GanttChart ganttChart = scheduler.GetGantthandler();
    std::set<std::string> s;
    GanttChart::const_iterator it;
    int cnt;
    for (it = ganttChart.begin(), cnt = 0; it != ganttChart.end(); it++, cnt < pidList.size()) {
        if (s.find(it->first) == s.end()) {
            s.insert(it->first);

            double temp = it->second;
            if (it != ganttChart.begin()) {
                temp = (--it)->second;
                ++it;
            }
            for (int i = 0; i < arrivalList.size(); i++) {
                if (arrivalList[i].first == it->first) {
                    temp -= arrivalList[i].second;
                    break;
                }
            }
            RTList.push_back(make_pair(it->first, temp));
            cnt++;
        }
    }

    return RTList;
}

////////////////////////////////////////////////////////////////////////////
