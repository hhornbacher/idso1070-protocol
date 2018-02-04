#ifndef _GRAPH_WIDGET_H_
#define _GRAPH_WIDGET_H_

#include <gtkmm/widget.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/styleproperty.h>

class ScopeWidget : public Gtk::Widget
{
public:
  static const int DivColumns = 10;
  static const int DivRows = 8;
  static const int ScopePadding = 10;

  ScopeWidget();
  virtual ~ScopeWidget();

protected:
  // Overrides:
  void on_size_allocate(Gtk::Allocation &allocation) override;
  void on_realize() override;
  void on_unrealize() override;
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;

  // Internal draw methods
  void drawGrid(const Cairo::RefPtr<Cairo::Context> &cr);
  void drawChannel1(const Cairo::RefPtr<Cairo::Context> &cr);
  void drawChannel2(const Cairo::RefPtr<Cairo::Context> &cr);

  Glib::RefPtr<Gdk::Window> refGdkWindow;
};

#endif // _GRAPH_WIDGET_H_