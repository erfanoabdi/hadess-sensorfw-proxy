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

#include "sensorfw_compass_sensor.h"
#include "event_loop_handler_registration.h"

#include "socketreader.h"

#include <stdexcept>

namespace
{
auto const null_handler = [](int){};
}

repowerd::SensorfwCompassSensor::SensorfwCompassSensor(
    std::shared_ptr<Log> const& log,
    std::string const& dbus_bus_address)
    : Sensorfw(log, dbus_bus_address, "Compass", PluginType::COMPASS),
      handler{null_handler}
{
}

repowerd::HandlerRegistration repowerd::SensorfwCompassSensor::register_compass_handler(
    CompassHandler const& handler)
{
    return EventLoopHandlerRegistration{
        dbus_event_loop,
        [this, &handler]{ this->handler = handler; },
        [this]{ this->handler = null_handler; }};
}

void repowerd::SensorfwCompassSensor::enable_compass_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            start();
        }).get();
}

void repowerd::SensorfwCompassSensor::disable_compass_events()
{
    dbus_event_loop.enqueue(
        [this]
        {
            stop();
        }).get();
}

void repowerd::SensorfwCompassSensor::data_recived_impl()
{
    QVector<CompassData> values;
    if(!m_socket->read<CompassData>(values))
        return;

    handler(values[0].degrees_);
}
