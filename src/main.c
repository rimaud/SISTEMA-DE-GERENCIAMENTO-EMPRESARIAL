/*
 * Sistema de Gerenciamento Empresarial
 * Algoritmos e Programacao II - UFRB / CETENS
 *
 * Modulos: clientes, produtos, vendas e consultas/relatorios.
 * Os dados ficam gravados em arquivos binarios na pasta "dados".
 */

#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

#include "cliente.h"
#include "produto.h"
#include "venda.h"
#include "relatorio.h"
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
                                const ListaProdutos *produtos,
                                const ListaVendas *vendas)
{
    limparTela();
    linha('=', 46);
    printf("     SISTEMA DE GERENCIAMENTO EMPRESARIAL\n");
    linha('=', 46);
    printf("1 - Gerenciar Clientes\n");
    printf("2 - Gerenciar Produtos\n");
    printf("3 - Gerenciar Vendas\n");
    printf("4 - Consultas e Relatorios\n");
    printf("0 - Sair\n");
    linha('=', 46);
    printf(" Clientes: %-4d | Produtos: %-4d | Vendas: %-4d\n",
           clientes->total, produtos->total, vendas->total);
    linha('=', 46);
    printf("\n");
}

int main(void)
{
    /* static: as listas sao grandes e ficam fora da pilha */
    static ListaClientes clientes;
    static ListaProdutos produtos;
    static ListaVendas vendas;
    int opcao;

    garantirPastaDados();

    carregarClientes(&clientes);
    carregarProdutos(&produtos);
    carregarVendas(&vendas);

    printf("Carregando dados...\n");
    printf("  %d cliente(s), %d produto(s) e %d venda(s) recuperados dos arquivos.\n",
           clientes.total, produtos.total, vendas.total);
    pausar();

    do {
        exibirMenuPrincipal(&clientes, &produtos, &vendas);
        opcao = lerInteiro("Escolha uma opcao: ", 0, 4);

        switch (opcao) {
            case 1:
                menuClientes(&clientes);
                break;
            case 2:
                menuProdutos(&produtos);
                break;
            case 3:
                menuVendas(&vendas, &clientes, &produtos);
                break;
            case 4:
                menuRelatorios(&clientes, &produtos, &vendas);
                break;
            case 0:
                break;
        }

    } while (opcao != 0);

    /* garantia extra: grava o estado atual antes de encerrar */
    salvarClientes(&clientes);
    salvarProdutos(&produtos);
    salvarVendas(&vendas);

    limparTela();
    linha('=', 46);
    printf("  Dados salvos. Sistema encerrado. Ate logo!\n");
    linha('=', 46);

    return 0;
}
