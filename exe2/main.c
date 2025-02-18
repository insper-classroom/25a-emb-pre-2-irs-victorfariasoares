#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED = 4;
volatile int desceu = 0;
volatile int subiu = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    desceu = 1;
  } else if (events == 0x8) { // rise edge
    subiu = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (desceu){
      gpio_put(LED, 1);
      desceu = 0;
    }
    else if(subiu){
      gpio_put(LED, 0);
      subiu = 0;
    }
  }
}
