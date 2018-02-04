#include "ScopeWidget.h"

#include <gdkmm/general.h>
#include <iostream>
#include <cstring>

using namespace std;

ScopeWidget::ScopeWidget() : Glib::ObjectBase("scopewidget"),
                             Gtk::Widget()
{
  set_has_window(true);
  set_name("scope-widget");
}

ScopeWidget::~ScopeWidget()
{
}

void ScopeWidget::on_size_allocate(Gtk::Allocation &allocation)
{
  set_allocation(allocation);

  if (refGdkWindow)
  {
    refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(),
                              allocation.get_width(), allocation.get_height());
  }
}

void ScopeWidget::on_realize()
{
  set_realized();

  // If there is no reference to a GdkWindow, then create one
  if (!refGdkWindow)
  {

    GdkWindowAttr attributes;
    memset(&attributes, 0, sizeof(attributes));

    Gtk::Allocation allocation = get_allocation();

    // Set initial position and size of the Gdk::Window:
    attributes.x = allocation.get_x();
    attributes.y = allocation.get_y();
    attributes.width = allocation.get_width();
    attributes.height = allocation.get_height();

    attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.wclass = GDK_INPUT_OUTPUT;

    refGdkWindow = Gdk::Window::create(get_parent_window(), &attributes,
                                       GDK_WA_X | GDK_WA_Y);
    set_window(refGdkWindow);

    // Make the widget receive expose events
    refGdkWindow->set_user_data(gobj());
  }
}

void ScopeWidget::on_unrealize()
{
  refGdkWindow.reset();

  Gtk::Widget::on_unrealize();
}

bool ScopeWidget::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
  const Gtk::Allocation allocation = get_allocation();
  double width = (double)(allocation.get_width() - (ScopePadding * 2));
  double height = (double)(allocation.get_height() - (ScopePadding * 2));

  double xScale = (width - (double)((int)width % DivColumns)) / (double)DivColumns;
  double yScale = (height - (double)((int)height % DivRows)) / (double)DivRows;

  double xTranslate;
  double yTranslate;

  if (xScale > yScale)
  {
    xTranslate = ((width - yScale * DivColumns) / 2) + (double)ScopePadding;
    yTranslate = ((height - yScale * DivRows) / 2) + (double)ScopePadding;
    cr->translate(xTranslate, yTranslate);
    cr->scale(yScale, yScale);
    cr->set_line_width(1.0 / yScale);
  }
  else if (yScale > xScale)
  {
    xTranslate = ((width - xScale * DivColumns) / 2) + (double)ScopePadding;
    yTranslate = ((height - xScale * DivRows) / 2) + (double)ScopePadding;
    cr->translate(xTranslate, yTranslate);
    cr->scale(xScale, xScale);
    cr->set_line_width(1.0 / xScale);
  }
  else
  {
    xTranslate = ((width - yScale * DivColumns) / 2) + (double)ScopePadding;
    yTranslate = ((height - yScale * DivRows) / 2) + (double)ScopePadding;
    cr->translate(xTranslate, yTranslate);
    cr->scale(yScale, yScale);
    cr->set_line_width(1.0 / yScale);
  }

  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("rgb(0, 0, 0)"));
  cr->paint();

  drawGrid(cr);
  drawChannel1(cr);
  drawChannel2(cr);

  return true;
}

void ScopeWidget::drawGrid(const Cairo::RefPtr<Cairo::Context> &cr)
{
  const double width = (double)DivColumns;
  const double height = (double)DivRows;

  // draw scope background
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("rgb(71, 71, 71)"));
  cr->rectangle(0.0, 0.0, width, height);
  cr->fill();

  // draw column divs
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("rgb(50, 50, 50)"));
  for (int c = 0; c < (DivColumns - 1); c++)
  {
    if ((c + 1) != (DivColumns / 2))
    {
      double xOffset = (width / (double)DivColumns) * ((double)c + 1);
      cr->move_to(xOffset, 0.0);
      cr->line_to(xOffset, height);
      cr->stroke();
    }
  }

  // draw row divs
  for (int r = 0; r < (DivRows - 1); r++)
  {
    if ((r + 1) != (DivRows / 2))
    {
      double yOffset = (height / (double)DivRows) * ((double)r + 1);
      cr->move_to(0.0, yOffset);
      cr->line_to(width, yOffset);
      cr->stroke();
      cr->stroke();
    }
  }

  // draw center cross
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("rgb(200, 200, 200)"));
  cr->move_to(width / 2, 0.0);
  cr->line_to(width / 2, height);
  cr->move_to(0.0, height / 2);
  cr->line_to(width, height / 2);
  cr->stroke();

  // draw frame
  Gdk::Cairo::set_source_rgba(cr, Gdk::RGBA("rgb(200, 200, 200)"));
  cr->move_to(0.0, 0.0);
  cr->line_to(width, 0.0);
  cr->line_to(width, height);
  cr->line_to(0.0, height);
  cr->line_to(0.0, 0.0);
  cr->stroke();
}

void ScopeWidget::drawChannel1(const Cairo::RefPtr<Cairo::Context> &cr)
{
}

void ScopeWidget::drawChannel2(const Cairo::RefPtr<Cairo::Context> &cr)
{
}