#include <stdio.h>
#include <string.h>

#include "produto.h"
#include "utils.h"

/* Persistencia em arquivo binario*/

int carregarProdutos(ListaProdutos *lista)
{
    FILE *arquivo;

    lista->total = 0;

    arquivo = fopen(ARQUIVO_PRODUTOS, "rb");
    if (arquivo == NULL) {
        return 0;
    }

    lista->total = (int) fread(lista->itens, sizeof(Produto), MAX_PRODUTOS, arquivo);
    fclose(arquivo);

    return lista->total;
}

int salvarProdutos(const ListaProdutos *lista)
{
    FILE *arquivo;
    size_t gravados;

    arquivo = fopen(ARQUIVO_PRODUTOS, "wb");
    if (arquivo == NULL) {
        printf("  [!] Nao foi possivel gravar em %s.\n", ARQUIVO_PRODUTOS);
        printf("      Verifique se a pasta 'dados' existe.\n");
        return 0;
    }

    gravados = fwrite(lista->itens, sizeof(Produto), (size_t) lista->total, arquivo);
    fclose(arquivo);

    if (gravados != (size_t) lista->total) {
        printf("  [!] Falha ao gravar todos os produtos no arquivo.\n");
        return 0;
    }

    return 1;
}

/* Buscas e calculos */

int buscarIndiceProduto(const ListaProdutos *lista, int codigo, int posicao)
{
    if (posicao >= lista->total) {
        return -1;
    }

    if (lista->itens[posicao].codigo == codigo) {
        return posicao;
    }

    return buscarIndiceProduto(lista, codigo, posicao + 1);
}

/* Soma recursiva: valor do item atual + valor do restante da lista. */
float valorTotalEstoque(const ListaProdutos *lista, int posicao)
{
    float valorItem;

    if (posicao >= lista->total) {
        return 0.0f;
    }

    valorItem = lista->itens[posicao].preco * (float) lista->itens[posicao].quantidade;

    return valorItem + valorTotalEstoque(lista, posicao + 1);
}

int proximoCodigoProduto(const ListaProdutos *lista)
{
    int i;
    int maior = 0;

    for (i = 0; i < lista->total; i++) {
        if (lista->itens[i].codigo > maior) {
            maior = lista->itens[i].codigo;
        }
    }
    return maior + 1;
}

/* Exibicao */

void exibirCabecalhoProdutos(void)
{
    printf("%-6s %-30s %-18s %12s %8s %14s\n",
           "COD", "PRODUTO", "CATEGORIA", "PRECO (R$)", "QTD", "SUBTOTAL");
    linha('-', 92);
}

void exibirLinhaProduto(const Produto *produto)
{
    printf("%-6d %-30.30s %-18.18s %12.2f %8d %14.2f%s\n",
           produto->codigo,
           produto->nome,
           produto->categoria,
           produto->preco,
           produto->quantidade,
           produto->preco * (float) produto->quantidade,
           produto->quantidade <= ESTOQUE_BAIXO ? "  <- estoque baixo" : "");
}

void exibirFichaProduto(const Produto *produto)
{
    linha('-', 46);
    printf(" Codigo     : %d\n", produto->codigo);
    printf(" Produto    : %s\n", produto->nome);
    printf(" Categoria  : %s\n", produto->categoria);
    printf(" Preco      : R$ %.2f\n", produto->preco);
    printf(" Quantidade : %d\n", produto->quantidade);
    printf(" Subtotal   : R$ %.2f\n", produto->preco * (float) produto->quantidade);

    if (produto->quantidade <= ESTOQUE_BAIXO) {
        printf(" Situacao   : ESTOQUE BAIXO (limite: %d)\n", ESTOQUE_BAIXO);
    } else {
        printf(" Situacao   : estoque normal\n");
    }
    linha('-', 46);
}

/* Leitura dos campos com validacao */

static void lerNomeProduto(char *destino)
{
    char entrada[TAM_NOME_PROD];

    while (1) {
        lerTexto("Produto    : ", entrada, sizeof(entrada));

        if (strlen(entrada) < 2) {
            printf("  [!] Informe um nome com pelo menos 2 caracteres.\n");
            continue;
        }

        strcpy(destino, entrada);
        return;
    }
}

static void lerCategoriaProduto(char *destino)
{
    char entrada[TAM_CATEGORIA];

    while (1) {
        lerTexto("Categoria  : ", entrada, sizeof(entrada));

        if (strlen(entrada) < 2) {
            printf("  [!] Informe a categoria do produto.\n");
            continue;
        }

        paraMaiusculas(entrada);
        strcpy(destino, entrada);
        return;
    }
}

/* Operacoes do CRUD */

