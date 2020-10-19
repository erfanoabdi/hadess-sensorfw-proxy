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

#include "sensorfw-core/console_log.h"
#include <cstring>
#include <iostream>

// Sensorfw
//#include "sensorfw_light_sensor.h"
#include "sensorfw-core/sensorfw_proximity_sensor.h"

char const* const log_tag = "main";

std::string the_dbus_bus_address()
{
    auto const address = std::unique_ptr<gchar, decltype(&g_free)>{
        g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SYSTEM, nullptr, nullptr),
        g_free};
    
    return address ? address.get() : std::string{};
}

void print_proximity(std::string const &s, repowerd::ProximityState state)
{
    if (state == repowerd::ProximityState::near)
        std::cout << s << "near" << std::endl;
    else
        std::cout << s << "far" << std::endl;
}

int main()
{
    auto const log = std::make_shared<repowerd::ConsoleLog>();
    std::shared_ptr<repowerd::ProximitySensor> proximity_sensor;

    log->log(log_tag, "Starting repowerd");

    try
    {
        proximity_sensor = std::make_shared<repowerd::SensorfwProximitySensor>(log,
            the_dbus_bus_address());
    }
    catch (std::exception const& e)
    {
        log->log(log_tag, "Failed to create SensorfwProximitySensor: %s", e.what());
        log->log(log_tag, "Falling back to NullProximitySensor");
    }

    auto registration = proximity_sensor->register_proximity_handler(
        [](repowerd::ProximityState state) {
            print_proximity("EVENT: ", state);
        });

    bool running = true;

    std::cout << "Commands (press enter after command letter): " << std::endl
              << "  e => enable proximity events" << std::endl
              << "  d => disable proximity events" << std::endl
              << "  s => display proximity state" << std::endl
              << "  q => quit" << std::endl;
    
    while (running)
    {
        std::string line;
        std::getline(std::cin, line);

        if (line == "q")
        {
            running = false;
        }
        else if (line == "d")
        {
            std::cout << "Disabling proximity events" << std::endl;
            proximity_sensor->disable_proximity_events();
        }
        else if (line == "e")
        {
            std::cout << "Enabling proximity events" << std::endl;
            proximity_sensor->enable_proximity_events();
        }
        else if (line == "s")
        {
            print_proximity("STATE: ", proximity_sensor->proximity_state());
        }
    }
    log->log(log_tag, "Exiting repowerd");
}
