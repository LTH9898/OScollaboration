#include "myframe.h"


MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, _T("Scheduling Simulator")), _m_clntDC(this), blockSize(0)
{
    // MyFrame 초기화
    SetMinSize(wxSize(512, 512));
    SetBackgroundColour(*wxWHITE);
    CreateStatusBar();
    wxInitAllImageHandlers();


    // 상단 메뉴바
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Open, _T("&열기\tCtrl-O"));
    menuFile->Append(ID_Save, _T("&저장\tCtrl-S"));
    menuFile->Append(ID_SaveAs, _T("&다른 이름으로 저장\tCtrl-Shift-S"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _T("&파일"));
    SetMenuBar(menuBar);


    // 상단 window
    // 프로세스 관리 버튼
    wxSize btnSize = wxSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new wxButton(this, BUTTON_CREATE, _T("Create"), wxPoint(20, 5), btnSize);
    new wxButton(this, BUTTON_DELETE, _T("Delete"), wxPoint(30 + BUTTON_WIDTH, 5), btnSize);
    new wxButton(this, BUTTON_CLEAR, _T("Clear"), wxPoint(40 + 2 * BUTTON_WIDTH, 5), btnSize);


    // 프로세스 입력 창
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


    // 하단 window
    lowerWindowY = upperScroll->GetPosition().y + upperScroll->GetSize().GetHeight();
    // schedular 선택
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

        wxMessageBox(_T("프로세스의 최대 생성 개수를 초과하였습니다"));
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
        if (InitializeScheduler())
            scheduler.StepForward();

    while (scheduler.IsRunning())
        scheduler.StepForward();
}

void MyFrame::StepScheduler(wxCommandEvent& event)
{
    if (!scheduler.IsRunning())
        if (!InitializeScheduler())
            return;
    scheduler.StepForward();
}




void MyFrame::OnPaint(wxPaintEvent& event) // 위아래 나누는 bar그리는 method , wxpaitDC의 경우 OnPaint안에서만 수행 가능
{
    wxSize size = GetClientSize();
    wxPaintDC dc(this);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxColour("#A4A4A4"));
    dc.DrawRectangle(wxRect(0, 0, size.GetX(), BAR_SIZE));
    dc.DrawRectangle(wxRect(0, lowerWindowY, size.GetX(), BAR_SIZE));
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
    auto pQ = CreateProcessQueue();
 
    for (auto i = 0; i + 3 < blockSize; i = i + 4) {
        std::string tempPid = textctrls[i]->GetValue().ToStdString();
        double tempArrivaltime;
        textctrls[i + 1]->GetValue().ToDouble(&tempArrivaltime);
        double tempBursttime;
        textctrls[i + 2]->GetValue().ToDouble(&tempBursttime);
        unsigned tempPriority;
        textctrls[i + 3]->GetValue().ToUInt(&tempPriority);

        tempArrivaltime = 0 ? tempArrivaltime : tempArrivaltime < 0;
        tempBursttime = 0 ? tempBursttime : tempBursttime < 0;

        pQ->emplace(tempPid, tempArrivaltime, tempBursttime, tempPriority);
    }
    return pQ;
}

bool MyFrame::InitializeScheduler()
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

    return true;
}
