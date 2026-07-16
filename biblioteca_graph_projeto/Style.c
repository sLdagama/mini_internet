#include <stdio.h>
#include <stdlib.h>
#include "Style.h"
#define MAX_OPTIONS 8

// Função auxiliar para limpar o buffer do teclado
void Style_cleanBuffer(){
    char c;
    while ((c = getchar()) != '\n');
}

// Função auxíliar que imprime uma mensagem customizada acima da caixa de input
void Stype_customMessage(int option){
    if(option == 0){
        printf(" "); //cadastro de site
    } else if(option == 1){
        printf(" "); //cadastro de palavra chave
    } else if((option == -2)||(option == -4)){
        printf("Insira o ID de origem");
    }else if((option == 2)||(option == 4)){
        printf("Insira o ID de destino");
    } else if(option == 3){
        printf("Insira o ID do site que deseja remover");
    } else if(option == 5){
        printf("Realize sua consulta");
    }
}

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

void Style_printBox(){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    Clic_printBox(w, h);
}

int Style_mostraMenu(){
    int linha = 0, tecla;
    char *menu[MAX_OPTIONS] = {"Cadastrar site",
                               "Cadastrar palavra-chave",
                               "Cadastrar link",
                               "Remover site",
                               "Remover link",
                               "Pesquisar",
                               "Sair",
                               "Mostrar rank"};

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
        for(int i = 0; i < MAX_OPTIONS; i++){
            if(i == linha){
                Clic_setBackgroundColor(Color_GREEN);
                Clic_setFontColor(Color_BLACK);
            }
            if((linha == 6)&&(i == 6)){
                Clic_setBackgroundColor(Color_RED);
                Clic_setFontColor(Color_BLACK);
            }
            Clic_move(20 + i, 20);
            printf("%s\n", menu[i]);
            Clic_resetColor();
        }
        tecla = Clic_keyCapture();
        if((tecla == Key_DOWN)&&(linha < (MAX_OPTIONS - 1))){
            linha++;
        } else if((tecla == Key_UP)&&(linha > 0)){
            linha--;
        }
    } while(tecla != Key_ENTER);

    return linha;
}

void Style_inputChar(int option, char *userAnswer){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    char temporaryAnswer[1000];
    char text1[] = "Aguarde...";
    char text2[] = "Operação realizada com sucesso!";
    char text3[] = "Encaminhando para uma outra tela...";

    // Para cada opção, será mostrado uma frase diferente, conforme o que for solicitado
    Clic_move((h*0.7)-1, w*0.27);
    Stype_customMessage(option);

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
    if(option != 5){
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

void Style_inputInt(int option, int *userAnswer){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    char text1[] = "Aguarde...";
    char text2[] = "Operação realizada com sucesso!";

    // Para cada opção, será mostrado uma frase diferente, conforme o que for solicitado
    Clic_move((h*0.7)-1, w*0.27);
    Stype_customMessage(option);

    // Printa a caixa de input
    Clic_move(h*0.7, w*0.27);
    Clic_saveCursorPosition();
    Clic_printBox(w/2, 5);
    Clic_restoreCursorPosition();

    // Move o cursor para dentro da caixa de input
    Clic_moveDown(2);
    Clic_moveRight(2);

    // Obtém a resposta propriamente dita
    scanf("%d", userAnswer);

    if(option > 0){
        // Retorno visual intermediário
        Clic_setFontColor(Color_YELLOW);
        Clic_move(h*0.7 + 5, 6);
        Clic_pause(1.5);
        Clic_printCenter(text1);

        // Retorno visual final
        Clic_move(h*0.7 + 6, 6);
        Clic_pause(1.5);
        Clic_setFontColor(Color_GREEN);
        Clic_printCenter(text2);
        Clic_pause(1.5);
    }

    // Ajustes finais necessários
    Clic_resetColor();
    Style_cleanBuffer();
}

void Style_mostraRank(Graph *g) {
    printf("\n[RANKING ATUAL DOS SITES]\n");
    Vertex *v = g->first;
    if (!v) {
        printf("  O grafo esta vazio!\n");
        return;
    }
    while (v) {
        Site *s = (Site *)v->value;
        printf("  ID: %d | Site: %-15s | Importancia: %.2f\n", v->label, s->nome, s->importancia);
        v = v->next;
    }
}

//int Style_executionVerification();