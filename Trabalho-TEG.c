#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct flower{
    float sepLength;
    float sepWidth;
    float petLength;
    float petWidth;
} flower;

float distancia_euclidiana(flower f1, flower f2);

int main(){

    flower flowers[150];
    float adjMatrix[150][150];

    FILE* file = fopen("irisDataset.csv", "r");
    if (file == NULL){
        printf("Erro ao abrir o arquivo\n");
        return -1;
    }

    //Pula a primeira linha do csv
    fscanf(file, "%*[^\n]\n");

    //Lê o resto das linhas
    for(int i = 0; i < 150; i++){
        fscanf(file, "%*[^,],%f,%f,%f,%f", &flowers[i].petLength, &flowers[i].petWidth, &flowers[i].sepLength, &flowers[i].sepWidth); //tem que usar [^,] pq a string da variedade não tem o \0 
        // printf("Flower %i: %f\t%f\t%f\t%f\n", i+1, flowers[i].petLength, flowers[i].petWidth, flowers[i].sepLength, flowers[i].sepWidth);
    }

    printf("%f\n", distancia_euclidiana(flowers[0], flowers[149]));

    //Preenche a matriz com distâncias euclidianas não normalizadas
    for(int i = 0; i < 150; i++){
        for(int j = i+1; j < 150; j++){
            if(i != j){
                adjMatrix[i][j] = distancia_euclidiana(flowers[i], flowers[j]);
                adjMatrix[j][i] = adjMatrix[i][j];
            }
        }
    }

    printf("%f\n", adjMatrix[0][149]);

    fclose(file);

    return 0;
}

float distancia_euclidiana(flower f1, flower f2){
    float resultado = 0;
    for(int i = 0; i < 150; i++){
        for(int j = 0; j < 150; j++){
            resultado = sqrt(pow((f1.sepLength - f2.sepLength), 2) +
            pow((f1.sepWidth - f2.sepWidth), 2) +
            pow((f1.petLength - f2.petLength), 2) +
            pow((f1.petWidth - f2.petWidth), 2));
        }
    }
    return resultado;
}