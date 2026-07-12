#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mecanismo_busca.h"


IndiceInvertido *Indice_alloc() {
    IndiceInvertido *ind = malloc(sizeof(IndiceInvertido));
    if (ind) {
        ind->first = NULL;
        ind->qtd_palavras = 0;
    }
    return ind;
}


void Indice_inserirPalavra(IndiceInvertido *ind, char *palavra, Vertex *v) {
    if (!ind || !palavra || !v) return;

    WordNode *atualWord = ind->first;
    WordNode *anteriorWord = NULL;
    int encontrou = 0;

    while (atualWord) {
        if (strcmp(atualWord->palavra, palavra) == 0) {
            encontrou = 1;
            break; //Achou a palavra
        }
        anteriorWord = atualWord;
        atualWord = atualWord->next;
    }

    
    if (!encontrou) {
        atualWord = malloc(sizeof(WordNode));
        if (!atualWord) return;

        strcpy(atualWord->palavra, palavra);
        atualWord->sites_first = NULL;
        atualWord->qtd_sites = 0;
        atualWord->next = NULL;

        //Inserir a palavra no início do indice invertido
        atualWord->next = ind->first;
        ind->first = atualWord;
        ind->qtd_palavras++;
    }


    // Verificar se o site já está registrado para esta palavra
    OcorrenciaNode *verificar = atualWord->sites_first;
    int ja_existe = 0;
    while (verificar) {
        if (verificar->vertex == v) {
            ja_existe = 1;
            break;
        }
        verificar = verificar->next;
    }

    if (!ja_existe) {
        OcorrenciaNode *novaOcorrencia = malloc(sizeof(OcorrenciaNode));
        if (!novaOcorrencia) return;
        novaOcorrencia->vertex = v; 
        novaOcorrencia->next = atualWord->sites_first;
        atualWord->sites_first = novaOcorrencia;
        atualWord->qtd_sites++;
    }

}

void Indice_removerReferenciasVertice(IndiceInvertido *ind, Vertex *v) {
    if (!ind || !v) return;

    WordNode *palavraAtual = ind->first;

    //Percorre cada palavra cadastrada no índice invertido
    while (palavraAtual) {
        OcorrenciaNode *ocorrenciaAtual = palavraAtual->sites_first;
        OcorrenciaNode *ocorrenciaAnterior = NULL;

        //Percorre a lista de sites associados a essa palavra específica
        while (ocorrenciaAtual) {
            if (ocorrenciaAtual->vertex == v) {
                
                if (ocorrenciaAnterior == NULL) {
                    //Era o primeiro site da lista daquela palavra
                    palavraAtual->sites_first = ocorrenciaAtual->next;
                } else {
                    ocorrenciaAnterior->next = ocorrenciaAtual->next;
                }

                //Salva o próximo antes de dar free pra não quebrar o loop
                OcorrenciaNode *proximo = ocorrenciaAtual->next;
                free(ocorrenciaAtual); 

                palavraAtual->qtd_sites--;
                ocorrenciaAtual = proximo; 
                continue; 
            }

            ocorrenciaAnterior = ocorrenciaAtual;
            ocorrenciaAtual = ocorrenciaAtual->next;
        }

        palavraAtual = palavraAtual->next;
    }
}

void Indice_free(IndiceInvertido *ind) {
    if (!ind) return;

    WordNode *p = ind->first;
    while (p) {
        //Limpa a lista de sites daquela palavra
        OcorrenciaNode *o = p->sites_first;
        while (o) {
            OcorrenciaNode *onext = o->next;
            free(o);
            o = onext;
        }

        //limpa o nó da palavra em si
        WordNode *pnext = p->next;
        free(p);
        p = pnext;
    }

    free(ind); //Limpa o índice invertido
}



