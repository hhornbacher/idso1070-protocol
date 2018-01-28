#include "AppWindow.h"

AppWindow::AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Builder> &refGlade)
    : Window(cobject),
      refGlade(refGlade),
      pButtonConnect(nullptr),
      pDeviceInfo(nullptr),
      pChannelsInfo(nullptr),
      pTriggerInfo(nullptr),
      pProgressbarConnection(nullptr),
      workerThread(nullptr)
{
    // Get references to widgets from glade file
    refGlade->get_widget("buttonConnect", pButtonConnect);
    refGlade->get_widget("labelDeviceInfo", pDeviceInfo);
    refGlade->get_widget("labelChannelsInfo", pChannelsInfo);
    refGlade->get_widget("labelTriggerInfo", pTriggerInfo);
    refGlade->get_widget("progressbarConnection", pProgressbarConnection);
    if (pButtonConnect)
    {
        pButtonConnect->signal_clicked().connect(sigc::mem_fun(*this, &AppWindow::onButtonConnect));
    }

    // Setup the worker thread

    // Connect the handler to the dispatcher.
    dispatcher.connect(sigc::mem_fun(*this, &AppWindow::onNotificationFromWorker));
    // Create the thread itself
    workerThread = new thread(
        [this] {
            worker.process(this);
        });
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

void AppWindow::onButtonConnect()
{
    if (!worker.isConnecting())
    {
        if (!worker.isConnected())
        {
            worker.connect("/dev/ttyACM0");
        }
        else
        {
            worker.disconnect();
        }
    }
}

void AppWindow::onNotificationFromWorker()
{
    IDSO1070 currentDeviceState;
    worker.getDevice(currentDeviceState);
    updateConnectionControls(currentDeviceState);
    updateDeviceInfo(currentDeviceState);
    updateChannelsInfo(currentDeviceState);
    updateTriggerInfo(currentDeviceState);
}

void AppWindow::updateConnectionControls(IDSO1070 &deviceState)
{
    if (worker.isConnecting())
    {
        pProgressbarConnection->set_fraction((double)worker.getProgress());
        pButtonConnect->set_label("Connecting...");
        pButtonConnect->set_sensitive(false);
    }
    else
    {
        pButtonConnect->set_sensitive(true);
        if (worker.isConnected())
        {
            pButtonConnect->set_label("Disconnect");
            pProgressbarConnection->set_fraction(1.0);
        }
        else
        {
            pButtonConnect->set_label("Connect");
            pProgressbarConnection->set_fraction(0.0);
        }
    }
}

void AppWindow::updateDeviceInfo(IDSO1070 &deviceState)
{
    if (!worker.isConnected())
    {
        pDeviceInfo->set_label("Not connected!");
    }
    else
    {
        ustring deviceInfo = ustring::compose(
            "User name: %1\n"
            "Product name: %2",
            deviceState.getUserName(),
            deviceState.getProductName());

        pDeviceInfo->set_label(deviceInfo);
    }
}

void AppWindow::updateChannelsInfo(IDSO1070 &deviceState)
{
    if (!worker.isConnected())
    {
        pChannelsInfo->set_label("Not connected!");
    }
    else
    {
        ustring channelsInfo = ustring::compose(
            "Channel 1: %1\n"
            "Channel 2: %2",
            deviceState.isChannelEnabled(CHANNEL_1) ? "ON" : "OFF",
            deviceState.isChannelEnabled(CHANNEL_2) ? "ON" : "OFF");

        pChannelsInfo->set_label(channelsInfo);
    }
}

void AppWindow::updateTriggerInfo(IDSO1070 &deviceState)
{
    if (!worker.isConnected())
    {
        pTriggerInfo->set_label("Not connected!");
    }
    else
    {
        ustring triggerInfo = ustring::compose(
            "Trigger Channel: %1",
            deviceState.getTriggerChannel());

        pTriggerInfo->set_label(triggerInfo);
    }
}

void AppWindow::notify()
{
    dispatcher.emit();
}