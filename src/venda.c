#include <stdio.h>
#include <string.h>
#include <time.h>

#include "venda.h"
#include "utils.h"

/* Persistencia em arquivo binario */

int carregarVendas(ListaVendas *lista)
{
    FILE *arquivo;

    lista->total = 0;

    arquivo = fopen(ARQUIVO_VENDAS, "rb");
    if (arquivo == NULL) {
        return 0;
    }

    lista->total = (int) fread(lista->itens, sizeof(Venda), MAX_VENDAS, arquivo);
    fclose(arquivo);

    return lista->total;
}

int salvarVendas(const ListaVendas *lista)
{
    FILE *arquivo;
    size_t gravados;

    arquivo = fopen(ARQUIVO_VENDAS, "wb");
    if (arquivo == NULL) {
        printf("  [!] Nao foi possivel gravar em %s.\n", ARQUIVO_VENDAS);
        printf("      Verifique se a pasta 'dados' existe.\n");
        return 0;
    }

    gravados = fwrite(lista->itens, sizeof(Venda), (size_t) lista->total, arquivo);
    fclose(arquivo);

    if (gravados != (size_t) lista->total) {
        printf("  [!] Falha ao gravar todas as vendas no arquivo.\n");
        return 0;
    }

    return 1;
}

/* Buscas e calculos */

int buscarIndiceVenda(const ListaVendas *vendas, int codigo, int posicao)
{
    if (posicao >= vendas->total) {
        return -1;
    }

    if (vendas->itens[posicao].codigo == codigo) {
        return posicao;
    }

    return buscarIndiceVenda(vendas, codigo, posicao + 1);
}

/* Soma recursiva: valor da venda atual + valor das vendas restantes. */
float valorTotalVendido(const ListaVendas *vendas, int posicao)
{
    if (posicao >= vendas->total) {
        return 0.0f;
    }

    return vendas->itens[posicao].valorTotal + valorTotalVendido(vendas, posicao + 1);
}

/* Igual a anterior, mas so soma as vendas do cliente informado. */
float valorCompradoPorCliente(const ListaVendas *vendas, int codigoCliente,
                              int posicao)
{
    float valorAtual = 0.0f;

    if (posicao >= vendas->total) {
        return 0.0f;
    }

    if (vendas->itens[posicao].codigoCliente == codigoCliente) {
        valorAtual = vendas->itens[posicao].valorTotal;
    }

    return valorAtual + valorCompradoPorCliente(vendas, codigoCliente, posicao + 1);
}

int quantidadeVendidaProduto(const ListaVendas *vendas, int codigoProduto)
{
    int i;
    int j;
    int quantidade = 0;

    for (i = 0; i < vendas->total; i++) {
        for (j = 0; j < vendas->itens[i].totalItens; j++) {
            if (vendas->itens[i].itens[j].codigoProduto == codigoProduto) {
                quantidade += vendas->itens[i].itens[j].quantidade;
            }
        }
    }
    return quantidade;
}

int proximoCodigoVenda(const ListaVendas *vendas)
{
    int i;
    int maior = 0;

    for (i = 0; i < vendas->total; i++) {
        if (vendas->itens[i].codigo > maior) {
            maior = vendas->itens[i].codigo;
        }
    }
    return maior + 1;
}

/* Preenche 'destino' com a data de hoje no formato dd/mm/aaaa. */
static void dataAtual(char *destino)
{
    time_t agora = time(NULL);
    struct tm *hoje = localtime(&agora);

    strftime(destino, TAM_DATA, "%d/%m/%Y", hoje);
}

/* Exibicao */

void exibirCabecalhoVendas(void)
{
    printf("%-6s %-12s %-8s %-28s %6s %14s\n",
           "COD", "DATA", "CLIENTE", "NOME", "ITENS", "TOTAL (R$)");
    linha('-', 80);
}

void exibirLinhaVenda(const Venda *venda)
{
    printf("%-6d %-12s %-8d %-28.28s %6d %14.2f\n",
           venda->codigo,
           venda->data,
           venda->codigoCliente,
           venda->nomeCliente,
           venda->totalItens,
           venda->valorTotal);
}

static void exibirItensVenda(const Venda *venda)
{
    int i;
    const ItemVenda *item;

    printf(" %-6s %-28s %6s %12s %14s\n",
           "COD", "PRODUTO", "QTD", "UNIT. (R$)", "SUBTOTAL");

    for (i = 0; i < venda->totalItens; i++) {
        item = &venda->itens[i];
        printf(" %-6d %-28.28s %6d %12.2f %14.2f\n",
               item->codigoProduto,
               item->nomeProduto,
               item->quantidade,
               item->precoUnitario,
               item->precoUnitario * (float) item->quantidade);
    }
}

