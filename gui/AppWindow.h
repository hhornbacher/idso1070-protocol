#ifndef _APP_WINDOW_H_
#define _APP_WINDOW_H_

#include <iomanip>
#include <iostream>
#include <gtkmm.h>
#include "ProtocolWorker.h"

using namespace Gtk;
using namespace Glib;

class AppWindow : public Window
{
public:
  AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
  virtual ~AppWindow();

  // Called by WorkerThread to notify the UI, that there's an update to do
  void notify();

protected:
  // Signal handlers
  void onButtonConnect();
  void onNotificationFromWorker();

  // Internal UI update methods
  void updateConnectionControls(IDSO1070 &deviceState);
  void updateDeviceInfo(IDSO1070 &deviceState);
  void updateChannelsInfo(IDSO1070 &deviceState);
  void updateTriggerInfo(IDSO1070 &deviceState);

  // Protected Members - Gtk
  Glib::RefPtr<Gtk::Builder> refGlade;
  Button *pButtonConnect;
  ProgressBar *pProgressbarConnection;
  Label *pDeviceInfo;
  Label *pChannelsInfo;
  Label *pTriggerInfo;

  // Protected Members - WorkerThread
  Glib::Dispatcher dispatcher;
  ProtocolWorker worker;
  thread *workerThread;
};

#endif // _APP_WINDOW_H_