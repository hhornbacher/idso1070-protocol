#ifndef _APP_WINDOW_H_
#define _APP_WINDOW_H_

#include <iomanip>
#include <iostream>

#include <gtkmm.h>

#include "GraphWidget.h"
#include "SettingsWidget.h"
#include "ProtocolWorker.h"

using namespace Gtk;
using namespace Glib;

class AppWindow : public Window
{
public:
  AppWindow(const Glib::RefPtr<Gtk::Builder> &refGlade);
  virtual ~AppWindow();

  // Called by WorkerThread to notify the UI, that there's an update to do
  void notify();

protected:
  // Signal handlers
  void onNotificationFromWorker();
  bool onUpdateBatteryLevel();

  // Protected Members - Gtk
  Glib::RefPtr<Gtk::Builder> refGlade;

  // Protected Members - WorkerThread
  Glib::Dispatcher dispatcher;
  ProtocolWorker worker;
  thread *workerThread;

  // Protected Members - Timers
  sigc::connection updateBatteryTimer;

  // Protected Members - Child widgets
  Box boxHorizontal;
  GraphWidget graphWidget;
  SettingsWidget *pSettingsWidget;
};

#endif // _APP_WINDOW_H_