void Buscador_recalcularRanking(Graph *g) {
    if (!g) return;

    Vertex *v = g->first;
    
    //Zera a importância de todos os sites do grafo
    while (v) {
        Site *site = (Site *)v->value;
        if (site) {
            site->importancia = 0.0;
        }
        v = v->next;
    }

    
    Vertex *w = g->first;
    //Percorre cada vértice 'w' pra ver as arestas que saem dele
    while (w) {
        Edge *e = w->first;
        
        // Percorre todas as arestas que saem de 'w'
        while (e) {
            Vertex *destino = e->head;
            
            if (destino) {
                Site *site_destino = (Site *)destino->value;
                if (site_destino) {
                    //Soma o valor da aresta na importância do destino.
                    site_destino->importancia += 1.0; 
                }
            }
            e = e->next;
        }
        w = w->next;
    }
}



Vertex **Buscador_buscaSimples(IndiceInvertido *ind, char *palavra, int *qtd_resultados) {
    if (!ind || !palavra || !qtd_resultados) {
        if (qtd_resultados) *qtd_resultados = 0;
        return NULL;
    }

    //Procura a palavra na lista encadeada do índice
    WordNode *atualWord = ind->first;
    while (atualWord != NULL) {
        if (strcmp(atualWord->palavra, palavra) == 0) {
            break; //achou a palavra no indice
        }
        atualWord = atualWord->next;
    }

    //Se a palavra não foi encontrada
    if (atualWord == NULL || atualWord->qtd_sites == 0) {
        *qtd_resultados = 0;
        return NULL;
    }

    //Aloca um vetor de ponteiros de Vertex para colocar os resultados
    Vertex **resultados = calloc(atualWord->qtd_sites, sizeof(Vertex *));
    if (!resultados) {
        *qtd_resultados = 0;
        return NULL;
    }

    OcorrenciaNode *atualOcorrencia = atualWord->sites_first;
    int i = 0;
    //passa os sites que possuem tal palavra para o vetor de resultados
    while (atualOcorrencia != NULL) {
        resultados[i] = atualOcorrencia->vertex;
        i++;
        atualOcorrencia = atualOcorrencia->next;
    }

    *qtd_resultados = atualWord->qtd_sites;

    return resultados;
}


Vertex **Buscador_operacaoAND(Vertex **resA, int qtdA, Vertex **resB, int qtdB, int *qtdFinal) {
    if (!qtdFinal) return NULL;
    *qtdFinal = 0;

    if (!resA || qtdA == 0 || !resB || qtdB == 0) {
        return NULL;
    }

    //O tamanho máximo da interseção é o tamanho do menor vetor
    int tamMax = (qtdA < qtdB) ? qtdA : qtdB;
    Vertex **resultado = calloc(tamMax, sizeof(Vertex *));
    if (!resultado) return NULL;

    int count = 0;

    //Compara cada elemento do vetor A com os elementos do vetor B
    for (int i = 0; i < qtdA; i++) {
        for (int j = 0; j < qtdB; j++) {
            if (resA[i] == resB[j]) {
                resultado[count] = resA[i];
                count++;
                break;
            }
        }
    }

    if (count == 0) {
        free(resultado);
        return NULL;
    }

    *qtdFinal = count;
    return resultado;
}


