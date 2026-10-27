#ifndef VENDA_H
#define VENDA_H

#include "cliente.h"
#include "produto.h"

#define MAX_VENDAS        200
#define MAX_ITENS_VENDA    10
#define TAM_DATA           11   /* dd/mm/aaaa */

#define ARQUIVO_VENDAS    "dados/vendas.dat"

/* Um produto dentro de uma venda. O nome e o preco sao copiados no momento
 * da venda para que o historico nao mude se o produto for alterado depois. */
typedef struct {
    int   codigoProduto;
    char  nomeProduto[TAM_NOME_PROD];
    int   quantidade;
    float precoUnitario;
} ItemVenda;

/* Estrutura que representa uma venda (pode ter varios produtos). */
typedef struct {
    int       codigo;
    int       codigoCliente;
    char      nomeCliente[TAM_NOME];
    char      data[TAM_DATA];
    ItemVenda itens[MAX_ITENS_VENDA];
    int       totalItens;
    float     valorTotal;
} Venda;

/* Colecao de vendas carregada em memoria. */
typedef struct {
    Venda itens[MAX_VENDAS];
    int   total;
} ListaVendas;

/* --- Persistencia em arquivo --- */
int carregarVendas(ListaVendas *lista);
int salvarVendas(const ListaVendas *lista);

/* --- Menu do modulo --- */
void menuVendas(ListaVendas *vendas, const ListaClientes *clientes,
                ListaProdutos *produtos);

/* --- Operacoes --- */
void registrarVenda(ListaVendas *vendas, const ListaClientes *clientes,
                    ListaProdutos *produtos);
void listarVendas(const ListaVendas *vendas);
void buscarVenda(const ListaVendas *vendas);

/* --- Apoio --- */

/* Busca RECURSIVA pelo codigo. Devolve o indice ou -1. Chame com posicao = 0. */
int buscarIndiceVenda(const ListaVendas *vendas, int codigo, int posicao);

/* Soma RECURSIVA do valor de todas as vendas. Chame com posicao = 0. */
float valorTotalVendido(const ListaVendas *vendas, int posicao);

/* Soma RECURSIVA do valor comprado por um cliente. Chame com posicao = 0. */
float valorCompradoPorCliente(const ListaVendas *vendas, int codigoCliente,
                              int posicao);

/* Quantidade total vendida de um produto em todas as vendas. */
int quantidadeVendidaProduto(const ListaVendas *vendas, int codigoProduto);

int proximoCodigoVenda(const ListaVendas *vendas);

void exibirCabecalhoVendas(void);
void exibirLinhaVenda(const Venda *venda);
void exibirFichaVenda(const Venda *venda);

#endif /* VENDA_H */
