#ifndef RABBIT_HW_H
#define RABBIT_HW_H

#include <stdint.h>
#include <stdbool.h>

void rabbit_hw_init(void);

void rabbit_hw_ptt_on(void);
void rabbit_hw_ptt_off(void);
bool rabbit_hw_ptt_is_on(void);

void rabbit_hw_tone_on(void);
void rabbit_hw_tone_off(void);
void rabbit_hw_tone_for_ms(uint32_t ms);

void rabbit_hw_cw_key_down(void);
void rabbit_hw_cw_key_up(void);

uint32_t rabbit_hw_now_ms(void);

#endif
