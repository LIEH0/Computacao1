
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

FILE *arquivo;

char linha[100];
char nome[100];
char nota[3][10];

float nota1, nota2, nota3;
float media, soma_medias = 0;

int contador = 0;
int i, j, campo;

int main() {

    setlocale(LC_ALL, "Portuguese");

    arquivo = fopen("notas.txt", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    fprintf(arquivo, "Luisa Fernanda, 7.8, 4.4, 6.5\n");
    fprintf(arquivo, "Pedro Henrique, 2.2, 9.5, 8.0\n");
    fprintf(arquivo, "Vitor Colatusso, 5.4, 10, 5.5\n");
    fprintf(arquivo, "Agatha Helena, 5.6, 7.7, 9.0\n");

    fclose(arquivo);

    arquivo = fopen("notas.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return 1;
    }

    printf("Notas da disciplina INF71A ao final de 2024/1\n\n");

    while (fgets(linha, 100, arquivo) != NULL) {

        j = 0;
        campo = 0;

        for (i = 0; linha[i] != '\0'; i++) {

            if (linha[i] == ',') {

                if (campo == 0) {
                    nome[j] = '\0';
                } else {
                    nota[campo - 1][j] = '\0';
                }

                campo++;
                j = 0;
            }

            else if (campo == 0) {
                nome[j] = linha[i];
                j++;
            }

            else if (linha[i] != ' ' && linha[i] != '\n') {
                nota[campo - 1][j] = linha[i];
                j++;
            }
        }

        nota[2][j] = '\0';

        nota1 = atof(nota[0]);
        nota2 = atof(nota[1]);
        nota3 = atof(nota[2]);

        media = (nota1 + nota2 + nota3) / 3;

        printf("Aluno: %s\n", nome);
        printf("Media: %.2f\n\n", media);

        soma_medias = soma_medias + media;

        contador++;
    }

    fclose(arquivo);

    // Calculando a média da turma
    if (contador > 0) {
        printf("Media da turma: %.2f\n", soma_medias / contador);
    }

    return 0;
}
