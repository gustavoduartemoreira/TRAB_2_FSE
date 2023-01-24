#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Used for UART
#include <fcntl.h>   // Used for UART
#include <termios.h> // Used for UART
#include <stdint.h>

#ifndef UART_H_
#define UART_H_

int start_UART();
int escreve_uart();
int ler_UART();
float ler_temperatura_interna(); // ler temperatura do sensor 0xC1
float ler_temperatura_ref(); // ler temperatura de referencia codigo 0xC2
int comando_usuario(); // ler comando usuario codigo 0xC3
void envia_sinal_ctrl(int sinal_ctrl); //enviar sinal de controle codigo 0xD1
void envia_sinal_ref(float sinal_ref); //enviar sinal de referencia codigo 0xD2
int envia_est_sistema(int est_sistema); //enviar estado de funcionamento codigo 0xD3
int controle_temperatura(int valor_temperatura); //enviar valor de temperatura codigo 0xD4
int envia_estado_func(int estado_func); //enviar estado de funcionamento codigo 0xD5
int envia_temp_amb(float temp_amb); //enviar temperatura ambiente codigo 0xD6

#endif /* UART_H_ */