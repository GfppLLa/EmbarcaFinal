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
void tocar_tom_buzzer(uint16_t frequency, uint32_t duration_ms);
void tratar_botoes(uint gpio, uint32_t eventos);
void acende_led();
void apaga_led();
uint32_t matrix_rgbFlag(double number);
void debounce();
void inicia_hardware();
void entrarModoBootloaderDois();
void init_pio_routine(void);
void desliga_tudo();
void ligar_todos_os_leds();

//

//variaveis universais
//variaveis pio
PIO pio_controlador = pio0;
bool status_init_pio = false;
uint state_machine =0;

//variaveis operação
volatile _Atomic uint numero_display=0;
volatile _Atomic uint cor_atual =1;//apenas 1, 2 e 3 
volatile _Atomic uint brilho =1; // 1 a 4
volatile _Atomic uint tempo =5; // 1 a 4
volatile uint32_t hora_presente;
volatile _Atomic uint var_teste=0;
volatile _Atomic uint saida_teste=0;
volatile _Atomic uint sobe_um=0;
volatile _Atomic uint desce_um=0;
int randon=0;
static bool randonTwo=0;

//objetivo nao muito claro
static volatile uint32_t ultimo_pressionamento = 0;


#endif