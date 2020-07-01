#include "serialTextPlus.h"

inline char c2Hex(const char c)
{
    if(c>='0' && c<='9') return c-'0';
    else if(c>='a' && c<='f') return c-'a'+10;
    else if(c>='A' && c<='F') return c-'A'+10;
    else return 0;
}

wxTextPlus::wxTextPlus(): wxTextCtrl() { }

wxTextPlus::wxTextPlus(wxWindow *parent, long style):
    wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition,
               wxDefaultSize, style, wxDefaultValidator, wxTextCtrlNameStr)
{ }

wxTextPlus::wxTextPlus(wxWindow *parent, wxWindowID id,
                       const wxString& value,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxValidator& validator,
                       const wxString& name):
    wxTextCtrl(parent, id, value, pos, size, style, validator, name)
{ }



std::vector<char> wxTextPlus::getHex() const
{
    // Get the correct hexadecimal sequence by a simple deterministic finite automaton
    enum hexDFA {
        IDLE,
        HEX1,
        HEX2,
        SYN_ERR
    }; // define DFA
    hexDFA dfa = IDLE;
    std::vector<char> cbuf;
    wxString str = GetValue() + " "; //  add a additional space to simplify the logic
    char tmp;
    for(auto i: str){
        auto valid = isxdigit(i);
        auto space = isspace(i);
        switch(dfa){
            case IDLE: {
                if(valid) {
                    tmp = c2Hex(i);
                    dfa = HEX1;
                }
                else if(!space) dfa = SYN_ERR;
                break;
            }
            case HEX1: {
                if(valid){
                    tmp = (tmp<<4) | c2Hex(i);
                    dfa = HEX2;
                }
                else if(space){
                    cbuf.push_back(tmp);
                    dfa = IDLE;
                }
                else dfa = SYN_ERR;
                break;
            }
            case HEX2: {
                if(space){
                    cbuf.push_back(tmp);
                    dfa = IDLE;
                }
                else dfa = SYN_ERR;
                break;
            }
            case SYN_ERR: {
                cbuf.clear();
                return cbuf;
            }
        }
    }
    return cbuf;
}

wxString wxTextPlus::getString() const
{
    return this->GetValue();
}

bool wxTextPlus::empty() const
{
    return IsEmpty();
}
