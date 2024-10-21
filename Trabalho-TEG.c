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
float normaliza(float x, float maior, float menor);

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
    }

    float temp = 0, maior_DE = 0, menor_DE;
    int i_maior = 0, j_maior = 0, i_menor = 0, j_menor = 0;
    temp = distancia_euclidiana(flowers[0], flowers[1]);// inicializa o maior e menor fora do loop
    maior_DE = temp;
    menor_DE = temp;

    //Preenche a matriz com distâncias euclidianas não normalizadas
    for(int i = 0; i < 150; i++){
        for(int j = i+1; j < 150; j++){
            temp = distancia_euclidiana(flowers[i], flowers[j]);
            adjMatrix[i][j] = temp;
            adjMatrix[j][i] = adjMatrix[i][j];
            if (temp > maior_DE){
                maior_DE = temp;
                i_maior = i;
                j_maior = j;
            } else if (temp < menor_DE){
                menor_DE = temp;
                i_menor = i;
                j_menor = j;
            }
        }
    }

    for(int i = 0; i < 150; i++){
        for(int j = i+1; j < 150; j++){
            adjMatrix[i][j] = normaliza(adjMatrix[i][j], maior_DE, menor_DE);
            adjMatrix[j][i] = adjMatrix[i][j];
        }
    }

    printf("Maior: %f (%i,%i)\nMenor: %f (%i,%i)\n", maior_DE, i_maior, j_maior, menor_DE, i_menor, j_menor);
    printf("Maior: %f (%i,%i)\nMenor: %f (%i,%i)\n", normaliza(maior_DE, maior_DE, menor_DE), i_maior, j_maior, normaliza(menor_DE, maior_DE, menor_DE), i_menor, j_menor);

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

float normaliza(float x, float maior, float menor){
    float resultado = 0;

    resultado = (x - menor)/(maior - menor);

    return resultado;
}