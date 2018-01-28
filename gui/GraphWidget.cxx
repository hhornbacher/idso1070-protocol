#include "GraphWidget.h"

#include <gdkmm/general.h> // for cairo helper functions
#include <iostream>
#include <cstring>

GraphWidget::GraphWidget() : //The GType name will actually be gtkmm__CustomObject_graphwidget
                             Glib::ObjectBase("graphwidget"),
                             Gtk::Widget()
{
    set_has_window(true);

    //This shows the GType name, which must be used in the CSS file.
    std::cout << "GType name: " << G_OBJECT_TYPE_NAME(gobj()) << std::endl;

    // Set the widget name to use in the CSS file.
    set_name("graph-widget");

    // If you make a custom widget in C code, based on gtk+'s GtkWidget, there is
    // an alternative to gtk_widget_set_name(): Set a CSS name for your custom
    // class (instead of the widget instance) with gtk_widget_class_set_css_name()
    // (new in gtk+ 3.19.1). That's not possible for custom widgets defined in gtkmm.
    // gtk_widget_class_set_css_name() must be called in the class init function,
    // which can't be customized, when the widget is based on gtkmm's Gtk::Widget.
    //
    // Another alternative: The custom widget inherits the CSS name "widget" from
    // GtkWidget. That name can be used in the CSS file. This is not a very good
    // alternative. GtkWidget's CSS name is not documented. It can probably be
    // changed or removed in the future.

    refCssProvider = Gtk::CssProvider::create();
    auto refStyleContext = get_style_context();
    refStyleContext->add_provider(refCssProvider,
                                  GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    refCssProvider->signal_parsing_error().connect(
        sigc::mem_fun(*this, &GraphWidget::on_parsing_error));

    try
    {
        refCssProvider->load_from_path("./graph-gtk.css");
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

GraphWidget::~GraphWidget()
{
}

Gtk::SizeRequestMode GraphWidget::get_request_mode_vfunc() const
{
    //Accept the default value supplied by the base class.
    return Gtk::Widget::get_request_mode_vfunc();
}

//Discover the total amount of minimum space and natural space needed by
//this widget.
//Let's make this simple example widget always need minimum 60 by 50 and
//natural 100 by 70.
void GraphWidget::get_preferred_width_vfunc(int &minimum_width, int &natural_width) const
{
    minimum_width = 60;
    natural_width = 100;
}

void GraphWidget::get_preferred_height_for_width_vfunc(int /* width */,
                                                       int &minimum_height, int &natural_height) const
{
    minimum_height = 50;
    natural_height = 70;
}

void GraphWidget::get_preferred_height_vfunc(int &minimum_height, int &natural_height) const
{
    minimum_height = 50;
    natural_height = 70;
}

void GraphWidget::get_preferred_width_for_height_vfunc(int /* height */,
                                                       int &minimum_width, int &natural_width) const
{
    minimum_width = 60;
    natural_width = 100;
}

void GraphWidget::on_size_allocate(Gtk::Allocation &allocation)
{
    //Do something with the space that we have actually been given:
    //(We will not be given heights or widths less than we have requested, though
    //we might get more)

    //Use the offered allocation for this container:
    set_allocation(allocation);

    if (refGdkWindow)
    {
        refGdkWindow->move_resize(allocation.get_x(), allocation.get_y(),
                                  allocation.get_width(), allocation.get_height());
    }
}

void GraphWidget::on_map()
{
    //Call base class:
    Gtk::Widget::on_map();
}

void GraphWidget::on_unmap()
{
    //Call base class:
    Gtk::Widget::on_unmap();
}

void GraphWidget::on_realize()
{
    //Do not call base class Gtk::Widget::on_realize().
    //It's intended only for widgets that set_has_window(false).

    set_realized();

    if (!refGdkWindow)
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

        refGdkWindow = Gdk::Window::create(get_parent_window(), &attributes,
                                           GDK_WA_X | GDK_WA_Y);
        set_window(refGdkWindow);

        //make the widget receive expose events
        refGdkWindow->set_user_data(gobj());
    }
}

void GraphWidget::on_unrealize()
{
    refGdkWindow.reset();

    //Call base class:
    Gtk::Widget::on_unrealize();
}

bool GraphWidget::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    const Gtk::Allocation allocation = get_allocation();
    const double width = (double)allocation.get_width();
    const double height = (double)allocation.get_height();
    auto refStyleContext = get_style_context();

    // paint the background
    refStyleContext->render_background(cr,
                                       allocation.get_x(), allocation.get_y(),
                                       allocation.get_width(), allocation.get_height());

    // draw the foreground
    const auto state = refStyleContext->get_state();
    Gdk::Cairo::set_source_rgba(cr, refStyleContext->get_color(state));
    cr->move_to(0, 0);
    cr->line_to(width / 2, height);
    cr->line_to(width, 0);
    cr->stroke();

    return true;
}

void GraphWidget::on_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section, const Glib::Error &error)
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