void cadastrarProduto(ListaProdutos *lista)
{
    Produto novo;
    int codigo;

    limparTela();
    linha('=', 46);
    printf("            CADASTRAR PRODUTO\n");
    linha('=', 46);

    if (lista->total >= MAX_PRODUTOS) {
        printf("\n  [!] Limite de %d produtos atingido.\n", MAX_PRODUTOS);
        pausar();
        return;
    }

    printf("\n(Codigo sugerido: %d)\n\n", proximoCodigoProduto(lista));

    while (1) {
        codigo = lerInteiro("Codigo     : ", 1, 999999);

        if (buscarIndiceProduto(lista, codigo, 0) >= 0) {
            printf("  [!] Ja existe um produto com o codigo %d.\n", codigo);
            continue;
        }
        break;
    }

    novo.codigo = codigo;
    lerNomeProduto(novo.nome);
    lerCategoriaProduto(novo.categoria);
    novo.preco = lerReal("Preco (R$) : ", 0.01f, 1000000.0f);
    novo.quantidade = lerInteiro("Quantidade : ", 1, 999999);

    printf("\nConfira os dados informados:\n");
    exibirFichaProduto(&novo);

    if (!confirmar("Confirmar o cadastro?")) {
        printf("\n  Cadastro cancelado.\n");
        pausar();
        return;
    }

    lista->itens[lista->total] = novo;
    lista->total++;

    if (salvarProdutos(lista)) {
        printf("\n  [OK] Produto cadastrado com sucesso!\n");
    }

    pausar();
}

void listarProdutos(const ListaProdutos *lista)
{
    int i;

    limparTela();
    linha('=', 92);
    printf("                              PRODUTOS CADASTRADOS\n");
    linha('=', 92);

    if (lista->total == 0) {
        printf("\n  Nenhum produto cadastrado ate o momento.\n");
        pausar();
        return;
    }

    printf("\n");
    exibirCabecalhoProdutos();

    for (i = 0; i < lista->total; i++) {
        exibirLinhaProduto(&lista->itens[i]);
    }

    linha('-', 92);
    printf("Total: %d produto(s) | Valor do estoque: R$ %.2f\n",
           lista->total, valorTotalEstoque(lista, 0));

    pausar();
}

void buscarProduto(const ListaProdutos *lista)
{
    int opcao;
    int indice;
    int codigo;
    int i;
    int encontrados = 0;
    char termo[TAM_NOME_PROD];

    limparTela();
    linha('=', 46);
    printf("             BUSCAR PRODUTO\n");
    linha('=', 46);

    if (lista->total == 0) {
        printf("\n  Nenhum produto cadastrado ate o momento.\n");
        pausar();
        return;
    }

    printf("\n1 - Por codigo\n");
    printf("2 - Por nome\n");
    printf("3 - Por categoria\n");
    printf("0 - Voltar\n\n");

    opcao = lerInteiro("Opcao: ", 0, 3);

    switch (opcao) {
        case 1:
            codigo = lerInteiro("\nCodigo: ", 1, 999999);
            indice = buscarIndiceProduto(lista, codigo, 0);

            if (indice < 0) {
                printf("\n  [!] Produto de codigo %d nao encontrado.\n", codigo);
            } else {
                printf("\n");
                exibirFichaProduto(&lista->itens[indice]);
            }
            break;

        case 2:
        case 3:
            if (opcao == 2) {
                lerTexto("\nNome (ou parte dele): ", termo, sizeof(termo));
            } else {
                lerTexto("\nCategoria: ", termo, sizeof(termo));
            }

            printf("\n");
            exibirCabecalhoProdutos();

            for (i = 0; i < lista->total; i++) {
                const char *campo = (opcao == 2) ? lista->itens[i].nome
                                                 : lista->itens[i].categoria;
                if (contemTexto(campo, termo)) {
                    exibirLinhaProduto(&lista->itens[i]);
                    encontrados++;
                }
            }

            linha('-', 92);
            printf("%d produto(s) encontrado(s).\n", encontrados);
            break;

        default:
            return;
    }

    pausar();
}

void alterarProduto(ListaProdutos *lista)
{
    Produto *produto;   /* ponteiro para o registro dentro do vetor */
    int codigo;
    int indice;
    int opcao;

    limparTela();
    linha('=', 46);
    printf("             ALTERAR PRODUTO\n");
    linha('=', 46);

    if (lista->total == 0) {
        printf("\n  Nenhum produto cadastrado ate o momento.\n");
        pausar();
        return;
    }

    codigo = lerInteiro("\nCodigo do produto: ", 1, 999999);
    indice = buscarIndiceProduto(lista, codigo, 0);

    if (indice < 0) {
        printf("\n  [!] Produto de codigo %d nao encontrado.\n", codigo);
        pausar();
        return;
    }

    produto = &lista->itens[indice];

    do {
        limparTela();
        printf("Dados atuais do produto:\n");
        exibirFichaProduto(produto);

        printf("\nO que deseja alterar?\n");
        printf("1 - Nome\n");
        printf("2 - Categoria\n");
        printf("3 - Preco\n");
        printf("4 - Quantidade em estoque\n");
        printf("0 - Concluir\n\n");

        opcao = lerInteiro("Opcao: ", 0, 4);

        switch (opcao) {
            case 1: lerNomeProduto(produto->nome); break;
            case 2: lerCategoriaProduto(produto->categoria); break;
            case 3: produto->preco = lerReal("Preco (R$) : ", 0.01f, 1000000.0f); break;
            case 4: produto->quantidade = lerInteiro("Quantidade : ", 0, 999999); break;
            default: break;
        }

        if (opcao != 0) {
            if (salvarProdutos(lista)) {
                printf("\n  [OK] Dado atualizado.\n");
            }
            pausar();
        }

    } while (opcao != 0);
}

