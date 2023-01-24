#include <controle.h>
#include <uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <gpio.h>


int forno_ligado = 0;
int forno_em_uso = 0;

void menu(){
    init_gpio();
    prepara_gpio();
    envia_est_sistema(0);


        int opcao;
    do{
        printf("-------------Airfryer-------------\n");
        printf("1 - Ligar\n"); 
        printf("2 - Desligar\n");
        printf("3 - Inicia\n");
        printf("4 - Parar\n");
        printf("5 - Aumentar temperatura\n");
        printf("6 - Diminuir temperatura\n");
        printf("7 - Sair\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
                printf("Ligar\n");
                //ligar o forno
                envia_est_sistema(1);
                ler_temperatura_interna();
                ler_temperatura_ref();
                forno_ligado = 1;                
                break;
            case 2:
                printf("Desligar\n");
                //desligar o forno
                envia_est_sistema(0);
                envia_estado_func(0);
                gpio_aquecedor(0);
                gpio_ventilador(0);
                forno_ligado = 0;
                forno_em_uso = 0;

                break;
            case 3:
                printf("Inicia\n");
                if(forno_ligado){
                    envia_estado_func(1);
                }

                break;
            case 4:
                printf("Parar\n");
                // parar o forno
                envia_estado_func(0);
                gpio_aquecedor(0);
                gpio_ventilador(0);
                break;
            case 5:
                printf("Aumentar temperatura\n");
                // aumentar a temperatura 1 grau

                break;
            case 6:
                printf("Diminuir temperatura\n");
               // diminuir_temperatura();
                break;
            case 7:
                printf("Sair\n");
                // sair com CTRL+C
                //sair do programa
                envia_estado_func(0);
                envia_est_sistema(0);
                gpio_aquecedor(0);
                gpio_ventilador(0);
                exit(0);
                break;
            default:
                printf("Opção inválida\n");
                break;
        }
    }while(opcao != 7);

}