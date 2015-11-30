/* Fuel gauge driver that uses I2C software bit banging. The I2C bit banging
 * and the fuel gauge driver have to be combined due to the dependency of the
 * fuel gauge functions on the gpio pin configuration functions.
 *
 * TODO/FIXME try to separate I2C from fuel driver.
 */

#ifndef FUEL_GAUGE_H
#define FUEL_GAUGE_H

/**
 * Get the battery level from the fuel gauge with resolution of 1%.
 *
 * Returns the battery level as a uint8_t in the range [0, 100].
 */
uint8_t fuel_get_battery_level();

#endif
