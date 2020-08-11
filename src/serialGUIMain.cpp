#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "serialGUIMain.h"

BEGIN_EVENT_TABLE(serialGUIFrame, wxFrame)
    EVT_CLOSE (serialGUIFrame::OnClose)
    EVT_BUTTON(idOpenPort, serialGUIFrame::evtOpenPort)
    //EVT_BUTTON(idStartShowOn_Graphic, serialGUIFrame::evtStartSampling)
    //EVT_BUTTON(idStopShowOn_Graphic , serialGUIFrame::evtStopSampling)
    //EVT_BUTTON(idClearGraph,    serialGUIFrame::evtClearGraph)
    EVT_BUTTON(idClearTextCtrl, serialGUIFrame::evtClearText)
    EVT_BUTTON(idSendData,      serialGUIFrame::evtSending)
    EVT_CHECKBOX(idISRecieve,   serialGUIFrame::evtFlagRecieve)
    EVT_CHECKBOX(idSendHex,     serialGUIFrame::evtSendHex)
    //EVT_CHECKBOX(idLoopSend,    serialGUIFrame::evtLoopSend)
    EVT_TIMER (idTimer,         serialGUIFrame::evtSampling)
    EVT_TIMER(idLoopClk,        serialGUIFrame::evtLoopClk)
    EVT_MENU(menuOpenFile,      serialGUIFrame::evtMenuOpenFile)
END_EVENT_TABLE()


serialGUIFrame::serialGUIFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(800, 600),
      wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER ),
      IO_svr(), IOdata(IO_svr), buf(64), scnt(0), rcnt(0),
      flagRecieve(false), isSendHex(false)
{
    init_elements();
    bind_boxsizer();
    modeIdle();
    IO_svr.run(); // 这里要激活一下service，否则第一次执行异步操作会阻塞
    top_panel->Layout();
    return;
}

void serialGUIFrame::bind_boxsizer()
{
    /**
     * ## some complaination ##
     * I should use wxSmith at the beginning...
     * but it's my final assignment before I opensource it. I'd like to make it more like handmake
     * and now I found it tough to make the interface looks well with such mess code...
     * I tried my best today to fix the size/position/style between checkbox/button/spinctrl
     * but I think I failed.
     * maybe I'll rebuild here in the future, or never. And now I'm really sleepy...
     * I rather than coding for another proj `ncmdump-GUI` (lol
    **/
    constexpr auto defStyle = wxEXPAND | wxALL;
    /* wxBoxSizer */
    sp0_ctrl_text_area  = new wxBoxSizer(wxHORIZONTAL);
    tain1_ctrl = new wxBoxSizer(wxVERTICAL);
        tain2_2buttons      = new wxBoxSizer(wxHORIZONTAL);
        tain2_choice_desc   = new wxBoxSizer(wxHORIZONTAL);
            tain3_text      = new wxBoxSizer(wxVERTICAL);
            tain3_choice    = new wxBoxSizer(wxVERTICAL);
        tain2_recieve_send_button = new wxBoxSizer(wxHORIZONTAL);
    tain1_text_graph    = new wxBoxSizer(wxVERTICAL);
        sp2_button_graph = new wxBoxSizer(wxHORIZONTAL);
            //tain3_buttons_leftof_graph = new wxBoxSizer(wxVERTICAL);
            tain4_btn_and_loop = new wxBoxSizer(wxVERTICAL);
                tain4_chkbox = new wxBoxSizer(wxVERTICAL);
                    tain5_delay = new wxBoxSizer(wxHORIZONTAL);
    top_panel -> SetSizer(sp0_ctrl_text_area);
    sp0_ctrl_text_area -> Add(tain1_ctrl, 1, defStyle, 10);        // 左侧元件容器
        tain1_ctrl -> Add(configBox, 1, defStyle, 0);     // 选项元件
        tain1_ctrl -> Add(tain2_2buttons, 1, defStyle, 0);
        tain2_2buttons -> Add(Open_serial_port, 1, wxALL, 5);          // 打开串口的按钮
        tain2_2buttons -> Add(Clear_recieve, 1, wxALL, 5);             // 清空右侧文本框的内容
        tain1_ctrl -> Add(Send_Message, 8, defStyle, 5);
        tain1_ctrl -> Add(tain2_recieve_send_button, 1, defStyle, 0); // 控制收发的一个勾选框和按钮
            tain2_recieve_send_button -> Add(tain4_chkbox, 1, defStyle, 0);
                tain4_chkbox->Add(is_Recieve_data, 1, defStyle, 5);
                tain4_chkbox->Add(send_hex, 1, defStyle, 5);
                //tain4_chkbox->Add(loop_send, 1, defStyle, 5);
            tain2_recieve_send_button->Add(tain4_btn_and_loop, 1, defStyle, 0);
                tain4_btn_and_loop->Add(tain5_delay, 1, defStyle, 0);
                    //tain5_delay->Add(text_each, 0, wxALL|wxSHAPED|wxALIGN_CENTER, 0);
                    tain5_delay->Add(loop_send, 0, wxALL|wxSHAPED|wxALIGN_CENTER, 0);
                    tain5_delay->Add(SendDelay, 1, wxALL|wxALIGN_CENTER, 0);
                    //tain5_delay->Add(text_ms, 0, wxALL|wxSHAPED|wxALIGN_CENTER, 0);
                tain4_btn_and_loop -> Add(Send_data_now, 1, defStyle, 0);

    sp0_ctrl_text_area -> Add(tain1_text_graph, 4, defStyle, 10);  // 右侧元件容器
        tain1_text_graph -> Add(Recieve_txtbox, 1, defStyle, 5);   // 保存接收到的内容
        tain1_text_graph -> Add((wxPanel*)Graph, 1, defStyle, 0);  // 按钮集合
}

