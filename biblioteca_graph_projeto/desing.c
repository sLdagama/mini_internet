#include <stdlib.h>
#include "desing.h"

// Centraliza uma frase para printá-la
void printCentralizado(char *c){
    int w = Clic_getScreenWidth(), tamanho;
    tamanho = strlen(c);
    w = (w - tamanho)/2;
    for(int i = 0; i < w; i++){
        printf(" ");
    }
    printf("%s", c);
}

// Lê o arquivo .txt que contém a arte ASCII da interface inicial
void interfaceInicial(){
    Clic_clearScreen();
    char arq[] = "interface_inicial.txt";
    FILE *f = fopen(arq, "r");
    char c;
    char c2[] = "Press any button to continue";
    if(f){
        while(fscanf(f, "%c", &c) == 1){
            printf("%c", c);
        }
        printf("\n\n");
        printCentralizado(c2);
        Clic_keyCapture();
        fclose(f);
    }
}

// Mostra o menu e retorna o respectivo número da opção selecionada
int mostraMenu(){
    int linha = 0, tecla;
    char *menu[6] = {"Cadastrar site",
                     "Cadastrar link",
                     "Remover site",
                     "Pesquisar",
                     "Sair",
                     "*Mostrar grafo"}; // ÚLTIMA OPÇÃO É TEMPORÁRIA

    // Aqui está o menu propriamente dito, se a linha selecionada é uma
    // qualquer, o seu fundo será verde. Mas, se for a opção "sair",
    // o seu fundo será vermelho.
    do{
        Clic_clearScreen();
        for(int i = 0; i < 6; i++){
            if(i == linha){
                Clic_setBackgroundColor(Color_GREEN);
            }
            if((linha == 4)&&(i == 4)){
                Clic_setBackgroundColor(Color_RED);
            }
            Clic_move(20 + i, 20);
            printf("%s\n", menu[i]);
            Clic_resetColor();
        }
        tecla = Clic_keyCapture();
        if((tecla == Key_DOWN)&&(linha < 6)){
            linha++;
        } else if((tecla == Key_UP)&&(linha > 0)){
            linha--;
        }
    } while(tecla != Key_ENTER);
    return linha;
}
