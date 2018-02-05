#include "AppWindow.h"

#include <iostream>

extern char resource_glade_data_start[];

int main(int argc, char *argv[])
{
  auto app =
      Gtk::Application::create(argc, argv,
                               "org.hhornbacher.idso1070.gui");

  // Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    ustring gladeData = (char *)resource_glade_data_start;
    refBuilder->add_from_string(gladeData);
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
