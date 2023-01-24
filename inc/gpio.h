#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>


#ifndef GPIO_H_
#define GPIO_H_

void init_gpio();
void prepara_gpio();
void gpio_aquecedor(int valor);
void gpio_ventilador(int valor);
void gpio_desliga();


#endif // GPIO_H_