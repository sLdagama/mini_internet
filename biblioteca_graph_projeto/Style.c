#include <stdio.h>
#include <stdlib.h>
#include "Style.h"

// Limpa o buffer do teclado
void Style_cleanBuffer(){
    int c;
    while ((c = getchar()) != '\n');
}

// Lê o arquivo .txt que contém a arte ASCII da interface inicial
void Style_interfaceInicial(){
    Clic_clearScreen();
    char c;
    char arq[] = "interface_inicial.txt";
    char text[] = "Press any button to continue";
    FILE *f = fopen(arq, "r");
    if(f){
        while(fscanf(f, "%c", &c) == 1){
            printf("%c", c);
        }
        printf("\n\n");
        Clic_printCenter(text);
        Clic_keyCapture();
        fclose(f);
    }
}

// Printa uma caixa do tamanho máximo da tela
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

// Printa uma caixa de entrada customizada para cada opção e obtém a resposta do usuário
void Style_input(int option, char *userAnswer){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    char temporaryAnswer[1000];
    char text1[] = "Aguarde...";
    char text2[] = "Operação realizada com sucesso!";
    char text3[] = "Encaminhando para uma outra tela...";

    // Para cada opção, será mostrado uma frase diferente, conforme o que for solicitado
    Clic_move((h*0.7)-1, w*0.27);
    if(option == 0){
        printf("Insira o site que deseja cadastrar");
    } else if(option == 1){
        printf("Insira os sites que deseja linkar");
    } else if(option == 2){
        printf("Insira o site que deseja remover");
    } else if(option == 3){
        printf("Realize sua pesquisa");
    }

    // Printa a caixa de input
    Clic_move(h*0.7, w*0.27);
    Clic_saveCursorPosition();
    Clic_printBox(w/2, 5);
    Clic_restoreCursorPosition();

    // Move o cursor para dentro da caixa de input
    Clic_moveDown(2);
    Clic_moveRight(2);

    // Obtém a resposta propriamente dita
    scanf("%[^\n]", temporaryAnswer);
    strcpy(userAnswer, temporaryAnswer);

    // Retorno visual intermediário
    Clic_setFontColor(Color_YELLOW);
    Clic_move(h*0.7 + 5, 6);
    Clic_pause(1.5);
    Clic_printCenter(text1);

    // Retorno visual final
    Clic_move(h*0.7 + 6, 6);
    Clic_pause(1.5);
    if(option != 3){
        Clic_setFontColor(Color_GREEN);
        Clic_printCenter(text2);
    } else {
        Clic_setFontColor(Color_YELLOW);
        Clic_printCenter(text3);
    }
    Clic_pause(1.5);

    // Ajustes finais necessários
    Clic_resetColor();
    Style_cleanBuffer();
}

//void Style_executionVerification();