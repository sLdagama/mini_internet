#include "Clic.h"

// BIBLIOTECAS IMPORTANTES PARA A PENÚLTIMA FUNÇÃO
#include "Graph.h"
#include "mecanismo_busca.h"

// Imprime a arte da interface inicial
void Style_interfaceInicial();

// Printa uma caixa do tamanho da tela (cria as bordas)
void Style_printBox();

// Printa o menu, retorna visualmente o que o usuário está selecionando e retorna o valor associado a cada opção
int Style_mostraMenu();

// Printa uma caixa de input e recebe a resposta do usuário (ideal para resposta em formato de string)
void Style_inputChar(int option, char *userAnswer);

// Printa uma caixa de input e recebe a resposta do usuário (ideal para resposta em valor inteiro)
void Style_inputInt(int option, int *userAnswer);

// Printa o id do site, o nome do site e o seu atual valor de importância (função adaptada para essa biblioteca)
void Style_mostraRank(Graph *g);

// Verifica se a tela possui tamanho ideal para execução do código
//int Style_executionVerification();