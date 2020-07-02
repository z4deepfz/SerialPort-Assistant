#ifndef SERIALTEXTPLUS_H
#define SERIALTEXTPLUS_H

#include <wx/wx.h>
#include <wx/app.h>
#include <vector>

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
        std::vector<char> getHex() const;
        wxString getString() const;
        bool empty() const;
};



#endif // SERIALTEXTPLUS_H
