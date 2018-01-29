#ifndef _SETTINGS_WIDGET_H_
#define _SETTINGS_WIDGET_H_

#include <gtkmm.h>

#include <IDSO1070.h>

#include "ProtocolWorker.h"

using namespace Gtk;
using namespace Glib;

class SettingsWidget : public Box
{
public:
  SettingsWidget(BaseObjectType *cobject, const RefPtr<Builder> &refGlade);
  virtual ~SettingsWidget();

  void setWorker(ProtocolWorker *worker);

  // Called by WorkerThread to notify the UI, that there's an update to do
  void update();

protected:
  // Signal handlers
  void onToggleChannel1Enable();
  void onToggleChannel2Enable();
  void onButtonConnect();
  void onToggleSampling();

  // Internal UI update methods
  void updateConnectionControls(IDSO1070 &deviceState);
  void updateDeviceInfo(IDSO1070 &deviceState);
  void updateChannelsInfo(IDSO1070 &deviceState);
  void updateTriggerInfo(IDSO1070 &deviceState);

  // Protected Members - Gtk
  Glib::RefPtr<Gtk::Builder> refGlade;
  Button *pButtonConnect;
  ToggleButton *pToggleSampling;
  ProgressBar *pProgressbarConnection;
  CheckButton *pChannel1Enabled;
  CheckButton *pChannel2Enabled;

  // Protocol worker thread reference
  ProtocolWorker *worker;
};

#endif // _SETTINGS_WIDGET_H_