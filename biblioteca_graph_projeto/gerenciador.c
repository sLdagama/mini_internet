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

    char linha[500]; // Buffer para ler a linha inteira do arquivo incluindo as palavras
    printf("Carregando sites do arquivo...\n");
    
    // Mudamos de fscanf para fgets para pegar a linha toda com as palavras-chave
    while (fgets(linha, sizeof(linha), arq_sites) != NULL) {
        int id;
        char url[150];
        char nome[100];

        // Extrai os 3 primeiros campos obrigatórios da linha lida
        if (sscanf(linha, "%d %s %s", &id, url, nome) < 3) {
            continue; // Pula linhas vazias ou mal formatadas
        }

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

        // --- CONSUMINDO OS TOKENS (ID, URL, Nome) PARA CHEGAR NAS PALAVRAS ---
        char *token = strtok(linha, " \n"); // Pega o ID
        token = strtok(NULL, " \n");        // Pega a URL
        token = strtok(NULL, " \n");        // Pega o Nome

        // Agora, todos os próximos tokens que vierem na sequência são as palavras-chave do site!
        token = strtok(NULL, " \n");
        while (token != NULL) {
            token = strtok(NULL, " \n"); // Avança para a próxima palavra da mesma linha
        }
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