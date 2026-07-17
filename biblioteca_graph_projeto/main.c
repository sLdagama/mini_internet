#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "mecanismo_busca.h"
#include "gerenciador.h"
#include "Style.h"

int main(){
    int option, verify;
    int id_origem, id_destino, id_site;
    char userAnswer[1000], userAnswer2[1000];
    Graph *grafo = Graph_alloc();
    IndiceInvertido *indice = Indice_alloc();
 
    // Garante uma exibição agradável
    while(1){
        verify = Style_executionVerify();
        if(verify == 1){
            break;
        }
    }

    //CARREGA UMA MINIATURA DA MINI_INTERNET PREVIAMENTE SALVA
    Buscador_carregarDados(grafo, indice, "sites.txt", "links.txt");

    Style_interfaceInicial();
    while(1){
        option = Style_showMenu();
        if(option == 1){
            //CADASTRAR SITE

            Style_input(option * NOT_FINISHED, &id_site, Say_IDsite);
            Style_input(option * NOT_FINISHED, userAnswer, Say_URLsite);
            Style_input(option, userAnswer2, Say_NAMEsite);
            Buscador_cadastrarSite(grafo, id_site, userAnswer, userAnswer2);
        } else if(option == 2){
            //CADASTRAR PALAVRA-CHAVE

            Style_input(option * NOT_FINISHED, &id_site, Say_IDsite);
            Style_input(option, userAnswer, Say_keyWord);
            Buscador_cadastrarPalavra(grafo, indice, id_site, userAnswer);
        } else if(option == 3){
            //CADASTRAR LINK 

            Style_input(option * NOT_FINISHED, &id_origem, Say_originID);
            Style_input(option, &id_destino, Say_destinyID);
            Buscador_cadastrarLink(grafo, id_origem, id_destino);
        } else if(option == 4){
            //REMOVER SITE 

            Style_input(option, &id_site, Say_removeID);
            Buscador_removerSite(grafo, indice, id_site);
        } else if(option == 5){
            //REMOVER LINK 

            Style_input(option * NOT_FINISHED, &id_origem, Say_originID);
            Style_input(option, &id_destino, Say_destinyID);
            Buscador_removerLink(grafo, id_origem, id_destino);
        } else if(option == 6){
            //PESQUISAR

            Style_input(option, userAnswer, Say_search);
            Clic_clearScreen();
            Buscador_printSites(grafo, indice, userAnswer);
            Clic_keyCapture();
        } else if(option == 7){
            // MOSTRA OS LINKS ENTRE OS SITES

            // (a ser implementada)
        } else if(option == 8){
            // MOSTRA O ID DE CADA SITE, O SEU NOME E SUA IMPORTÂNCIA
            
            Clic_clearScreen();
            Style_showRank(grafo);
            Clic_keyCapture();
        } else {
            // SAIR

            Buscador_salvarDados(grafo, "sites.txt", "links.txt"); 
            break;
        }
    }
    Graph_free(grafo);
    Indice_free(indice);
}