void serialGUIFrame::init_elements()
{
    /* wxStatusBar */
    Stb = new wxStatusBar(this);
    Stb->SetFieldsCount(3);
    SetStatusBar(Stb);

    /* wxPanel */
    top_panel = new wxPanel(this);

    /* config box */
#ifdef DEBUG
    // if debugging, here send an empty COM list to avoid throwing exceptions
    configBox = new serialGUIConfigBox(top_panel, std::vector<wxString>());
#else
    configBox = new serialGUIConfigBox(top_panel, enum_ports());
#endif // DEBUG_NOW
    ElementsEnabledOnlyIFPort_Close.push_back(configBox);

    /* Line Chart */
    Graph = new GUILineChart(top_panel);
    /// generally users needn't adv functions of wxMathPlot. But I cant find a good way to block it.

    /* wxButton */
    Open_serial_port= new wxButton(top_panel, idOpenPort, _("Open"));
    Clear_recieve   = new wxButton(top_panel, idClearTextCtrl, _("Clear Recieve"));
    Send_data_now   = new wxButton(top_panel, idSendData, _("Send"));
    ElementsEnabledOnlyIFPort_Open.push_back(Send_data_now);

    /* wxCheckBox */
    is_Recieve_data = new wxCheckBox(top_panel, idISRecieve, _("Recieve Data"));
    is_Recieve_data->SetValue(flagRecieve=true);
    send_hex = new wxCheckBox(top_panel, idSendHex, _("HEX Mode"));
    loop_send = new wxCheckBox(top_panel, idLoopSend, _("Loop"));
    ElementsEnabledOnlyIFPort_Open.push_back(is_Recieve_data);
    ElementsEnabledOnlyIFPort_Open.push_back(send_hex);
    ElementsEnabledOnlyIFPort_Open.push_back(loop_send);

    /* wxTimer */
    sampling_clk = new wxTimer(this, idTimer);
    loop_clk = new wxTimer(this, idLoopClk);

    /* wxTextCtrl */
    Recieve_txtbox = new wxTextCtrl(top_panel, wxID_ANY, wxEmptyString,
            wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_MULTILINE|wxTE_NO_VSCROLL);
    Send_Message = new wxTextPlus(top_panel, wxTE_MULTILINE|wxTE_NO_VSCROLL);

    /* Menu bar */
    menubar = new wxMenuBar;
        menu_file = new wxMenu;
            menu_file->Append(menuOpenFile, _("&Open\tCtrl-O"));
        menubar->Append(menu_file, _("&File"));
    SetMenuBar(menubar);

    /* Others */
    SendDelay = new wxSpinCtrl(top_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1));
    SendDelay->SetRange(0, 5000);

    return;
}

