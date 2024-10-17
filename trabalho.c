#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define MAX_LINE_LENGTH 1024


int main() {
int i = -1;
    FILE *file = fopen("IrisDataset.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        i++;
        char *token;
        token = strtok(line, ",");
        token = strtok(NULL, ",");
      
         while (token != NULL) {
            printf("%s\t%i\t", token,i);
            token = strtok(NULL, ","); 
        }
       
    

        
    }
    


    fclose(file);
    return 0;
}