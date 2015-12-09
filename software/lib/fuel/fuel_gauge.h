/* This file is part of eecs473-fitness-watch.
 *   
 * The code / board schematics created by the authors of eecs473-fitness-watch
 * are free software/hardware: you can redistribute them and/or modify them
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * The code / board schematics are distributed in the hope that they will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the code / board schematics.  If not, see <http://www.gnu.org/licenses/>.
 *
 * All code / schematics not created by the authors of this repository fall
 * under their original licenses.
 *
 *
 * Fuel gauge driver that uses I2C software bit banging. The I2C bit banging
 * and the fuel gauge driver have to be combined due to the dependency of the
 * fuel gauge functions on the gpio pin configuration functions.
 *
 * TODO/FIXME try to separate I2C from fuel driver.
 */

#ifndef FUEL_GAUGE_H
#define FUEL_GAUGE_H

/**
 * Initialize the fuel gauge.
 *
 * sda -- the pin number to use for SW I2C SDA.
 * scl -- the pin number to use for SW I2C SCL.
 * qst -- the pin number to use for the fuel gauge qst pin.
 */
void fuel_init(uint32_t sda, uint32_t scl, uint32_t qst);

/**
 * Get the battery level from the fuel gauge with resolution of 1%.
 *
 * Returns the battery level as a uint8_t in the range [0, 100].
 */
uint8_t fuel_get_battery_level();

/**
 * Restart fuel-gauge calculations in the same manner as initial power-up of
 * the IC. If an application’s power-up sequence is exceedingly noisy such that
 * excess error is introduced into the IC’s “first guess” of SOC, the host can
 * issue a quick-start to reduce the error.
 *
 * software -- if true, do a software quickstart, else use the QST pin.
 */
void fuel_quick_start(bool software);

#endif
