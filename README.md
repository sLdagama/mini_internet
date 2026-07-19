# Mini Internet

## Sobre o projeto

O presente projeto foi desenvolvido como requisito para obtenção da nota referente a AP3 do componente curricular "Estrutura de Dados", usa o Tipo Abstrado de Dado (TAD) [Grafo](https://github.com/sLdagama/mini_internet/blob/main/biblioteca_graph_projeto/Graph.c) ativamente em sua implementação e, para a criação de efeitos visuais, foi-se usada a biblioteca [Clic](https://github.com/sLdagama/mini_internet/blob/main/biblioteca_graph_projeto/Clic.c).

> [!IMPORTANT]
> Para uma execução agradável do código, é necessário que o terminal tenha, pelo menos, uma proporção de 168 x 44.

## Funcionalidades
O usuário pode:
- Cadastrar um site;
- Cadastrar uma palavra-chave;
- Cadastrar um link;
- Remover um site;
- Remover um link;
- Realizar uma consulta com uma ou mais palavras;
- Realizar uma consulta usando operadores OR ou AND;
- Sair e salvar.

As demais funcionalidades não são obrigatórias, mas servem para demonstrar o funcionamento das demais, são elas:
- Mostrar links;
- Mostrar ranking.

## Como usar

### Gerando executável
Para gerar o arquivo executável, faça:

```
gcc *c
```

Após gerar o execuável, faça:

```
./a.out
```

### Navegação no menu
Para navegar no menu, é importante saber que:
- A tecla "⬆️" faz a caixa de seleção subir;
- A tecla "⬇️" faz a caixa de seleção descer;
- A tecla "Enter" executa a opção.

## Fluxo do programa

Para uma melhor compreensão de como funciona o código internamente, veja o [Diagrama](https://github.com/sLdagama/mini_internet/tree/main/diagrama) criado.
