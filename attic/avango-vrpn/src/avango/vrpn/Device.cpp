#include <osg/Vec4>
#include <osg/Geode>

#include <iostream>

#include <avango/Logger.h>
#include <avango/osg/Types.h>

#include <avango/vrpn/Device.h>

#include <boost/bind.hpp>

#include "vrpn_Tracker.h"
#include "vrpn_Button.h"
#include "vrpn_Analog.h"

using namespace std;

namespace
{
av::Logger& logger(av::getLogger("av::vrpn::Device"));

void VRPN_CALLBACK handle_analog_cb(void* userdata, const vrpn_ANALOGCB a)
{
    av::vrpn::Device* device = static_cast<av::vrpn::Device*>(userdata);
    device->handleAnalog(a);
}

void VRPN_CALLBACK handle_button_cb(void* userdata, const vrpn_BUTTONCB b)
{
    av::vrpn::Device* device = static_cast<av::vrpn::Device*>(userdata);
    device->handleButton(b);
}

void VRPN_CALLBACK handle_tracker_cb(void* userdata, const vrpn_TRACKERCB t)
{
    av::vrpn::Device* device = static_cast<av::vrpn::Device*>(userdata);
    device->handleTracker(t);
}
} // namespace

AV_FC_DEFINE(av::vrpn::Device);

AV_FIELD_DEFINE(av::vrpn::SFDevice);
AV_FIELD_DEFINE(av::vrpn::MFDevice);

av::vrpn::Device::Device() : ::av::FieldContainer()
{
    vrpnConnected = false;
    mVRPNID = "";

    AV_FC_ADD_FIELD(Buttons, MFButton::ContainerType());
    AV_FC_ADD_FIELD(Channels, MFFloat::ContainerType());
    AV_FC_ADD_FIELD(VRPNPort, 3883);
    AV_FC_ADD_FIELD(TrackerInfo, MFTrackerInformation::ContainerType());
    AV_FC_ADD_ADAPTOR_FIELD(VRPNID, boost::bind(&av::vrpn::Device::getVRPNIDCB, this, _1), boost::bind(&av::vrpn::Device::setVRPNIDCB, this, _1));
}

av::vrpn::Device::~Device() { closeVRPNConnections(); }

void av::vrpn::Device::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::vrpn::Device, true);

        SFDevice::initClass("av::vrpn::SFDevice", "av::Field");
        MFDevice::initClass("av::vrpn::MFDevice", "av::Field");
    }
}

/* virtual */ void av::vrpn::Device::getVRPNIDCB(const av::SFString::GetValueEvent& event) { *(event.getValuePtr()) = mVRPNID; }

/* virtual */ void av::vrpn::Device::setVRPNIDCB(const av::SFString::SetValueEvent& event)
{
    const std::string vrpnId = (event.getValue());
    if(vrpnId != mVRPNID)
    {
        mVRPNID = vrpnId;
        if(vrpnConnected)
        {
            closeVRPNConnections();
        }
        openVRPNConnections();
    }
}

void av::vrpn::Device::openVRPNConnections()
{
    // mVRPNConnection = boost::shared_ptr<vrpn_Connection>(vrpn_create_server_connection(VRPNPort.getValue()));

    std::cout << "openVRPNConnections: " << mVRPNID << std::endl;
    mVRPNAnalog = boost::shared_ptr<vrpn_Analog_Remote>(new vrpn_Analog_Remote(mVRPNID.c_str()));
    mVRPNButton = boost::shared_ptr<vrpn_Button_Remote>(new vrpn_Button_Remote(mVRPNID.c_str()));
    mVRPNTracker = boost::shared_ptr<vrpn_Tracker_Remote>(new vrpn_Tracker_Remote(mVRPNID.c_str()));

    mVRPNAnalog->register_change_handler(this, static_cast<vrpn_ANALOGCHANGEHANDLER>(::handle_analog_cb));
    mVRPNButton->register_change_handler(this, static_cast<vrpn_BUTTONCHANGEHANDLER>(::handle_button_cb));
    mVRPNTracker->register_change_handler(this, static_cast<vrpn_TRACKERCHANGEHANDLER>(::handle_tracker_cb));

    vrpnConnected = true;

    openVRPNConnectionsTemplateFunc();
}

