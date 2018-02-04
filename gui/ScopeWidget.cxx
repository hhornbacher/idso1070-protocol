#include "ScopeWidget.h"

#include <gdkmm/general.h>
#include <iostream>
#include <cstring>

extern char _binary_scope_css_start[];
extern char _binary_scope_css_size[];

ScopeWidget::ScopeWidget() : Glib::ObjectBase("scopewidget"),
                             Gtk::Widget(),
                             //Install a style property so that an aspect of this widget may be themed
                             //via a CSS style sheet file:
                             m_scale_prop(*this, "example_scale", 500),
                             m_scale(1000)
{
  char *data_start = _binary_scope_css_start;
  Glib::ustring scopeCSS = data_start;

  set_has_window(true);
  set_name("scope-widget");

  m_refCssProvider = Gtk::CssProvider::create();
  auto refStyleContext = get_style_context();
  refStyleContext->add_provider(m_refCssProvider,
                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  m_refCssProvider->signal_parsing_error().connect(
      sigc::mem_fun(*this, &ScopeWidget::on_parsing_error));

  try
  {
    m_refCssProvider->load_from_data(scopeCSS);
  }
  catch (const Gtk::CssProviderError &ex)
  {
    std::cerr << "CssProviderError, Gtk::CssProvider::load_from_path() failed: "
              << ex.what() << std::endl;
  }
  catch (const Glib::Error &ex)
  {
    std::cerr << "Error, Gtk::CssProvider::load_from_path() failed: "
              << ex.what() << std::endl;
  }
}

ScopeWidget::~ScopeWidget()
{
}

Gtk::SizeRequestMode ScopeWidget::get_request_mode_vfunc() const
{
  //Accept the default value supplied by the base class.
  return Gtk::Widget::get_request_mode_vfunc();
}

//Discover the total amount of minimum space and natural space needed by
//this widget.
//Let's make this simple example widget always need minimum 60 by 50 and
//natural 100 by 70.
void ScopeWidget::get_preferred_width_vfunc(int &minimum_width, int &natural_width) const
{
  minimum_width = 60;
  natural_width = 100;
}

void ScopeWidget::get_preferred_height_for_width_vfunc(int /* width */,
                                                       int &minimum_height, int &natural_height) const
{
  minimum_height = 50;
  natural_height = 70;
}

void ScopeWidget::get_preferred_height_vfunc(int &minimum_height, int &natural_height) const
{
  minimum_height = 50;
  natural_height = 70;
}

void ScopeWidget::get_preferred_width_for_height_vfunc(int /* height */,
                                                       int &minimum_width, int &natural_width) const
{
  minimum_width = 60;
  natural_width = 100;
}

void ScopeWidget::on_size_allocate(Gtk::Allocation &allocation)
{
  set_allocation(allocation);

  if (m_refGdkWindow)
  {
    m_refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(),
                                allocation.get_width(), allocation.get_height());
  }
}

void ScopeWidget::on_map()
{
  Gtk::Widget::on_map();
}

void ScopeWidget::on_unmap()
{
  Gtk::Widget::on_unmap();
}

void ScopeWidget::on_realize()
{
  //Do not call base class Gtk::Widget::on_realize().
  //It's intended only for widgets that set_has_window(false).

  set_realized();

  //Get the themed style from the CSS file:
  m_scale = m_scale_prop.get_value();

  if (!m_refGdkWindow)
  {
    //Create the GdkWindow:

    GdkWindowAttr attributes;
    memset(&attributes, 0, sizeof(attributes));

    Gtk::Allocation allocation = get_allocation();

    //Set initial position and size of the Gdk::Window:
    attributes.x = allocation.get_x();
    attributes.y = allocation.get_y();
    attributes.width = allocation.get_width();
    attributes.height = allocation.get_height();

    attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.wclass = GDK_INPUT_OUTPUT;

    m_refGdkWindow = Gdk::Window::create(get_parent_window(), &attributes,
                                         GDK_WA_X | GDK_WA_Y);
    set_window(m_refGdkWindow);

    //make the widget receive expose events
    m_refGdkWindow->set_user_data(gobj());
  }
}

void ScopeWidget::on_unrealize()
{
  m_refGdkWindow.reset();

  //Call base class:
  Gtk::Widget::on_unrealize();
}

bool ScopeWidget::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
  const Gtk::Allocation allocation = get_allocation();
  const double scale_x = (double)allocation.get_width() / m_scale;
  const double scale_y = (double)allocation.get_height() / m_scale;
  auto refStyleContext = get_style_context();

  // paint the background
  refStyleContext->render_background(cr,
                                     allocation.get_x(), allocation.get_y(),
                                     allocation.get_width(), allocation.get_height());

  // draw the foreground
  const auto state = refStyleContext->get_state();
  Gdk::Cairo::set_source_rgba(cr, refStyleContext->get_color(state));
  cr->move_to(155. * scale_x, 165. * scale_y);
  cr->line_to(155. * scale_x, 838. * scale_y);
  cr->line_to(265. * scale_x, 900. * scale_y);
  cr->line_to(849. * scale_x, 564. * scale_y);
  cr->line_to(849. * scale_x, 438. * scale_y);
  cr->line_to(265. * scale_x, 100. * scale_y);
  cr->line_to(155. * scale_x, 165. * scale_y);
  cr->move_to(265. * scale_x, 100. * scale_y);
  cr->line_to(265. * scale_x, 652. * scale_y);
  cr->line_to(526. * scale_x, 502. * scale_y);
  cr->move_to(369. * scale_x, 411. * scale_y);
  cr->line_to(633. * scale_x, 564. * scale_y);
  cr->move_to(369. * scale_x, 286. * scale_y);
  cr->line_to(369. * scale_x, 592. * scale_y);
  cr->move_to(369. * scale_x, 286. * scale_y);
  cr->line_to(849. * scale_x, 564. * scale_y);
  cr->move_to(633. * scale_x, 564. * scale_y);
  cr->line_to(155. * scale_x, 838. * scale_y);
  cr->stroke();

  return true;
}

void ScopeWidget::on_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section, const Glib::Error &error)
{
  std::cerr << "on_parsing_error(): " << error.what() << std::endl;
  if (section)
  {
    const auto file = section->get_file();
    if (file)
    {
      std::cerr << "  URI = " << file->get_uri() << std::endl;
    }

    std::cerr << "  start_line = " << section->get_start_line() + 1
              << ", end_line = " << section->get_end_line() + 1 << std::endl;
    std::cerr << "  start_position = " << section->get_start_position()
              << ", end_position = " << section->get_end_position() << std::endl;
  }
}