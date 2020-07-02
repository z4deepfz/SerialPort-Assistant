#include "GUI_Graphic.h"

BEGIN_EVENT_TABLE(GUILineChart, GUILineChart)

END_EVENT_TABLE()

GUILineChart::GUILineChart(wxWindow *parent):
    wxBoxSizer(wxHORIZONTAL),
    sizerCtrl( new wxBoxSizer(wxVERTICAL) ),
    isShow( new wxCheckBox(parent, wxID_ANY, _("Show on graph")) ),
    buttClean( new wxButton(parent, wxID_ANY, _("Clear")) ),
    graphSize( new wxSpinCtrl(parent, wxID_ANY, wxT("0")) ),
    mpGraph( new mpWindow(parent, wxID_ANY) ),
    scaX( new mpScaleX(_("Cases"), mpALIGN_BORDER_LEFT, false) ),
    scaY( new mpScaleY(_("Value"), mpALIGN_BORDER_BOTTOM, false) ),
    flagShow(false)
{
    constexpr auto defStyle = wxEXPAND | wxALL;
    /* init elements */
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
    wxBoxSizer::Add(sizerCtrl, 1, defStyle, 5);
    wxBoxSizer::Add(mpGraph, 5, defStyle, 5);
    sizerCtrl->Add(buttClean, 0, defStyle, 5);
    sizerCtrl->Add(isShow, 0, defStyle, 5);
    sizerCtrl->Add(graphSize, 0, defStyle, 5);
}

void GUILineChart::OnButton_Clean(wxCommandEvent &event)
{
    Serial_data::Clear();
}

void GUILineChart::OnCheckBox_Show(wxCommandEvent &event)
{
    if(isShow->GetValue()){
        flagShow = true;
    }
    else{
        flagShow = false;
    }
}

void GUILineChart::addElement(double x)
{
    if(flagShow){
        Serial_data::addElement(x);
    }
    return;
}

void GUILineChart::Update()
{
    mpGraph->Update();
}
