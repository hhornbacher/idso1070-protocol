#ifndef _TRANSMISSION_LOG_DIALOG_H_
#define _TRANSMISSION_LOG_DIALOG_H_

#include <gtkmm.h>

#include <Protocol.h>

using namespace Gtk;
using namespace Glib;

class TransmissionLogDialog : public Window
{
public:
  TransmissionLogDialog(BaseObjectType *cobject, const RefPtr<Builder> &refGlade);
  virtual ~TransmissionLogDialog();

  void updateLog(Protocol::TransmissionLog &log);

protected:
  // Signal handlers

  // Internal UI methods

  // Protected Members - Gtk
  Glib::RefPtr<Gtk::Builder> refGlade;

  TextView *pTransmissionLogView;
  RefPtr<TextBuffer> pTransmissionLogBuffer;

private:
  ustring hexdump(uint8_t *data, size_t length, int cols = 32);

  Protocol::TransmissionLog transmissionLog;
  int transmissionCounter;
};

#endif // _TRANSMISSION_LOG_DIALOG_H_