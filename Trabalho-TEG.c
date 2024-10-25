#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024

typedef struct flower{
    float sepLength;
    float sepWidth;
    float petLength;
    float petWidth;
} flower;

void distancia_euclideana(float matriz[150][150], flower flores[]);
void achamaioremenor(float *maior, float *menor, float matriz[150][150], int *a, int *b, int *c, int *d);
void distancia_normalizada(float *maior, float *menor, float matriz[150][150]);
void printa_matriz(float matriz[150][150]);
void close_file(int matriz[150][150], int total, float demaior, float demenor, float denmaior, float denmenor);
void leituramatriz(int matriz[150][150]);



int main() {
    flower flores[150];
    int matriz_adjacencia[150][150] = {0};
    float matrix[150][150] = {0};
    int i = 0;
    float demaior = 0, demenor = 0, denmaior = 0, denmenor = 0;
    int imaior = 0, jmaior = 0, imenor = 0, jmenor = 0, total = 0;
    int opcao = -1;

    printf("Qual opcaoo voce deseja?");
    printf("\n(0) - Sair");
    printf("\n(1) - Ler dados do Dataset");
    printf("\n(2) - Ler matriz de adjacencia\n");
    scanf("%i",&opcao);
    
    switch (opcao){
    case 0:
        return 0;
        break;
    case 1:
      //abertura e leitura arquivo
        FILE *file = fopen("IrisDataset.csv", "r");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return -1;
        }
        //Pula a primeira linha do csv
    fscanf(file, "%*[^\n]\n");

    //Lê o resto das linhas
    for(int i = 0; i < 150; i++){
        total++;
        fscanf(file, "%*[^,],%f,%f,%f,%f", &flores[i].petLength, &flores[i].petWidth, &flores[i].sepLength, &flores[i].sepWidth); //tem que usar [^,] pq a string da variedade não tem o \0 
        // printf("Flower %i: %f\t%f\t%f\t%f\n", i+1, flowers[i].petLength, flowers[i].petWidth, flowers[i].sepLength, flowers[i].sepWidth);
    }

    distancia_euclideana(matrix, flores);
    achamaioremenor(&demaior, &demenor, matrix, &imaior, &jmaior, &imenor, &jmenor);
    //printa_matriz(matrix);
    distancia_normalizada(&demaior, &demenor, matrix);
    //printa_matriz(matrix);
    for(int i = 0; i < 149; i++){
        for(int j = i+1; j < 150; j++){
            if(matrix[i][j] <= 0.2){
                matriz_adjacencia[i][j] = 1; //tem relacao
                matriz_adjacencia[j][i] = 1;
            }
        }
    } 
        printf("Total de flores: %i", total);
        denmaior = matrix[imaior][jmaior];
        denmenor = matrix[imenor][jmenor];
        printf("\nDEmaior: %lf - (%i,%i)\n", demaior, imaior, jmaior);
        printf("DEmenor: %lf - (%i,%i)\n", demenor, imenor, jmenor);
        printf("DENmaior: %lf - (%i,%i)\n", denmaior, imaior, jmaior);
        printf("DENmenor: %lf - (%i,%i)", denmenor, imenor, jmenor);

        fclose(file);
        close_file(matriz_adjacencia,total, demaior,demenor,denmaior, denmenor);
        
        break;
    case 2:
    int i = 0;
    int j = 0;
        FILE *csvMatriz = fopen("Grafo.txt", "r");
        if (csvMatriz == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return -1;
        }
        char line[MAX_LINE_LENGTH];
        fgets(line, sizeof(line), csvMatriz);
    while (fgets(line, sizeof(line), csvMatriz)) {
        i = 0;
        char *token;
        token = strtok(line, ",");
        while (token != NULL) {
            
            matriz_adjacencia[j][i] = atoi(token);
            token = strtok(NULL, ","); 
            i++;
        }
        j++;
    }
    printf("\n");

    for(int i = 0; i < 150; i ++){
        for( int j = 0; j < 150 ; j++){
            printf("%i",matriz_adjacencia[i][j]);
        }
        printf("\n");
    }


    fclose(csvMatriz);
    
    return 0;
        
        break;
    
    default:
        printf("\nOpçao invalida\nTente novamente");
        break;
    }
        return 0;
}

void leituramatriz(int matriz[150][150]){

}

void distancia_euclideana(float matrix[150][150], flower flores[]){
    int i = 0, j = 0;
    for(i = 0; i < 149; i++){
        for(j = i+1; j < 150; j++){
            matrix[i][j] = sqrt(pow((flores[i].sepLength - flores[j].sepLength), 2) +
                                pow((flores[i].sepWidth - flores[j].sepWidth), 2) +
                                pow((flores[i].petLength - flores[j].petLength), 2) +
                                pow((flores[i].petWidth - flores[j].petWidth), 2));
            matrix[j][i] = matrix[i][j];
        }
    }
}

void achamaioremenor(float *maior, float *menor, float matriz[150][150], int *a, int *b, int *c, int *d){
    int i = 0, j = 0;

    *maior = matriz[0][1];
    *menor = matriz[0][1]; //elemento inicial e ir comparando
    for(i = 0; i < 149; i++){
        for(j = i+1; j < 150; j++){
            if(matriz[i][j] > *maior){
                *maior = matriz[i][j];
                *a = i;
                *b = j;
            }
            if((matriz[i][j] < *menor)){
                *menor = matriz[i][j];
                *c = i;
                *d = j;
            }
        }
    }
}

void distancia_normalizada(float *maior, float *menor, float matrix[150][150]){

    for(int i = 0; i < 149; i++){
        for(int j = i+1; j < 150; j++){
            if(i != j){
                matrix[i][j] = (matrix[i][j] - *menor) / (*maior - *menor);
                matrix[j][i] = matrix[i][j];
            }
        }
    }
}

void printa_matriz(float matriz[150][150]){
    for(int i = 0; i < 150; i++){
        printf("%i -> ", i);
        for(int j = 0; j < 150; j++){
            printf("%.2f ", matriz[i][j]);
        }
        printf("\n");
    }
}

void close_file(int matriz[150][150], int total, float demaior, float demenor, float denmaior, float denmenor){
    FILE *close = fopen("Grafo.txt", "w");
    if(close){
        fprintf(close,"%i,%f,%f,%f,%f",total,demaior,demenor,denmaior, denmenor);
        fprintf(close,"\n");
        for(int i = 0; i < 150; i++){
            for(int j = 0; j < 150; j++){
                fprintf(close, "%i", matriz[i][j]);
                if(j < 149){
                    fprintf(close, ",");
                }
            }
            fprintf(close, "\n");
        }
    }
    fclose(close);
}