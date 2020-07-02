#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "serialGUIApp.h"
#include "serialGUIMain.h"

IMPLEMENT_APP(serialGUIApp);

bool serialGUIApp::OnInit()
{
    m_locale.AddCatalogLookupPathPrefix(wxT("locale"));
    m_locale.AddCatalog(wxT("zh_CN"));
    serialGUIFrame* frame = new serialGUIFrame(0L, _("SerialPort-Assistant"));
    #ifdef __WINDOWS__
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    #endif // __WINDOWS__
    frame->Show();
    return true;
}
