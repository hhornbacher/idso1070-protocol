#include "AppWindow.h"

AppWindow::AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Builder> &refGlade)
    : Window(cobject),
      refGlade(refGlade),
      pButtonConnect(nullptr),
      pDeviceInfo(nullptr),
      pChannelsInfo(nullptr),
      pTriggerInfo(nullptr)
{
    refGlade->get_widget("buttonConnect", pButtonConnect);
    refGlade->get_widget("labelDeviceInfo", pDeviceInfo);
    refGlade->get_widget("labelChannelsInfo", pChannelsInfo);
    refGlade->get_widget("labelTriggerInfo", pTriggerInfo);
    if (pButtonConnect)
    {
        pButtonConnect->signal_clicked().connect(sigc::mem_fun(*this, &AppWindow::onButtonConnect));
    }
}

// The first two parameters are mandatory in a constructor that will be called
// from Builder::get_widget_derived().
// Additional parameters, if any, correspond to additional arguments in the call
// to Builder::get_widget_derived().
// AppWindow::AppWindow(BaseObjectType *cobject, const Glib::RefPtr<Builder> &refGlade,
//                      bool is_glad)
//     : AppWindow(cobject, refGlade) // Delegate to the other constructor
// {
//     // Show an icon.
//     auto pImage = manage(new Image());
//     pImage->set_from_icon_name("face-smile" : "face-sad");
//     pImage->set_icon_size(IconSize::LARGE);
//     get_content_area()->pack_start(*pImage, PackOptions::EXPAND_WIDGET);
// }

AppWindow::~AppWindow()
{
}

void AppWindow::onButtonConnect()
{
    printf("Connect!\n");
    pDeviceInfo->set_label("Connecting...");
}