#ifndef _SETTINGS_WIDGET_H_
#define _SETTINGS_WIDGET_H_

#include <gtkmm.h>

#include <IDSO1070.h>

#include "ProtocolWorker.h"
#include "TransmissionLogDialog.h"

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
  void onButtonTransmissionLog();
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
  RefPtr<Builder> refGlade;

  TransmissionLogDialog *pTransmissionLogDialog;
  Button *pButtonTransmissionLog;
  Button *pButtonConnect;
  ProgressBar *pProgressbarConnection;
  Label *pConnectionStatus;

  LevelBar *pBatteryLevel;
  Label *pDeviceInfo;
  ToggleButton *pToggleSampling;
  ComboBox *pTimeBase;
  ComboBox *pScopeMode;
  ComboBox *pCaptureMode;
  RefPtr<ListStore> pTimeBaseStore;
  RefPtr<ListStore> pScopeModeStore;
  RefPtr<ListStore> pCaptureModeStore;

  ToggleButton *pChannel1Enabled;
  ToggleButton *pChannel2Enabled;

  ComboBox *pTriggerMode;
  ComboBox *pTriggerChannel;
  ComboBox *pTriggerSlope;
  RefPtr<ListStore> pTriggerModeStore;
  RefPtr<ListStore> pTriggerChannelStore;
  RefPtr<ListStore> pTriggerSlopeStore;

  // Protocol worker thread pointer
  ProtocolWorker *worker;

private:
  class TextComboColumns : public TreeModel::ColumnRecord
  {
  public:
    TextComboColumns()
    {
      // This order must match the column order in the .glade file
      this->add(this->title);
      this->add(this->value);
    }

    // These types must match those for the model in the .glade file
    Gtk::TreeModelColumn<Glib::ustring> title;
    Gtk::TreeModelColumn<int> value;
  };

  TextComboColumns textComboColumns;
};

#endif // _SETTINGS_WIDGET_H_