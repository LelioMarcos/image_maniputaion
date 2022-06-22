#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float **rotacionar_n(float b, float **imagem, int *w, int *h) {
    int x, y, new_i, new_j, new_w, new_h;

    x = 0;
    y = 0;

    if (b < 0) new_i = ((x - (*w/2)) * sin(-b) + (y - (*h/2)) * cos(-b)) + (*h/2);
    else new_i = ((x - (*w/2)) * sin(b) + (y - (*h/2)) * cos(b)) + (*h/2);
    
    x = 0;
    y = *h - 1;
    
    if (b < 0) new_j = ((x - (*w/2)) * cos(-b) - (y - (*h/2)) * sin(-b)) + (*w/2);
    else new_j = ((x - (*w/2)) * cos(b) - (y - (*h/2)) * sin(b)) + (*w/2);

    new_h = *h + (-new_i)*2;
    new_w = *w + (-new_j)*2;

    float **nova_imagem = alocar_matriz(new_w, new_h);

    if (nova_imagem == NULL) {
        return NULL;
    }

    for (int i = 0; i < new_w; i++) {
        for (int j = 0; j < new_h; j++) {
            new_i = ((i - (new_w/2)) * cos(b) - (j - (new_h/2)) * sin(b)) + (*w/2);
            new_j = ((i - (new_w/2)) * sin(b) + (j - (new_h/2)) * cos(b)) + (*h/2);

            if (new_j >= 0 && new_j < *h && new_i >= 0 && new_i < *w) {
                nova_imagem[i][j] = imagem[new_i][new_j];
            }
        }
    }

    *w = new_w;
    *h = new_h;

    return nova_imagem;
}