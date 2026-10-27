#include <stdio.h>
#include <string.h>

#include "relatorio.h"
#include "utils.h"

#define MAX_ITENS_VENDIDOS (MAX_VENDAS * MAX_ITENS_VENDA)

/* Linha do ranking de produtos. */
typedef struct {
    int   codigo;
    char  nome[TAM_NOME_PROD];
    int   quantidade;
    float valor;
} ResumoProduto;

/* Linha do ranking de clientes. */
typedef struct {
    int   codigo;
    char  nome[TAM_NOME];
    int   compras;
    float valor;
} ResumoCliente;

/* Agrupa os itens de todas as vendas por produto.
 * Devolve quantos produtos diferentes foram vendidos. */
static int agruparProdutosVendidos(const ListaVendas *vendas, ResumoProduto *resumo)
{
    const ItemVenda *item;
    int total = 0;
    int i;
    int j;
    int k;

    for (i = 0; i < vendas->total; i++) {
        for (j = 0; j < vendas->itens[i].totalItens; j++) {
            item = &vendas->itens[i].itens[j];

            for (k = 0; k < total; k++) {
                if (resumo[k].codigo == item->codigoProduto) {
                    break;
                }
            }

            if (k == total) {
                resumo[k].codigo = item->codigoProduto;
                strcpy(resumo[k].nome, item->nomeProduto);
                resumo[k].quantidade = 0;
                resumo[k].valor = 0.0f;
                total++;
            }

            resumo[k].quantidade += item->quantidade;
            resumo[k].valor += item->precoUnitario * (float) item->quantidade;
        }
    }
    return total;
}

/* Agrupa as vendas por cliente. Devolve quantos clientes compraram. */
static int agruparClientes(const ListaVendas *vendas, ResumoCliente *resumo)
{
    const Venda *venda;
    int total = 0;
    int i;
    int k;

    for (i = 0; i < vendas->total; i++) {
        venda = &vendas->itens[i];

        for (k = 0; k < total; k++) {
            if (resumo[k].codigo == venda->codigoCliente) {
                break;
            }
        }

        if (k == total) {
            resumo[k].codigo = venda->codigoCliente;
            strcpy(resumo[k].nome, venda->nomeCliente);
            resumo[k].compras = 0;
            resumo[k].valor = valorCompradoPorCliente(vendas, venda->codigoCliente, 0);
            total++;
        }

        resumo[k].compras++;
    }
    return total;
}

/* Ordenacao por selecao (decrescente). */
static void ordenarProdutos(ResumoProduto *resumo, int total)
{
    ResumoProduto troca;
    int i;
    int j;
    int maior;

    for (i = 0; i < total - 1; i++) {
        maior = i;
        for (j = i + 1; j < total; j++) {
            if (resumo[j].quantidade > resumo[maior].quantidade) {
                maior = j;
            }
        }
        troca = resumo[i];
        resumo[i] = resumo[maior];
        resumo[maior] = troca;
    }
}

static void ordenarClientes(ResumoCliente *resumo, int total)
{
    ResumoCliente troca;
    int i;
    int j;
    int maior;

    for (i = 0; i < total - 1; i++) {
        maior = i;
        for (j = i + 1; j < total; j++) {
            if (resumo[j].valor > resumo[maior].valor) {
                maior = j;
            }
        }
        troca = resumo[i];
        resumo[i] = resumo[maior];
        resumo[maior] = troca;
    }
}

