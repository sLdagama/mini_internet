#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "gerenciador.h"
#include "mecanismo_busca.h"

// --- FUNÇÕES AUXILIARES PARA VERIFICAÇÃO VISUAL NO TERMINAL ---

// Verifica o requisito: "calcular a importância de cada site" e "recalcular o ranking após inserção/remoção"
void mostrarRankingGeral(Graph *g) {
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

// Verifica os requisitos: "pesquisar uma palavra", "mais de uma palavra" e "operadores AND e OR"
void testarBusca(Graph *g, IndiceInvertido *ind, char *expressao) {
    printf("\n[BUSCA] Expressao: '%s'\n", expressao);
    int qtd = 0;
    
    // Supondo que a sua função completa já chame internamente a buscaSimples, AND ou OR
    Vertex **resultados = Buscador_realizarConsultaCompleta(g, ind, expressao, &qtd);
    
    if (resultados && qtd > 0) {
        for (int i = 0; i < qtd; i++) {
            Site *s = (Site *)resultados[i]->value;
            // Verifica o requisito de ordenação por importância
            printf("  %d. %s (PageRank: %.2f)\n", i + 1, s->nome, s->importancia);
        }
        free(resultados); // Evita vazamento de memória do array retornado
    } else {
        printf("  -> Nenhum resultado encontrado para esta pesquisa.\n");
    }
}

// --- BATERIA DE TESTES PRINCIPAL ---
/*
int main() {
    printf("====================================================\n");
    printf("   INICIANDO BATERIA DE TESTES - MINI INTERNET      \n");
    printf("====================================================\n\n");

    Graph *grafo = Graph_alloc();
    IndiceInvertido *indice = Indice_alloc();

    // 1. REQUISITO: "carregar uma miniatura da internet previamente salva"
    printf(">>> TESTE 1: Carregamento de Dados\n");
    Buscador_carregarDados(grafo, indice, "sites.txt", "links.txt");
    mostrarRankingGeral(grafo);

    // 2. REQUISITO: "pesquisar uma palavra e listar os sites"
    printf("\n>>> TESTE 2: Busca de palavra simples\n");
    testarBusca(grafo, indice, "codigo");
    testarBusca(grafo, indice, "internet");

    // 3. REQUISITO: "realizar consultas utilizando os operadores AND e OR"
    printf("\n>>> TESTE 3: Consultas Avancadas (AND / OR)\n");
    testarBusca(grafo, indice, "codigo OR sistema"); 
    testarBusca(grafo, indice, "linux terminal"); 

    // 4. REQUISITO: "cadastrar novos links" e "recalcular o ranking"
    printf("\n>>> TESTE 4: Cadastro de Link e Recalculo de Ranking\n");
    printf("Acao: Inserindo link do Google (0) para PopOS (2)\n");
    Buscador_cadastrarLink(grafo, 0, 2);
    mostrarRankingGeral(grafo);

    // 5. REQUISITO: "remover links existentes" e "recalcular o ranking"
    printf("\n>>> TESTE 5: Remocao de Link e Recalculo de Ranking\n");
    printf("Acao: Removendo link do Google (0) para GitHub (1)\n");
    Buscador_removerLink(grafo, 0, 1);
    mostrarRankingGeral(grafo);

    // 6. REQUISITO: "remover sites"
    printf("\n>>> TESTE 6: Remocao Completa de Site\n");
    printf("Acao: Removendo o site GitHub (ID 1)\n");
    Buscador_removerSite(grafo, indice, 1);
    mostrarRankingGeral(grafo);
    
    // Validando se o índice invertido realmente limpou as referências do site deletado
    printf("Acao: Buscando por 'codigo' (palavra que pertencia ao site deletado)\n");
    testarBusca(grafo, indice, "codigo");

    // 7. REQUISITO: "salvar todos os dados em arquivo"
    printf("\n>>> TESTE 7: Persistencia de Dados\n");
    printf("Acao: Salvando estado atual em arquivos de backup (sites_backup.txt, links_backup.txt)\n");
    //Buscador_salvarDados(grafo, "sites_backup.txt", "links_backup.txt"); (A SER IMPLEMENTADA)
    printf("  -> Dados salvos com sucesso!\n");

    // 8. FINALIZAÇÃO: Limpeza rigorosa para o Valgrind
    printf("\n>>> TESTE 8: Limpeza de Memoria\n");
    Graph_free(grafo);
    Indice_free(indice);
    printf("  -> Memoria liberada. Bateria finalizada!\n");

    printf("\n====================================================\n");
    return 0;
}*/
