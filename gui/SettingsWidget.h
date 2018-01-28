#ifndef _SETTINGS_WIDGET_H_
#define _SETTINGS_WIDGET_H_

#include <gtkmm.h>

#include <device/IDSO1070.h>

#include "ProtocolWorker.h"

using namespace Gtk;
using namespace Glib;

class SettingsWidget : public Box
{
  public:
    SettingsWidget(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
    virtual ~SettingsWidget();

    // Called by WorkerThread to notify the UI, that there's an update to do
    void update(ProtocolWorker &worker, IDSO1070 &deviceState);

  protected:
    // Signal handlers
    void onToggleChannel1Enable();
    void onToggleChannel2Enable();
    void onButtonConnect();

    // Internal UI update methods
    void updateConnectionControls(ProtocolWorker &worker, IDSO1070 &deviceState);
    void updateDeviceInfo(ProtocolWorker &worker, IDSO1070 &deviceState);
    void updateChannelsInfo(ProtocolWorker &worker, IDSO1070 &deviceState);
    void updateTriggerInfo(ProtocolWorker &worker, IDSO1070 &deviceState);

    // Protected Members - Gtk
    Glib::RefPtr<Gtk::Builder> refGlade;
    Button *pButtonConnect;
    ProgressBar *pProgressbarConnection;
    CheckButton *pChannel1Enabled;
    CheckButton *pChannel2Enabled;
};

#endif // _SETTINGS_WIDGET_H_