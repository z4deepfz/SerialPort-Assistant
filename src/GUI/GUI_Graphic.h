#ifndef GUI_GRAPHIC_H
#define GUI_GRAPHIC_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <mathplot.h>
#include "../Serial_data.h"

class GUILineChart: public Serial_data, public wxPanel
{
    public:
        GUILineChart(wxWindow* parent);
        ~GUILineChart();
        void addElement(double x);
        wxBoxSizer* GetCtrlSizer() const;
        mpWindow* GetmpWindowPtr() const;
        unsigned int GetDataCount() const;

    protected:
        //wxPanel    *top_panel;
        wxBoxSizer *top_box;
        wxBoxSizer *sizerCtrl;
        wxCheckBox *isShow;
        wxButton   *buttClean;
        wxSpinCtrl *graphSize;
        mpWindow   *mpGraph;
        mpScaleX   *scaX;
        mpScaleY   *scaY;
    protected:
        bool flagShow;

    protected: // Event functions
        void OnCheckBox_Show(wxCommandEvent &event);
        void OnButton_Clean(wxCommandEvent &event);
        void OnSpin_size(wxSpinEvent &event);

        enum {
            idButton_Clean,
            idCheckBox_Show,
            idChangeSize
        };

    public: // Assistant functions
        void update_display_range();

    private:
        DECLARE_EVENT_TABLE()
};





#endif // GUI_GRAPHIC_H


