#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include "Graph.h"
#include "mecanismo_busca.h"

// Funções de Carregamento e Salvamento
void Buscador_carregarDados(Graph *g, IndiceInvertido *ind, const char *nome_arq_sites, const char *nome_arq_links);
void Buscador_salvarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links);

// Funções de Cadastro (Para o Menu)
void Buscador_cadastrarSite(Graph *g, IndiceInvertido *ind, int id, const char *url, const char *nome, double peso);
void Buscador_cadastrarPalavra(Graph *g, IndiceInvertido *ind, int id_site, const char *palavra);
void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino);

// Funções de Remoção
void Buscador_removerLink(Graph *g, int id_origem, int id_destino);
void Buscador_removerSite(Graph *g, IndiceInvertido *ind, int id_site);

#endif