#include <iostream>

#include "AppWindow.h"

int main(int argc, char *argv[])
{
  auto app =
      Gtk::Application::create(argc, argv,
                               "org.hhornbacher.idso1070.gui");

  // Load the GtkBuilder file and instantiate its widgets:
  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_file("./gui.glade");
    // refBuilder->add_from_file("/usr/local/share/idso1070-gui/gui.glade");
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

  AppWindow *pAppWindow = nullptr;
  refBuilder->get_widget_derived("AppWindowDerived", pAppWindow);
  if (pAppWindow)
  {
    app->run(*pAppWindow);
  }

  delete pAppWindow;

  return 0;
}
