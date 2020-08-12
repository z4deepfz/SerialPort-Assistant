#ifndef SERIALGUIMAIN_H
#define SERIALGUIMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/spinctrl.h>
#include <wx/file.h>
#include <mathplot.h>
#include <boost/bind/bind.hpp>

#include "serialGUIApp.h"
#include "Serial_data.h"
#include "GUI/serialGUIConfigBox.h"
#include "GUI/serialTextPlus.h"
#include "GUI/GUI_Graphic.h"

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
                    wxBoxSizer* tain4_btn_and_loop;
                        wxBoxSizer* tain5_delay;
                    wxBoxSizer* tain4_chkbox;
            wxBoxSizer* tain1_ctrl;             // 放置串口控制区元件
                wxBoxSizer* tain2_2buttons;
                wxBoxSizer* tain2_choice_desc;  // 放置选项区元件的容器，水平
                    wxBoxSizer* tain3_text;   // 顺序放置静态文本
                    wxBoxSizer* tain3_choice;   // 顺序放置选项框
                wxBoxSizer* tain2_recieve_send_button; // 放置发送和接收控件
        wxTextCtrl *Recieve_txtbox;         // 存放接收到的数据
        wxTextPlus *Send_Message;           // 缓存待发送的数据
        wxButton   *Open_serial_port, *Clear_recieve;
        wxCheckBox *is_Recieve_data, *send_hex, *loop_send;
        wxButton   *Send_data_now;
        wxStatusBar *Stb;
        wxMenuBar   *menubar;
        wxMenu      *menu_file, *menu_help;
        wxTimer    *sampling_clk, *loop_clk;
        wxSpinCtrl *displayWidth;
        GUILineChart *Graph;
        serialGUIConfigBox* configBox;
        wxSpinCtrl *SendDelay;
        boost::asio::io_service IO_svr;
        boost::asio::serial_port IOdata;
        std::vector<uint8_t> buf;

        std::vector<wxWindow*> ElementsEnabledOnlyIFPort_Open;
        std::vector<wxWindow*> ElementsEnabledOnlyIFPort_Close;

        /* counter */
        unsigned int scnt;  // sending counter
        unsigned int rcnt;  // recieving counter

        /* flags */
        bool flagRecieve;           // 是否接收串口信号
        bool isSendHex;
        bool flagLoopReadyToSend;   // Managed by Timer
        bool flagSent;              // Managed by handle function

        void bind_boxsizer();

    protected:
        void init_elements();
        void modeIdle();
        void modeWorking();
        void SetLoopFlagBeforeSend();
        /* 串口读写函数 */
        void asioOpen_serial_port(const char* port, int baud, int dbt, int par, int sbits);
        void asioClose_serial_port();
        void Read_data(uint8_t buf[]);       // 从串口中捕获一个采样点
        void handle_read(const boost::system::error_code& e, std::size_t sz);
        void handle_write(const boost::system::error_code &e, std::size_t sz);

        /* 事件处理函数 */
        void OnAbout(wxCommandEvent& event);
        void evtOpenPort(wxCommandEvent& event);
        void evtSampling(wxTimerEvent& event);
        void evtClearText(wxCommandEvent& event);
        void evtSending(wxCommandEvent& event);
        void evtFlagRecieve(wxCommandEvent& event);
        void evtSendHex(wxCommandEvent& event);
        void evtMenuOpenFile(wxCommandEvent& event);
        void evtLoopClk(wxTimerEvent& event);
        void evtSaveData(wxCommandEvent& event);
        void evtSaveImage(wxCommandEvent& event);
        void update_display_range();

        /* 事件无关方法 */
        void LoadDataFromFile();
        void SendDataNow();

    protected: // Assistant functions
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
            idSendHex,
            menuOpenFile,
            idLoopSend,
            idLoopClk,
            idSaveTxt,
            idSaveImg
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};


#endif // SERIALGUIMAIN_H
