#ifndef SERIALGUICONFIGBOX_H
#define SERIALGUICONFIGBOX_H

#include <wx/wx.h>
#include <vector>



class serialGUIConfigBox: public wxBoxSizer
{

    /* internal data structure */
    public:
        struct portConfig{
            char COM[6];
            unsigned baud; // a uint
            unsigned databits;   // 5, 6, 7, 8
            unsigned stopbits;   // 10, 15, 20
            unsigned parity;     // none=0, odd=1, even=2
        };

    protected:
        class boxConfig: public wxChoice{
            public:
                boxConfig(wxWindow* parent, const std::initializer_list<wxString>& s);
                boxConfig(wxWindow* parent, const std::vector<wxString>& s);
        };
        class txtConfig: public wxStaticText{
            public: txtConfig(wxWindow* parent, const wxString& s);
        };
        enum eBaud {baud9600, baud19200, baud38400, baud57600, baud115200};
        enum eParity {none, odd, even};
        enum eStopBits {b10=10, b15=15, b20=20};

    /* class implement */
    public:
        serialGUIConfigBox(wxWindow* parent, const std::vector<wxString>& portCOMs);
        virtual ~serialGUIConfigBox();
        portConfig getConfig();
        void Enable();
        void Disable();
    protected:
        wxBoxSizer stBox, cBox; // [static text] boxSizer, [choice] boxSizer
        boxConfig *bPort, *bBaud, *bDBit, *bPari, *bSBit;
        txtConfig *tPort, *tBaud, *tDBit, *tPari, *tSBit;
    protected:
        const std::vector<wxString> a = { "9600", "19200", "38400", "57600", "115200" };
        const std::vector<wxString> b = { "5", "6", "7", "8" };
        const std::vector<wxString> c = { "None", "Odd", "Even" };
        const std::vector<wxString> d = { "1", "1.5", "2"};
};


#endif // SERIALGUICONFIGBOX_H
