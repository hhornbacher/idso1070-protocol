#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(BaseObjectType *cobject, const RefPtr<Builder> &refGlade)
    : Box(cobject),
      refGlade(refGlade),
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
        if (!worker->isSampling())
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
            ustring selected = (*iter)[textComboColumns.value];
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
            ustring selected = (*iter)[textComboColumns.value];
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
            ustring selected = (*iter)[textComboColumns.value];
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
            ustring selected = (*iter)[textComboColumns.value];
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
            ustring selected = (*iter)[textComboColumns.value];
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
            ustring selected = (*iter)[textComboColumns.value];
        }
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

void SettingsWidget::updateDeviceInfo(IDSO1070 &deviceState)
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

        pTimeBase->set_active(pTimeBaseStore->children()[(int)deviceState.getDeviceTimeBase()]);
        pScopeMode->set_active(pScopeModeStore->children()[(int)deviceState.getDeviceScopeMode()]);
        pCaptureMode->set_active(pCaptureModeStore->children()[(int)deviceState.getDeviceCaptureMode()]);

        pBatteryLevel->set_value((double)deviceState.getBatteryLevel() / 100);

        if (!worker->isSampling())
        {
            pToggleSampling->set_label("Start sampling");
        }
        else
        {
            pToggleSampling->set_label("Stop sampling");
        }
    }
}

void SettingsWidget::updateChannelsInfo(IDSO1070 &deviceState)
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
        if (deviceState.isChannelEnabled(CHANNEL_1))
        {
            pChannel1Enabled->set_active(true);
            pChannel1Enabled->set_label("Disable");
        }
        else
        {
            pChannel1Enabled->set_active(false);
            pChannel1Enabled->set_label("Enable");
        }
        if (deviceState.isChannelEnabled(CHANNEL_2))
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

void SettingsWidget::updateTriggerInfo(IDSO1070 &deviceState)
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

        pTriggerMode->set_active(pTriggerModeStore->children()[(int)deviceState.getTriggerMode()]);
        pTriggerChannel->set_active(pTriggerChannelStore->children()[(int)deviceState.getTriggerChannel()]);
        pTriggerSlope->set_active(pTriggerSlopeStore->children()[(int)deviceState.getTriggerSlope()]);
    }
}