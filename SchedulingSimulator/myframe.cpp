#include "myframe.h"


MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, _T("Scheduling Simulator")), _m_clntDC(this), blockSize(0)
{
    // MyFrame 초기화
    SetMinSize(wxSize(512, 512));
    SetBackgroundColour(*wxWHITE);
    CreateStatusBar();


    // 상단 메뉴바
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Open, _T("&열기\tCtrl-O"));
    menuFile->Append(ID_Save, _T("&저장\tCtrl-S"));
    menuFile->Append(ID_SaveAs, _T("&다른 이름으로 저장\tCtrl-Shift-S"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&파일");
    SetMenuBar(menuBar);


    // 상단 window
    // 프로세스 관리 버튼
    wxSize btnSize = wxSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    new wxButton(this, BUTTON_CREATE, _T("Create"), wxPoint(20, 5), btnSize);
    new wxButton(this, BUTTON_DELETE, _T("Delete"), wxPoint(30 + BUTTON_WIDTH, 5), btnSize);

    new wxButton(this, BUTTON_CLEAR, _T("Clear"), wxPoint(40 + 2 * BUTTON_WIDTH, 5), btnSize);
    new wxButton(this, BUTTON_CONFIRM, _T("Confirm"), wxPoint(50 + 3 * BUTTON_WIDTH, 5), btnSize);


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
    new wxButton(this, BUTTON_RUN, _T("Run"), wxPoint(200, lowerWindowY + 6), btnSize);
    new wxButton(this, BUTTON_STEPRUN, _T("StepRun"), wxPoint(200 + BUTTON_WIDTH + 10, lowerWindowY + 6), btnSize);

    new wxButton(this, BUTTON_GANTTCLEAR, _T("GanttClear"), wxPoint(200 + (BUTTON_WIDTH + 10) * 2, lowerWindowY + 6), btnSize);

    // Event
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, ID_Open);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, ID_Save);
    Bind(wxEVT_MENU, &MyFrame::OnSaveAs, this, ID_SaveAs);

    Bind(wxEVT_BUTTON, &MyFrame::CreateProcessBlock, this, BUTTON_CREATE);
    Bind(wxEVT_BUTTON, &MyFrame::DeleteProcessBlock, this, BUTTON_DELETE);
    Bind(wxEVT_BUTTON, &MyFrame::ClearProcessBlock, this, BUTTON_CLEAR);

    Bind(wxEVT_SCROLL_THUMBTRACK, &MyFrame::OnUpperScroll, this, SCROLL_UPPER);
    Bind(wxEVT_SCROLL_PAGEUP, &MyFrame::OnUpperScroll, this, SCROLL_UPPER);
    Bind(wxEVT_SCROLL_PAGEDOWN, &MyFrame::OnUpperScroll, this, SCROLL_UPPER);
    


    Bind(wxEVT_BUTTON, &MyFrame::CreateGanttChart, this, BUTTON_RUN);

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

        wxMessageBox("프로세스의 최대 생성 개수를 초과하였습니다");
        return;
    }

    wxSize ctrlSize = wxSize(TEXTCTRL_WIDTH, TEXT_HEIGHT); 
    //auto CreatePID = [](int blockSize) { return "P" + std::to_string(blockSize); };

    textctrls.emplace_back(new wxTextCtrl(this, wxID_ANY, "P" + std::to_string(blockSize),
        CreateBlockPos(0, blockSize), ctrlSize, wxBORDER_SIMPLE));
    for (int i = 1; i != 4; i++)
        textctrls.emplace_back(new wxTextCtrl(this, wxID_ANY, "0",
            CreateBlockPos(i, blockSize), ctrlSize, wxBORDER_SIMPLE));
    ++blockSize;

    SetUpperScroll();
}



void MyFrame::ConFirmProcessBlock(wxCommandEvent& event) // Process 정의 후에 이제 Confirm 하면 넘어가는 것.
{


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


void MyFrame::CreateGanttChart(wxCommandEvent& event)
{
    /*
    wxPaintDC dc(this);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxColour("#e31919"));
    dc.DrawRectangle(wxRect(0, lowerWindowY, 10, 10));*/
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

        SetStatusText(std::to_string(direction));

        upperScroll->SetThumbPosition(upperScroll->GetThumbPosition() - direction);
        ScrollUpperWindow();
    }
}


void MyFrame::GetSelectedAlgorithm() {
    int idx = choiceAlgorithms->GetSelection();
    
    
    //MyFrame ���� CpuSchedular�� ��ü
    CpuScheduler cs;

    
    switch (idx) 
    {
    case 0://FCFS
        cs.SetAlgorithm(Scheduling::FCFS, false, false);
        break;
    case 1://SJF
        cs.SetAlgorithm(Scheduling::SJF, false, false);
        break;
    case 2://SRJF
        cs.SetAlgorithm(Scheduling::FCFS, true, false);
        break;
    case 3://RR
        cs.SetAlgorithm(Scheduling::FCFS, false, true);
        break;
    case 4://Non-preemptive Priority
        cs.SetAlgorithm(Scheduling::Priority, false, false);
        break;
    case 5://Preemptive Priority
        cs.SetAlgorithm(Scheduling::Priority, true, false);
        break;
    case 6://Non-preemptive Priority with RR
        cs.SetAlgorithm(Scheduling::Priority, false, true);
        break;
    default://Not selected
        break;
            //����ó��
    }

}



std::unique_ptr<ProcessQueue> MyFrame::MakeProcessQueue()
{
    std::unique_ptr<ProcessQueue> pQ = CreateProcessQueue();

    for (auto i = 0; i + 3 < blockSize; i = i + 4) {
        std::string tempPid = textctrls[i]->GetValue().ToStdString();
        double tempArrivaltime;
        textctrls[i + 1]->GetValue().ToDouble(&tempArrivaltime);
        double tempBursttime;
        textctrls[i + 2]->GetValue().ToDouble(&tempBursttime);
        unsigned tempPriority;
        textctrls[i + 3]->GetValue().ToUInt(&tempPriority);
        pQ.get()->push(Process(tempPid, tempArrivaltime, tempBursttime, tempPriority));
    }
    return std::move(pQ);
}

