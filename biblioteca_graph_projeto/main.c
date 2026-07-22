#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "mecanismo_busca.h"
#include "gerenciador.h"
#include "Style.h"

int main(){
    int option, verify;
    int id_origem, id_destino, id_site, NEXTid;
    double peso;
    char userAnswer[1000], userAnswer2[1000];
    Graph *grafo = Graph_alloc();
    IndiceInvertido *indice = Indice_alloc();
    
    // Garante uma exibição agradável
    while(1){
        verify = Style_executionVerify();
        if(verify == OKAY){
            break;
        }
    }
    
    Style_startProgram();
    
    //CARREGA UMA MINIATURA DA MINI_INTERNET PREVIAMENTE SALVA
    Buscador_carregarDados(grafo, indice, "sites.txt", "links.txt");
    

    Style_interfaceInicial();
    while(1){
        option = Style_showMenu();
        NEXTid = Buscador_gerarProximoID(grafo);

        if(option == 1){
            //CADASTRAR SITE

            Style_input(userAnswer, Say_URLsite);
            Style_input(userAnswer2, Say_NAMEsite);
            Style_input(&peso, Say_PESOsite);

            Style_underInput(WAIT);
            Style_underInput(SUCCESS);

            Buscador_cadastrarSite(grafo, indice, NEXTid, userAnswer, userAnswer2, peso);
        } else if(option == 2){
            //CADASTRAR PALAVRA-CHAVE

            Style_input(&id_site, Say_IDsite);

            if(Style_idVerify(NEXTid, id_site) == NOT_OKAY){
                Style_underInput(ERROR);
            } else {
                Style_input(userAnswer, Say_keyWord);

                Style_underInput(WAIT);
                Style_underInput(SUCCESS);
                
                Buscador_cadastrarPalavra(grafo, indice, id_site, userAnswer);
            }
        } else if(option == 3){
            //CADASTRAR LINK 

            Style_input(&id_origem, Say_originID);

            if(Style_idVerify(NEXTid, id_origem) == NOT_OKAY){
                Style_underInput(ERROR);
            } else {
                Style_input(&id_destino, Say_destinyID);

                if(Style_idVerify(NEXTid, id_destino) == NOT_OKAY){
                    Style_underInput(ERROR);
                } else {
                    Style_underInput(WAIT);
                    Style_underInput(SUCCESS);
                    
                    Buscador_cadastrarLink(grafo, id_origem, id_destino);
                }
            }
        } else if(option == 4){
            //REMOVER SITE 

            Style_input(&id_site, Say_removeID);

            if(Style_idVerify(NEXTid, id_site) == NOT_OKAY){
                Style_underInput(ERROR);
            } else {
                Style_underInput(WAIT);
                Style_underInput(SUCCESS);
                
                Buscador_removerSite(grafo, indice, id_site);
            }
        } else if(option == 5){
            //REMOVER LINK 

            Style_input(&id_origem, Say_originID);

            if(Style_idVerify(NEXTid, id_origem) == NOT_OKAY){
                Style_underInput(ERROR);
            } else {
                Style_input(&id_destino, Say_destinyID);

                if(Style_idVerify(NEXTid, id_destino) == NOT_OKAY){
                    Style_underInput(ERROR);
                } else {
                    Style_underInput(WAIT);
                    Style_underInput(SUCCESS);
                    
                    Buscador_removerLink(grafo, id_origem, id_destino);
                }
            }
        } else if(option == 6){
            //PESQUISAR

            Style_input(userAnswer, Say_search);

            Style_underInput(WAIT);
            Style_underInput(OTHER_SCREEN);

            Style_searchResult(grafo, indice, userAnswer);
        } else if(option == 7){
            // MOSTRA OS LINKS ENTRE OS SITES

            Style_showTable(grafo);
            Style_showLinks(grafo);
        } else {
            // SAIR

            Buscador_salvarDados(grafo, "sites.txt", "links.txt");
            Clic_showCursor();
            break;
        }
    }
    Graph_free(grafo);
    Indice_free(indice);
}
