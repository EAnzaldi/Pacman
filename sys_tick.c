#include "LPC17xx.h"
#include <stdio.h>

volatile uint32_t start_time;
volatile uint32_t end_time;

extern uint32_t SystemFrequency;

void init_SysTick(void) {
    // Abilita SysTick Timer
    ST->CTRL_STAT = 0;         // Disabilita il timer per la configurazione
    ST->RELOAD = SystemFrequency / 1000 - 1; // Imposta il carico per 1 ms (assumendo 1 kHz)
    ST->CURRENT = 0;          // Reset del contatore
    ST->CTRL_STAT = 0x07;      // Abilita il timer, l'interrupt e il clock di sistema
}

void start_timer(void) {
    start_time = ST->CURRENT;  // Salva il valore iniziale del contatore
}

uint32_t get_elapsed_time(void) {
    uint32_t elapsed_time = start_time - ST->CURRENT;  // Calcola il tempo trascorso
    return elapsed_time;
}