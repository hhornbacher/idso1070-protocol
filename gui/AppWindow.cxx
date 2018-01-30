#include "AppWindow.h"

AppWindow::AppWindow(const Glib::RefPtr<Gtk::Builder> &refGlade) : refGlade(refGlade),
                                                                   pSettingsWidget(nullptr),
                                                                   boxHorizontal(ORIENTATION_HORIZONTAL)
{
    // Create main window
    set_title("IDSO1070 GUI");
    set_default_size(1000, 800);

    add(boxHorizontal);

    boxHorizontal.pack_start(graphWidget, PACK_EXPAND_WIDGET);

    refGlade->get_widget_derived("SettingsWidget", pSettingsWidget);
    pSettingsWidget->setWorker(&worker);
    boxHorizontal.pack_start(*pSettingsWidget, PACK_SHRINK);

    show_all_children();

    // Setup the worker thread ...
    // Connect the handler to the dispatcher.
    dispatcher.connect(sigc::mem_fun(*this, &AppWindow::onNotificationFromWorker));
    // Create the thread itself
    workerThread = new thread(
        [this] {
            worker.process(this);
        });

    // Create battery level update timer
    sigc::slot<bool> my_slot = sigc::mem_fun(*this, &AppWindow::onUpdateBatteryLevel);
    updateBatteryTimer = Glib::signal_timeout().connect(my_slot, 5000);
}

AppWindow::~AppWindow()
{
    if (!workerThread)
    {
        worker.stop();
        if (workerThread->joinable())
            workerThread->join();
    }
}

void AppWindow::onNotificationFromWorker()
{
    pSettingsWidget->update();
}

bool AppWindow::onUpdateBatteryLevel()
{
    if (worker.isConnected())
    {
        worker.readBatteryLevel();
    }
    return true;
}

void AppWindow::notify()
{
    dispatcher.emit();
}