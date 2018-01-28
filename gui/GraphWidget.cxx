#include "GraphWidget.h"

#include <iostream>
#include <cstring>

GraphWidget::GraphWidget() : Frame(),
                             plot("X-Axis"),
                             canvas(plot, Gdk::RGBA("rgb(71, 71, 71)"))
{
    //let's make this more interesting by adding more data
    std::valarray<double> x_va(1000), y_va(1000);
    for (unsigned int i = 0; i < 1000; i++)
    {
        x_va[i] = 8 * M_PI * i / 999;
    }
    y_va = 2 * cos(x_va) - 1;
    // Gtk::manage can be used for memory management here...
    plot.add_data(*Gtk::manage(new Gtk::PLplot::PlotData2D(x_va, y_va, Gdk::RGBA("rgb(182, 246, 255)"), Gtk::PLplot::LineStyle::LONG_DASH_LONG_GAP, 5.0)));
    plot.hide_legend();
    canvas.set_hexpand(true);
    canvas.set_vexpand(true);
    add(canvas);

    // show_all_children();
}

GraphWidget::~GraphWidget()
{
}