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
    EVT_BUTTON(idStartShowOn_Graphic, serialGUIFrame::evtStartSampling)
    EVT_BUTTON(idStopShowOn_Graphic , serialGUIFrame::evtStopSampling)
    EVT_BUTTON(idClearGraph,    serialGUIFrame::evtClearGraph)
    EVT_BUTTON(idClearTextCtrl, serialGUIFrame::evtClearText)
    EVT_BUTTON(idSendData,      serialGUIFrame::evtSending)
    EVT_CHECKBOX(idISRecieve,   serialGUIFrame::evtFlagRecieve)
    EVT_CHECKBOX(idSendHex,     serialGUIFrame::evtSendHex)
    EVT_TIMER (idTimer,         serialGUIFrame::evtSampling)
END_EVENT_TABLE()

serialGUIFrame::serialGUIFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(800, 550),
      wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER ),
      IO_svr(), IOdata(IO_svr), buf(512),
      flagRecieve(false), flagShowOnGrapgic(false), isSendHex(false)
{
    init_elements();
    init_graphic();
    bind_boxsizer();
    modeIdle();
    IO_svr.run(); // 这里要激活一下service，否则第一次执行异步操作会阻塞
    return;
}

void serialGUIFrame::bind_boxsizer()
{
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
            tain3_buttons_leftof_graph = new wxBoxSizer(wxVERTICAL);
                tain4_chkbox = new wxBoxSizer(wxVERTICAL);
    top_panel -> SetSizer(sp0_ctrl_text_area);
    sp0_ctrl_text_area -> Add(tain1_ctrl, 1, defStyle, 10);        // 左侧元件容器
        tain1_ctrl -> Add(configBox, 1, defStyle, 0);     // 选项元件
        tain1_ctrl -> Add(tain2_2buttons, 1, defStyle, 0);
        tain2_2buttons -> Add(Open_serial_port, 1, wxALL, 5);          // 打开串口的按钮
        tain2_2buttons -> Add(Clear_recieve, 1, wxALL, 5);             // 清空右侧文本框的内容
        tain1_ctrl -> Add(Send_Message, 8, defStyle, 5);
        tain1_ctrl -> Add(tain2_recieve_send_button, 1, defStyle, 0); // 控制收发的一个勾选框和按钮
            tain2_recieve_send_button -> Add(tain4_chkbox, 1, defStyle, 5);
                tain4_chkbox->Add(is_Recieve_data, 1, defStyle, 0);
                tain4_chkbox->Add(send_hex, 1, defStyle, 0);
            tain2_recieve_send_button -> Add(Send_data_now, 1, defStyle, 5);

    sp0_ctrl_text_area -> Add(tain1_text_graph, 4, defStyle, 10);  // 右侧元件容器
        tain1_text_graph -> Add(Recieve_txtbox, 1, defStyle, 5);   // 保存接收到的内容
        tain1_text_graph -> Add(sp2_button_graph, 1, defStyle, 0);  // 按钮集合
            sp2_button_graph -> Add(tain3_buttons_leftof_graph, 1, defStyle, 0);
                tain3_buttons_leftof_graph -> Add(Start_display, 1, defStyle, 5);
                tain3_buttons_leftof_graph -> Add(Stop_display, 1, defStyle, 5);
                tain3_buttons_leftof_graph -> Add(Init_display, 1, defStyle, 5);
            sp2_button_graph -> Add(Graph, 10, defStyle, 5);
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
    configBox = new serialGUIConfigBox(top_panel, enum_ports());

    /* wxButton */
    Open_serial_port= new wxButton(top_panel, idOpenPort, _("Open"));
    Clear_recieve   = new wxButton(top_panel, idClearTextCtrl, _("Clear Recieve"));
    Send_data_now   = new wxButton(top_panel, idSendData, _("Send"));
    Start_display   = new wxButton(top_panel, idStartShowOn_Graphic, _("Start Drawing"));
    Stop_display    = new wxButton(top_panel, idStopShowOn_Graphic, _("Stop Drawing"));
    Init_display    = new wxButton(top_panel, idClearGraph, _("Init"));

    /* wxCheckBox */
    is_Recieve_data = new wxCheckBox(top_panel, idISRecieve, _("Recieve Data"));
    send_hex = new wxCheckBox(top_panel, idSendHex, _("HEX Mode"));

    /* wxTimer */
    sampling_clk = new wxTimer(this, idTimer);

    /* wxTextCtrl */
    Recieve_txtbox = new wxTextCtrl(top_panel, wxID_ANY, wxEmptyString,
            wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_MULTILINE|wxTE_NO_VSCROLL);
    Send_Message = new wxTextPlus(top_panel, wxTE_MULTILINE|wxTE_NO_VSCROLL);
    return;
}

