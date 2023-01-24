//#include <curses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

//#include <gpio.h>
#include <controle.h>
//#include <bme280.h>


float temperatura_interna = 0.0;
float temperatura_referencia = 0.0;
int estado_funcionamento = 0;
int estado_sistema = 0;
float temperatura_ambiente = 0.0;
int sinal_controle = 0;
float sinal_referencia = 0.0;


int main(int argc, char const *argv[])
{
    
    pid_configura_constantes(30.0, 0.2, 400.0);
    while (1)
    {
        menu();
    }
    
    return 0;
}
