#ifndef SERIALTEXTPLUS_H
#define SERIALTEXTPLUS_H

#include <wx/wx.h>
#include <wx/app.h>
#include <vector>
#include <boost/xpressive/xpressive.hpp>

class wxTextPlus: public wxTextCtrl
{
    public:
        wxTextPlus();
        wxTextPlus(wxWindow *parent, long style);
        wxTextPlus(wxWindow *parent, wxWindowID id,
                   const wxString& value = wxEmptyString,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxTextCtrlNameStr);
        bool ValidateText() const;
        std::vector<char> getHex() const;
        wxString getString() const;
        bool empty() const;
/*
    private:
        static const boost::xpressive::sregex seg =
             boost::xpressive::bos
        >>  *( boost::xpressive::repeat<1,2>(boost::xpressive::xdigit)
            >> +( boost::xpressive::icase(" ") | boost::xpressive::eos ));
*/
};

// ^([0-9a-zA-Z]{1,2}( |$)+)*

#endif // SERIALTEXTPLUS_H
