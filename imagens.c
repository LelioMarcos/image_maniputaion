/*
        Manipulação de Imagens
*/

#include <stdio.h>
#include <stdlib.h>

// Funcao para trocar os valores de 2 posicoes na matriz.
void swap(float **matriz, int i, int j, int dest_i, int dest_j) {
    float aux = matriz[i][j];
    matriz[i][j] = matriz[dest_i][dest_j];
    matriz[dest_i][dest_j] = aux;
}

float **alocar_matriz(int w, int h) {
    float **matriz;

    matriz = (float**)calloc(w, sizeof(float*));

    if (matriz == NULL) {
        printf("Memoria nao alocada");
        return NULL;
    }

    for (int i = 0; i < w; i++) {
        matriz[i] = (float*)calloc(h, sizeof(float));

        if (matriz[i] == NULL) {
            printf("Memoria nao alocada");

            for (int k = 0; k < i; k++) {
                free(matriz[k]);
            }

            free(matriz);
            return NULL;
        }
    }

    return matriz;
}

void dealocar_matriz(float **matriz, int w) {
    for (int i = 0; i < w; i++) {
        free(matriz[i]);
    }

    free(matriz);
}

float **abrir_imagem(FILE *input, int *w, int *h) {
    fscanf(input,"%d %d\n", w, h);

    float **imagem = alocar_matriz(*w, *h);

    if (imagem == NULL) {
        fclose(input);
        return NULL;
    }

    for (int i = 0; i < *w; i++) {
        for (int j = 0; j < *h; j++) {
            fscanf(input, "%f ", &imagem[i][j]);
        }
    }

    return imagem;
}

void salvar_imagem(FILE* output, float **imagem, int w, int h) {
    fprintf(output, "%d %d\n", w, h);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            fprintf(output, "%.2f ", imagem[i][j]);
        }
        fprintf(output, "\n");
    }
}

// Mudar o brilho da imagem de acordo com uma constante b.
void brilho(float b, float **imagem, int w, int h) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            imagem[i][j] = b * imagem[i][j];

            if (imagem[i][j] > 1) {
                imagem[i][j] = 1;
            }
        }
    }
}

// Muda a direcao da imagem. dir = 0 muda verticalmente e
// dir = 1 muda horizontalmente.
void inverter(int dir, float **imagem, int w, int h) {
    if (dir) { // Horizontal
        for (int i = 0; i < w / 2; i++) {
            for (int j = 0; j < h; j++) {
                swap(imagem, i, j, (w - i) - 1, j);
            }
        }
    } else { // Vertical
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h / 2; j++) {
                swap(imagem, i, j, i, (h - j) - 1);
            }
        }
    }
}

// Rotaciona a imagem 90 graus para alguma direcao. dir = 0 gira para esquerda
// e dir = 1 gira para a direita.
float **rotacionar(int dir, float **imagem, int *w, int *h) {
    float **nova_imagem = alocar_matriz(*h, *w);

    if (nova_imagem == NULL) {
        return NULL;
    }

    if (dir) { // Direita.
        for (int i = 0; i < *w; i++) {
            for (int j = 0; j < *h; j++) {
                nova_imagem[(*h - j) - 1][i] = imagem[i][j];
            }
        }
    } else { // Esquerda
        for (int i = 0; i < *w; i++) {
            for (int j = 0; j < *h; j++) {
                nova_imagem[j][(*w - i) - 1] = imagem[i][j];
            }
        }
    }

    int aux = *w;
    *w = *h;
    *h = aux;

    return nova_imagem;
}

float **escalonar(float x, float **imagem, int *w, int *h) {
    int new_w = *w * x;
    int new_h = *h * x;

    float **nova_imagem = alocar_matriz(new_w, new_h);

    if (nova_imagem == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_w; i++) {
        for (int j = 0; j < new_h; j++) {
            nova_imagem[i][j] = imagem[(int)(i / x)][(int)(j / x)];
        }
    }

    *w *= x;
    *h *= x;

    return nova_imagem;
}

int menu() {
    int op;
    printf("1 - Abrir Imagem\n");
    printf("2 - Brilho\n");
    printf("3 - Inverter\n");
    printf("4 - Rotacionar 90 graus\n");
    printf("5 - Escalonar\n");
    printf("6 - Salvar\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");

    scanf("%d", &op);

    return op;
}

int main() {
    FILE *input, *output;
    char file_name[100];
    float **image_pixels = NULL, **new_image_pixels, b;
    int w, h, d, op;
    int oldw;

    do {
        op = menu();

        switch (op) {
            case 0:
                printf("Saindo");
                break;
            case 1:
                printf("Digite o nome do arquivo (exemplo: arquivo.img): ");
                scanf("%s", &file_name);

                input = fopen(file_name, "r");

                if (input == NULL) {
                    printf("Erro ao abrir o arquivo");
                    return 1;
                }

                image_pixels = abrir_imagem(input, &w, &h);

                image_pixels = abrir_imagem(input, &w, &h);

                if (image_pixels == NULL) {
                    return 1;
                }

                fclose(input);

                fclose(input);

                printf("Imagem Aberta");
                break;
            case 2:
                printf("Digite a intensidade do brilho: ");
                scanf("%f", &b);

                brilho(b, image_pixels, w, h);

                printf("Brilho alterado");
                break;
            case 3:
                printf("Digite a direcao (0 = vertical, 1 = horizintal): ");
                scanf("%d", &d);

                inverter(d, image_pixels, w, h);

                printf("Direcao mudada");
                break;
            case 4:
                printf("Digite a direcao (0 = esquerda, 1 = direita): ");
                scanf("%d", &d);

                // para as operações de rotacionar e escalonar, é preciso criar uma
                // nova matriz, já que essas operações mudam a resolução da imagem.
                oldw = w;
                new_image_pixels = rotacionar(d, image_pixels, &w, &h);
                dealocar_matriz(image_pixels, oldw);

                if (new_image_pixels == NULL) {
                    return 1;
                }

                image_pixels = new_image_pixels;

                printf("Imagem rotacionada");
                break;
            case 5:
                printf("Digite a intensidade do escalonamento: ");
                scanf("%f", &b);

                oldw = w;
                new_image_pixels = escalonar(b, image_pixels, &w, &h);
                dealocar_matriz(image_pixels, oldw);

                if (new_image_pixels == NULL) {
                    return 1;
                }

                image_pixels = new_image_pixels;

                printf("Imagem escalonada");
                break;
            case 6:
                printf("Digite o nome para o arquivo de saida (exemplo: saida.img): ");
                scanf("%s", &file_name);

                printf("Salvando\n");
                output = fopen(file_name, "w");

                if (output == NULL) {
                    printf("Erro ao abrir o arquivo");
                    dealocar_matriz(image_pixels, w);
                    return 1;
                }

                salvar_imagem(output, image_pixels, w, h);

                fclose(output);
                printf("Imagem Salva");
                break;
            default:
                printf("Operacao Invalida");
        }
        printf("\n\n");
    } while (op != 0);

    dealocar_matriz(image_pixels, w);

    return 0;
}