void relatorioGeral(const ListaClientes *clientes, const ListaProdutos *produtos,
                    const ListaVendas *vendas)
{
    static ResumoProduto rankingProdutos[MAX_ITENS_VENDIDOS];
    static ResumoCliente rankingClientes[MAX_VENDAS];
    int totalProdutos;
    int totalClientes;
    int estoqueBaixo = 0;
    int i;

    for (i = 0; i < produtos->total; i++) {
        if (produtos->itens[i].quantidade <= ESTOQUE_BAIXO) {
            estoqueBaixo++;
        }
    }

    totalProdutos = agruparProdutosVendidos(vendas, rankingProdutos);
    ordenarProdutos(rankingProdutos, totalProdutos);

    totalClientes = agruparClientes(vendas, rankingClientes);
    ordenarClientes(rankingClientes, totalClientes);

    limparTela();
    linha('=', 60);
    printf("                    RELATORIO GERAL\n");
    linha('=', 60);

    printf("\n Clientes cadastrados       : %d\n", clientes->total);
    printf(" Produtos cadastrados       : %d\n", produtos->total);
    printf(" Valor estimado do estoque  : R$ %.2f\n", valorTotalEstoque(produtos, 0));
    printf(" Vendas realizadas          : %d\n", vendas->total);
    printf(" Valor total vendido        : R$ %.2f\n", valorTotalVendido(vendas, 0));

    if (totalProdutos > 0) {
        printf(" Produto mais vendido       : %d - %s (%d un.)\n",
               rankingProdutos[0].codigo, rankingProdutos[0].nome,
               rankingProdutos[0].quantidade);
    } else {
        printf(" Produto mais vendido       : (nenhuma venda)\n");
    }

    if (totalClientes > 0) {
        printf(" Cliente que mais comprou   : %d - %s (R$ %.2f)\n",
               rankingClientes[0].codigo, rankingClientes[0].nome,
               rankingClientes[0].valor);
    } else {
        printf(" Cliente que mais comprou   : (nenhuma venda)\n");
    }

    printf(" Produtos com estoque baixo : %d (limite: %d un.)\n",
           estoqueBaixo, ESTOQUE_BAIXO);

    linha('=', 60);
    pausar();
}

void relatorioProdutosMaisVendidos(const ListaVendas *vendas)
{
    static ResumoProduto ranking[MAX_ITENS_VENDIDOS];
    int total;
    int i;

    limparTela();
    linha('=', 72);
    printf("                     PRODUTOS MAIS VENDIDOS\n");
    linha('=', 72);

    total = agruparProdutosVendidos(vendas, ranking);

    if (total == 0) {
        printf("\n  Nenhuma venda registrada ate o momento.\n");
        pausar();
        return;
    }

    ordenarProdutos(ranking, total);

    printf("\n%-4s %-6s %-32s %10s %16s\n", "#", "COD", "PRODUTO", "QTD", "FATURADO (R$)");
    linha('-', 72);

    for (i = 0; i < total; i++) {
        printf("%-4d %-6d %-32.32s %10d %16.2f\n",
               i + 1, ranking[i].codigo, ranking[i].nome,
               ranking[i].quantidade, ranking[i].valor);
    }

    linha('-', 72);
    pausar();
}

void relatorioClientesPorCompras(const ListaVendas *vendas)
{
    static ResumoCliente ranking[MAX_VENDAS];
    int total;
    int i;

    limparTela();
    linha('=', 72);
    printf("                 CLIENTES POR VOLUME DE COMPRAS\n");
    linha('=', 72);

    total = agruparClientes(vendas, ranking);

    if (total == 0) {
        printf("\n  Nenhuma venda registrada ate o momento.\n");
        pausar();
        return;
    }

    ordenarClientes(ranking, total);

    printf("\n%-4s %-6s %-32s %8s %16s\n", "#", "COD", "CLIENTE", "COMPRAS", "TOTAL (R$)");
    linha('-', 72);

    for (i = 0; i < total; i++) {
        printf("%-4d %-6d %-32.32s %8d %16.2f\n",
               i + 1, ranking[i].codigo, ranking[i].nome,
               ranking[i].compras, ranking[i].valor);
    }

    linha('-', 72);
    pausar();
}

/* Menu do modulo */

void menuRelatorios(const ListaClientes *clientes, const ListaProdutos *produtos,
                    const ListaVendas *vendas)
{
    int opcao;

    do {
        limparTela();
        linha('=', 46);
        printf("        CONSULTAS E RELATORIOS\n");
        linha('=', 46);
        printf("1 - Relatorio geral\n");
        printf("2 - Produtos mais vendidos\n");
        printf("3 - Clientes por volume de compras\n");
        printf("4 - Produtos com estoque baixo\n");
        printf("5 - Consultar estoque\n");
        printf("0 - Voltar ao menu principal\n");
        linha('=', 46);
        printf("\n");

        opcao = lerInteiro("Escolha uma opcao: ", 0, 5);

        switch (opcao) {
            case 1: relatorioGeral(clientes, produtos, vendas); break;
            case 2: relatorioProdutosMaisVendidos(vendas);      break;
            case 3: relatorioClientesPorCompras(vendas);        break;
            case 4: listarEstoqueBaixo(produtos);               break;
            case 5: consultarEstoque(produtos);                 break;
            case 0: break;
        }

    } while (opcao != 0);
}
