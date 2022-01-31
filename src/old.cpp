#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/binary_info/code.h"
#include "pico/error.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "joybusComms.hpp"
#include <stdio.h>

const uint LED_PIN = 25;
const uint SCL_PIN = 8;
const uint SDA_PIN = 9;

const uint8_t INIT_0[2] = {0x40, 0x00};
const uint8_t INIT_1[2] = {0xF0, 0x55};
const uint8_t INIT_2[2] = {0xFB, 0x00};
const uint8_t IDENT_MESSAGE = 0x20;
const uint8_t STATUS_MESSAGE = 0x00;

const uint8_t ADDR = 0x52;

int main() {
  bi_decl(bi_program_description("Reads I2C from nunchuk"));
  bi_decl(bi_2pins_with_func(SCL_PIN, SDA_PIN, GPIO_FUNC_I2C));
  puts("Awaiting poll...");
  initComms(28, 133);
  awaitPoll();

  i2c_inst_t *i2c = i2c0;
  uint8_t data[6] = {0, 0, 0, 0, 0, 0};
  int bytes_read;

  stdio_init_all();
  gpio_put(LED_PIN, 1);

  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SCL_PIN);
  gpio_pull_up(SDA_PIN);
  i2c_init(i2c, 400 * 1000);

  puts("Sending init 1");
  bytes_read = i2c_write_blocking(i2c, ADDR, INIT_1, 2, false);

  puts("Sending init 2");
  i2c_write_blocking(i2c, ADDR, INIT_2, 2, false);

  puts("Init complete");
  puts("Starting read loop");

  while (true) {
    bytes_read = i2c_read_blocking(i2c, ADDR, data, 6, false);
    i2c_write_blocking(i2c, ADDR, &STATUS_MESSAGE, 1, false);
    printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", data[0], data[1], data[2],
           data[3], data[4], data[5]);
    sleep_ms(1000);
  }

  return 0;
}
