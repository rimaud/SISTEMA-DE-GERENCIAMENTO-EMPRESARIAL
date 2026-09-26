#ifndef PRODUTO_H
#define PRODUTO_H

#define MAX_PRODUTOS      300
#define TAM_NOME_PROD      51
#define TAM_CATEGORIA      31

#define ESTOQUE_BAIXO       5   /* limite para alerta de estoque */

#define ARQUIVO_PRODUTOS  "dados/produtos.dat"

/* Estrutura  que representa um produto (artigo) do estoque. */
typedef struct {
    int   codigo;
    char  nome[TAM_NOME_PROD];
    char  categoria[TAM_CATEGORIA];
    float preco;
    int   quantidade;
} Produto;

/* Colecao de produtos carregada em memoria. */
typedef struct {
    Produto itens[MAX_PRODUTOS];
    int     total;
} ListaProdutos;

/* --- Persistencia em arquivo --- */
int carregarProdutos(ListaProdutos *lista);
int salvarProdutos(const ListaProdutos *lista);

/* --- Menu do modulo --- */
void menuProdutos(ListaProdutos *lista);

/* --- Operacoes (CRUD) --- */
void cadastrarProduto(ListaProdutos *lista);
void listarProdutos(const ListaProdutos *lista);
void buscarProduto(const ListaProdutos *lista);
void alterarProduto(ListaProdutos *lista);
void excluirProduto(ListaProdutos *lista);
void consultarEstoque(const ListaProdutos *lista);
void listarEstoqueBaixo(const ListaProdutos *lista);

/* --- Apoio --- */

/* Busca RECURSIVA pelo codigo. Devolve o indice ou -1. Chame com posicao = 0. */
int buscarIndiceProduto(const ListaProdutos *lista, int codigo, int posicao);

/* Soma RECURSIVA do valor do estoque (preco x quantidade). */
float valorTotalEstoque(const ListaProdutos *lista, int posicao);

/* Sugere o proximo codigo livre. */
int proximoCodigoProduto(const ListaProdutos *lista);

void exibirCabecalhoProdutos(void);
void exibirLinhaProduto(const Produto *produto);
void exibirFichaProduto(const Produto *produto);

#endif /* PRODUTO_H */
