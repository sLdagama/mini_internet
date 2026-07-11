#include "gerenciador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Buscador_carregarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links) {
    // 1. ABRIR E LER O ARQUIVO DE SITES
    FILE *arq_sites = fopen(nome_arq_sites, "r");
    if (arq_sites == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo de sites (%s)\n", nome_arq_sites);
        return;
    }

    int id;
    char url[150];
    char nome[100];

    printf("Carregando sites do arquivo...\n");
    // Ler linha por linha do arquivo de sites (ID, URL, Nome)
    while (fscanf(arq_sites, "%d %s %s", &id, url, nome) != EOF) {
        // Aloca memória para a struct do Site que definimos no gerenciador.h
        Site *novo_site = (Site *)malloc(sizeof(Site));
        if (novo_site == NULL) {
            printf("[ERRO] Falha de memoria ao alocar o site %s.\n", nome);
            fclose(arq_sites);
            return;
        }

        // Copia os dados lidos para dentro da struct do site
        strcpy(novo_site->url, url);
        strcpy(novo_site->nome, nome);
        novo_site->importancia = 1.0; // Importância inicial padrão (Passo do Henrique depois)

        // Insere o site como o valor do vértice no Grafo genérico da biblioteca
        Graph_insertVertex(g, novo_site);
    }
    fclose(arq_sites);
    printf("-> %s carregado com sucesso!\n", nome_arq_sites);

    // 2. ABRIR E LER O ARQUIVO DE LINKS
    FILE *arq_links = fopen(nome_arq_links, "r");
    if (arq_links == NULL) {
        printf("[ERRO] Nao foi possivel abrir o arquivo de links (%s)\n", nome_arq_links);
        return;
    }

    int id_origem, id_destino;

    printf("Conectando links entre os sites...\n");
    // Ler linha por linha do arquivo de links (ID_Origem, ID_Destino)
    while (fscanf(arq_links, "%d %d", &id_origem, &id_destino) != EOF) {
        // Insere a aresta direcionada ligando os dois vértices pelos seus IDs
        // Como a biblioteca pede um dado para a aresta e nao precisamos, passamos NULL
        Graph_insertEdge(g, id_origem, id_destino, NULL);
    }
    fclose(arq_links);
    printf("-> %s carregado com sucesso!\n", nome_arq_links);
    printf("[SUCESSO] Toda a base de dados foi carregada no Grafo!\n");
}

void Buscador_salvarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links) {
    // Código de salvar arquivos (vamos implementar quando o projeto avançar)
}

void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino) {
    // Insere um novo link quando o usuário usar o menu
    Graph_insertEdge(g, id_origem, id_destino, NULL);
}

void Buscador_removerLink(Graph *g, int id_origem, int id_destino) {
    // Código de remover link (próximas semanas)
}

void Buscador_removerSite(Graph *g, int id_site) {
    // Código de remover site (próximas semanas)
}