void exibirFichaVenda(const Venda *venda)
{
    linha('-', 72);
    printf(" Venda   : %d\n", venda->codigo);
    printf(" Data    : %s\n", venda->data);
    printf(" Cliente : %d - %s\n", venda->codigoCliente, venda->nomeCliente);
    linha('-', 72);
    exibirItensVenda(venda);
    linha('-', 72);
    printf(" VALOR TOTAL: R$ %.2f\n", venda->valorTotal);
    linha('-', 72);
}

/* Operacoes */

/* Quanto deste produto ja esta no carrinho da venda em andamento. */
static int quantidadeNoCarrinho(const Venda *venda, int codigoProduto)
{
    int i;

    for (i = 0; i < venda->totalItens; i++) {
        if (venda->itens[i].codigoProduto == codigoProduto) {
            return venda->itens[i].quantidade;
        }
    }
    return 0;
}

/* Le um codigo de cliente existente (0 cancela). Devolve o indice ou -1. */
static int lerClienteDaVenda(const ListaClientes *clientes)
{
    int codigo;
    int indice;

    while (1) {
        codigo = lerInteiro("Codigo do cliente (0 cancela): ", 0, 999999);

        if (codigo == 0) {
            return -1;
        }

        indice = buscarIndicePorCodigo(clientes, codigo, 0);
        if (indice < 0) {
            printf("  [!] Cliente de codigo %d nao encontrado.\n", codigo);
            continue;
        }

        return indice;
    }
}

/* Adiciona produtos ao carrinho ate o usuario digitar 0. */
static void lerItensDaVenda(Venda *venda, const ListaProdutos *produtos)
{
    const Produto *produto;
    ItemVenda *item;
    int codigo;
    int indice;
    int disponivel;
    int quantidade;
    int i;

    while (venda->totalItens < MAX_ITENS_VENDA) {
        printf("\n");
        codigo = lerInteiro("Codigo do produto (0 finaliza): ", 0, 999999);

        if (codigo == 0) {
            return;
        }

        indice = buscarIndiceProduto(produtos, codigo, 0);
        if (indice < 0) {
            printf("  [!] Produto de codigo %d nao encontrado.\n", codigo);
            continue;
        }

        produto = &produtos->itens[indice];
        disponivel = produto->quantidade - quantidadeNoCarrinho(venda, codigo);

        printf("  %s | R$ %.2f | disponivel: %d\n",
               produto->nome, produto->preco, disponivel);

        if (disponivel <= 0) {
            printf("  [!] Sem estoque disponivel para este produto.\n");
            continue;
        }

        quantidade = lerInteiro("Quantidade: ", 1, disponivel);

        /* se o produto ja esta no carrinho, so soma a quantidade */
        item = NULL;
        for (i = 0; i < venda->totalItens; i++) {
            if (venda->itens[i].codigoProduto == codigo) {
                item = &venda->itens[i];
            }
        }

        if (item == NULL) {
            item = &venda->itens[venda->totalItens];
            item->codigoProduto = produto->codigo;
            strcpy(item->nomeProduto, produto->nome);
            item->precoUnitario = produto->preco;
            item->quantidade = 0;
            venda->totalItens++;
        }

        item->quantidade += quantidade;
        venda->valorTotal += produto->preco * (float) quantidade;

        printf("  [OK] Item adicionado. Total parcial: R$ %.2f\n", venda->valorTotal);
    }

    printf("\n  [!] Limite de %d produtos diferentes por venda atingido.\n",
           MAX_ITENS_VENDA);
}