void excluirProduto(ListaProdutos *lista)
{
    int codigo;
    int indice;
    int i;

    limparTela();
    linha('=', 46);
    printf("             EXCLUIR PRODUTO\n");
    linha('=', 46);

    if (lista->total == 0) {
        printf("\n  Nenhum produto cadastrado ate o momento.\n");
        pausar();
        return;
    }

    codigo = lerInteiro("\nCodigo do produto: ", 1, 999999);
    indice = buscarIndiceProduto(lista, codigo, 0);

    if (indice < 0) {
        printf("\n  [!] Produto de codigo %d nao encontrado.\n", codigo);
        pausar();
        return;
    }

    printf("\nProduto selecionado:\n");
    exibirFichaProduto(&lista->itens[indice]);

    if (!confirmar("Confirmar a EXCLUSAO deste produto?")) {
        printf("\n  Exclusao cancelada.\n");
        pausar();
        return;
    }

    for (i = indice; i < lista->total - 1; i++) {
        lista->itens[i] = lista->itens[i + 1];
    }
    lista->total--;

    if (salvarProdutos(lista)) {
        printf("\n  [OK] Produto excluido com sucesso.\n");
    }

    pausar();
}

void consultarEstoque(const ListaProdutos *lista)
{
    int i;
    int totalPecas = 0;

    limparTela();
    linha('=', 92);
    printf("                              CONSULTA DE ESTOQUE\n");
    linha('=', 92);

    if (lista->total == 0) {
        printf("\n  Nenhum produto cadastrado ate o momento.\n");
        pausar();
        return;
    }

    printf("\n");
    exibirCabecalhoProdutos();

    for (i = 0; i < lista->total; i++) {
        exibirLinhaProduto(&lista->itens[i]);
        totalPecas += lista->itens[i].quantidade;
    }

    linha('-', 92);
    printf("Itens diferentes : %d\n", lista->total);
    printf("Pecas em estoque : %d\n", totalPecas);
    printf("Valor do estoque : R$ %.2f\n", valorTotalEstoque(lista, 0));

    pausar();
}

void listarEstoqueBaixo(const ListaProdutos *lista)
{
    int i;
    int encontrados = 0;

    limparTela();
    linha('=', 92);
    printf("                    PRODUTOS COM ESTOQUE BAIXO (<= %d)\n", ESTOQUE_BAIXO);
    linha('=', 92);

    if (lista->total == 0) {
        printf("\n  Nenhum produto cadastrado ate o momento.\n");
        pausar();
        return;
    }

    printf("\n");
    exibirCabecalhoProdutos();

    for (i = 0; i < lista->total; i++) {
        if (lista->itens[i].quantidade <= ESTOQUE_BAIXO) {
            exibirLinhaProduto(&lista->itens[i]);
            encontrados++;
        }
    }

    linha('-', 92);
    if (encontrados == 0) {
        printf("Nenhum produto com estoque baixo. Tudo certo!\n");
    } else {
        printf("%d produto(s) precisam de reposicao.\n", encontrados);
    }

    pausar();
}

/*  Menu do modulo */

void menuProdutos(ListaProdutos *lista)
{
    int opcao;

    do {
        limparTela();
        linha('=', 46);
        printf("          GERENCIAR PRODUTOS\n");
        linha('=', 46);
        printf("1 - Cadastrar produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Buscar produto\n");
        printf("4 - Alterar produto\n");
        printf("5 - Excluir produto\n");
        printf("6 - Consultar estoque\n");
        printf("7 - Produtos com estoque baixo\n");
        printf("0 - Voltar ao menu principal\n");
        linha('=', 46);
        printf("Produtos cadastrados: %d\n\n", lista->total);

        opcao = lerInteiro("Escolha uma opcao: ", 0, 7);

        switch (opcao) {
            case 1: cadastrarProduto(lista);   break;
            case 2: listarProdutos(lista);     break;
            case 3: buscarProduto(lista);      break;
            case 4: alterarProduto(lista);     break;
            case 5: excluirProduto(lista);     break;
            case 6: consultarEstoque(lista);   break;
            case 7: listarEstoqueBaixo(lista); break;
            case 0: break;
        }

    } while (opcao != 0);
}
