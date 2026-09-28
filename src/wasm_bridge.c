#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "produto.h"

// Instâncias globais das listas mantidas em memória
static ListaClientes g_clientes;
static ListaProdutos g_produtos;

// Inicializa e carrega os dados dos arquivos .dat
void init_wasm(void) {
    g_clientes.total = 0;
    g_produtos.total = 0;
    carregarClientes(&g_clientes);
    carregarProdutos(&g_produtos);
}

// Cadastra um cliente vindo dos campos da página Web
int api_cadastrar_cliente(const char* cpf, const char* nome, const char* telefone, const char* email, const char* cidade) {
    if (g_clientes.total >= MAX_CLIENTES) return 0;
    
    Cliente *c = &g_clientes.itens[g_clientes.total];
    c->codigo = proximoCodigoCliente(&g_clientes);
    
    strncpy(c->cpf, cpf, TAM_CPF - 1);
    strncpy(c->nome, nome, TAM_NOME - 1);
    strncpy(c->telefone, telefone, TAM_TELEFONE - 1);
    strncpy(c->email, email, TAM_EMAIL - 1);
    strncpy(c->cidade, cidade, TAM_CIDADE - 1);
    
    g_clientes.total++;
    salvarClientes(&g_clientes);
    return c->codigo;
}

// Cadastra um produto vindo dos campos da página Web
int api_cadastrar_produto(const char* nome, const char* categoria, float preco, int quantidade) {
    if (g_produtos.total >= MAX_PRODUTOS) return 0;
    
    Produto *p = &g_produtos.itens[g_produtos.total];
    p->codigo = proximoCodigoProduto(&g_produtos);
    
    strncpy(p->nome, nome, TAM_NOME_PROD - 1);
    strncpy(p->categoria, categoria, TAM_CATEGORIA - 1);
    p->preco = preco;
    p->quantidade = quantidade;
    
    g_produtos.total++;
    salvarProdutos(&g_produtos);
    return p->codigo;
}

int api_get_total_clientes(void) { return g_clientes.total; }
int api_get_total_produtos(void) { return g_produtos.total; }