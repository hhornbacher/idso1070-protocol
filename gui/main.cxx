#include "AppWindow.h"

#include <iostream>

extern char _binary_glade_data_start[];
extern char _binary_glade_data_size[];

int main(int argc, char *argv[])
{
  auto app =
      Gtk::Application::create(argc, argv,
                               "org.hhornbacher.idso1070.gui");

  char *data_start = _binary_glade_data_start;
  size_t data_size = (size_t)_binary_glade_data_size;
  ustring gladeDate = data_start;

  // Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_string(gladeDate);
  }
  catch (const Glib::FileError &ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
    return 1;
  }
  catch (const Glib::MarkupError &ex)
  {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
    return 1;
  }
  catch (const Gtk::BuilderError &ex)
  {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
    return 1;
  }

  AppWindow appWindow(refBuilder);

  return app->run(appWindow);
}
