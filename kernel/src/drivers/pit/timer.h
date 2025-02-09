#pragma once
#include <stdint.h>

extern uint64_t ticks;
extern uint32_t frequncy;

void InitializeTimer();
// Sleep in miliseconds
void sleep(uint64_t duration);
void timerTick();

