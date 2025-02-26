//
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
#include "include/libraries/operacaoPio.h"//lib do pio 5x5
#include "include/libraries/ssd1306.h"
//

//definição pinos
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BOTAO_A 5
#define BOTAO_B 6
#define BOTAO_B 22
#define OUT_PIN 7
#define BUZZER_A_PIN 10
#define BUZZER_B_PIN 21
#define NUM_PIXELS 25
#define DEBOUNCING_TIME_US 220
//
//display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADRRS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
ssd1306_t ssd;
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
float vardbA=0.0f;
float _Atomic vardbC=0.0f;
mutex_t mutexDecibeis;
float vardbATratado=0.0f;
#define SUAVIZACAO 0.1f;
//objetivo nao muito claro
static volatile uint32_t ultimo_pressionamento = 0;

//callback()
void tratar_botoes(uint gpio, uint32_t events)
{
    //debounce();
    
      hora_presente = to_us_since_boot(get_absolute_time());
    if(gpio==BOTAO_A)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_A);
            if(botao_pressionado){
                    sobe_um=1;//resetar o alarme
            }
        }
    
    //

    } else if( gpio ==BOTAO_B)
    {
         if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_B);
            if(botao_pressionado){
                    desce_um=1;//desativar a exibição do alerta visual
            }
        }
     


    //
    
    } else if(gpio==BOTAO_B)
    {
        if(hora_presente-ultimo_pressionamento>DEBOUNCING_TIME_US*1000)
        {
            bool botao_pressionado=!gpio_get(BOTAO_B);
            if(botao_pressionado)
            {saida_teste=1;}//
        }

    }
            ultimo_pressionamento=hora_presente;

}
void iniciaHardware()
{
        //iniciar o pwm
    //iniciar o adc
    //inicia a irq
    //inicia o ssd(funções herdadas do prof)
    
    //leds
    //gpio_init(LED_AZUL);
    //gpio_set_dir(LED_AZUL, GPIO_OUT);

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE,0);

    //gpio_init(LED_VERMELHO);
    //gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    //gpio pwm azul
    gpio_set_function(LED_AZUL, GPIO_FUNC_PWM);//define led azul como pẃm
    uint slice=pwm_gpio_to_slice_num(LED_AZUL);//numero do slice do led azul
    pwm_set_clkdiv(slice, 4.0);//configura divisor de clock e reduz freq pwm
    pwm_set_wrap(slice, WRAP);//define a onda pwm maximo
    pwm_set_gpio_level(LED_AZUL, 0);//define nivel inicial pwm
    pwm_set_enabled(slice, true);//habilita o slice pra gerar sinais

    //gpio pwm led vermelho
    gpio_set_function(LED_VERMELHO, GPIO_FUNC_PWM);
    slice=pwm_gpio_to_slice_num(LED_VERMELHO);
    pwm_set_clkdiv(slice, 4.0);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(LED_VERMELHO, 0);
    pwm_set_enabled(slice, true);

    //adc
    adc_init();//inicializa o analog digital converter
    adc_gpio_init(EIXO_X);//inicia gio joy x para adc
    adc_gpio_init(EIXO_Y);//inicia gpio joy y para adc

//display no i2c
    i2c_init(I2C_PORT, 400*1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL);
    gpio_pull_up(I2C_SDA);

    ssd1306_init(&ssd, SCREEN_WIDTH, SCREEN_HEIGHT, false, ADRRS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    //limpar display]
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    //botoes
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_C);
    gpio_set_dir(BOTAO_C, GPIO_IN);
    gpio_pull_up(BOTAO_C);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
}

//rotina ler e converter microfone para dba e dbc
//uso do core 1 pra ler e converter constantemente audio pra dba e dbc
float converteDBA(float valorRetorno, uint16_t valorADC)
{
    float tensao=(valorADC/4095.0f)*3.3f;
    float dbInst=20.0f*log10f(tensao/0.00002f);
    //atenuação das baixas frequencias
    float pondA=dbInst-(dbInst*0.2f);
    //resposta lenta
    valorRetorno=(SUAVIZACAO*pondA)+((1.0f-SUAVIZACAO)*valorRetorno);
    return valorRetorno;
}
float converteDBC(uint16_t valorADC)
{
    float tensao=(valoADC/4095.0f)*3.3f;
    float db=20.0f*log10f(tensao/0.00002f);
    float valorRetorno =db-(db*0.05f);
    return valorRetorno;
}
void subRotinaDois()
{
    //vai por em duas variaveis universais o valor de dba e dbc
    adc_init();
    adc_gpio_init(28);
    adc_select_input(2);
    float valorDBA=0.0f;
    while(true)
    {
        uint16_t valorADC=adc_read();
        valorDBA=converteDBA(valorDBA, valorADC);
        float valorDBC =converteDBC(valorADC);
        float mediaDBA=0;
        int contado=0;
        for (int i=0; i<600; i++)
        {
            mediaDBA=mediaDBA+valorDBA;
            contador++;
            //mutex_enter_blocking(&mutexDecibeis);
            vardbC=valorDBC;
            //mutex_exit(&mutexDecibeis);
            sleep_ms(100);
        }
        mutex_enter_blocking(&mutexDecibeis);
        vardbATratado=mediaDBA/contador;
        mutex_exit(&mutexDecibeis);

    }
}
    bool repetir_exibicao_tela(struct repeating_timer *t)
    {
        //fução que a cada 150ms pega os valores de dba e dbc e envia para tela
        //e atualiza indice risco
    } 
int main()
{
    stdio_init_all();
    inicia_hardware();
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    gpio_set_irq_enabled_with_callback(BOTAO_C, GPIO_IRQ_EDGE_FALL, true, &tratar_botoes);
    boas_vindas(&ssd);
    sleep_ms(5000);
    struct repeating_timer time;

    //inicia mutex
    mutex_init(&mutexDecibeis);
    multicore_launch_core1(subRotinaDois);
    int indicesDBA[116];
    add_repeating_timer_ms(150, repetir_exibicao_tela, NULL, &timer);
    while (true) {
        
        float temp =0.0f;
        mutex_enter_blocking(&mutexDecibeis);
        temp=vardbATratado;
        mutex_exit(&mutexDecibeis);
        
        if(temp >=80 &&temp<=115)
        {
            indicesDBA[temp]++;
            //verificar se na mesma faixa de indice o numero é compativel com 
            //um indice referencia
            //e se nfor acima enviar sinal para mudar faixa ou iniciar alerta visual
        }
        if(saida_teste==1)
        {
            desliga_tudo();
            mensagem_bootloader(&ssd);
            reset_usb_boot(0, 0);
        }
    }
}
