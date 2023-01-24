#include "gpio.h"
#include <stdio.h>

int PIN_AQUECEDOR = 4;
int PIN_VENTILADOR = 5;

void init_gpio()
{
   wiringPiSetup();
}
    
void prepara_gpio()
{
    softPwmCreate(PIN_AQUECEDOR, 0, 100);
    softPwmCreate(PIN_VENTILADOR, 0, 100);

}

void gpio_aquecedor(int valor)
{
    printf("Aquecedor: %d\n", valor);
    softPwmWrite(PIN_AQUECEDOR, valor);
}
void gpio_ventilador(int valor)
{
    printf("Valor do ventilador: %d \n", valor);
    softPwmWrite(PIN_VENTILADOR, valor);
}

void gpio_desliga()
{
    softPwmStop(PIN_AQUECEDOR);
    softPwmStop(PIN_VENTILADOR);
}


void gpio_controle_pwm(int signal)
{
    if(signal == 0){
        gpio_aquecedor(signal);
        gpio_ventilador(0);
    }
    else 
    {
        if(signal < -40)
        {
            gpio_ventilador(signal * -1);
        }
        else
        {
            gpio_ventilador(40);
        }
    gpio_aquecedor(0);
    
    }
}


