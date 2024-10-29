#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LINE_LENGTH 1024

typedef struct flower{
    float sepLength;
    float sepWidth;
    float petLength;
    float petWidth;
} flower;

// Acho q dá pra isso ser só uma flor normal, dps vou ver
typedef struct Media{
    double msepLength;
    double msepWidth;
    double mpetLength;
    double mpetWidth;
} Media;

void distancia_euclideana(float matriz[150][150], flower flores[]);
void achamaioremenor(float *maior, float *menor, float matriz[150][150], int *a, int *b, int *c, int *d);
void distancia_normalizada(float *maior, float *menor, float matriz[150][150]);
void printa_matriz(float matriz[150][150]);
void close_file(int matriz[150][150], int total, float demaior, float demenor, float denmaior, float denmenor);
void DFS(int **grupo, int *tam, bool visitado[150], int ponto, int matriz[150][150]);
void centro(int *grupo, int tam, int indice, Media m[], flower flores[]);



int main() {
    flower flores[150];
    Media media[2] = {0};
    int matriz_adjacencia[150][150] = {0};
    float matrix[150][150] = {0};
    int i = 0;
    float demaior = 0, demenor = 0, denmaior = 0, denmenor = 0;
    int imaior = 0, jmaior = 0, imenor = 0, jmenor = 0, total = 0, opcao = -1;
    int ponto = 0;
    bool visitado[150] = {false};

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
    // Leitura do dataset e montagem do grafo

        // Abertura e leitura arquivo
        FILE *file = fopen("IrisDataset.csv", "r");
        if (file == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return -1;
        }

        // Pula a primeira linha do csv
        fscanf(file, "%*[^\n]\n");

        // Lê o resto das linhas
        for(int i = 0; i < 150; i++){
            total++;
            fscanf(file, "%*[^,],%f,%f,%f,%f", &flores[i].sepLength, &flores[i].sepWidth, &flores[i].petLength, &flores[i].petWidth); //tem que usar [^,] pq a string da variedade não tem o \0 
            //printf("Flower %i: %f\t%f\t%f\t%f\n", i+1, flowers[i].petLength, flowers[i].petWidth, flowers[i].sepLength, flowers[i].sepWidth);
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

        printf("Total de flores: %i\n", total);
        denmaior = matrix[imaior][jmaior];
        denmenor = matrix[imenor][jmenor];
        printf("DEmaior: %lf - (%i,%i)\n", demaior, imaior, jmaior);
        printf("DEmenor: %lf - (%i,%i)\n", demenor, imenor, jmenor);
        printf("DENmaior: %lf - (%i,%i)\n", denmaior, imaior, jmaior);
        printf("DENmenor: %lf - (%i,%i)\n", denmenor, imenor, jmenor);

        fclose(file);

        close_file(matriz_adjacencia,total, demaior,demenor,denmaior, denmenor);
        
        break;
    case 2:
    // Releitura do grafo

        FILE *txtGrafo = fopen("grafo.txt", "r");
        if (txtGrafo == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return -1;
        }

        // Pula as primeiras 5 linhas do txt (Cabeçalho)
        for(int i = 0; i < 5; i++){
            fscanf(txtGrafo, "%*[^\n]\n");
        }

        int i = 0, j = 0, aux = 0;

        do {
            aux = fscanf(txtGrafo, "%d, %d", &i, &j);
            matriz_adjacencia[i][j] = 1;
            matriz_adjacencia[j][i] = 1;
        }
        while(aux != EOF); // EOF = End of File = -1


        for(int i = 0; i < 150; i++){
            for( int j = 0; j < 150 ; j++){
                printf("%i",matriz_adjacencia[i][j]);
            }
            printf("\n");
        }


        fclose(txtGrafo);
        
        break;
    
    default:
        printf("\nOpçao invalida\nTente novamente");
        break;
    }

    // Aloca o primeiro grupo (Pois sempre existirá pelo menos 1 grupo)
    int* tamanhos_dos_grupos = malloc(sizeof(int));
    tamanhos_dos_grupos[0] = 0;
    int** grupos = malloc(sizeof(int*));
    *(grupos) = malloc(sizeof(int));

    int num_grupos = 0;

    do{
        DFS((grupos + num_grupos), (tamanhos_dos_grupos + num_grupos), visitado, ponto, matriz_adjacencia); // Preenche o grupo

        for (i = 0; i < 150; i++) { // Vê se tem algum vértice não visitado sobrando
            if (!visitado[i]) { // Se sim, cria mais um grupo
                ponto = i;
                num_grupos++;
                grupos = realloc(grupos, sizeof(int*) * (num_grupos + 1));// Aloca um ponteiro de grupo
                *(grupos + num_grupos) = malloc(sizeof(int)); // Aloca um novo grupo
                tamanhos_dos_grupos = realloc(tamanhos_dos_grupos, sizeof(int) * num_grupos);
                *(tamanhos_dos_grupos + num_grupos) = 0;
                break;
            }
        }

    } // O while deve para quando todo o vetor de visitados for true
    while(visitado[ponto] != true);

    printf("Numero de grupos: %i\n", num_grupos + 1);

    for(int i = 0; i <= num_grupos; i++){
        printf("Grupo %i:\n", i);
        for(int j = 0; j < tamanhos_dos_grupos[i]; j++){
            printf("%i ", grupos[i][j]);
        }
        printf("\n");
    }

    // printf("===================\n");
    // printf("Buscando o centro de cada grupo:");
    // centro(grupos[0], tamSetosa, 1, media, flores);
    // centro(grupoNaoSetosa, tamNaoSetosa, 2, media, flores);

    // for(int i = 1; i < 3; i++){
    //     printf("\nCentro Grupo[%i]:\nSepLength: %lf\nSepWidth: %lf\nPetLength: %lf\nPetWidth: %lf\n", i, media[i].msepLength, media[i].msepWidth, media[i].mpetLength, media[i].mpetWidth);
    //     printf("===================");
    // }

    // Liberar a memória dos grupos
    for(int i = 0; i < num_grupos; i++){
        free(grupos[i]);
    }
    free(tamanhos_dos_grupos);

    return 0;
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

// Tem que lembrar de passar o i e j dos valores tbm
void close_file(int matriz[150][150], int total, float demaior, float demenor, float denmaior, float denmenor){
    FILE *close = fopen("grafo.txt", "w");

    if(close){
        fprintf(close,"%i\n", total);
        fprintf(close,"Maior Distância Euclideana: %f\n", demaior);
        fprintf(close,"Menor Distância Euclideana: %f\n", demenor);
        fprintf(close,"Maior Distância Euclideana Normalizada: %f\n", denmaior);
        fprintf(close,"Menor Distância Euclideana Normalizada: %f", denmenor);
        for(int i = 0; i < 150; i++){
            for(int j = i+1; j < 150; j++){
                if(matriz[i][j] != 0)
                fprintf(close, "\n%i, %i", i, j);
            }
        }
    }

    fclose(close);
}

void DFS(int **grupo, int *tam, bool visitado[150], int ponto, int matriz[150][150]) {
    visitado[ponto] = true; 
    // Aumentar o tam do grupo e realocar memória
    *tam += 1;
    *grupo = realloc(*grupo, sizeof(int) * (*tam));
    (*grupo)[(*tam) - 1] = ponto; // Adiciona o ponto ao grupo

    // Varrer o grafo e procurar conexões
    for (int i = 0; i < 150; i++) {
        if (matriz[ponto][i] == 1 && !visitado[i]) {
            DFS(grupo, tam, visitado, i, matriz); //recursao
        }
    }  
}

void centro(int *grupo, int tam, int indice, Media m[], flower flores[]){
    int vertice = 0;
    m[indice].msepLength = 0;
    m[indice].msepWidth = 0;
    m[indice].mpetLength = 0;
    m[indice].mpetWidth = 0;

    for(int i = 0; i < tam; i++){
        vertice = grupo[i];
        m[indice].msepLength += flores[vertice].sepLength;
        m[indice].msepWidth += flores[vertice].sepWidth;
        m[indice].mpetLength += flores[vertice].petLength;
        m[indice].mpetWidth += flores[vertice].petWidth;
    }

    m[indice].msepLength = m[indice].msepLength/tam;
    m[indice].msepWidth = m[indice].msepWidth/tam;
    m[indice].mpetLength = m[indice].mpetLength/tam;
    m[indice].mpetWidth = m[indice].mpetWidth/tam;
}