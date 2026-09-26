/*
 * Sistema de Gerenciamento Empresarial
 * Algoritmos e Programacao II - UFRB / CETENS
 *
 * Etapa 1: menu principal, cadastro de clientes e cadastro de produtos.
 * Etapa 2: vendas, consultas e relatorios.
 */

#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

#include "cliente.h"
#include "produto.h"
#include "utils.h"

#define PASTA_DADOS "dados"

/* Garante que a pasta onde os arquivos .dat sao gravados exista. */
static void garantirPastaDados(void)
{
#ifdef _WIN32
    _mkdir(PASTA_DADOS);
#else
    mkdir(PASTA_DADOS, 0755);
#endif
}

static void exibirMenuPrincipal(const ListaClientes *clientes,
                                const ListaProdutos *produtos)
{
    limparTela();
    linha('=', 46);
    printf("     SISTEMA DE GERENCIAMENTO EMPRESARIAL\n");
    linha('=', 46);
    printf("1 - Gerenciar Clientes\n");
    printf("2 - Gerenciar Produtos\n");
    printf("3 - Gerenciar Vendas          (Etapa 2)\n");
    printf("4 - Consultas e Relatorios    (Etapa 2)\n");
    printf("5 - Estatisticas\n");
    printf("0 - Sair\n");
    linha('=', 46);
    printf(" Clientes: %-4d | Produtos: %-4d\n", clientes->total, produtos->total);
    linha('=', 46);
    printf("\n");
}

/* Estatisticas possiveis com os modulos ja concluidos. */
static void exibirEstatisticas(const ListaClientes *clientes,
                               const ListaProdutos *produtos)
{
    int i;
    int pecas = 0;
    int estoqueBaixo = 0;
    int indiceMaisCaro = -1;

    limparTela();
    linha('=', 46);
    printf("              ESTATISTICAS\n");
    linha('=', 46);

    for (i = 0; i < produtos->total; i++) {
        pecas += produtos->itens[i].quantidade;

        if (produtos->itens[i].quantidade <= ESTOQUE_BAIXO) {
            estoqueBaixo++;
        }

        if (indiceMaisCaro < 0 ||
            produtos->itens[i].preco > produtos->itens[indiceMaisCaro].preco) {
            indiceMaisCaro = i;
        }
    }

    printf("\n Clientes cadastrados     : %d\n", clientes->total);
    printf(" Produtos cadastrados     : %d\n", produtos->total);
    printf(" Pecas em estoque         : %d\n", pecas);
    printf(" Valor total do estoque   : R$ %.2f\n", valorTotalEstoque(produtos, 0));
    printf(" Produtos em falta (<= %d) : %d\n", ESTOQUE_BAIXO, estoqueBaixo);

    if (indiceMaisCaro >= 0) {
        printf(" Produto mais caro        : %s (R$ %.2f)\n",
               produtos->itens[indiceMaisCaro].nome,
               produtos->itens[indiceMaisCaro].preco);
    }

    printf("\n Vendas, faturamento e produto mais vendido\n");
    printf(" serao incluidos na Etapa 2 do projeto.\n");

    linha('=', 46);
    pausar();
}

static void moduloEmDesenvolvimento(const char *nome)
{
    limparTela();
    linha('=', 46);
    printf("  MODULO: %s\n", nome);
    linha('=', 46);
    printf("\n  Este modulo faz parte da Etapa 2 do trabalho\n");
    printf("  e ainda esta em desenvolvimento.\n");
    pausar();
}

int main(void)
{
    ListaClientes clientes;
    ListaProdutos produtos;
    int opcao;

    garantirPastaDados();

    carregarClientes(&clientes);
    carregarProdutos(&produtos);

    printf("Carregando dados...\n");
    printf("  %d cliente(s) e %d produto(s) recuperados dos arquivos.\n",
           clientes.total, produtos.total);
    pausar();

    do {
        exibirMenuPrincipal(&clientes, &produtos);
        opcao = lerInteiro("Escolha uma opcao: ", 0, 5);

        switch (opcao) {
            case 1:
                menuClientes(&clientes);
                break;
            case 2:
                menuProdutos(&produtos);
                break;
            case 3:
                moduloEmDesenvolvimento("GERENCIAR VENDAS");
                break;
            case 4:
                moduloEmDesenvolvimento("CONSULTAS E RELATORIOS");
                break;
            case 5:
                exibirEstatisticas(&clientes, &produtos);
                break;
            case 0:
                break;
        }

    } while (opcao != 0);

    /* garantia extra: grava o estado atual antes de encerrar */
    salvarClientes(&clientes);
    salvarProdutos(&produtos);

    limparTela();
    linha('=', 46);
    printf("  Dados salvos. Sistema encerrado. Ate logo!\n");
    linha('=', 46);

    return 0;
}
