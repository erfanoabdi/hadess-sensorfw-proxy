/*
 * Copyright © 2016 Canonical Ltd.
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
 * Authored by: Alexandros Frantzis <alexandros.frantzis@canonical.com>
 */

#pragma once

#include "handler_registration.h"

#include <functional>

namespace repowerd
{

struct AccelerometerData {
    int accel_x;
    int accel_y;
    int accel_z;
};

using AccelerometerHandler = std::function<void(AccelerometerData)>;

class AccelerometerSensor
{
public:
    virtual ~AccelerometerSensor() = default;

    virtual HandlerRegistration register_accelerometer_handler(
        AccelerometerHandler const& handler) = 0;

    virtual void enable_accelerometer_events() = 0;
    virtual void disable_accelerometer_events() = 0;

protected:
    AccelerometerSensor() = default;
    AccelerometerSensor (AccelerometerSensor const&) = default;
    AccelerometerSensor& operator=(AccelerometerSensor const&) = default;
};

}
