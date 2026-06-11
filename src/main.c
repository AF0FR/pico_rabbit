#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"

#include "config.h"
#include "rabbit_hw.h"
#include "morse.h"

static void rabbit_transmit_cycle(void) {
    printf("TX start\n");

    rabbit_hw_ptt_on();
    sleep_ms(TX_SETTLE_MS);

    rabbit_hw_tone_for_ms(PRE_ID_TONE_MS);

    morse_play_message(RABBIT_MESSAGE);

    rabbit_hw_tone_for_ms(POST_ID_TONE_MS);

    sleep_ms(TX_RELEASE_DELAY_MS);
    rabbit_hw_ptt_off();

    printf("TX end\n");
}

int main(void) {
    stdio_init_all();

    sleep_ms(2000);

    printf("\n================================\n");
    printf(" PICO RABBIT\n");
    printf(" Version: %s\n", FIRMWARE_VERSION);
    printf(" Callsign: %s\n", CALLSIGN);
    printf(" Message: %s\n", RABBIT_MESSAGE);
    printf(" WPM: %d\n", WPM);
    printf(" Tone: %d Hz\n", TONE_HZ);
    printf(" Interval: %d ms\n", RABBIT_INTERVAL_MS);
    printf("================================\n\n");

    rabbit_hw_init();

    printf("Arming delay: %d ms\n", RABBIT_ARM_DELAY_MS);
    sleep_ms(RABBIT_ARM_DELAY_MS);

    while (true) {
        uint32_t start_ms = rabbit_hw_now_ms();

        rabbit_transmit_cycle();

        uint32_t elapsed_ms = rabbit_hw_now_ms() - start_ms;

        if (elapsed_ms < RABBIT_INTERVAL_MS) {
            uint32_t sleep_time = RABBIT_INTERVAL_MS - elapsed_ms;
            printf("Idle: %lu ms\n", (unsigned long)sleep_time);
            sleep_ms(sleep_time);
        } else {
            printf("Warning: TX cycle exceeded interval\n");
        }
    }
}