void av::vrpn::Device::closeVRPNConnections()
{
    if(vrpnConnected)
    {
        mVRPNAnalog->unregister_change_handler(this, static_cast<vrpn_ANALOGCHANGEHANDLER>(::handle_analog_cb));
        mVRPNButton->unregister_change_handler(this, static_cast<vrpn_BUTTONCHANGEHANDLER>(::handle_button_cb));
        mVRPNTracker->unregister_change_handler(this, static_cast<vrpn_TRACKERCHANGEHANDLER>(::handle_tracker_cb));
        vrpnConnected = false;
    }

    closeVRPNConnectionsTemplateFunc();
}

void av::vrpn::Device::handleAnalog(const vrpn_ANALOGCB a)
{
    std::vector<float> channels;
    channels.reserve(a.num_channel);

    for(int i = 0; i < a.num_channel; i++)
    {
        channels[i] = a.channel[i];
    }
    Channels.setValue(channels);

    handleAnalogTemplFunc(a);
}

void av::vrpn::Device::handleButton(const vrpn_BUTTONCB b)
{
    int buttonNumber = b.button;

    // check if the button is already in the list
    bool buttonKnown = false;
    MFButton::ContainerType::iterator iter;
    MFButton::ContainerType buttons = Buttons.getValue();
    for(iter = buttons.begin(); iter != buttons.end(); ++iter)
    {
        if((*iter)->Number.getValue() == buttonNumber)
        {
            (*iter)->State.setValue(b.state);
            Buttons.touch();
            buttonKnown = true;
            break;
        }
    }

    // new button
    if(!buttonKnown)
    {
        av::Link<Button> button = av::Link<Button>(new Button());
        button->Number.setValue(b.button);
        button->State.setValue(b.state);
        Buttons.add1Value(button);
    }
    handleButtonTemplFunc(b);
}

void av::vrpn::Device::handleTracker(const vrpn_TRACKERCB t)
{
    long sensorNumber = t.sensor;
    ::osg::Vec3 pos = ::osg::Vec3(t.pos[0], t.pos[1], t.pos[2]);
    ::osg::Quat quat = ::osg::Quat(t.quat[0], t.quat[1], t.quat[2], t.quat[3]);

    // check if the trackerInfo is already in the list
    bool sensorKnown = false;
    MFTrackerInformation::ContainerType::iterator iter;
    MFTrackerInformation::ContainerType trackerInfo = TrackerInfo.getValue();

    for(iter = trackerInfo.begin(); iter != trackerInfo.end(); ++iter)
    {
        if((*iter)->Number.getValue() == sensorNumber)
        {
            (*iter)->Pos.setValue(pos);
            (*iter)->Quat.setValue(quat);

            TrackerInfo.touch();
            sensorKnown = true;
            break;
        }
    }

    // new tracking info
    if(!sensorKnown)
    {
        av::Link<TrackerInformation> info = av::Link<TrackerInformation>(new TrackerInformation());

        info->Number.setValue(sensorNumber);
        info->Pos.setValue(pos);
        info->Quat.setValue(quat);

        TrackerInfo.add1Value(info);
    }

    handleTrackerTemplFunc(t);
}

/*virtual */ void av::vrpn::Device::triggerVRPNMainLoop()
{
    // std::cout << "triggerVRPNMainLoop" << std::endl;
    if(mVRPNAnalog.get())
        mVRPNAnalog->mainloop();
    if(mVRPNButton.get())
        mVRPNButton->mainloop();
    if(mVRPNTracker.get())
        mVRPNTracker->mainloop();

    triggerVRPNMainLoopTemplateFunc();

    vrpn_SleepMsecs(1);
}
