#ifndef _APP_WINDOW_H_
#define _APP_WINDOW_H_

#include <gtkmm.h>

using namespace Gtk;

class AppWindow : public Window
{
public:
  AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
  // AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refGlade,
  //           bool is_glad);
  virtual ~AppWindow();

protected:
  //Signal handlers:
  void onButtonConnect();

  Glib::RefPtr<Gtk::Builder> refGlade;
  Button *pButtonConnect;
  Label *pDeviceInfo;
  Label *pChannelsInfo;
  Label *pTriggerInfo;
};

#endif // _APP_WINDOW_H_