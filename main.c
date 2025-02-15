#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "ssd1306.h"
#include "hardware/i2c.h" 
#include "hardware/adc.h" 

#define BUTTON_A 5
#define BUTTON_B 6
#define LED_COUNT 25
#define LED_PIN 7
#define MAX_BRIGHTNESS 255
uint8_t brightnessLevel = MAX_BRIGHTNESS;

const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;
uint8_t buf[SSD1306_BUF_LEN];
struct render_area frame_area = {
    .start_col = 0,
    .end_col = SSD1306_WIDTH - 1,
    .start_page = 0,
    .end_page = SSD1306_NUM_PAGES - 1
};

struct pixel_t {
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
pixel_t leds[LED_COUNT];

PIO np_pio;
uint sm;

void npInit(uint pin);
void setBrightness(uint8_t brightness);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npWrite();
void initDisplay();
void displayMessage(const char *msg);
int getIndex(int x, int y);

int main() {
    stdio_init_all();
    sleep_ms(5000);

    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    npInit(LED_PIN);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    initDisplay();
    displayMessage("   Starting");
    setBrightness(128);
    sleep_ms(2000);

    uint8_t colors[3][3] = {
        {255, 0, 0},   
        {0, 0, 255},   
        {0, 255, 0}    
    };
    int color_index = 0;
    bool prev_button_a = false;
    bool prev_button_b = false;

    while (true) {
        displayMessage("Press A or B");

        adc_select_input(0);
        uint adc_y_raw = adc_read();
        adc_select_input(1);
        uint adc_x_raw = adc_read();
        const uint adc_max = (1 << 12) - 1;
        uint col = adc_x_raw * 5 / (adc_max + 1);
        uint row = 4 - (adc_y_raw * 5 / (adc_max + 1));
        int pos = getIndex(col, row);

        bool current_button_a = !gpio_get(BUTTON_A);
        bool current_button_b = !gpio_get(BUTTON_B);

        if (current_button_a && !prev_button_a) {
            color_index = (color_index + 1) % 3;
        } 
        if (current_button_b && !prev_button_b) {
            color_index = (color_index - 1 + 3) % 3;  
        }

        prev_button_a = current_button_a;
        prev_button_b = current_button_b;

        npSetLED(pos, colors[color_index][0], colors[color_index][1], colors[color_index][2]);
        npWrite();

        sleep_ms(100);
    }
    return 0;
}

void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

void setBrightness(uint8_t brightness) {
    if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    }
    brightnessLevel = brightness;
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (r * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].G = (g * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].B = (b * brightnessLevel) / MAX_BRIGHTNESS;
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

void initDisplay() {
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    SSD1306_init();
}

void displayMessage(const char *msg) {
    calc_render_area_buflen(&frame_area);
    memset(buf, 0, SSD1306_BUF_LEN);
    WriteString(buf, 5, 0, (char *)msg);
    render(buf, &frame_area);
}

int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x);
    } else {
        return 24 - (y * 5 + (4 - x));
    }
}