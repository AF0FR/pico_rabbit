#include <stdint.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "config.h"
#include "rabbit_hw.h"

static uint tone_slice;
static uint tone_channel;
static bool tone_is_on = false;

uint32_t rabbit_hw_now_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

static uint16_t rabbit_hw_tone_duty(void) {
    uint16_t wrap = pwm_hw->slice[tone_slice].top;
    return wrap / 2;
}

static uint16_t raised_cosine_level(uint16_t max_duty, uint step, uint steps) {
    if (step >= steps) {
        return max_duty;
    }

    /*
     * Smoothstep approximation:
     * y = 3x^2 - 2x^3
     *
     * Uses integer math scaled to 0..1000.
     */
    uint32_t x = (step * 1000u) / steps;
    uint32_t y = (3u * x * x) / 1000u;
    y -= (2u * x * x * x) / 1000000u;

    return (uint16_t)(((uint32_t)max_duty * y) / 1000u);
}

static void ramp_tone_up(void) {
    uint16_t max_duty = rabbit_hw_tone_duty();
    uint delay_us = (CW_ENVELOPE_MS * 1000u) / CW_ENVELOPE_STEPS;

    for (uint step = 0; step <= CW_ENVELOPE_STEPS; step++) {
        uint16_t level = raised_cosine_level(max_duty, step, CW_ENVELOPE_STEPS);
        pwm_set_chan_level(tone_slice, tone_channel, level);
        sleep_us(delay_us);
    }

    tone_is_on = true;
}

static void ramp_tone_down(void) {
    uint16_t max_duty = rabbit_hw_tone_duty();
    uint delay_us = (CW_ENVELOPE_MS * 1000u) / CW_ENVELOPE_STEPS;

    for (int step = CW_ENVELOPE_STEPS; step >= 0; step--) {
        uint16_t level = raised_cosine_level(max_duty, (uint)step, CW_ENVELOPE_STEPS);
        pwm_set_chan_level(tone_slice, tone_channel, level);
        sleep_us(delay_us);
    }

    pwm_set_chan_level(tone_slice, tone_channel, 0);
    tone_is_on = false;
}

void rabbit_hw_init(void) {
    gpio_init(PTT_PIN);
    gpio_set_dir(PTT_PIN, GPIO_OUT);
    gpio_put(PTT_PIN, 0);

    gpio_init(CW_KEY_PIN);
    gpio_set_dir(CW_KEY_PIN, GPIO_OUT);
    gpio_put(CW_KEY_PIN, 0);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    gpio_set_function(TONE_PIN, GPIO_FUNC_PWM);

    tone_slice = pwm_gpio_to_slice_num(TONE_PIN);
    tone_channel = pwm_gpio_to_channel(TONE_PIN);

    /*
     * 125 MHz / 125 = 1 MHz PWM clock.
     * wrap = 1,000,000 / tone_hz - 1
     */
    float clock_divider = 125.0f;
    uint16_t wrap = (uint16_t)((1000000.0f / TONE_HZ) - 1);

    pwm_set_clkdiv(tone_slice, clock_divider);
    pwm_set_wrap(tone_slice, wrap);
    pwm_set_chan_level(tone_slice, tone_channel, 0);
    pwm_set_enabled(tone_slice, true);
}

void rabbit_hw_ptt_on(void) {
    gpio_put(PTT_PIN, 1);
    gpio_put(LED_PIN, 1);
}

void rabbit_hw_ptt_off(void) {
    if (tone_is_on) {
        rabbit_hw_tone_off();
    }

    gpio_put(CW_KEY_PIN, 0);
    gpio_put(PTT_PIN, 0);
    gpio_put(LED_PIN, 0);
}

bool rabbit_hw_ptt_is_on(void) {
    return gpio_get(PTT_PIN);
}

void rabbit_hw_tone_on(void) {
    if (!tone_is_on) {
        ramp_tone_up();
    }
}

void rabbit_hw_tone_off(void) {
    if (tone_is_on) {
        ramp_tone_down();
    }
}

void rabbit_hw_tone_for_ms(uint32_t ms) {
    rabbit_hw_tone_on();
    sleep_ms(ms);
    rabbit_hw_tone_off();

    /*
     * Small gap so the next Morse element or tone does not smear together.
     */
    sleep_ms(UNIT_MS);
}

void rabbit_hw_cw_key_down(void) {
    gpio_put(CW_KEY_PIN, 1);
    rabbit_hw_tone_on();
}

void rabbit_hw_cw_key_up(void) {
    rabbit_hw_tone_off();
    gpio_put(CW_KEY_PIN, 0);
}
