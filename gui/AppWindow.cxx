#include "AppWindow.h"

AppWindow::AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Builder> &refGlade)
    : Window(cobject),
      refGlade(refGlade),
      pButtonConnect(nullptr),
      pDeviceInfo(nullptr),
      pChannelsInfo(nullptr),
      pTriggerInfo(nullptr),
      workerThread(nullptr)
{
    refGlade->get_widget("buttonConnect", pButtonConnect);
    refGlade->get_widget("labelDeviceInfo", pDeviceInfo);
    refGlade->get_widget("labelChannelsInfo", pChannelsInfo);
    refGlade->get_widget("labelTriggerInfo", pTriggerInfo);
    if (pButtonConnect)
    {
        pButtonConnect->signal_clicked().connect(sigc::mem_fun(*this, &AppWindow::onButtonConnect));
    }

    // Connect the handler to the dispatcher.
    dispatcher.connect(sigc::mem_fun(*this, &AppWindow::onNotificationFromWorker));

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
    worker.connect("/dev/ttyACM0");
}

void AppWindow::onNotificationFromWorker()
{
    if (workerThread && worker.hasStopped())
    {
        if (workerThread->joinable())
            workerThread->join();
        delete workerThread;
        workerThread = nullptr;
        close();
    }

    ustring label = ustring::compose("Progress: %1 %%", ustring::format(fixed, setprecision(1), worker.getProgress() * 100));
    pDeviceInfo->set_label(label);

    cout << label << endl;
}

void AppWindow::notify()
{
    dispatcher.emit();
}