void serialGUIFrame::init_graphic()
{
    auto scaX = new mpScaleX(_("Cases"), mpALIGN_BORDER_LEFT, false);
    auto scaY = new mpScaleY(_("Value"), mpALIGN_BORDER_BOTTOM, false);
    data = new Serial_data();
    data->SetPen(wxPen(wxColor(0, 128, 255), 5));
    data->SetDrawOutsideMargins(false);
    scaX->SetPen(wxPen(wxColor(0,0,0), 2));
    scaY->SetPen(wxPen(wxColor(0,0,0), 2));
    Graph = new mpWindow(top_panel, wxID_ANY);
    Graph->AddLayer( scaX );
    Graph->AddLayer( scaY );
    Graph->AddLayer( data );
    Graph->EnableMousePanZoom(false);
    Graph->Fit(0, 16, -10, 1000);
    scaX->SetLabelFormat(wxString("%.0f"));
    scaY->SetLabelFormat(wxString("%0.f"));
}

/** 以下是事件处理函数 **/
void serialGUIFrame::evtOpenPort(wxCommandEvent& event)
{
    if(IOdata.is_open()){
        asioClose_serial_port();
        modeIdle();
    }
    else{
        sampling_clk->Stop();
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
void serialGUIFrame::evtSending(wxCommandEvent& event)
{
    if(IOdata.is_open()) { // make sure the port has been opened before sending
        if(Send_Message->empty()){
            wxMessageBox(_("No Data"), _("Note"));
            return;
        }
        if(isSendHex){
            auto cbuf = Send_Message->getHex();
            if(cbuf.size() > 0){
                IOdata.async_write_some( boost::asio::buffer( cbuf, cbuf.size() ),
                    boost::bind(&serialGUIFrame::handle_write, this, boost::placeholders::_1, boost::placeholders::_2)
                );
                scnt += cbuf.size();
            }
            else{
                wxMessageBox(_("Syntax Error"), _("Error"), wxICON_ERROR);
                return;
            }
        }
        else{
            auto msg = Send_Message->getString();
            IOdata.async_write_some( boost::asio::buffer( msg, msg.size() ),
                boost::bind(&serialGUIFrame::handle_write, this, boost::placeholders::_1, boost::placeholders::_2)
            );
            scnt += msg.size();
        }
        IO_svr.run();
        update_rs_bytes();
        Send_Message->Clear();
    }
    else{
        wxMessageBox(_("Port is closed"), _("Note"), wxICON_ERROR);
    }
}

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
void serialGUIFrame::evtStartSampling(wxCommandEvent& event){ flagShowOnGrapgic = true; }  // 启动采样
void serialGUIFrame::evtStopSampling(wxCommandEvent& event) { flagShowOnGrapgic = false; } // 停止采样
void serialGUIFrame::evtClearGraph(wxCommandEvent& event)   { data->Clear(); Graph->UpdateAll(); }// 清空
void serialGUIFrame::evtClearText(wxCommandEvent& event)    { Recieve_txtbox->Clear(); }

void serialGUIFrame::modeIdle()
{
    //configBox->Enable();
    is_Recieve_data->Disable();
    send_hex->Disable();
    Send_data_now->Disable();
    Open_serial_port->SetLabel(_("Open"));
    Stb->SetStatusText(_("Idle"), 0);
    update_rs_bytes();
    return;
}

void serialGUIFrame::modeWorking()
{
    //configBox->Disable();
    is_Recieve_data->Enable();
    send_hex->Enable();
    Send_data_now->Enable();
    Open_serial_port->SetLabel(_("Close"));
    Stb->SetStatusText(_("Working"), 0);
    update_rs_bytes();
    return;
}

void serialGUIFrame::update_display_range()
{
    int cnt = data->getSize();
    Graph->Fit(std::max(0,cnt-16), cnt, 0, std::max(data->getMaxValue()+64,255.0));
}

serialGUIFrame::~serialGUIFrame()
{
}

void serialGUIFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}


