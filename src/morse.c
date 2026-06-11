#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "pico/stdlib.h"

#include "config.h"
#include "rabbit_hw.h"
#include "morse.h"

static const char *morse_for_char(char c) {
    switch (c) {
        case 'A': return ".-";
        case 'B': return "-...";
        case 'C': return "-.-.";
        case 'D': return "-..";
        case 'E': return ".";
        case 'F': return "..-.";
        case 'G': return "--.";
        case 'H': return "....";
        case 'I': return "..";
        case 'J': return ".---";
        case 'K': return "-.-";
        case 'L': return ".-..";
        case 'M': return "--";
        case 'N': return "-.";
        case 'O': return "---";
        case 'P': return ".--.";
        case 'Q': return "--.-";
        case 'R': return ".-.";
        case 'S': return "...";
        case 'T': return "-";
        case 'U': return "..-";
        case 'V': return "...-";
        case 'W': return ".--";
        case 'X': return "-..-";
        case 'Y': return "-.--";
        case 'Z': return "--..";

        case '0': return "-----";
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        case '9': return "----.";

        case '/': return "-..-.";
        case '?': return "..--..";
        case '.': return ".-.-.-";
        case ',': return "--..--";

        default: return NULL;
    }
}

static void morse_play_element(bool dah) {
    uint32_t units = dah ? 3 : 1;

    rabbit_hw_cw_key_down();
    sleep_ms(UNIT_MS * units);
    rabbit_hw_cw_key_up();

    /*
     * Inter-element gap.
     */
    sleep_ms(UNIT_MS);
}

void morse_play_message(const char *text) {
    printf("Morse: %s\n", text);

    for (size_t i = 0; i < strlen(text); i++) {
        char c = text[i];

        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }

        if (c == ' ') {
            printf(" / ");
            sleep_ms(UNIT_MS * 7);
            continue;
        }

        const char *pattern = morse_for_char(c);

        if (pattern == NULL) {
            printf("[skip %c]", c);
            continue;
        }

        printf("%c", c);

        for (size_t j = 0; j < strlen(pattern); j++) {
            if (pattern[j] == '.') {
                morse_play_element(false);
            } else if (pattern[j] == '-') {
                morse_play_element(true);
            }
        }

        /*
         * Character gap.
         * One unit was already sent after the last element,
         * so this adds two more units for a total of three.
         */
        sleep_ms(UNIT_MS * 2);
    }

    printf("\n");
}
