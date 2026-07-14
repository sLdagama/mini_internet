#include <stdlib.h>
#include "Style.h"

// Função auxiliar que centraliza uma frase para printá-la
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
void Style_interfaceInicial(){
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

void Style_printBox(){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    Clic_printBox(w, h);
}

// Mostra o menu e retorna o respectivo número da opção selecionada
int Style_mostraMenu(){
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
        Style_printBox();
        Clic_move(10, 20);
        printf("█   █ █████ █   █ █   █ ");
        Clic_move(11, 20);
        printf("██ ██ █     ██  █ █   █ ");
        Clic_move(12, 20);
        printf("█ █ █ ████  █ █ █ █   █ ");
        Clic_move(13, 20);
        printf("█   █ █     █  ██ █   █ ");
        Clic_move(14, 20);
        printf("█   █ █████ █   █  ███  ");
        for(int i = 0; i < 6; i++){
            if(i == linha){
                Clic_setBackgroundColor(Color_GREEN);
                Clic_setFontColor(Color_BLACK);
            }
            if((linha == 4)&&(i == 4)){
                Clic_setBackgroundColor(Color_RED);
                Clic_setFontColor(Color_BLACK);
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

char *Style_input(){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    Clic_move(2, w/2);
    Clic_printBox(w/2, 5);
    Clic_keyCapture();
}