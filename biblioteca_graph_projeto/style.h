#include "Clic.h"
#include "Graph.h"           // Biblioteca importante para as duas últimas funções
#include "mecanismo_busca.h" // Biblioteca importante para as duas últimas funções

#define MAX_OPTIONS    8  // total de índices no vetor de strings do menu
#define LEAVE_OPTION   8  // respectivo índice do vetor (para saída) somado a 1; index = i + 1
#define OKAY           1
#define NOT_OKAY       0

typedef enum Say{
    // Discrimina uma variável do tipo inteiro
    Say_IDsite    = 0,
    Say_originID  = 1,
    Say_destinyID = 2,
    Say_removeID  = 3,

    // Discrimina uma variável do tipo char
    Say_URLsite   = 4,
    Say_NAMEsite  = 5,
    Say_keyWord   = 6,
    Say_search    = 7,

    // Discrimina uma variável do tipo float
    Say_PESOsite  = 8,
}Say;

typedef enum State {
    ERROR = 0,
    WAIT = 1,
    SUCCESS = 2,
    OTHER_SCREEN = 3,
}State;

// Verifica se o id é válido
int Style_idVerify(int MAIORid, int userAnswer);

// Mostra uma mensagem abaixo da caixa de input
void Style_underInput(State State);

// Imprime a arte da interface inicial
void Style_interfaceInicial();

// Printa o menu, retorna visualmente o que o usuário está selecionando e retorna o valor associado a cada opção
int Style_showMenu();

// Printa uma caixa de input e recebe a resposta do usuário (aceita diferentes formatos)
void Style_input(void *userAnswer, Say Say);

// Verifica se a tela possui tamanho ideal para execução do código (garante uma exebição agradável dos elementos visuais)
int Style_executionVerify();

// Animação inicial de inicialização
void Style_startProgram();

// Mostra os links entre os sites
void Style_showLinks();

// Printa o id do site, o nome do site, o somatório da sua importância e o peso da sua aresta
void Style_showTable(Graph *g);

// Encontra os sites que possuem a expressão desejada e retorna para a tela
void Style_searchResult(Graph *g, IndiceInvertido *ind, char *expressao);