#ifndef OPERACAOPIO_H
#define OPERACAOPIO_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include <math.h>
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"  //
#include "hardware/flash.h"
#include "hardware/structs/systick.h" 
#include "pio_matrix.pio.h"
#include "include/libraries/escalaAlerta.h"
//#include "numbers_n_array.h"
#include <time.h>
#include <stdlib.h>

//definição pinos
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_C 22
#define OUT_PIN 7
#define BUZZER_A_PIN 10
#define BUZZER_B_PIN 21
#define NUM_PIXELS 25
#define DEBOUNCING_TIME_US 220
//
//void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms);
void tratar_botoes(uint gpio, uint32_t eventos);
void acende_led();
void apaga_led();
uint32_t matrix_rgbFlag(double number);
void debounce();

void entrarModoBootloaderDois();
void init_pio_routine(void);
void desliga_tudo();
void ligar_todos_os_leds();
void atualiza_matrix();

//

//variaveis universais
//variaveis pio
// variáveis pio
extern PIO pio_controlador;
extern bool status_init_pio;
extern uint state_machine;

// variáveis operação
extern volatile _Atomic uint numero_display;
extern volatile _Atomic uint cor_atual;
extern volatile _Atomic uint brilho;
extern volatile _Atomic uint tempo;
extern volatile uint32_t hora_presente;
extern volatile _Atomic uint var_teste;
extern volatile _Atomic uint saida_teste;
extern volatile _Atomic uint sobe_um;
extern volatile _Atomic uint desce_um;
extern int randon;

extern volatile uint32_t ultimo_pressionamento;



#endif