#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define servo_motor 22 // Pino GPIO 22 do servo motor

// Definição das variáveis principais
uint32_t nivel_pwm;
uint32_t inicio_us, fim_us, incremento_us, tempo_delay_ms;

// Configuração do PWM
void configurar_pwm()
{
    gpio_set_function(servo_motor, GPIO_FUNC_PWM); // Habilitar o pino GPIO como PWM
    uint slice_pwm = pwm_gpio_to_slice_num(servo_motor); // Obter o canal PWM da GPIO
    pwm_set_clkdiv(slice_pwm, 125.0); // Define o divisor de clock do PWM
    pwm_set_wrap(slice_pwm, 20000); // Define o valor de wrap
    pwm_set_enabled(slice_pwm, true); // Habilita o pwm no slice correspondente
}

// Definição da largura de pulso
void definir_pwm(uint32_t nivel_us)
{
    nivel_pwm = (nivel_us * 20000) / ((1000/50) * 1000); // Calcula o nível PWM
    pwm_set_gpio_level(servo_motor, nivel_pwm); // Define o nível PWM no pino GPIO 22
}

// Função para movimentação do servo
void mover_servo(uint32_t inicio_us, uint32_t fim_us, int incremento_us, int tempo_delay_ms)
{
    nivel_pwm = inicio_us;
    inicio_us = inicio_us;
    fim_us = fim_us; 
    incremento_us = incremento_us; 
    tempo_delay_ms = tempo_delay_ms; 
    
    // Variável auxiliar
    int movimento_tipo = (inicio_us < fim_us) ? 1 : 2;

    switch (movimento_tipo)
    {
        case 1: // Movimento crescente
            for (nivel_pwm = inicio_us; nivel_pwm <= fim_us; nivel_pwm += incremento_us)
            {
                definir_pwm(nivel_pwm); 
                sleep_ms(tempo_delay_ms); 
            }
            break;

        case 2: // Movimento decrescente
            for (nivel_pwm = inicio_us; nivel_pwm >= fim_us; nivel_pwm -= incremento_us)
            {
                definir_pwm(nivel_pwm); 
                sleep_ms(tempo_delay_ms); 
            }
            break;
        
        default:
            printf("Movimento inválido!\n");
            break;
    }
}

// Função principal
int main()
{
    stdio_init_all();
    configurar_pwm();

    // Realizar movimentos 0°, 90° e 180°
    definir_pwm(2400); // PWM de 2400us, isto 180 graus
    sleep_ms(5000); // Espera 5 segundos
    definir_pwm(1470); // PWM de 1470us, isto é 90 graus
    sleep_ms(5000); // Espera 5 segundos
    definir_pwm(500); // PWM de 500us, isto é 0 graus
    sleep_ms(5000); // Espera 5 segundos

    while (true)
    {
        mover_servo(500, 2400, 5, 10); // Movimento de 0 a 180 graus
        sleep_ms(10); // Espera 10 milissegundos

        mover_servo(2400, 500, 5, 10); // Movimento de 180 a 0 graus
        sleep_ms(10); // Espera 10 milissegundos
    }

    return 0;
}
