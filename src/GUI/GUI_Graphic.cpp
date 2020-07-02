#include "GUI_Graphic.h"

BEGIN_EVENT_TABLE(GUILineChart, wxPanel)
    EVT_BUTTON(idButton_Clean, GUILineChart::OnButton_Clean)
    EVT_CHECKBOX(idCheckBox_Show, GUILineChart::OnCheckBox_Show)
    EVT_SPINCTRL(idChangeSize, GUILineChart::OnSpin_size)
END_EVENT_TABLE()

GUILineChart::GUILineChart(wxWindow *parent):
    wxPanel(parent, wxID_ANY),
    top_box( new wxBoxSizer(wxHORIZONTAL) ),
    sizerCtrl( new wxBoxSizer(wxVERTICAL) ),
    isShow( new wxCheckBox((wxPanel*)this, idCheckBox_Show, _("Show on graph")) ),
    buttClean( new wxButton((wxPanel*)this, idButton_Clean, _("Clear")) ),
    graphSize( new wxSpinCtrl((wxPanel*)this, idChangeSize, wxT("16")) ),
    mpGraph( new mpWindow((wxPanel*)this, wxID_ANY) ),
    scaX( new mpScaleX(_("Cases"), mpALIGN_BORDER_LEFT, false) ),
    scaY( new mpScaleY(_("Value"), mpALIGN_BORDER_BOTTOM, false) ),
    flagShow(false)
{
    constexpr auto defStyle = wxEXPAND | wxALL;
    /* init elements */
    isShow->SetValue(flagShow=true); // as default, program draw the cases on the graph
    SetDrawOutsideMargins(false);
    SetPen(wxPen(wxColor(0, 128, 255), 5));
    scaX->SetPen(wxPen(wxColor(0,0,0), 2));
    scaY->SetPen(wxPen(wxColor(0,0,0), 2));
    scaX->SetLabelFormat(wxString("%.0f"));
    scaY->SetLabelFormat(wxString("%0.f"));
    mpGraph->EnableMousePanZoom(false);
    mpGraph->AddLayer( scaX );
    mpGraph->AddLayer( scaY );
    mpGraph->AddLayer( (Serial_data*)this );
    mpGraph->Fit(0, 16, -10, 1000);

    /* bind boxsizer */
    SetSizer(top_box);
    top_box->Add(sizerCtrl, 1, defStyle, 0);
    top_box->Add(mpGraph, 8, defStyle, 5);
    sizerCtrl->Add(buttClean, 0, defStyle, 5);
    sizerCtrl->Add(isShow, 0, defStyle, 5);
    sizerCtrl->Add(graphSize, 0, defStyle, 5);
}

void GUILineChart::OnButton_Clean(wxCommandEvent &event)
{
    Serial_data::Clear();
    update_display_range();
}

void GUILineChart::OnCheckBox_Show(wxCommandEvent &event)
{
    flagShow = isShow->GetValue();
}

void GUILineChart::addElement(double x)
{
    if(flagShow){
        Serial_data::addElement(x);
        update_display_range();
        mpGraph->Update();
    }
    return;
}

void GUILineChart::update_display_range()
{
    int cnt = data.size();
    mpGraph->Fit(std::max(0,cnt-std::max(1,graphSize->GetValue())), cnt, // [Xmin, Xmax]
                 0, std::max(bufmax+64,8.0));                            // [Ymin, Ymax]
}

void GUILineChart::OnSpin_size(wxSpinEvent &event)
{
    update_display_range();
}

GUILineChart::~GUILineChart()
{
    mpGraph->DelLayer((Serial_data*)this);
}
