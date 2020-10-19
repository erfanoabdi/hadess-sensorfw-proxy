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

#pragma once

#include "accelerometer_sensor.h"
#include "sensorfw_common.h"

namespace repowerd
{

class SensorfwAccelerometerSensor : public AccelerometerSensor, public Sensorfw
{
public:
    SensorfwAccelerometerSensor(std::shared_ptr<Log> const& log,
                        std::string const& dbus_bus_address);

    HandlerRegistration register_accelerometer_handler(AccelerometerHandler const& handler) override;

    void enable_accelerometer_events() override;
    void disable_accelerometer_events() override;
private:
    void data_recived_impl() override;

    AccelerometerHandler handler;
};

}
