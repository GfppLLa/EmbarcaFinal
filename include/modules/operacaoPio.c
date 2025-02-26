#include "include/libraries/operacaoPio.h"

PIO pio_controlador = pio0;
bool status_init_pio = false;
uint state_machine =0;

void ligar_todos_os_leds()
{
    uint32_t valor_led = 0;
    double ligar_todos_os_leds_20[25] =   {1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0, 
                                        1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0,
                                        1.0, 1.0, 1.0, 1.0, 1.0};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgbFlag(ligar_todos_os_leds_20[i]);
        pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
    }
}
void desliga_tudo()
{
    uint32_t valor_led = 0;

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgbFlag(0.0);
        pio_sm_put_blocking(pio_controlador, state_machine, valor_led);

    }
  
}
void init_pio_routine()
{
    //inicia o PIO
    //sem struct
    //coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
   status_init_pio = set_sys_clock_khz(128000, false);

    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (status_init_pio){ printf("clock set to %ld\n", clock_get_hz(clk_sys));}//clocks.h

    //configurações da PIO
    uint deslocamento = pio_add_program(pio_controlador, &pio_matrix_program);
    state_machine= pio_claim_unused_sm(pio_controlador, true);
    pio_matrix_program_init(pio_controlador, state_machine, deslocamento, OUT_PIN);
}
void atualiza_matrix()//atualiza o matrix
{
 uint32_t valor_led = 0;

    //tocar_tom_buzzer(1000, 200); 
    sleep_ms(20);              

 
        for (int16_t i = 0; i < NUM_PIXELS; i++) //laço pixels ou colnas
        {

            valor_led = matrix_rgbFlag(escala[numero_display][24-i]);//mudar aqui
          
            pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
            sleep_us(50);
   
    }
        sleep_ms(50);
           
    
}
uint32_t matrix_rgbFlag(double number)//atualiza valor das cores
    {   // Certifica-se de que 'number' esteja no intervalo [0, 1]
    number = fmax(0.0, fmin(1.0, number));
    
    // Define o fator de brilho: se var_teste == 1, brilho máximo; senão, escala com 'brilho'
    double brilhoLeds = (var_teste == 1) ? 1.0 : 0.25 * brilho;
    double factor = number * brilhoLeds;
    
    // Define a cor base.
    // Usamos o padrão dado: 0xff028200 (ARGB: alfa=0xff, red=0x02, green=0x82, blue=0x00)
    // Se var_teste ou randonTwo forem verdadeiros, pode ser definido para branco (0xffffffff)
    uint32_t baseColor = 0xff028200;
   
    
    // Extrai os canais de cor do baseColor (no formato ARGB: 0xAA RR GG BB)
    unsigned char A = (baseColor >> 24) & 0xFF;
    unsigned char R = (baseColor >> 16) & 0xFF;
    unsigned char G = (baseColor >> 8)  & 0xFF;
    unsigned char B = baseColor & 0xFF;
    
    // Ajusta o brilho de cada canal multiplicando pelo fator (garante que não ultrapasse 255)
    R = (unsigned char) fmin(255, R * factor);
    G = (unsigned char) fmin(255, G * factor);
    B = (unsigned char) fmin(255, B * factor);
    
    // Retorna o valor ARGB no formato 0xAARRGGBB (alpha fixo em 0xFF)
    return (0xFF << 24) | (R << 16) | (G << 8) | B;
}