void registrarVenda(ListaVendas *vendas, const ListaClientes *clientes,
                    ListaProdutos *produtos)
{
    Venda nova;
    int indiceCliente;
    int indiceProduto;
    int i;

    limparTela();
    linha('=', 46);
    printf("             REGISTRAR VENDA\n");
    linha('=', 46);

    if (vendas->total >= MAX_VENDAS) {
        printf("\n  [!] Limite de %d vendas atingido.\n", MAX_VENDAS);
        pausar();
        return;
    }

    if (clientes->total == 0 || produtos->total == 0) {
        printf("\n  [!] Cadastre ao menos um cliente e um produto antes.\n");
        pausar();
        return;
    }

    memset(&nova, 0, sizeof(nova));
    nova.codigo = proximoCodigoVenda(vendas);
    dataAtual(nova.data);

    printf("\nVenda numero %d - %s\n\n", nova.codigo, nova.data);

    indiceCliente = lerClienteDaVenda(clientes);
    if (indiceCliente < 0) {
        printf("\n  Venda cancelada.\n");
        pausar();
        return;
    }

    nova.codigoCliente = clientes->itens[indiceCliente].codigo;
    strcpy(nova.nomeCliente, clientes->itens[indiceCliente].nome);
    printf("  Cliente: %s\n", nova.nomeCliente);

    lerItensDaVenda(&nova, produtos);

    if (nova.totalItens == 0) {
        printf("\n  Nenhum produto informado. Venda cancelada.\n");
        pausar();
        return;
    }

    printf("\nResumo da venda:\n");
    exibirFichaVenda(&nova);

    if (!confirmar("Finalizar a venda?")) {
        printf("\n  Venda cancelada. O estoque nao foi alterado.\n");
        pausar();
        return;
    }

    /* baixa no estoque de cada produto vendido */
    for (i = 0; i < nova.totalItens; i++) {
        indiceProduto = buscarIndiceProduto(produtos, nova.itens[i].codigoProduto, 0);
        produtos->itens[indiceProduto].quantidade -= nova.itens[i].quantidade;
    }

    vendas->itens[vendas->total] = nova;
    vendas->total++;

    if (salvarVendas(vendas) && salvarProdutos(produtos)) {
        printf("\n  [OK] Venda %d registrada. Estoque atualizado.\n", nova.codigo);
    }

    pausar();
}

void listarVendas(const ListaVendas *vendas)
{
    int i;

    limparTela();
    linha('=', 80);
    printf("                              VENDAS REALIZADAS\n");
    linha('=', 80);

    if (vendas->total == 0) {
        printf("\n  Nenhuma venda registrada ate o momento.\n");
        pausar();
        return;
    }

    printf("\n");
    exibirCabecalhoVendas();

    for (i = 0; i < vendas->total; i++) {
        exibirLinhaVenda(&vendas->itens[i]);
    }

    linha('-', 80);
    printf("Total: %d venda(s) | Valor vendido: R$ %.2f\n",
           vendas->total, valorTotalVendido(vendas, 0));

    pausar();
}

void buscarVenda(const ListaVendas *vendas)
{
    int opcao;
    int indice;
    int codigo;
    int i;
    int encontradas = 0;

    limparTela();
    linha('=', 46);
    printf("              BUSCAR VENDA\n");
    linha('=', 46);

    if (vendas->total == 0) {
        printf("\n  Nenhuma venda registrada ate o momento.\n");
        pausar();
        return;
    }

    printf("\n1 - Por codigo da venda (detalhada)\n");
    printf("2 - Por codigo do cliente\n");
    printf("0 - Voltar\n\n");

    opcao = lerInteiro("Opcao: ", 0, 2);

    switch (opcao) {
        case 1:
            codigo = lerInteiro("\nCodigo da venda: ", 1, 999999);
            indice = buscarIndiceVenda(vendas, codigo, 0);

            if (indice < 0) {
                printf("\n  [!] Venda de codigo %d nao encontrada.\n", codigo);
            } else {
                printf("\n");
                exibirFichaVenda(&vendas->itens[indice]);
            }
            break;

        case 2:
            codigo = lerInteiro("\nCodigo do cliente: ", 1, 999999);
            printf("\n");
            exibirCabecalhoVendas();

            for (i = 0; i < vendas->total; i++) {
                if (vendas->itens[i].codigoCliente == codigo) {
                    exibirLinhaVenda(&vendas->itens[i]);
                    encontradas++;
                }
            }

            linha('-', 80);
            printf("%d venda(s) encontrada(s) | Total comprado: R$ %.2f\n",
                   encontradas, valorCompradoPorCliente(vendas, codigo, 0));
            break;

        default:
            return;
    }

    pausar();
}

/* Menu do modulo */

void menuVendas(ListaVendas *vendas, const ListaClientes *clientes,
                ListaProdutos *produtos)
{
    int opcao;

    do {
        limparTela();
        linha('=', 46);
        printf("           GERENCIAR VENDAS\n");
        linha('=', 46);
        printf("1 - Registrar venda\n");
        printf("2 - Listar vendas\n");
        printf("3 - Buscar venda\n");
        printf("0 - Voltar ao menu principal\n");
        linha('=', 46);
        printf("Vendas registradas: %d\n\n", vendas->total);

        opcao = lerInteiro("Escolha uma opcao: ", 0, 3);

        switch (opcao) {
            case 1: registrarVenda(vendas, clientes, produtos); break;
            case 2: listarVendas(vendas);                       break;
            case 3: buscarVenda(vendas);                        break;
            case 0: break;
        }

    } while (opcao != 0);
}
