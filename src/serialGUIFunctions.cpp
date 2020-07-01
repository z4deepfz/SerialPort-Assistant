#include "serialGUIMain.h"
#include <iostream>
#include <cctype>

inline char c2Hex(const char c)
{
    if(c>='0' && c<='9') return c-'0';
    else if(c>='a' && c<='f') return c-'a'+10;
    else if(c>='A' && c<='F') return c-'A'+10;
    else return 0;
}

std::vector<char> serialGUIFrame::wxstr2hex(const wxString& a)
{

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
        if(flagShowOnGrapgic){
            data->addElement(buf[i]);
        }
        Recieve_txtbox->AppendText( oct2hex(buf[i]) );
    }
    if(flagShowOnGrapgic){
        Graph->Update();
        update_display_range();
    }
    rcnt += sz;
    update_rs_bytes();
    return;
}

void serialGUIFrame::handle_write(const boost::system::error_code &e, std::size_t sz)
{
    // looks like if update the scnt here, there is a delay in data update
    // so when sending, I update the scnt at once if IOdata.run() executed sucessfully
    update_rs_bytes();
    return;
}

void serialGUIFrame::update_rs_bytes()
{
    Stb->SetStatusText(wxString::Format("Recieve: %u bytes", rcnt), 1);
    Stb->SetStatusText(wxString::Format("Send: %u bytes", scnt), 2);
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
