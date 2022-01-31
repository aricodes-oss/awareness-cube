#include "nunchuk.hpp"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pins.hpp"
#include <cstdint>
#include <stdio.h>

i2c_inst_t *i2c = i2c0;
bool i2c_initialized = false;
bool nunchuk_initialized = false;
uint8_t nunchuk_report_data[2] = {0x00, 0x00};

void init_i2c() {
  if (i2c_initialized) {
    return;
  }

  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SCL_PIN);
  gpio_pull_up(SDA_PIN);
  i2c_init(i2c, 400 * 1000);

  puts("i2c init complete!");

  i2c_initialized = true;
}

void init_nunchuk() {
  init_i2c();
  if (nunchuk_initialized) {
    return;
  }

  puts("Sending nunchuk init 1");
  i2c_write_blocking(i2c, ADDR, INIT, 2, false);

  puts("Sending nunchuk init 2");
  i2c_write_blocking(i2c, ADDR, &INIT[2], 2, false);

  puts("Nunchuk init complete!");
  nunchuk_initialized = true;
}

void read_nunchuk() {
  i2c_write_blocking(i2c, ADDR, &STATUS_REPORT, 1, false);
  i2c_read_blocking(i2c, ADDR, nunchuk_report_data, 2, false);
}

void fetch_nunchuk_reports() {
  while (true) {
    read_nunchuk();
    sleep_ms(2);
  }
}
