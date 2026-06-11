#ifndef CONFIG_H
#define CONFIG_H

#include "pico/stdlib.h"

#define FIRMWARE_VERSION "0.1.0"

/*
 * Rabbit identity.
 * Keep the callsign in every transmit cycle.
 */
#define CALLSIGN              "AF0FR"
#define RABBIT_MESSAGE        "AF0FR FOX"

/*
 * CW / tone settings.
 */
#define WPM                   15
#define TONE_HZ               700
#define UNIT_MS               (1200 / WPM)

/*
 * Rabbit timing.
 *
 * A common beginner foxhunt pattern is roughly:
 *   10-ish seconds transmitting
 *   50-ish seconds silent
 */
#define RABBIT_ARM_DELAY_MS   10000
#define RABBIT_INTERVAL_MS    60000

#define TX_SETTLE_MS          300
#define TX_RELEASE_DELAY_MS   300

#define PRE_ID_TONE_MS        2000
#define POST_ID_TONE_MS       3000

/*
 * Pico GPIO pins.
 *
 * These match the Keyer In The Rye defaults:
 *   PTT  = GPIO15
 *   Tone = GPIO16 PWM
 *   CW   = GPIO17
 */
#define PTT_PIN               15
#define TONE_PIN              16
#define CW_KEY_PIN            17
#define LED_PIN               PICO_DEFAULT_LED_PIN

/*
 * Audio envelope.
 * Prevents harsh clicks on the generated audio tone.
 */
#define CW_ENVELOPE_MS        9
#define CW_ENVELOPE_STEPS     32

#endif
