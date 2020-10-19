/*
 * Copyright © 2020 UBports foundation
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Marius Gripsgard <marius@ubports.com>
 */

#include "sensorfw_accelerometer_sensor.h"
#include "event_loop_handler_registration.h"

#include "socketreader.h"

#include <stdexcept>

namespace
{
auto const null_handler = [](repowerd::AccelerometerData){};
}

repowerd::SensorfwAccelerometerSensor::SensorfwAccelerometerSensor(
    std::shared_ptr<Log> const& log,
    std::string const& dbus_bus_address)
    : Sensorfw(log, dbus_bus_address, "Accelerometer", PluginType::ACCELEROMETER),
      handler{null_handler}
{
}

repowerd::HandlerRegistration repowerd::SensorfwAccelerometerSensor::register_accelerometer_handler(
    AccelerometerHandler const& handler)
{
    return EventLoopHandlerRegistration{
        dbus_event_loop,
        [this, &handler]{ this->handler = handler; },
        [this]{ this->handler = null_handler; }};
}

void repowerd::SensorfwAccelerometerSensor::enable_accelerometer_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            start();
        }).get();
}

void repowerd::SensorfwAccelerometerSensor::disable_accelerometer_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            stop();
        }).get();
}

void repowerd::SensorfwAccelerometerSensor::data_recived_impl()
{
    QVector<AccelerationData> values;
    repowerd::AccelerometerData output;
    if(m_socket->read<AccelerationData>(values)) {
        output.accel_x = values[0].x_;
        output.accel_y = values[0].y_;
        output.accel_z = values[0].z_;
    }

    handler(output);
}
