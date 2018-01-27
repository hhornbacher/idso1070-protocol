#include <gtkmm.h>

int main(int argc, char *argv[])
{
  auto app =
    Gtk::Application::create(argc, argv,
      "org.hhornbacher.idso1070.gui");

    Gtk::Window window;

    return app->run(window);
}
