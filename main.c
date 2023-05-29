#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //biblioteca da funcao sleep()

void current_time(int time){
    //funcao de passar o tempo de minutos para horas
    int aux, h, m;
    h = (int) (time)/60;//horas pela parte inteira
    aux = h*60;
    m = time-aux; //minutos
    printf("%dh%dmin\n", h, m);
}

float velocidade_media_atual(float km_1min){
    float vm_atual = km_1min*60;
    //calculo da velocidade media atual sendo feita  dividindo a distancia percorrida em um minuto
    //por um minuto convertido em horas
    return vm_atual;
}

float velocidade_media_acumulada(int tempo_acum, float vm_acum, float vm_atual){
    float vm = ((tempo_acum*vm_acum) + (vm_atual))/(tempo_acum+1);
    //calculo da velocidade media acumulada sendo realizada por media ponderada
    return vm;
}

float consumo_atual(float km_1min, float vol_1min, float vol_anterior){
    float delta = abs(vol_anterior - vol_1min);
    float consumo = km_1min/delta;
    //calculo do consumo atual sendo a distancia percorrida sobre a variacao de combustivel
    return consumo;
}

float consumo_acumulado(float con_atual, int tempo_acum, float consumo_acum){
    float consumo = ((tempo_acum*consumo_acum) + (con_atual))/(tempo_acum+1);
    //calculo do consumo acumulado sendo realizado por media ponderada
    return consumo;
}

int main()
{
    //declarando as variaveis
    int tempo_acum,tempo_atual=0;
    float vm_acum, consumo_acum, autonomia;
    float vm_atual, con_atual, km_atual=0;
    float km_1min, vol_1min, vol_anterior=50;

    while(1){//loop para funcionar enquanto o carro tiver ligado
        //lendo os dados acumulados
        FILE *read;
        read = fopen("BorderComputerData.txt", "r");

        fscanf(read,"%d %f %f %f", &tempo_acum, &vm_acum, &consumo_acum, &autonomia);

        fclose(read);

        //lendo os dados do carro (distancia, volume de combustivel)
        scanf("%f %f", &km_1min, &vol_1min);
        system("cls");

        //fazendo calculos e chmando funcoes

        km_atual = km_atual + km_1min; //km percorrido
        tempo_atual++; //incrementando tempo
        tempo_acum++; //tempo acumulado

        //consumo
        con_atual = consumo_atual(km_1min, vol_1min, vol_anterior);
        consumo_acum = consumo_acumulado(con_atual, tempo_acum, consumo_acum);

        //sutonomia: o quanto o carro consegue andar com o combustivel disponivel
        autonomia = autonomia + (con_atual*vol_1min);

        //velocidades medias
        vm_atual = velocidade_media_atual(km_1min);
        vm_acum = velocidade_media_acumulada(tempo_acum, vm_acum, vm_atual);

        //atualizando volume anterior para atualizar no calculo do consumo
        vol_anterior = vol_1min;

        //escrever acumuladas no arquivo
        FILE *write;
        write = fopen("BorderComputerData.txt", "w");
        fprintf(write,"%d %.2f %.2f %.2f", tempo_acum, vm_acum, consumo_acum, autonomia);
        fclose(write);

        //print acumuladas
        printf("----ACUMULADAS----\n");
        current_time(tempo_acum);
        printf("%.2f km/h \n", vm_acum);
        printf("%.2f km/l\n", consumo_acum);
        printf("%.2f km\n", autonomia);
        //print atuais
        printf("----ATUAIS----\n");
        current_time(tempo_atual);
        printf("%.2f km/h\n", vm_atual);
        printf("%.2f km/l\n", con_atual);
        printf("%.2f km\n", km_atual);
        sleep(60); // só executara a proxima linha de codigo apos 60 segundos
    }
    return 0;
}
