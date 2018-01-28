#ifndef _GRAPH_WIDGET_H_
#define _GRAPH_WIDGET_H_

#include <gtkmm-plplot.h>
#include <gtkmm.h>

using namespace Gtk;

class GraphWidget : public Frame
{
public:
  GraphWidget();
  virtual ~GraphWidget();

protected:
  // PLplot::PlotData2D plot_data1;
  PLplot::Plot2D plot;
  PLplot::Canvas canvas;
};

#endif // _GRAPH_WIDGET_H_