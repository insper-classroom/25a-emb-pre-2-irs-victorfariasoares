#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_RED = 4;
const int LED_GREEN = 6;
volatile int red_status = 0;
volatile int green_status = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4 && gpio == BTN_PIN_R) { // fall edge
    red_status = 1;
  }
  else if (events == 0x8 && gpio == BTN_PIN_G) { // rise edge
    green_status = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  gpio_init(LED_RED);
  gpio_init(LED_GREEN);

  gpio_set_dir(LED_RED, GPIO_OUT);
  gpio_set_dir(LED_GREEN, GPIO_OUT);

  // callback led red (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                     &btn_callback);

  // callback led green (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  int led_red = 0;
  int led_green = 0;
  while (true) {
    if (red_status == 1){
      led_red = !led_red;
      gpio_put(LED_RED, led_red);
      red_status = 0;
    }
    else if (green_status == 1){
      led_green = !led_green;
      gpio_put(LED_GREEN, led_green);
      green_status = 0;
    }
  }
}