/** Following are event handling functions **/
void serialGUIFrame::evtOpenPort(wxCommandEvent& event)
{
    sampling_clk->Stop();
    if(IOdata.is_open()){
        asioClose_serial_port();
        modeIdle();
    }
    else{
        auto cfg = configBox->getConfig();
        try{
            asioOpen_serial_port(cfg.COM, cfg.baud, cfg.databits, cfg.parity, cfg.stopbits);
        }
        catch (std::exception &e){
            wxMessageBox(e.what(), _("Failed to open port"));
            return;
        }
        sampling_clk->Start(1000);
        modeWorking();
    }
    return;
}

void serialGUIFrame::evtSampling(wxTimerEvent& event)
{
    if(flagRecieve && IOdata.is_open()) { // make sure the port has been opened before receiving
        IOdata.async_read_some(boost::asio::buffer(buf, buf.size()),
            boost::bind(&serialGUIFrame::handle_read, this, boost::placeholders::_1, boost::placeholders::_2)
            // Warn: if not use `&serialGUIFrame::` here, the compiler will report
            //  `error: invalid use of non-static member function`
        );
        IO_svr.run();
        update_rs_bytes();
    }
    else{
        return;
    }
}  // triggered by Timer
void serialGUIFrame::evtSending(wxCommandEvent& event){ SetLoopFlagBeforeSend(); SendDataNow(); }

void serialGUIFrame::evtFlagRecieve(wxCommandEvent& event)
{
    flagRecieve = is_Recieve_data->GetValue();
    if(flagRecieve){
        sampling_clk->Start(1000);
    }
    else{
        sampling_clk->Stop();
    }
}

void serialGUIFrame::evtSendHex(wxCommandEvent& event) { isSendHex=send_hex->GetValue(); }
void serialGUIFrame::evtClearText(wxCommandEvent& event)    { Recieve_txtbox->Clear(); }
void serialGUIFrame::evtMenuOpenFile(wxCommandEvent& event){ LoadDataFromFile(); }

void serialGUIFrame::modeIdle()
{
    //configBox->Enable();
    //is_Recieve_data->Disable();
    //send_hex->Disable();
    //Send_data_now->Disable();
    for(auto i: ElementsEnabledOnlyIFPort_Close){
        i->Enable();
    }
    for(auto i: ElementsEnabledOnlyIFPort_Open){
        i->Disable();
    }
    Open_serial_port->SetLabel(_("Open"));
    Stb->SetStatusText(_("Idle"), 0);
    update_rs_bytes();
    return;
}

void serialGUIFrame::modeWorking()
{
    //configBox->Disable();
    //is_Recieve_data->Enable();
    //send_hex->Enable();
    //Send_data_now->Enable();
    for(auto i: ElementsEnabledOnlyIFPort_Close){
        i->Disable();
    }
    for(auto i: ElementsEnabledOnlyIFPort_Open){
        i->Enable();
    }
    Open_serial_port->SetLabel(_("Close"));
    Stb->SetStatusText(_("Working"), 0);
    update_rs_bytes();
    return;
}

/*
void serialGUIFrame::evtLoopSend(wxCommandEvent& event)
{
    isLoopSend = loop_send->GetValue();
}
*/

void serialGUIFrame::evtLoopClk(wxTimerEvent& event)
{
    loop_clk->Stop();
    flagLoopReadyToSend = true;
    if( loop_send->GetValue() && flagSent ){
        /// `flagsent=true` means previous sending task has finished before this function called
        /// on such situation, handle function will not call `SendDataNow()`, so call it in this functioin
        SetLoopFlagBeforeSend();
        SendDataNow();
    }
}


serialGUIFrame::~serialGUIFrame() { }
void serialGUIFrame::OnClose(wxCloseEvent &event) { Destroy(); }
