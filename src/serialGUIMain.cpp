#ifdef WX_PRECOMP
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
    EVT_CLOSE (OnClose)
    EVT_BUTTON(idOpenPort, evtOpenPort)
    EVT_BUTTON(idStartShowOn_Graphic, evtStartSampling)
    EVT_BUTTON(idStopShowOn_Graphic , evtStopSampling)
    EVT_BUTTON(idClearGraph,    evtClearGraph)
    EVT_BUTTON(idClearTextCtrl, evtClearText)
    EVT_BUTTON(idSendData,      evtSending)
    EVT_CHECKBOX(idISRecieve,   evtFlagRecieve)
    EVT_CHECKBOX(idSendHex,     evtSendHex)
    EVT_TIMER (idTimer,         evtSampling)
END_EVENT_TABLE()

serialGUIFrame::serialGUIFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxSize(800, 500),
              wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX),
      IO_svr(), IOdata(IO_svr),
      flagRecieve(false), flagShowOnGrapgic(false), isSendHex(false)
{
    /* wxPanel */
    top_panel = new wxPanel(this);
    init_choice_boxes();
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
    Send_Message = new wxTextCtrl(top_panel, wxID_ANY);

    /* wxBoxSizer */
    sp0_ctrl_text_area  = new wxBoxSizer(wxHORIZONTAL);

    tain1_ctrl = new wxBoxSizer(wxVERTICAL);
        tain2_choice_desc   = new wxBoxSizer(wxHORIZONTAL);
            tain3_text      = new wxBoxSizer(wxVERTICAL);
            tain3_choice    = new wxBoxSizer(wxVERTICAL);
        tain2_recieve_send_button = new wxBoxSizer(wxHORIZONTAL);
    tain1_text_graph    = new wxBoxSizer(wxVERTICAL);
        sp2_button_graph = new wxBoxSizer(wxHORIZONTAL);
            tain3_buttons_leftof_graph = new wxBoxSizer(wxVERTICAL);
                tain4_chkbox = new wxBoxSizer(wxVERTICAL);

    /** Graphic **/
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

    bind_boxsizer();

}

void serialGUIFrame::init_choice_boxes()
{
    /* wxChoice (with StaticText) */
    for(int i=0; i<5; ++i){
        selection[i].first = new wxStaticText(top_panel, wxID_ANY, sel_label[i]);
        selection[i].second = new wxChoice(
            top_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices_cnt[i], choices[i]
        );
        selection[i].second->SetSelection(0);
    }
    return;
}

void serialGUIFrame::bind_boxsizer()
{
    constexpr auto defStyle = wxEXPAND | wxALL;
    top_panel -> SetSizer(sp0_ctrl_text_area);

    sp0_ctrl_text_area -> Add(tain1_ctrl, 1, defStyle, 10);        // 左侧元件容器
        tain1_ctrl -> Add(tain2_choice_desc, 1, defStyle, 0);     // 选项元件
            tain2_choice_desc->Add(tain3_text, 1, defStyle, 0);
            tain2_choice_desc->Add(tain3_choice, 1, defStyle, 0);
                for(int i=0; i<5; ++i){
                    tain3_text->Add(selection[i].first, 1, defStyle, 5);
                    tain3_choice->Add(selection[i].second, 1, defStyle, 5);
                }
        tain1_ctrl -> Add(Open_serial_port, 1, defStyle, 5);          // 打开串口的按钮
        tain1_ctrl -> Add(Clear_recieve, 1, defStyle, 5);             // 清空右侧文本框的内容
        tain1_ctrl -> Add(Send_Message, 1, defStyle, 5);
        tain1_ctrl -> Add(tain2_recieve_send_button, 1, defStyle, 0); // 控制收发的一个勾选框和按钮
            tain2_recieve_send_button -> Add(tain4_chkbox, 1, defStyle, 5);
                tain4_chkbox->Add(is_Recieve_data, 1, defStyle, 0);
                tain4_chkbox->Add(send_hex, 1, defStyle, 0);
            tain2_recieve_send_button -> Add(Send_data_now, 1, defStyle, 5);

    sp0_ctrl_text_area -> Add(tain1_text_graph, 3, defStyle, 10);  // 右侧元件容器
        tain1_text_graph -> Add(Recieve_txtbox, 1, defStyle, 5);   // 保存接收到的内容
        tain1_text_graph -> Add(sp2_button_graph, 1, defStyle, 0);  // 按钮集合
            sp2_button_graph -> Add(tain3_buttons_leftof_graph, 1, defStyle, 0);
                tain3_buttons_leftof_graph -> Add(Start_display, 1, defStyle, 5);
                tain3_buttons_leftof_graph -> Add(Stop_display, 1, defStyle, 5);
                tain3_buttons_leftof_graph -> Add(Init_display, 1, defStyle, 5);
            sp2_button_graph -> Add(Graph, 5, defStyle, 5);
}

