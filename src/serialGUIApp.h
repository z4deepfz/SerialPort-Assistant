﻿#ifndef SERIALGUIAPP_H
#define SERIALGUIAPP_H

#include <wx/app.h>

/* version: 0.6-alpha */
class serialGUIApp : public wxApp
{
    public:
        virtual bool OnInit();
    protected:
        wxLocale m_locale;
};

#endif // SERIALGUIAPP_H
