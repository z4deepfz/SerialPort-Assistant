﻿#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "serialGUIMain.h"
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

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
    init_choice_boxes();
    bind_boxsizer();
    modeIdle();
    IO_svr.run(); // 这里要激活一下service，否则第一次执行异步操作会阻塞
    return;
}

void serialGUIFrame::init_choice_boxes()
{
    /* wxChoice (with StaticText) */
    choices[0] = enum_ports();
    for(int i=0; i<5; ++i){
        selection[i].first = new wxStaticText(top_panel, wxID_ANY, sel_label[i]);
        selection[i].second = new wxChoice(
            top_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices[i].size(), choices[i].data()
        );
        selection[i].second->SetSelection(0);
    }
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
        tain1_ctrl -> Add(tain2_choice_desc, 1, defStyle, 1);     // 选项元件
            tain2_choice_desc->Add(tain3_text, 1, defStyle, 0);
            tain2_choice_desc->Add(tain3_choice, 1, defStyle, 0);
                for(int i=0; i<5; ++i){
                    tain3_text->Add(selection[i].first, 1, defStyle, 5);
                    tain3_choice->Add(selection[i].second, 1, defStyle, 5);
                }
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

    /* wxButton */
    Open_serial_port= new wxButton(top_panel, idOpenPort, wxT("打开串口"));
    Clear_recieve   = new wxButton(top_panel, idClearTextCtrl, wxT("清空接收"));
    Send_data_now   = new wxButton(top_panel, idSendData, wxT("发送"));
    Start_display   = new wxButton(top_panel, idStartShowOn_Graphic, wxT("开始显示"));
    Stop_display    = new wxButton(top_panel, idStopShowOn_Graphic, wxT("停止显示"));
    Init_display    = new wxButton(top_panel, idClearGraph, wxT("初始化"));

    /* wxCheckBox */
    is_Recieve_data = new wxCheckBox(top_panel, idISRecieve, wxT("接受数据"));
    send_hex = new wxCheckBox(top_panel, idSendHex, wxT("HEX模式"));

    /* wxTimer */
    sampling_clk = new wxTimer(this, idTimer);

    /* wxTextCtrl */
    Recieve_txtbox = new wxTextCtrl(top_panel, wxID_ANY, wxEmptyString,
            wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_MULTILINE|wxTE_NO_VSCROLL);
    Send_Message = new wxTextCtrl(top_panel, wxID_ANY, wxEmptyString,
            wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_NO_VSCROLL);
    return;
}

void serialGUIFrame::init_graphic()
{
    auto scaX = new mpScaleX(wxT("Cases"), mpALIGN_BORDER_LEFT, false);
    auto scaY = new mpScaleY(wxT("Value"), mpALIGN_BORDER_BOTTOM, false);
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
        auto gcid = [&](int id){return selection[id].second->GetSelection();};
        auto tport = choices[0][gcid(0)].c_str(); // 串口地址，传递char指针
        auto tbaud = serialGUIFrame::rbaud[gcid(1)]; // 波特率，转换成数字即可
        auto tlen  = rlen[gcid(2)] ;  // 字长，下标+5即可
        auto tpari = gcid(3);   // 校验位，下标已经对应选项，无需处理
        auto tsbit = rstop[gcid(4)];   // 停止位，转换成浮点数后乘以10变成整数即可
        try{
            asioOpen_serial_port(tport, tbaud, tlen, tpari, tsbit);
        }
        catch (std::exception &e){
            wxMessageBox(e.what(), wxT("串口打开失败"));
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
        auto msg = Send_Message->GetValue();
        Send_Message->Clear();
        if(msg.size() == 0){
            wxMessageBox(wxT("没有数据"), wxT("提示"));
            return;
        }
        if(isSendHex){
            auto cbuf = wxstr2hex(msg);
            if(cbuf.size() > 0){
                IOdata.async_write_some( boost::asio::buffer( cbuf, cbuf.size() ),
                    boost::bind(&serialGUIFrame::handle_write, this, boost::placeholders::_1, boost::placeholders::_2)
                );
                scnt += cbuf.size();
            }
            else{
                wxMessageBox(wxT("格式错误"), wxT("错误"), wxICON_ERROR);
            }
        }
        else{
            IOdata.async_write_some( boost::asio::buffer( msg, msg.size() ),
                boost::bind(&serialGUIFrame::handle_write, this, boost::placeholders::_1, boost::placeholders::_2)
            );
            scnt += msg.size();
        }
        IO_svr.run();
        update_rs_bytes();
    }
    else{
        wxMessageBox(wxT("端口尚未打开"), wxT("提示"), wxICON_ERROR);
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
    for(int i=0; i<5; ++i){
        selection[i].second->Enable();
    }
    is_Recieve_data->Disable();
    send_hex->Disable();
    Send_data_now->Disable();
    Open_serial_port->SetLabel(wxT("打开串口"));
    Stb->SetStatusText(_("Idle"), 0);
    update_rs_bytes();
    return;
}

void serialGUIFrame::modeWorking()
{
    for(int i=0; i<5; ++i){
        selection[i].second->Disable();
    }
    is_Recieve_data->Enable();
    send_hex->Enable();
    Send_data_now->Enable();
    Open_serial_port->SetLabel(wxT("关闭串口"));
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

void serialGUIFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void serialGUIFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}


