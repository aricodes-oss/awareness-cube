#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "nunchuk.hpp"
#include "pico/binary_info.h"
#include "pico/binary_info/code.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include <stdio.h>

int main() {
  bi_decl(bi_program_description(
      "This is firmware for a frame1/b0xx-style controller"));

  stdio_init_all();
  init_nunchuk();
  puts("We here!");

  return 0;
}
