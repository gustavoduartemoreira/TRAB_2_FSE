#include <uart.h>
#include "crc16.h"
#include <termios.h>
#include <unistd.h> // used for UART
#include <fcntl.h> // used for UART
#include <stdio.h>


int rx_length;
unsigned char tx_buffer[20];
unsigned char rx_buffer[256];
unsigned char *p_tx_buffer;
int uart0_filestream = -1;

int start_UART(){

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        printf("Erro - UART. Não foi possivel abrir a UART\n");
        return 0;
    }
    else{
        printf("UART aberta com sucesso\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // baudrate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return 1;
}
int escreve_UART(char endreco, char code, char subcode){

 p_tx_buffer = &tx_buffer[0];
  *p_tx_buffer++ = endreco;
  *p_tx_buffer++ = code;
  *p_tx_buffer++ = subcode;
  *p_tx_buffer++ = 8;
  *p_tx_buffer++ = 1;
  *p_tx_buffer++ = 4;
  *p_tx_buffer++ = 1;

  short resposta = calcula_CRC(tx_buffer, 7);
  memcpy(p_tx_buffer, &resposta, sizeof(resposta));
  p_tx_buffer+=sizeof(resposta);

    if (uart0_filestream != -1)
    {
        // escrever na uart
        int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("Erro na UART\n");
            sleep(1);
            return 0;
        }
        else
        {
            //printf("escrito.\n");
            sleep(1);
        }
    }
    return 1;
}

void fecha_UART(){
  close(uart0_filestream);
  memset(tx_buffer, 0, 20);
  memset(rx_buffer, 0, 256);
}

int valida_CRC(){
    short crc_envidado;
    memcpy(&crc_envidado, &rx_buffer[rx_length-2], sizeof(crc_envidado));
    short crc_calculado = calcula_CRC(rx_buffer, rx_length-2);
    if(crc_envidado == crc_calculado){
        //printf("CRC valido\n crc enviado: %d, crc calculado: %d\n", crc_envidado, crc_calculado);
        return 1;
    }
    else{
        //printf("CRC invalido\n crc enviado: %d, crc calculado: %d\n", crc_envidado, crc_calculado);
        return 0;
    }
}

int ler_UART(){
    rx_length = read(uart0_filestream, (void*)rx_buffer, sizeof(rx_buffer));      //Filestream, buffer to store in, number 
    if (rx_length < 0)
    {
        printf("Erro na leitura.\n");
        return 0; 
    }
    else if (rx_length == 0)
    {
        printf("Nenhum dado disponível.\n");
        return 0;
    }
    if(valida_CRC() == 0){
        printf("CRC invalido\n");
        fecha_UART(); 
        return 0;
    }
    return 1;
}

// verifica UART
int verifica_UART(char endereco, char code, char subcode){
    if(start_UART() == 0){
    printf("Falha na Conexão da UART\n");
    return 0;
    }

    if(escreve_UART(endereco, code, subcode)==0){
        printf("Falha na escrita da UART\n");
        return 0;
    }
    if(ler_UART()==0){
        //printf("Falha na leitura da UART\n");
        return 0;
    }
    return 1;
}



// ler temperatura de referencia codigo 0xC1
float ler_temperatura_interna(){
  float temperatura;
   if(verifica_UART(0x01, 0x23, 0xC1) == 0){
    printf("Falha na Conexão da UART\n");
    return 0;
  }
    else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&temperatura, &rx_buffer[3], sizeof(temperatura));
      printf("Valor da Temperatura: %f\n", temperatura);
      
    }
    fecha_UART();
    return temperatura;
}

// ler temperatura de referencia codigo 0xC2
float ler_temperatura_ref(){
  float temperatura_ref;
  if(verifica_UART(0x01, 0x23, 0xC2) == 0){
    printf("Falha na Conexão da UART\n");
    return 0;
  }
  else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&temperatura_ref, &rx_buffer[3], sizeof(temperatura_ref));
      printf("Valor da Temperatura de referencia: %f\n", temperatura_ref);
      
    }
    fecha_UART();
    return temperatura_ref;
}

