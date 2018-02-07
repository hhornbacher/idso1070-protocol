#include "AppWindow.h"

AppWindow::AppWindow(const Glib::RefPtr<Gtk::Builder> &refGlade) : refGlade(refGlade),
                                                                   scopeWidget(&worker),
                                                                   pSettingsWidget(nullptr),
                                                                   boxHorizontal(ORIENTATION_HORIZONTAL)
{
    // Create main window
    set_title("IDSO1070 GUI");
    set_default_size(1800, 1000);

    add(boxHorizontal);

    boxHorizontal.pack_start(scopeWidget, PACK_EXPAND_WIDGET);

    refGlade->get_widget_derived("SettingsWidget", pSettingsWidget);
    pSettingsWidget->setWorker(&worker);
    boxHorizontal.pack_start(*pSettingsWidget, PACK_SHRINK);

    show_all_children();

    // Connect the handler to the dispatcher and start the worker thread ...
    worker.setNotifyHandler(bind(&AppWindow::notify, this));
    dispatcher.connect(sigc::mem_fun(*this, &AppWindow::onNotificationFromWorker));
    worker.startThread();

    // Create battery level update timer
    // updateBatteryTimer = signal_timeout().connect(sigc::mem_fun(*this, &AppWindow::onUpdateBatteryLevel), 5000);

    // Create graph update timer
    updateScopeTimer = signal_timeout().connect(sigc::mem_fun(*this, &AppWindow::onUpdateScope), 100);
}

AppWindow::~AppWindow()
{
    worker.stopThread();
    updateBatteryTimer.disconnect();
    updateScopeTimer.disconnect();
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

bool AppWindow::onUpdateScope()
{
    // if (worker.isSampling())
    // {
    scopeWidget.update();
    // }
    return true;
}

void AppWindow::notify()
{
    dispatcher.emit();
}