Vertex **Buscador_operacaoOR(Vertex **resA, int qtdA, Vertex **resB, int qtdB, int *qtdFinal) {
    if (!qtdFinal) return NULL;
    *qtdFinal = 0;

    //Casos base se um dos vetores for nulo ou vazio
    if ((!resA || qtdA == 0) && (!resB || qtdB == 0)) return NULL;

    if (!resA || qtdA == 0) {
        Vertex **resultado = calloc(qtdB, sizeof(Vertex *));
        if (!resultado) return NULL;
        memcpy(resultado, resB, qtdB * sizeof(Vertex *));
        *qtdFinal = qtdB;
        return resultado; //Retorna uma cópia de B
    }

    if (!resB || qtdB == 0) {
        Vertex **resultado = calloc(qtdA, sizeof(Vertex *));
        if (!resultado) return NULL;
        memcpy(resultado, resA, qtdA * sizeof(Vertex *));
        *qtdFinal = qtdA;
        return resultado; //Retorna uma cópia de A
    }

    //O tamanho máximo da união é a soma dos dois tamanhos
    Vertex **resultado = calloc(qtdA + qtdB, sizeof(Vertex *));
    if (!resultado) return NULL;

    int count = 0;

    //Copia todos os elementos de A para o resultado
    for (int i = 0; i < qtdA; i++) {
        resultado[count++] = resA[i];
    }

    //Copia os elementos de B, mas só se eles já não estiverem no resultado
    for (int j = 0; j < qtdB; j++) {
        int duplicado = 0;
        for (int i = 0; i < qtdA; i++) {
            if (resB[j] == resA[i]) {
                duplicado = 1;
                break;
            }
        }
        if (!duplicado) {
            resultado[count++] = resB[j];
        }
    }

    *qtdFinal = count;
    return resultado;
}

Vertex **Buscador_realizarConsultaCompleta(Graph *g, IndiceInvertido *ind, char *expressao, int *qtd_final) {
    if (!ind || !expressao || !qtd_final) {
        if (qtd_final) *qtd_final = 0;
        return NULL;
    }

    char *expr_copia = strdup(expressao);
    
    Vertex **resultado_acumulado = NULL;
    int qtd_acumulada = 0;

    //Guarda o operador lógico encontrado na última leitura 0 = inicial/nenhum, 1 = AND, 2 = OR
    int operador_pendente = 0; 

    //Quebra a string por espaços para ler palavra por palavra
    char *token = strtok(expr_copia, " \n");

    int eh_primeira_palavra = 1; // Flag para controlar o início real da busca

    while (token != NULL) {
        if (strcmp(token, "AND") == 0) {
            operador_pendente = 1;
        } 
        else if (strcmp(token, "OR") == 0) {
            operador_pendente = 2;
        } 
        else {
            int qtd_termo = 0;
            Vertex **res_termo = Buscador_buscaSimples(ind, token, &qtd_termo);

            if (eh_primeira_palavra) {
                resultado_acumulado = res_termo;
                qtd_acumulada = qtd_termo;
                eh_primeira_palavra = 0; // Próximas palavras não são a primeira
            } 
            else if (operador_pendente == 0 || operador_pendente == 1) {
                // Entra aqui tanto no AND explícito quanto no implícito (operador 0)
                Vertex **novo_acumulado = Buscador_operacaoAND(resultado_acumulado, qtd_acumulada, res_termo, qtd_termo, &qtd_acumulada);
                free(resultado_acumulado);
                free(res_termo);
                resultado_acumulado = novo_acumulado;
            } 
            else if (operador_pendente == 2) {
                Vertex **novo_acumulado = Buscador_operacaoOR(resultado_acumulado, qtd_acumulada, res_termo, qtd_termo, &qtd_acumulada);
                free(resultado_acumulado);
                free(res_termo);
                resultado_acumulado = novo_acumulado;
            }
            operador_pendente = 0;
        }

        token = strtok(NULL, " \r\n"); 
    }

    free(expr_copia);

    if (resultado_acumulado && qtd_acumulada > 0) {
        qsort(resultado_acumulado, qtd_acumulada, sizeof(Vertex *), Buscador_compararSites);
    }

    *qtd_final = qtd_acumulada;
    return resultado_acumulado;
}


int Buscador_compararSites(const void *a, const void *b) {
    Vertex *vA = *(Vertex **)a;
    Vertex *vB = *(Vertex **)b;

    Site *siteA = (Site *)vA->value;
    Site *siteB = (Site *)vB->value;

    //Ordena por importância
    if (siteB->importancia > siteA->importancia) return 1;
    if (siteB->importancia < siteA->importancia) return -1;

    //Se as importâncias forem iguais ordena por ordem alfabática
    return strcmp(siteA->nome, siteB->nome);
}