#include "Clic.h"

// BIBLIOTECAS IMPORTANTES PARA A PENÚLTIMA FUNÇÃO
#include "Graph.h"
#include "mecanismo_busca.h"

#define MAX_OPTIONS    9  // total de índices no vetor de strings do menu
#define LEAVE_OPTION   9  // respectivo índice do vetor (para saída) somado a 1; index = i + 1
#define SEARCH_OPTION  6  // respectivo índice do vetor (para consulta/pesquisa) somado a 1; index = i + 1
#define NOT_FINISHED  -1
#define FINISHED       1

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
    Say_PESOsite  = 8,
}Say;

// Imprime a arte da interface inicial
void Style_interfaceInicial();

// Printa o menu, retorna visualmente o que o usuário está selecionando e retorna o valor associado a cada opção
int Style_showMenu();

// Printa uma caixa de input e recebe a resposta do usuário (ideal para resposta em formato de string)
void Style_input(int option, void *userAnswer, Say Say);

// Verifica se a tela possui tamanho ideal para execução do código (garante uma exebição agradável dos elementos visuais)
int Style_executionVerify();

// Animação inicial de inicialização
void Style_startProgram();

// Mostra os links
void Style_showLinks();

// Printa o id do site, o nome do site e o seu atual valor de importância (função adaptada para essa biblioteca)
void Style_showRank(Graph *g);

// Encontra os sites que possuem a expressão desejada e retorna para a tela
void Style_searchResult(Graph *g, IndiceInvertido *ind, char *expressao);