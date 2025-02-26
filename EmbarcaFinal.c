#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include <math.h>

// Definições dos pinos
#define LED_VERDE 11
#define LED_AZUL 12
#define LED_VERMELHO 13
#define BUZZER_PIN 10  // Pino do buzzer
#define EIXO_X 27      // Pino do eixo X do joystick (usado para proximidade)

// Variáveis do joystick
volatile uint16_t posicao_x;  // Posição no eixo X

// PWM do buzzer
#define BUZZER_PWM_WRAP 65535
uint slice_buzzer;  // Slice PWM para o buzzer

// Função para ler o valor do joystick
uint16_t ler_adc(uint canaladc) {
    adc_select_input(canaladc);
    uint16_t leitura = adc_read();
    return leitura;
}

// Função para configurar o PWM para o buzzer
void configurar_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    slice_buzzer = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_clkdiv(slice_buzzer, 4.0);  // Reduz a frequência PWM
    pwm_set_wrap(slice_buzzer, BUZZER_PWM_WRAP);  // Define o wrap para a onda PWM
    pwm_set_gpio_level(BUZZER_PIN, 0);  // Inicia o buzzer desligado
    pwm_set_enabled(slice_buzzer, true);  // Habilita o PWM
}

// Função para controlar o buzzer com base no valor do joystick
void controle_buzzer(uint16_t posicao) {
    if (posicao < 100) {
        pwm_set_gpio_level(BUZZER_PIN, 0);  // Desliga o buzzer se o valor for muito baixo
    } else if (posicao > 4000) {
        // Se o valor for maior que 4000, buzzer apita de forma diferente (alerta de risco iminente)
        pwm_set_gpio_level(BUZZER_PIN, BUZZER_PWM_WRAP);  // Frequência máxima contínua no buzzer
        sleep_ms(200);  // Intervalo para apito intermitente
        pwm_set_gpio_level(BUZZER_PIN, 0);  // Desliga o buzzer
        sleep_ms(200);  // Intervalo entre os apitos
    } else {
        // Controla o buzzer conforme a posição, diminuindo o som conforme o valor diminui
        int frequencia = 1000 + (posicao * 1000 / 4095);  // Frequência proporcional à posição
        pwm_set_gpio_level(BUZZER_PIN, (frequencia / 2));  // Ajusta a intensidade do som
    }
}

// Função para animar os LEDs
void animar_leds(uint16_t posicao) {
    if (posicao == 0) {
        // Quando o valor estiver em 0, todos os LEDs devem apagar
        gpio_put(LED_VERDE, 0);
        gpio_put(LED_AZUL, 0);
        gpio_put(LED_VERMELHO, 0);
    } else if (posicao == 4095) {
        // Quando o valor estiver em 4095, os LEDs devem estar acesos continuamente
        gpio_put(LED_VERDE, 1);
        gpio_put(LED_AZUL, 1);
        gpio_put(LED_VERMELHO, 1);
    } else {
        // Quando o valor estiver entre 0 e 4095, os LEDs devem piscar
        gpio_put(LED_VERDE, (posicao % 2 == 0) ? 1 : 0);
        gpio_put(LED_AZUL, (posicao % 2 == 0) ? 1 : 0);
        gpio_put(LED_VERMELHO, (posicao % 2 == 0) ? 1 : 0);
    }
}

// Função para atualizar os LEDs com base na posição do joystick
void atualizar_leds(uint16_t posicao) {
    if (posicao > 3000) {
        gpio_put(LED_VERDE, 1);  // Aciona o LED verde para proximidade alta
        gpio_put(LED_AZUL, 0);   // Desliga o LED azul
        gpio_put(LED_VERMELHO, 0);  // Desliga o LED vermelho
    } else if (posicao > 2000) {
        gpio_put(LED_VERDE, 0);  // Desliga o LED verde
        gpio_put(LED_AZUL, 1);   // Aciona o LED azul
        gpio_put(LED_VERMELHO, 0);  // Desliga o LED vermelho
    } else {
        gpio_put(LED_VERDE, 0);  // Desliga o LED verde
        gpio_put(LED_AZUL, 0);   // Desliga o LED azul
        gpio_put(LED_VERMELHO, 1);  // Aciona o LED vermelho
    }
}

int main() {
    stdio_init_all();

    adc_init();  // Inicializa o ADC
    adc_gpio_init(EIXO_X);  // Configura o pino X do joystick para leitura do ADC

    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_put(LED_VERDE, 0);

    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_put(LED_AZUL, 0);

    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_put(LED_VERMELHO, 0);

    configurar_buzzer();  // Configura o buzzer

    while (true) {
        // Lê a posição no eixo X do joystick
        posicao_x = ler_adc(1);  // A leitura do eixo X vai para o canal 1

        // Imprime o valor do ADC no terminal
        printf("Valor do ADC: %d\n", posicao_x);

        // Aciona o buzzer de acordo com a posição no eixo X
        controle_buzzer(posicao_x);

        // Atualiza os LEDs de acordo com a posição no eixo X
        animar_leds(posicao_x);

        // Atualiza LEDs principais com base na posição do joystick
        atualizar_leds(posicao_x);

        sleep_ms(100);  // Intervalo entre leituras
    }

    return 0;
}
