#ifndef GUI_GRAPHIC_H
#define GUI_GRAPHIC_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <mathplot.h>
#include "../Serial_data.h"
#include <iostream>

class GUILineChart: public Serial_data, public wxBoxSizer
{
    public:
        GUILineChart(wxWindow* parent);
        ~GUILineChart(){
            std::cout << "GUILineGraph dtor\n";
        }
        void resize();
        void addElement(double x);
        void Update();

    protected:
        wxBoxSizer *sizerCtrl;
        wxCheckBox *isShow;
        wxButton   *buttClean;
        wxSpinCtrl *graphSize;
        mpWindow   *mpGraph;
        mpScaleX   *scaX;
        mpScaleY   *scaY;
        bool flagShow;

    protected:
        void OnCheckBox_Show(wxCommandEvent &event);
        void OnButton_Clean(wxCommandEvent &event);

    private:
        DECLARE_EVENT_TABLE()
};





#endif // GUI_GRAPHIC_H


