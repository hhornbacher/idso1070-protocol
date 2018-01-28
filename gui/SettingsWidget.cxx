#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(BaseObjectType *cobject, const Glib::RefPtr<Builder> &refGlade)
    : Box(cobject),
      refGlade(refGlade),
      pButtonConnect(nullptr),
      pProgressbarConnection(nullptr),
      pChannel1Enabled(nullptr),
      pChannel2Enabled(nullptr),
      worker(nullptr)
{
    // Get references to widgets from glade file
    refGlade->get_widget("buttonConnect", pButtonConnect);
    pButtonConnect->signal_clicked().connect(sigc::mem_fun(*this, &SettingsWidget::onButtonConnect));

    refGlade->get_widget("checkbuttonChannel1Enabled", pChannel1Enabled);
    pChannel1Enabled->signal_toggled().connect(sigc::mem_fun(*this, &SettingsWidget::onToggleChannel1Enable));

    refGlade->get_widget("checkbuttonChannel2Enabled", pChannel2Enabled);
    pChannel2Enabled->signal_toggled().connect(sigc::mem_fun(*this, &SettingsWidget::onToggleChannel2Enable));

    refGlade->get_widget("progressbarConnection", pProgressbarConnection);
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::setWorker(ProtocolWorker *worker)
{
    this->worker = worker;
}

void SettingsWidget::onToggleChannel1Enable()
{
}

void SettingsWidget::onToggleChannel2Enable()
{
}

void SettingsWidget::onButtonConnect()
{
    if (worker->isConnected())
    {
        worker->disconnect();
    }
    else
    {
        worker->connect("/dev/ttyACM0");
    }
}

void SettingsWidget::update()
{
    IDSO1070 currentDeviceState;
    worker->getDevice(currentDeviceState);
    updateConnectionControls(currentDeviceState);
    updateDeviceInfo(currentDeviceState);
    updateChannelsInfo(currentDeviceState);
    updateTriggerInfo(currentDeviceState);
}

void SettingsWidget::updateConnectionControls(IDSO1070 &deviceState)
{
    if (worker->isConnecting())
    {
        pProgressbarConnection->set_fraction((double)worker->getProgress());
        pButtonConnect->set_label("Connecting...");
        pButtonConnect->set_sensitive(false);
    }
    else
    {
        pButtonConnect->set_sensitive(true);
        if (worker->isConnected())
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

void SettingsWidget::updateDeviceInfo(IDSO1070 &deviceState)
{
    if (!worker->isConnected())
    {
    }
    else
    {
    }
}

void SettingsWidget::updateChannelsInfo(IDSO1070 &deviceState)
{
    if (worker->isConnecting())
    {
        pChannel1Enabled->set_sensitive(false);
        pChannel2Enabled->set_sensitive(false);
    }
    else
    {
        if (!worker->isConnected())
        {
            pChannel1Enabled->set_sensitive(false);
            pChannel2Enabled->set_sensitive(false);
        }
        else
        {

            pChannel1Enabled->set_sensitive(true);
            pChannel2Enabled->set_sensitive(true);
            pChannel1Enabled->set_active(deviceState.isChannelEnabled(CHANNEL_1));
            pChannel2Enabled->set_active(deviceState.isChannelEnabled(CHANNEL_2));
        }
    }
}

void SettingsWidget::updateTriggerInfo(IDSO1070 &deviceState)
{
    if (!worker->isConnected())
    {
    }
    else
    {
    }
}