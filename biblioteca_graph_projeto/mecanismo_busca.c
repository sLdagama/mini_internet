#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mecanismo_busca.h"
#include <ctype.h>

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

int Buscador_gerarProximoID(Graph *g) {
    if (!g || !g->first) {
        return 0; // Se o grafo estiver completamente vazio, o primeiro ID é 1
    }

    int maior_id = 0;
    Vertex *v = g->first;

    // Varre o grafo inteiro procurando o maior número de ID (label) atual
    while (v) {
        if (v->label > maior_id) {
            maior_id = v->label;
        }
        v = v->next;
    }

    return maior_id + 1; // O próximo ID será o maior encontrado + 1
}

void Buscador_recalcularRanking(Graph *g) {
    if (!g || g->n == 0) return;

    int tam_vetor = g->index + 1; 
    double *novas_importancias = (double *)calloc(tam_vetor, sizeof(double));
    if (!novas_importancias) return;

    // 1. Percorre todos os vértices do grafo
    Vertex *w = g->first;
    while (w) {
        Site *site_origem = (Site *)w->value;
        
        if (site_origem) {
            // O valor transmitido é o peso fixo de indicação do site de origem!
            int peso_a_transmitir = site_origem->peso;

            Edge *e = w->first;
            while (e) {
                Vertex *destino = e->head;
                if (destino) {
                    // O destino recebe o peso_indicacao da origem
                    novas_importancias[destino->label] += peso_a_transmitir;
                }
                e = e->next;
            }
        }
        w = w->next;
    }

    // 2. Atualiza a importância acumulada de cada site
    Vertex *v = g->first;
    while (v) {
        Site *site = (Site *)v->value;
        if (site) {
            site->importancia = novas_importancias[v->label];
        }
        v = v->next;
    }

    free(novas_importancias);
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
    string_para_minusculo(expr_copia);

    Vertex **resultado_acumulado = NULL;
    int qtd_acumulada = 0;

    //Guarda o operador lógico encontrado na última leitura 0 = inicial/nenhum, 1 = AND, 2 = OR
    int operador_pendente = 0; 

    //Quebra a string por espaços para ler palavra por palavra
    char *token = strtok(expr_copia, " \t\r\n\n");

    int eh_primeira_palavra = 1; // Flag para controlar o início real da busca

    while (token != NULL) {
        if (strcmp(token, "and") == 0) {
            operador_pendente = 1;
        } 
        else if (strcmp(token, "or") == 0) {
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

        token = strtok(NULL, " \t\r\n"); 
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

// Função auxiliar para converter string para minúsculo de forma segura
void string_para_minusculo(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}