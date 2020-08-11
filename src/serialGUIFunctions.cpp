#include "serialGUIMain.h"
#include <cctype>

inline char c2Hex(const char c)
{
    if(c>='0' && c<='9') return c-'0';
    else if(c>='a' && c<='f') return c-'a'+10;
    else if(c>='A' && c<='F') return c-'A'+10;
    else return 0;
}

/** following docking Boost::asio **/
void serialGUIFrame::asioOpen_serial_port
    (const char* port, int baud, int dbt, int par, int sbits)
{
    using namespace boost::asio;
    IOdata.open(port);
    IOdata.set_option(serial_port::baud_rate(baud));
    IOdata.set_option(serial_port::character_size(dbt));
    switch(par){
        case 0: IOdata.set_option(serial_port::parity(serial_port::parity::none)); break;
        case 1: IOdata.set_option(serial_port::parity(serial_port::parity::odd)); break;
        case 2: IOdata.set_option(serial_port::parity(serial_port::parity::even)); break;
        default: throw std::bad_exception();
    }
    switch(sbits){
        case 10: IOdata.set_option(serial_port::stop_bits(serial_port::stop_bits::one)); break;
        case 15: IOdata.set_option(serial_port::stop_bits(serial_port::stop_bits::onepointfive)); break;
        case 20: IOdata.set_option(serial_port::stop_bits(serial_port::stop_bits::two)); break;
        default: throw std::bad_exception();
    }
    return;
}

void serialGUIFrame::asioClose_serial_port()
{
    IOdata.close();
}


inline std::string serialGUIFrame::oct2hex(uint8_t x)
{
    static constexpr char rehex[17] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    return std::string() + rehex[((x>>4)&0xf)] + rehex[x&0xf] + ' ';
};

void serialGUIFrame::handle_read(const boost::system::error_code& e, std::size_t sz)
{
    for(std::size_t i=0; i<sz; ++i){
        Graph->addElement(buf[i]);
        Recieve_txtbox->AppendText( oct2hex(buf[i]) );
    }
    rcnt += sz;
    update_rs_bytes();
    return;
}

void serialGUIFrame::handle_write(const boost::system::error_code &e, std::size_t sz)
{
    // looks like if update the scnt here, there is a delay in data update
    // so when sending, I update the scnt at once if IOdata.run() executed sucessfully
    /// situation1(mostly): sending task already finished before Timer triggered
    /// situation2: sending task takes too long so that Timer triggered earlier than task finished
    update_rs_bytes();
    flagSent = true;
    if( loop_send->GetValue() && flagLoopReadyToSend ){ // 1
        SetLoopFlagBeforeSend();
        SendDataNow();
    }
    return;
}

void serialGUIFrame::update_rs_bytes()
{
    Stb->SetStatusText(wxString::Format(_("Recieve: %u bytes"), rcnt), 1);
    Stb->SetStatusText(wxString::Format(_("Send: %u bytes"), scnt), 2);
}

std::vector<wxString> serialGUIFrame::enum_ports()
{
    std::vector<wxString> ans;
    for(int i=1; i<=255; ++i){
        wxString tmp = "COM";
        tmp << i;
        //std::cout << "Trying " << tmp << std::endl;
        if(try_open_port(tmp)){
            ans.push_back(tmp);
        }
    }
    return ans;
}

bool serialGUIFrame::try_open_port(const wxString& a)
{
    const auto tbaud = 9600;
    const auto tlen  = 8;
    const auto tpari = 1;
    const auto tsbit = 10;
    try{
        asioOpen_serial_port(a.c_str(), tbaud, tlen, tpari, tsbit);
    }
    catch (...){
        return false;
    }
    asioClose_serial_port();
    return true;
}

void serialGUIFrame::LoadDataFromFile()
{
    wxFileDialog selfile(this);
    if(selfile.ShowModal() == wxID_OK){
        auto s = selfile.GetPath();
        //wxMessageBox(s);
        wxFile fio(s);
        wxString tmp;
        fio.ReadAll(&tmp);
        Send_Message->ChangeValue(tmp);
    }
    return;
}

void serialGUIFrame::SendDataNow()
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
        if( loop_send->GetValue() ){
            loop_clk->Start(SendDelay->GetValue());
        }
        else{
            Send_Message->Clear();
        }
    }
    else{
        wxMessageBox(_("Port is closed"), _("Note"), wxICON_ERROR);
    }
}

void serialGUIFrame::SetLoopFlagBeforeSend()
{
    flagLoopReadyToSend = false;
    flagSent = false;
    loop_clk->Start(SendDelay->GetValue());
}
