#include "SettingsWidget.h"

#include <sstream>
#include <iomanip>

SettingsWidget::SettingsWidget(BaseObjectType *cobject, const RefPtr<Builder> &refGlade)
    : Box(cobject),
      refGlade(refGlade),
      pTransmissionLogDialog(nullptr),
      pButtonTransmissionLog(nullptr),
      pButtonConnect(nullptr),
      pProgressbarConnection(nullptr),
      pConnectionStatus(nullptr),
      pBatteryLevel(nullptr),
      pDeviceInfo(nullptr),
      pToggleSampling(nullptr),
      pTimeBase(nullptr),
      pScopeMode(nullptr),
      pCaptureMode(nullptr),
      pTimeBaseStore(nullptr),
      pScopeModeStore(nullptr),
      pCaptureModeStore(nullptr),
      pChannel1Enabled(nullptr),
      pChannel2Enabled(nullptr),
      pTriggerMode(nullptr),
      pTriggerChannel(nullptr),
      pTriggerSlope(nullptr),
      pTriggerModeStore(nullptr),
      pTriggerChannelStore(nullptr),
      pTriggerSlopeStore(nullptr),
      worker(nullptr)
{
    // Get references to widgets from glade file
    refGlade->get_widget_derived("TransmissionLogDialog", pTransmissionLogDialog);
    refGlade->get_widget("buttonTransmissionLog", pButtonTransmissionLog);
    refGlade->get_widget("buttonConnect", pButtonConnect);
    refGlade->get_widget("progressConnection", pProgressbarConnection);
    refGlade->get_widget("labelConnectionStatus", pConnectionStatus);
    refGlade->get_widget("toggleSampling", pToggleSampling);
    refGlade->get_widget("levelBattery", pBatteryLevel);
    refGlade->get_widget("labelDeviceInfo", pDeviceInfo);
    refGlade->get_widget("comboTimeBase", pTimeBase);
    refGlade->get_widget("comboScopeMode", pScopeMode);
    refGlade->get_widget("comboCaptureMode", pCaptureMode);
    pTimeBaseStore = RefPtr<ListStore>::cast_dynamic(refGlade->get_object("TimeBase"));
    pScopeModeStore = RefPtr<ListStore>::cast_dynamic(refGlade->get_object("ScopeMode"));
    pCaptureModeStore = RefPtr<ListStore>::cast_dynamic(refGlade->get_object("CaptureMode"));
    refGlade->get_widget("toggleChannel1Enabled", pChannel1Enabled);
    refGlade->get_widget("toggleChannel2Enabled", pChannel2Enabled);
    refGlade->get_widget("comboTriggerMode", pTriggerMode);
    refGlade->get_widget("comboTriggerChannel", pTriggerChannel);
    refGlade->get_widget("comboTriggerSlope", pTriggerSlope);
    pTriggerModeStore = RefPtr<ListStore>::cast_dynamic(refGlade->get_object("TriggerMode"));
    pTriggerChannelStore = RefPtr<ListStore>::cast_dynamic(refGlade->get_object("TriggerChannel"));
    pTriggerSlopeStore = RefPtr<ListStore>::cast_dynamic(refGlade->get_object("TriggerSlope"));

    // Connect signals with handlers
    pButtonTransmissionLog->signal_clicked().connect(sigc::mem_fun(*this, &SettingsWidget::onButtonTransmissionLog));
    pButtonConnect->signal_clicked().connect(sigc::mem_fun(*this, &SettingsWidget::onButtonConnect));

    pToggleSampling->signal_toggled().connect(sigc::mem_fun(*this, &SettingsWidget::onToggleSampling));
    pChannel1Enabled->signal_toggled().connect(sigc::mem_fun(*this, &SettingsWidget::onToggleChannel1Enable));
    pChannel2Enabled->signal_toggled().connect(sigc::mem_fun(*this, &SettingsWidget::onToggleChannel2Enable));

    pTimeBase->signal_changed().connect(sigc::mem_fun(*this, &SettingsWidget::onTimeBaseSelected));
    pScopeMode->signal_changed().connect(sigc::mem_fun(*this, &SettingsWidget::onScopeModeSelected));
    pCaptureMode->signal_changed().connect(sigc::mem_fun(*this, &SettingsWidget::onCaptureModeSelected));

    pTriggerMode->signal_changed().connect(sigc::mem_fun(*this, &SettingsWidget::onTriggerModeSelected));
    pTriggerChannel->signal_changed().connect(sigc::mem_fun(*this, &SettingsWidget::onTriggerChannelSelected));
    pTriggerSlope->signal_changed().connect(sigc::mem_fun(*this, &SettingsWidget::onTriggerSlopeSelected));
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
    if (worker->isConnected())
    {
    }
}

