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
  void onButtonConnect();
  void onToggleChannel1Enable();
  void onToggleChannel2Enable();
  void onToggleSampling();

  void onTimeBaseSelected();
  void onScopeModeSelected();
  void onCaptureModeSelected();

  void onTriggerModeSelected();
  void onTriggerChannelSelected();
  void onTriggerSlopeSelected();

  // Internal UI update methods
  void updateConnectionControls(IDSO1070 &deviceState);
  void updateDeviceInfo(IDSO1070 &deviceState);
  void updateChannelsInfo(IDSO1070 &deviceState);
  void updateTriggerInfo(IDSO1070 &deviceState);

  // Protected Members - Gtk
  Glib::RefPtr<Gtk::Builder> refGlade;

  Button *pButtonConnect;
  ProgressBar *pProgressbarConnection;
  Label *pConnectionStatus;

  LevelBar *pBatteryLevel;
  Label *pDeviceInfo;
  ToggleButton *pToggleSampling;
  ComboBox *pTimeBase;
  ComboBox *pScopeMode;
  ComboBox *pCaptureMode;

  ToggleButton *pChannel1Enabled;
  ToggleButton *pChannel2Enabled;

  ComboBox *pTriggerMode;
  ComboBox *pTriggerChannel;
  ComboBox *pTriggerSlope;

  // Protocol worker thread reference
  ProtocolWorker *worker;
};

#endif // _SETTINGS_WIDGET_H_