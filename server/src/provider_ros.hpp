/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2022/04/01
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

#ifndef MIFSA_GNSS_PROVIDER_REALIZE_ROS_H
#define MIFSA_GNSS_PROVIDER_REALIZE_ROS_H

#ifdef MIFSA_SUPPORT_ROS

#include "mifsa/gnss/provider.h"
#include <rclcpp/rclcpp.hpp>
#include <mifsa/base/semaphore.h>
#include <mifsa/base/thread.h>
#include <mifsa_gnss_idl/msg/command.hpp>
#include <mifsa_gnss_idl/msg/location.hpp>
#include <mifsa_gnss_idl/srv/nmea.hpp>

using namespace mifsa_gnss_idl;

MIFSA_NAMESPACE_BEGIN

namespace Gnss {

class ProviderImplementation : public Provider {
public:
    ProviderImplementation()
    {
        Semaphore sema;
        m_thread.start([&]() {
            rclcpp::init(0, nullptr);
            m_node = rclcpp::Node::make_shared("mifsa_gnss_server");
            m_callbackGroup = m_node->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
            auto qosConfig = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
            m_locationPub = m_node->create_publisher<msg::Location>("/mifsa/gnss/location", qosConfig);
            m_commandSub = m_node->create_subscription<msg::Command>("/mifsa/gnss/command", qosConfig, [this](msg::Command::UniquePtr ros_command) {
                if (ros_command->type == msg::Command::START_NAVIGATION) {
                    if (cbStartNavigation) {
                        cbStartNavigation();
                    }
                } else if (ros_command->type == msg::Command::STOP_NAVIGATION) {
                    if (cbStopNavigation) {
                        cbStopNavigation();
                    }
                }
            });
            m_nmeaService = m_node->create_service<srv::Nmea>(
                "mifsa_gnss_server_nmea", [this](const std::shared_ptr<srv::Nmea::Request> request, std::shared_ptr<srv::Nmea::Response> response) {
                    if (request->type == srv::Nmea::Request::QUERY_NMEA) {
                        std::string nmea;
                        if (cbNmea) {
                            cbNmea(nmea);
                        }
                        response->data = nmea;
                    } else {
                        response->data = "";
                    }
                },
                ::rmw_qos_profile_default, m_callbackGroup);
            sema.reset();
            rclcpp::executors::MultiThreadedExecutor exec;
            exec.add_node(m_node);
            exec.spin();
        });
        sema.acquire();
    }
    ~ProviderImplementation()
    {
        rclcpp::shutdown();
        m_thread.stop();
    }
    virtual void setCbNmea(const CbNmea& cb) override
    {
        cbNmea = cb;
    }
    virtual void reportGnss(const Location& location) override
    {
        msg::Location ros_location;
        ros_location.size = location.size;
        ros_location.flags = location.flags;
        ros_location.latitude = location.latitude;
        ros_location.longitude = location.longitude;
        ros_location.altitude = location.altitude;
        ros_location.speed = location.speed;
        ros_location.bearing = location.bearing;
        ros_location.accuracy = location.accuracy;
        ros_location.timestamp = location.timestamp;
        ros_location.data = location.data;
        m_locationPub->publish(ros_location);
    }
    virtual void setCbStartNavigation(const CbStartNavigation& cb) override
    {
        cbStartNavigation = cb;
    }
    virtual void setCbStopNavigation(const CbStopNavigation& cb) override
    {
        cbStopNavigation = cb;
    }

private:
    Thread m_thread;
    rclcpp::Node::SharedPtr m_node;
    rclcpp::CallbackGroup::SharedPtr m_callbackGroup;
    rclcpp::Publisher<msg::Location>::SharedPtr m_locationPub;
    rclcpp::Subscription<msg::Command>::SharedPtr m_commandSub;
    rclcpp::Service<srv::Nmea>::SharedPtr m_nmeaService;
    //
    CbNmea cbNmea;
    CbStartNavigation cbStartNavigation;
    CbStopNavigation cbStopNavigation;
};

}

MIFSA_NAMESPACE_END

#endif

#endif // MIFSA_GNSS_PROVIDER_REALIZE_ROS_H
