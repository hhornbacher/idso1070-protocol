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

  void notify();

protected:
  void onButtonConnect();
  void onNotificationFromWorker();

  Glib::RefPtr<Gtk::Builder> refGlade;
  Button *pButtonConnect;
  Label *pDeviceInfo;
  Label *pChannelsInfo;
  Label *pTriggerInfo;
  ProgressBar *pProgressbarConnection;

  Glib::Dispatcher dispatcher;
  ProtocolWorker worker;
  thread *workerThread;
};

#endif // _APP_WINDOW_H_