#include "serialGUIConfigBox.h"
#include <string>
serialGUIConfigBox::serialGUIConfigBox(
    wxWindow* parent,
    const std::vector<wxString>& portCOMs
):
    wxPanel(parent), topBox(wxHORIZONTAL), stBox(wxVERTICAL), cBox(wxVERTICAL),
    bPort( new boxConfig(this, portCOMs) ),
    bBaud( new boxConfig(this, { "9600", "19200", "38400", "57600", "115200" } )),
    bDBit( new boxConfig(this, { "5", "6", "7", "8" }) ),
    bPari( new boxConfig(this, { "None", "Odd", "Even" }) ),
    bSBit( new boxConfig(this, { "1", "1.5", "2"}) ),
    tPort( new txtConfig(this, _("Port")) ),
    tBaud( new txtConfig(this, _("Baudrate")) ),
    tDBit( new txtConfig(this, _("Data Bits")) ),
    tPari( new txtConfig(this, _("Parity")) ),
    tSBit( new txtConfig(this, _("Stop Bits")) )

{
    constexpr auto defStyle = wxEXPAND | wxALL;
    /* add two boxSizer to top box */
    wxPanel::SetSizer(&topBox);
    topBox.Add(&stBox, 1, defStyle, 0);
    topBox.Add(&cBox, 1, defStyle, 0);
    /* add choice boxes */
    cBox.Add(bPort, 1, defStyle, 5);
    cBox.Add(bBaud, 1, defStyle, 5);
    cBox.Add(bDBit, 1, defStyle, 5);
    cBox.Add(bPari, 1, defStyle, 5);
    cBox.Add(bSBit, 1, defStyle, 5);
    /* add static text */
    stBox.Add(tPort, 1, defStyle, 5);
    stBox.Add(tBaud, 1, defStyle, 5);
    stBox.Add(tDBit, 1, defStyle, 5);
    stBox.Add(tPari, 1, defStyle, 5);
    stBox.Add(tSBit, 1, defStyle, 5);
    /* */
    bPort->SetSelection(0);
    bBaud->SetSelection(0);
    bDBit->SetSelection(3);
    bPari->SetSelection(0);
    bSBit->SetSelection(0);
    return;
}

serialGUIConfigBox::~serialGUIConfigBox()
{
    //dtor
}

serialGUIConfigBox::portConfig serialGUIConfigBox::getConfig()
{
    portConfig ans;
    eBaud baud;
    eParity par;
    eStopBits sb;
    switch(bPort->GetCurrentSelection()){
        case 0:
            baud = baud9600; break;
        case 1:
            baud = baud19200; break;
        case 2:
            baud = baud38400; break;
        case 3:
            baud = baud57600; break;
        case 4:
            baud = baud115200; break;
        default:
            throw std::exception();
    }
    switch(bPari->GetCurrentSelection()){
        case 0:
            par = none; break;
        case 1:
            par = odd; break;
        case 2:
            par = even; break;
        default:
            throw std::exception();
    }
    switch(bSBit->GetCurrentSelection()){
        case 0:
            sb = b10; break;
        case 1:
            sb = b15; break;
        case 2:
            sb = b20; break;
        default:
            throw std::exception();
    }
    strncpy(ans.COM, bPort->GetString(bPort->GetCurrentSelection()).c_str(), 6);
    ans.baud = baud;
    ans.databits = bDBit->GetCurrentSelection() + 5;
    ans.parity = par;
    ans.stopbits = sb;
    return ans;
}

void serialGUIConfigBox::Enable()
{
    bPort->Enable();
    bBaud->Enable();
    bDBit->Enable();
    bPari->Enable();
    bSBit->Enable();
}

void serialGUIConfigBox::Disable()
{
    bPort->Disable();
    bBaud->Disable();
    bDBit->Disable();
    bPari->Disable();
    bSBit->Disable();
}

serialGUIConfigBox::portConfig::portConfig()
{
    baud = 0;
    databits = 0;
    stopbits = 0;
    parity = 0;
    strncpy(COM, "", 6);
}

serialGUIConfigBox::boxConfig::boxConfig(wxWindow* parent, const std::initializer_list<wxString>& s):
    wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, s.size(), s.begin())
{ }

serialGUIConfigBox::boxConfig::boxConfig(wxWindow* parent, const std::vector<wxString>& s):
    wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, s.size(), s.data())
{ }

serialGUIConfigBox::txtConfig::txtConfig(wxWindow* parent, const wxString& s):
    wxStaticText(parent, wxID_ANY, s) { }

