#ifndef _APP_WINDOW_H_
#define _APP_WINDOW_H_

#include <iomanip>
#include <iostream>

#include <gtkmm.h>

#include "ScopeWidget.h"
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
  bool onUpdateScope();

  // Protected Members - Gtk
  Glib::RefPtr<Gtk::Builder> refGlade;

  // Protected Members - WorkerThread
  Glib::Dispatcher dispatcher;
  ProtocolWorker worker;

  // Protected Members - Timers
  sigc::connection updateBatteryTimer;
  sigc::connection updateScopeTimer;

  // Protected Members - Child widgets
  Box boxHorizontal;
  ScopeWidget scopeWidget;
  SettingsWidget *pSettingsWidget;
};

#endif // _APP_WINDOW_H_