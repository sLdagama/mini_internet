#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "mecanismo_busca.h"
#include "gerenciador.h"
#include "Style.h"



int main(){
    int option;
    char userAnswer[1000];
    Graph *grafo = Graph_alloc();
    IndiceInvertido *indice = Indice_alloc();

    //1° REQUISITO: "carregar uma miniatura da internet previamente salva"
    Buscador_carregarDados(grafo, indice, "sites.txt", "links.txt");

    Style_interfaceInicial();
    while(1){
        option = Style_mostraMenu();
        if(option == 0){
            //CADASTRAR SITE
            
            Style_input(option, userAnswer);
        } else if(option == 1){
            //CADASTRAR LINK

            Style_input(option, userAnswer);
        } else if(option == 2){
            //REMOVER SITE

            Style_input(option, userAnswer);
        } else if(option == 3){
            //PESQUISAR

            Style_input(option, userAnswer);
            Buscador_printSites(grafo, indice, userAnswer); 
        } else if(option == 4){
            //SAIR

            // 7° REQUISITO: SALVAR TODOS OS DADOS EM UM ARQUIVO
            //Buscador_salvarDados(grafo, "sites_backup.txt", "links_backup.txt"); 

            break;
        } else {
            //(TEMPORÁRIO) MOSTRA GRAFO

        }
    }
    Graph_free(grafo);
    Indice_free(indice);
}