#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include "Graph.h"
#include "mecanismo_busca.h"

void Buscador_carregarDados(Graph *g, IndiceInvertido *ind, const char *nome_arq_sites, const char *nome_arq_links);
void Buscador_salvarDados(Graph *g, const char *nome_arq_sites, const char *nome_arq_links);

void Buscador_cadastrarLink(Graph *g, int id_origem, int id_destino);
void Buscador_removerLink(Graph *g, int id_origem, int id_destino);
void Buscador_removerSite(Graph *g, IndiceInvertido *ind, int id_site);

#endif