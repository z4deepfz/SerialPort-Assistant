#ifndef SERIALGUIMAIN_H
#define SERIALGUIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "serialGUIApp.h"
#include "Serial_data.h"
#include <mathplot.h>
#include <iostream>
#include <boost/bind/bind.hpp>

class serialGUIFrame: public wxFrame
{
    public:
        serialGUIFrame(wxFrame *frame, const wxString& title);
        ~serialGUIFrame();
    protected: //这边就拿来放放控件指针好了
        wxPanel* top_panel;                 // 顶层面板
        wxBoxSizer* sp0_ctrl_text_area;     // 弹性控制器
            wxBoxSizer* tain1_text_graph;   // 放置接收文本框和图像
                wxBoxSizer* sp2_button_graph;   // 分割按钮和图像
                    wxBoxSizer* tain3_buttons_leftof_graph; // 放置图像左边的按钮
                    wxBoxSizer* tain4_chkbox;
            wxBoxSizer* tain1_ctrl;             // 放置串口控制区元件
                wxBoxSizer* tain2_2buttons;
                wxBoxSizer* tain2_choice_desc;  // 放置选项区元件的容器，水平
                    wxBoxSizer* tain3_text;   // 顺序放置静态文本
                    wxBoxSizer* tain3_choice;   // 顺序放置选项框
                wxBoxSizer* tain2_recieve_send_button; // 放置发送和接收控件
        std::pair<wxStaticText*,wxChoice*> selection[5]; //按照从上到下的顺序分配
        wxTextCtrl *Recieve_txtbox;         // 存放接收到的数据
        wxTextCtrl *Send_Message;           // 缓存待发送的数据
        wxButton   *Open_serial_port, *Clear_recieve;             // 【打开串口】和【清空接收】
        wxCheckBox *is_Recieve_data, *send_hex;
        wxButton   *Send_data_now;
        wxButton   *Start_display, *Stop_display, *Init_display;  // 图形控件的指令
        wxStatusBar *Stb;
        wxTimer    *sampling_clk;
        mpWindow   *Graph;
        Serial_data* data;
        boost::asio::io_service IO_svr;
        boost::asio::serial_port IOdata;
        std::vector<uint8_t> buf;
        /* flags */
        bool flagRecieve;           // 是否接收串口信号
        bool flagShowOnGrapgic;     // 是否将收到的内容展示在图像上
        bool isSendHex;
        void bind_boxsizer();

        /* counter */
        unsigned int rcnt;  // recieving counter
        unsigned int scnt;  // sending counter

    protected:
        void init_choice_boxes();
        void init_elements();
        void init_graphic();
        void modeIdle();
        void modeWorking();
        /* 串口读写函数 */
        void asioOpen_serial_port(const char* port, int baud, int dbt, int par, int sbits);
        void asioClose_serial_port();
        void Read_data(uint8_t buf[]);       // 从串口中捕获一个采样点
        void handle_read(const boost::system::error_code& e, std::size_t sz);
        void handle_write(const boost::system::error_code &e, std::size_t sz);

        /* 事件处理函数 */
        void evtOpenPort(wxCommandEvent& event);
        void evtSampling(wxTimerEvent& event);
        void evtStartSampling(wxCommandEvent& event);
        void evtClearGraph(wxCommandEvent& event);
        void evtStopSampling(wxCommandEvent& event);
        void evtClearText(wxCommandEvent& event);
        void evtSending(wxCommandEvent& event);
        void evtFlagRecieve(wxCommandEvent& event);
        void evtSendHex(wxCommandEvent& event);
        void update_display_range();
    protected: // Assistant functions
        std::vector<char> wxstr2hex(const wxString& a);
        void update_rs_bytes();
        std::vector<wxString> enum_ports();
        bool try_open_port(const wxString& a);
        std::string oct2hex(uint8_t x);
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
            idOpenPort,
            idTimer,
            idISRecieve,
            idClearGraph,
            idClearTextCtrl,
            idSendData,
            idStartShowOn_Graphic,
            idStopShowOn_Graphic,
            idSendHex
        };
        const wxString sel_label[5] = {
            wxT("串口"), wxT("波特率"), wxT("数据位"), wxT("校验位"), wxT("停止位")
        };
        std::vector<wxString> choices[5] = {
            { },
            { "9600", "19200", "38400", "57600", "115200" },
            { "5", "6", "7", "8" },
            { "None", "Odd", "Even" },
            { "1", "1.5", "2" }
        };
        const int rbaud[5] = {9600, 19200, 38400, 57600, 115200};
        const int rlen[4] = {5, 6, 7, 8};
        const int rstop[3] = {10, 15, 20};
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // SERIALGUIMAIN_H