/** 以下是事件处理函数 **/
void serialGUIFrame::evtOpenPort(wxCommandEvent& event)
{
    auto gcid = [&](int id){return selection[id].second->GetSelection();};
    auto tport = choices[0][gcid(0)].c_str(); // 串口地址，传递char指针
    auto tbaud = rbaud[gcid(1)]; // 波特率，转换成数字即可
    auto tlen  = rlen[gcid(2)] ;  // 字长，下标+5即可
    auto tpari = gcid(3);   // 校验位，下标已经对应选项，无需处理
    auto tsbit = rstop[gcid(4)];   // 停止位，转换成浮点数后乘以10变成整数即可
    if(IOdata.is_open()){
        asioClose_serial_port();
    }
    try{
        asioOpen_serial_port(tport, tbaud, tlen, tpari, tsbit);
    }
    catch (std::exception &e){
        wxMessageBox(e.what(), wxT("串口打开失败"));
        return;
    }
    wxMessageBox(wxT("串口已成功打开！"), wxT("提示"));
    return;
}

void serialGUIFrame::evtSampling(wxTimerEvent& event)
{
    IOdata.async_read_some(boost::asio::buffer(buf,1),
        boost::bind(handle_read, this, boost::placeholders::_1, boost::placeholders::_2)
    );
    IO_svr.run();
}  // 单次读，计时器触发
void serialGUIFrame::evtSending(wxCommandEvent& event)
{
    auto msg = Send_Message->GetValue();
    Send_Message->Clear();
    if(msg.size() == 0){
        wxMessageBox(wxT("没有数据"), wxT("提示"));
    }
    if(isSendHex){
        auto cbuf = wxstr2hex(msg);
        if(cbuf.size() > 0){
            IOdata.async_write_some( boost::asio::buffer( cbuf, cbuf.size() ),
                boost::bind(handle_write, this, boost::placeholders::_1, boost::placeholders::_2)
            );
        }
        else{
            wxMessageBox(wxT("格式错误"), wxT("错误"));
        }
    }
    else{
        IOdata.async_write_some( boost::asio::buffer( msg, msg.size() ),
            boost::bind(handle_write, this, boost::placeholders::_1, boost::placeholders::_2)
        );
    }
    IO_svr.run();
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
    return;
}

void serialGUIFrame::evtSendHex(wxCommandEvent& event) { isSendHex=send_hex->GetValue(); }
void serialGUIFrame::evtStartSampling(wxCommandEvent& event){ flagShowOnGrapgic = true; }  // 启动采样
void serialGUIFrame::evtStopSampling(wxCommandEvent& event) { flagShowOnGrapgic = false; } // 停止采样
void serialGUIFrame::evtClearGraph(wxCommandEvent& event)   { data->Clear(); Graph->UpdateAll(); }// 清空
void serialGUIFrame::evtClearText(wxCommandEvent& event)    { Recieve_txtbox->Clear(); }

void serialGUIFrame::update_display_range()
{
    int cnt = data->getSize();
    Graph->Fit(std::max(0,cnt-10), cnt, 0, std::max(data->getMaxValue()+100,255.0));
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


