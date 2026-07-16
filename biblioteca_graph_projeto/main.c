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
            //CADASTRAR SITE (?)
            
        } else if(option == 1){
            //CADASTRAR PALAVRA-CHAVE (?)

        } else if(option == 2){
            //CADASTRAR LINK (usuário fornece dois números inteiros)

        } else if(option == 3){
            //REMOVER SITE (usuário fornece um número inteiro)

        } else if(option == 4){
            //REMOVER LINK (remove link a partir de dois números inteiros)
            
        } else if(option == 5){
            //PESQUISAR
            Style_inputChar(option, userAnswer);
            Clic_clearScreen();
            Buscador_printSites(grafo, indice, userAnswer);
            Clic_keyCapture();
        } else if(option == 6){
            //SAIR

            // 7° REQUISITO: SALVAR TODOS OS DADOS EM UM ARQUIVO
            //Buscador_salvarDados(grafo, "sites_backup.txt", "links_backup.txt"); 

            break;
        } else {
            //MOSTRA O GRAFO E O ID DOS GRAFOS
            Clic_clearScreen();
            Graph_print(grafo);
            Clic_keyCapture();
        }
    }
    Graph_free(grafo);
    Indice_free(indice);
}