void SettingsWidget::onToggleChannel2Enable()
{
    if (worker->isConnected())
    {
    }
}

void SettingsWidget::onButtonTransmissionLog()
{
    pTransmissionLogDialog->show();
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

void SettingsWidget::onToggleSampling()
{
    if (worker->isConnected())
    {

        IDSO1070 device;
        worker->fetchDevice(device);
        if (!device.isSampling())
            worker->startSampling();
        else
            worker->stopSampling();
    }
}

void SettingsWidget::onTimeBaseSelected()
{
    if (worker->isConnected())
    {
        ListStore::iterator iter = pTimeBase->get_active();
        if (iter)
        {
            TimeBase selected = (TimeBase)((int)(*iter)[textComboColumns.value]);
            worker->setTimeBase(selected);
        }
    }
}

void SettingsWidget::onScopeModeSelected()
{
    if (worker->isConnected())
    {
        ListStore::iterator iter = pTimeBase->get_active();
        if (iter)
        {
            ScopeMode selected = (ScopeMode)((int)(*iter)[textComboColumns.value]);
            worker->setScopeMode(selected);
        }
    }
}

void SettingsWidget::onCaptureModeSelected()
{
    if (worker->isConnected())
    {
        ListStore::iterator iter = pTimeBase->get_active();
        if (iter)
        {
            CaptureMode selected = (CaptureMode)((int)(*iter)[textComboColumns.value]);
            worker->setCaptureMode(selected);
        }
    }
}

void SettingsWidget::onTriggerModeSelected()
{
    if (worker->isConnected())
    {
        ListStore::iterator iter = pTimeBase->get_active();
        if (iter)
        {
            TriggerMode selected = (TriggerMode)((int)(*iter)[textComboColumns.value]);
            worker->setTriggerMode(selected);
        }
    }
}

void SettingsWidget::onTriggerChannelSelected()
{
    if (worker->isConnected())
    {
        ListStore::iterator iter = pTriggerChannel->get_active();
        if (iter)
        {
            TriggerChannel selected = (TriggerChannel)((int)(*iter)[textComboColumns.value]);
            worker->setTriggerChannel(selected);
        }
    }
}

void SettingsWidget::onTriggerSlopeSelected()
{
    if (worker->isConnected())
    {
        ListStore::iterator iter = pTriggerSlope->get_active();
        if (iter)
        {
            TriggerSlope selected = (TriggerSlope)((int)(*iter)[textComboColumns.value]);
            worker->setTriggerSlope(selected);
        }
    }
}

void SettingsWidget::update()
{
    worker->fetchDevice(device);
    updateConnectionControls();
    updateDeviceInfo();
    updateChannelsInfo();
    updateTriggerInfo();

    Protocol::TransmissionLog currentLog;
    worker->fetchTransmissionLog(currentLog);
    pTransmissionLogDialog->updateLog(currentLog);
}

void SettingsWidget::updateConnectionControls()
{
    if (worker->isConnecting())
    {
        pProgressbarConnection->set_fraction((double)worker->getProgress());
        pButtonConnect->set_sensitive(false);
        pConnectionStatus->set_label("Status: Connecting...");
    }
    else
    {
        pButtonConnect->set_sensitive(true);
        if (worker->isConnected())
        {
            pConnectionStatus->set_label("Status: Connected to USB @ /dev/ttyACM0");
            pButtonConnect->set_label("Disconnect");
            pProgressbarConnection->set_fraction(1.0);
        }
        else
        {
            if (worker->isConnectionLost())
            {
                string label = "Error: ";
                label += worker->getConnectionLostReason();
                pConnectionStatus->set_label(label);
            }
            else
                pConnectionStatus->set_label("Status: Not connected");
            pButtonConnect->set_label("Connect");
            pProgressbarConnection->set_fraction(0.0);
        }
    }
}

void SettingsWidget::updateDeviceInfo()
{
    if (worker->isConnecting() || !worker->isConnected())
    {
        pTimeBase->set_sensitive(false);
        pScopeMode->set_sensitive(false);
        pCaptureMode->set_sensitive(false);
        pToggleSampling->set_sensitive(false);

        pToggleSampling->set_label("Start sampling");
    }
    else
    {
        pTimeBase->set_sensitive(true);
        pScopeMode->set_sensitive(true);
        pCaptureMode->set_sensitive(true);
        pToggleSampling->set_sensitive(true);

        // pTimeBase->set_active(pTimeBaseStore->children()[(int)device.getTimeBase()]);
        // pScopeMode->set_active(pScopeModeStore->children()[(int)device.getScopeMode()]);
        // pCaptureMode->set_active(pCaptureModeStore->children()[(int)device.getCaptureMode()]);

        pBatteryLevel->set_value((double)device.getBatteryLevel() / 100);

        stringstream deviceInfo;

        deviceInfo << "Device Info:" << endl
                   << "Freq div: 0x" << setfill('0') << setw(8) << hex << (uint32_t)device.getTimeBase();

        pDeviceInfo->set_label(deviceInfo.str());

        if (!device.isSampling())
        {
            pToggleSampling->set_label("Start sampling");
            // pToggleSampling->set_active(true);
        }
        else
        {
            pToggleSampling->set_label("Stop sampling");
            // pToggleSampling->set_active(false);
        }
    }
}

void SettingsWidget::updateChannelsInfo()
{
    if (worker->isConnecting() || !worker->isConnected())
    {
        pChannel1Enabled->set_sensitive(false);
        pChannel2Enabled->set_sensitive(false);
    }
    else
    {
        pChannel1Enabled->set_sensitive(true);
        pChannel2Enabled->set_sensitive(true);
        if (device.isChannelEnabled(CHANNEL_1))
        {
            pChannel1Enabled->set_active(true);
            pChannel1Enabled->set_label("Disable");
        }
        else
        {
            pChannel1Enabled->set_active(false);
            pChannel1Enabled->set_label("Enable");
        }
        if (device.isChannelEnabled(CHANNEL_2))
        {
            pChannel2Enabled->set_active(true);
            pChannel2Enabled->set_label("Disable");
        }
        else
        {
            pChannel2Enabled->set_active(false);
            pChannel2Enabled->set_label("Enable");
        }
    }
}

void SettingsWidget::updateTriggerInfo()
{
    if (worker->isConnecting() || !worker->isConnected())
    {
        pTriggerMode->set_sensitive(false);
        pTriggerChannel->set_sensitive(false);
        pTriggerSlope->set_sensitive(false);
    }
    else
    {
        pTriggerMode->set_sensitive(true);
        pTriggerChannel->set_sensitive(true);
        pTriggerSlope->set_sensitive(true);

        // pTriggerMode->set_active(pTriggerModeStore->children()[(int)device.getTriggerMode()]);
        // pTriggerChannel->set_active(pTriggerChannelStore->children()[(int)device.getTriggerChannel()]);
        // pTriggerSlope->set_active(pTriggerSlopeStore->children()[(int)device.getTriggerSlope()]);
    }
}