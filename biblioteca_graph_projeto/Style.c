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
        case Say_NAMEsite:  printf("Insira o nome do site"); 
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
        case Say_PESOsite:  printf("Insira o peso da aresta do site"); 
                            break;                    
    }
}

// Função auxiliar que printa uma caixa do tamanho máximo da tela
void Style_printBox(){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();

    Clic_printBox(w, h);
}

// Função auxiliar para criar uma animação no texto
void Style_animationText(char **text, int times, int state){
    int half_h = Clic_getScreenHeight()/2;

    if(text){
        Clic_clearScreen();

        if(state == FINISHED){
            //BORDAS VERDES AO CONCLUIR/FINALIZAR PROCESSO

            Clic_setFontColor(Color_GREEN);
            Style_printBox();
            Clic_resetColor();
        } else {
            Style_printBox();
        }

        Clic_move(half_h, 0);
        for(int i = 0; i < times; i++){
            Clic_printCenter(text[i]);
            Clic_pause(1);
        }
    }
}

// Função auxiliar que só permite trocar de tela se o usuário apertar qualquer botão
void Style_canProceed(){
    Clic_showCursor();
    printf("\nPressione qualquer tecla para retornar");
    Clic_keyCapture();
    Clic_hideCursor();
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
        Clic_showCursor();
        Clic_keyCapture();
        fclose(f);
    }
}

int Style_showMenu(){
    int linha = 1, tecla, index;
    char *menu[MAX_OPTIONS] = {"Cadastrar site          ",
                               "Cadastrar palavra-chave ",
                               "Cadastrar link          ",
                               "Remover site            ",
                               "Remover link            ",
                               "Pesquisar               ",
                               "Mostrar links           ",
                               "Mostrar ranking         ",
                               "Sair e salvar           ",};

    /* Aqui está o menu propriamente dito, se a linha selecionada é uma
     * qualquer, o seu fundo será verde. Mas, se for a opção "sair",
     * o seu fundo será vermelho.
     */

    do{
        // AJUSTES INICIAIS
        Clic_clearScreen();
        Style_printBox();
        Clic_hideCursor();

        // ARTE ASCII "MENU"
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

        // EXIBIÇÃO DAS OPÇÕES DO MENU
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
            Clic_move(18 + index, 20);
            printf("%s\n", menu[i]);
            Clic_resetColor();
        }

        // MOVIMENTAÇÃO ENTRE AS OPÇÕES DO MENU
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
    double *doubleAnswer = NULL;

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
     * os valores de 4 a 7 se referem a uma variável char. Caso não seja int nem char,
     * é uma variável do tipo float.
     */
    Clic_showCursor();
    if(Say < 4) {
        intAnswer = userAnswer; 
        scanf("%d", intAnswer);
    } else if (Say < 8){
        charAnswer = userAnswer;
        scanf("%[^\n]", temporaryAnswer);
        strcpy(charAnswer, temporaryAnswer);
    } else {
        doubleAnswer = userAnswer;
        scanf("%lf", doubleAnswer);
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


int Style_executionVerify(){
    int w = Clic_getScreenWidth();
    int h = Clic_getScreenHeight();

    Clic_clearScreen();
    if((w < 168)||(h < 44)){
        printf("Proporção da sua tela: %d x %d\n", w, h);
        printf("Proporção ideal: 168 x 44...\n");
        printf("\nAjuste a proporção da sua tela e pressione qualquer botão para verificar novamente");

        // Aguardando confirmação do usuário
        Clic_showCursor();
        Clic_keyCapture();
        Clic_hideCursor();

        // Retorno visual
        Clic_setFontColor(Color_YELLOW);
        printf("\n\nVerificando...");
        Clic_resetColor();
        Clic_pause(1);

        return 0;
    } else {
        return 1;
    }
}

void Style_startProgram(){
    char  *text1[4] = {"Montando as arestas (links)",
                       "Montando as arestas (links).",
                       "Montando as arestas (links)..",
                       "Montando as arestas (links)..."};
    char  *text2[4] = {"Puxando os sites para a memoria", 
                       "Puxando os sites para a memoria.", 
                       "Puxando os sites para a memoria..", 
                       "Puxando os sites para a memoria..."};
    char  *text3[1] = {"Base de dados totalmente carregada no Grafo!"};

    Clic_hideCursor();
    Style_animationText(text1, 4, NOT_FINISHED);
    Style_animationText(text2, 4, NOT_FINISHED);
    Style_animationText(text3, 1, FINISHED);
    Clic_showCursor();
}

void Style_showLinks(Graph *g){
    if(g){
        Clic_clearScreen();
        Graph_print(g);
        Style_canProceed();
    }
}

void Style_showRank(Graph *g) {
    Vertex *v = g->first;

    Clic_clearScreen();
    printf("\n[RANKING ATUAL DOS SITES]\n");
    if (!v) {
        printf("  O grafo esta vazio!\n");
        return;
    }
    while (v) {
        Site *s = (Site *)v->value;
        printf("  ID: %d | Site: %-15s | Importancia: %.2f\n", v->label, s->nome, s->importancia);
        v = v->next;
    }

    Style_canProceed();
}

void Style_searchResult(Graph *g, IndiceInvertido *ind, char *expressao) {
    int qtd = 0;
    Vertex **resultados = Buscador_realizarConsultaCompleta(g, ind, expressao, &qtd); /* A função realizar a consulta para os seguintes casos:
                                                                                       * - Expressão com mais de uma palavra (AND implícito)
                                                                                       * - Expressão com AND;
                                                                                       * - Expressão com OR.
                                                                                       */
    
    Clic_clearScreen();
    printf("\nResultados para: '%s'\n", expressao);
    if (resultados && qtd > 0) {
        for (int i = 0; i < qtd; i++) {
            Site *s = (Site *)resultados[i]->value;

            // Os resultados saem seguindo uma ordem de importância
            printf("  %d. %s (Importância: %.2f)\n", i + 1, s->url, s->importancia);
        }
        free(resultados); // Evita vazamento de memória do array retornado
    } else {
        Clic_setFontColor(Color_RED);
        printf("  -> Nenhum resultado encontrado para esta pesquisa.\n");
        Clic_resetColor();
    }

    Style_canProceed();
}