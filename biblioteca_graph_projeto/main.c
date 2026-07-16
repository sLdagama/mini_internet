#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "mecanismo_busca.h"
#include "gerenciador.h"
#include "Style.h"

int main(){
    int option;
    int id_origem, id_destino, id_site;
    char userAnswer[1000];
    Graph *grafo = Graph_alloc();
    IndiceInvertido *indice = Indice_alloc();
 
    //CARREGA UMA MINIATURA DA INTERNET PREVIAMENTE SALVA
    Buscador_carregarDados(grafo, indice, "sites.txt", "links.txt");

    Style_interfaceInicial();
    while(1){
        option = Style_mostraMenu();
        if(option == 0){
            //CADASTRAR SITE (?)(usuário fornece uma string)
            
        } else if(option == 1){
            //CADASTRAR PALAVRA-CHAVE (?)(usuário fornece uma string)

        } else if(option == 2){
            //CADASTRAR LINK 

            Style_inputInt(-option, &id_origem);
            Style_inputInt(option, &id_destino);
            Buscador_cadastrarLink(grafo, id_origem, id_destino);
        } else if(option == 3){
            //REMOVER SITE 

            Style_inputInt(option, &id_site);
            Buscador_removerSite(grafo, indice, id_site);
        } else if(option == 4){
            //REMOVER LINK 

            Style_inputInt(-option, &id_origem);
            Style_inputInt(option, &id_destino);
            Buscador_removerLink(grafo, id_origem, id_destino);
        } else if(option == 5){
            //PESQUISAR

            Style_inputChar(option, userAnswer);
            Clic_clearScreen();
            Buscador_printSites(grafo, indice, userAnswer);
            Clic_keyCapture();
        } else if(option == 6){
            //SAIR

            //Buscador_salvarDados(grafo, "sites_backup.txt", "links_backup.txt"); 
            break;
        } else {
            //MOSTRA O GRAFO E O ID DOS GRAFOS
            
            Clic_clearScreen();
            Style_mostraRank(grafo);
            Clic_keyCapture();
        }
    }
    Graph_free(grafo);
    Indice_free(indice);
}