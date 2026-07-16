#include <stdio.h>
#include <stdlib.h>
#include "Style.h"

// Função auxiliar para limpar o buffer do teclado
void Style_cleanBuffer(){
    char c;
    while ((c = getchar()) != '\n');
}

// Função auxíliar que imprime uma mensagem customizada acima da caixa de input
void Style_customMessage(Say Say){
    switch(Say){
        case Say_IDsite:    printf("Insira o ID do site"); 
                            break;
        case Say_URLsite:   printf("Insira a URL do site"); 
                            break;
        case Say_NAMEsite:  printf("Insira o DNS do site"); 
                            break;
        case Say_keyWord:   printf("Insira a palavra que deseja cadastrar"); 
                            break;
        case Say_originID:  printf("Insira o ID de origem"); 
                            break;
        case Say_destinyID: printf("Insira o ID de destino"); 
                            break;
        case Say_removeID:  printf("Insira o ID do site que deseja remover"); 
                            break;
        case Say_search:    printf("Realize sua consulta"); 
                            break;
    }
}

void Style_printBox(){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();

    Clic_printBox(w, h);
}

void Style_interfaceInicial(){
    char c;
    char arq[] = "interface_inicial.txt";
    char text[] = "Press any button to continue";
    FILE *f = fopen(arq, "r");

    Clic_clearScreen();
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

int Style_showMenu(){
    int linha = 1, tecla, index;
    char *menu[MAX_OPTIONS] = {"Cadastrar site",
                               "Cadastrar palavra-chave",
                               "Cadastrar link",
                               "Remover site",
                               "Remover link",
                               "Pesquisar",
                               "Sair",
                               "Mostrar rank"};

    /* Aqui está o menu propriamente dito, se a linha selecionada é uma
     * qualquer, o seu fundo será verde. Mas, se for a opção "sair",
     * o seu fundo será vermelho.
     */

    do{
        Clic_clearScreen();
        Style_printBox();
        Clic_hideCursor();
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
            index = i + 1;
            if(index == linha){
                Clic_setBackgroundColor(Color_GREEN);
                Clic_setFontColor(Color_BLACK);
            }
            if((linha == LEAVE_OPTION)&&(index == LEAVE_OPTION)){
                Clic_setBackgroundColor(Color_RED);
                Clic_setFontColor(Color_BLACK);
            }
            Clic_move(20 + index, 20);
            printf("%s\n", menu[i]);
            Clic_resetColor();
        }
        tecla = Clic_keyCapture();
        if((tecla == Key_DOWN)&&(linha < MAX_OPTIONS)){
            linha++;
        } else if((tecla == Key_UP)&&(linha > 1)){
            linha--;
        }
    } while(tecla != Key_ENTER);

    return linha;
}

void Style_input(int option, void *userAnswer, Say Say){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();
    char text1[] = "Aguarde...";
    char text2[] = "Operação realizada com sucesso!";
    char text3[] = "Encaminhando para uma outra tela...";
    char temporaryAnswer[1000];
    char *charAnswer = NULL;
    int *intAnswer = NULL;

    // Para cada opção, será mostrado uma frase diferente, conforme o que for solicitado
    Clic_move((h*0.7)-1, w*0.27);
    Style_customMessage(Say);

    // Printa a caixa de input
    Clic_move(h*0.7, w*0.27);
    Clic_saveCursorPosition();
    Clic_printBox(w/2, 5);
    Clic_restoreCursorPosition();

    // Move o cursor para dentro da caixa de input
    Clic_moveDown(2);
    Clic_moveRight(2);

    /* Note que a variável "Say" serve como fator discriminante para saber se estamos
     * lidando com uma variável do tipo int ou char. Veja no arquivo "Style.h" o tipo
     * "Say" que criamos, note que dos valores 0 ao 3, se referem a uma variável int,
     * caso contrário, é uma variável char.
     */
    Clic_showCursor();
    if(Say < 4){
        intAnswer = userAnswer; 
        scanf("%d", intAnswer);
    } else {
        charAnswer = userAnswer;
        scanf("%[^\n]", temporaryAnswer);
        strcpy(charAnswer, temporaryAnswer);
    }
    Clic_hideCursor();

    /* Se "option" for negativo, não haverá retorno visual final
     * (ideal para opções que exigem mais de uma entrada)
     */  
    if(option > 0){
        // Retorno visual intermediário
        Clic_setFontColor(Color_YELLOW);
        Clic_move(h*0.7 + 5, 6);
        Clic_pause(1.5);
        Clic_printCenter(text1);

        // Retorno visual final
        Clic_move(h*0.7 + 6, 6);
        Clic_pause(1.5);
        if(option != SEARCH_OPTION){
            Clic_setFontColor(Color_GREEN);
            Clic_printCenter(text2);
        } else {
            /* Entende-se pela cor verde como "processo concluído", se a opção escolhida 
             * for a opção "pesquisar", não haverá retorno visual de cor verde.
             */ 

            // Segundo retorno visual intermediário
            Clic_setFontColor(Color_YELLOW);
            Clic_printCenter(text3);
        }
        Clic_pause(1.5);
    }

    // Ajustes finais necessários
    Clic_resetColor();
    Style_cleanBuffer();
}

void Style_showRank(Graph *g) {
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