// ler comando usuario codigo 0xC3
int comando_usuario(){
  int comando_usuario;
  if(verifica_UART(0x01, 0x23, 0xC3) == 0){
    printf("Falha na Conexão da UART\n");
    return 0;
  }
  else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&comando_usuario, &rx_buffer[3], sizeof(comando_usuario));
      printf("Valor do comando do usuario: %d\n", comando_usuario);
      
    }
    fecha_UART();
    return comando_usuario;
}

//enviar sinal de controle codigo 0xD1
void envia_sinal_ctrl(int sg_ctrl){
  if(verifica_UART(0x01, 0x16, 0xD1) == 0){
    printf("Falha na Conexão da UART\n");
  }
  memcpy(p_tx_buffer,&sg_ctrl, sizeof(int));
  p_tx_buffer+=sizeof(int);

  short resp = calcula_CRC(tx_buffer, 11);
  memcpy(p_tx_buffer, &resp, sizeof(resp));
  p_tx_buffer+=sizeof(resp);

  if(escreve_UART(0x01, 0x16, 0xD1)==0){
    printf("Falha na escrita da UART\n");
  }

  }

//envia sinal de referencia codigo 0xD2
void envia_sinal_ref(float sg_ref){
  if(verifica_UART(0x01, 0x16, 0xD2) == 0){
    printf("Falha na Conexão da UART\n");
    fecha_UART();
  }
  memcpy(p_tx_buffer,&sg_ref, sizeof(sg_ref));
  p_tx_buffer+=sizeof(sg_ref);

  short resp = calcula_CRC(tx_buffer, 11);
  memcpy(p_tx_buffer, &resp, sizeof(resp));
  p_tx_buffer+=sizeof(resp);

  if(escreve_UART(0x01, 0x16, 0xD2)==0){
    printf("Falha na escrita da UART\n");
    fecha_UART();
  }

  }

  // envia estado do sistema codigo 0xD3
  int envia_est_sistema(int estado_sistema){
    int estado;
    if(verifica_UART(0x01, 0x16, 0xD3) == 0){
    printf("Falha na Conexão da UART1\n");
    fecha_UART();
    }
    memcpy(p_tx_buffer,&estado_sistema, sizeof(estado_sistema));
    p_tx_buffer+=sizeof(estado_sistema);

    short resp = calcula_CRC(tx_buffer, 8);
    memcpy(p_tx_buffer, &resp, sizeof(resp));
    p_tx_buffer+=sizeof(resp);

    if(escreve_UART(0x01, 0x16, 0xD3)==0){
    printf("Falha na escrita da UART2\n");
    fecha_UART();
    }
   if(ler_UART() == 0){
    printf("Falha na leitura da UART3\n");
    fecha_UART();
    }
    else{
      //Bytes received
      rx_buffer[rx_length] = '\0';
      memcpy(&estado, &rx_buffer[3], sizeof(estado));
      printf("Valor do estado do sistema: %d\n", estado);
      
    }
    fecha_UART();
    return estado;
}

// envia estado de funcionamento codigo 0xD5
int envia_estado_func(int sg_usr){
    float est_func;
    unsigned char func_state = sg_usr;
    if(verifica_UART(0x01, 0x16, 0xD5) == 0){
    printf("Falha na Conexão da UART\n");
    fecha_UART();
    }
    memcpy(p_tx_buffer,&func_state, sizeof(func_state));
    p_tx_buffer+=sizeof(func_state);

    short resp = calcula_CRC(tx_buffer, 8);
    memcpy(p_tx_buffer, &resp, sizeof(resp));
    p_tx_buffer+=sizeof(resp);

    if(verifica_UART(0x01, 0x16, 0xD5)==0){
    printf("Falha na escrita da UART\n");
    fecha_UART();
    }
    else{
        //Bytes received
        rx_buffer[rx_length] = '\0';
        memcpy(&est_func, &rx_buffer[3], sizeof(est_func));
        //printf("Valor do estado de funcionamento: %f\n", est_func);
        
    }
    fecha_UART();
    return est_func;    
}
