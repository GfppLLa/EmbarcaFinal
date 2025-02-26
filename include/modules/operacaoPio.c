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

    tocar_tom_buzzer(1000, 200); 
    sleep_ms(20);              

 
        for (int16_t i = 0; i < NUM_PIXELS; i++) //laço pixels ou colnas
        {

            valor_led = matrix_rgbFlag(numbers[numero_display][24-i]);
          
            pio_sm_put_blocking(pio_controlador, state_machine, valor_led);
            sleep_us(50);
   
    }
        sleep_ms(50);
           
    
}
uint32_t matrix_rgbFlag(double number)//atualiza valor das cores
{
     // Certifica-se de que 'number' esteja no intervalo [0, 1]
    number = fmax(0.0, fmin(1.0, number));
    double brilhoLeds=0;
    if(var_teste==1){
         brilhoLeds=1;
         randon=4;
    }else{
     brilhoLeds=0.25*brilho;
        randon=(rand()%4)+1;

    }


    // Definir as variáveis de cor RGB
    unsigned char R = 0, G = 0, B = 0;
if(randon>=1&&randon<=3){
    // Ajuste de cor baseado na variável 'cor_atual'
    if (randon == 1) { // Vermelho
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
    } else if (randon == 2) { // Verde
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
    } else if (randon == 3) { // Azul
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    }
}else if(randon==4)//se or and for 4 o led fica branco
{
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    
}
if( randonTwo==true)//se or and for 4 o led fica branco
{
        R = (unsigned char)(number *brilhoLeds* 255.0); // R é o máximo, G e B são 0
        G = (unsigned char)(number *brilhoLeds*  255.0); // G é o máximo, R e B são 0
        B = (unsigned char)(number *brilhoLeds*  255.0); // B é o máximo, R e G são 0
    
}
randon=0;
    // Retorna o valor RGB no formato ARGB (Alpha, Red, Green, Blue)
    return (G << 24) | (R << 16) | (B << 8) | 0xFF;
}
void operacaoDoPio()
{
    //recebe como parametro um numero e exibe um alerta em tela referente
}