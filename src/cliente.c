#include <stdio.h>
#include <string.h>

#include "cliente.h"
#include "utils.h"

/* Persistencia em arquivo binario */

int carregarClientes(ListaClientes *lista)
{
    FILE *arquivo;

    lista->total = 0;

    arquivo = fopen(ARQUIVO_CLIENTES, "rb");
    if (arquivo == NULL) {
        /* primeira execucao: ainda nao existe arquivo gravado */
        return 0;
    }

    lista->total = (int) fread(lista->itens, sizeof(Cliente), MAX_CLIENTES, arquivo);
    fclose(arquivo);

    return lista->total;
}

int salvarClientes(const ListaClientes *lista)
{
    FILE *arquivo;
    size_t gravados;

    arquivo = fopen(ARQUIVO_CLIENTES, "wb");
    if (arquivo == NULL) {
        printf("  [!] Nao foi possivel gravar em %s.\n", ARQUIVO_CLIENTES);
        printf("      Verifique se a pasta 'dados' existe.\n");
        return 0;
    }

    gravados = fwrite(lista->itens, sizeof(Cliente), (size_t) lista->total, arquivo);
    fclose(arquivo);

    if (gravados != (size_t) lista->total) {
        printf("  [!] Falha ao gravar todos os clientes no arquivo.\n");
        return 0;
    }

    return 1;
}

/* Buscas */

int buscarIndicePorCodigo(const ListaClientes *lista, int codigo, int posicao)
{
    if (posicao >= lista->total) {
        return -1;                       /* caso base: acabou a lista */
    }

    if (lista->itens[posicao].codigo == codigo) {
        return posicao;                  /* caso base: encontrou */
    }

    return buscarIndicePorCodigo(lista, codigo, posicao + 1);
}

int buscarIndicePorCPF(const ListaClientes *lista, const char *cpf)
{
    int i;

    for (i = 0; i < lista->total; i++) {
        if (strcmp(lista->itens[i].cpf, cpf) == 0) {
            return i;
        }
    }
    return -1;
}

int proximoCodigoCliente(const ListaClientes *lista)
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

/* Exibicao*/

void exibirCabecalhoClientes(void)
{
    printf("%-6s %-28s %-16s %-14s %-18s\n",
           "COD", "NOME", "CPF", "TELEFONE", "CIDADE");
    linha('-', 86);
}

void exibirLinhaCliente(const Cliente *cliente)
{
    char cpfFormatado[20];

    formatarCPF(cliente->cpf, cpfFormatado);

    printf("%-6d %-28.28s %-16s %-14s %-18.18s\n",
           cliente->codigo,
           cliente->nome,
           cpfFormatado,
           cliente->telefone,
           cliente->cidade);
}

void exibirFichaCliente(const Cliente *cliente)
{
    char cpfFormatado[20];

    formatarCPF(cliente->cpf, cpfFormatado);

    linha('-', 46);
    printf(" Codigo   : %d\n", cliente->codigo);
    printf(" Nome     : %s\n", cliente->nome);
    printf(" CPF      : %s\n", cpfFormatado);
    printf(" Telefone : %s\n", cliente->telefone);
    printf(" E-mail   : %s\n", cliente->email);
    printf(" Cidade   : %s\n", cliente->cidade);
    linha('-', 46);
}

/* Leitura dos campos com validacao */

static void lerNomeCliente(char *destino)
{
    char entrada[TAM_NOME];

    while (1) {
        lerTexto("Nome     : ", entrada, sizeof(entrada));

        if (!validarNome(entrada)) {
            printf("  [!] Nome invalido (minimo de 3 letras, sem simbolos).\n");
            continue;
        }

        strcpy(destino, entrada);
        return;
    }
}

static void lerCPFCliente(const ListaClientes *lista, char *destino, int indiceAtual)
{
    char entrada[32];
    int encontrado;

    while (1) {
        lerTexto("CPF      : ", entrada, sizeof(entrada));
        somenteDigitos(entrada);

        if (!validarCPF(entrada)) {
            printf("  [!] CPF invalido. Informe os 11 digitos.\n");
            continue;
        }

        encontrado = buscarIndicePorCPF(lista, entrada);
        if (encontrado >= 0 && encontrado != indiceAtual) {
            printf("  [!] Este CPF ja pertence ao cliente %d - %s.\n",
                   lista->itens[encontrado].codigo,
                   lista->itens[encontrado].nome);
            continue;
        }

        strcpy(destino, entrada);
        return;
    }
}

static void lerTelefoneCliente(char *destino)
{
    char entrada[32];

    while (1) {
        lerTexto("Telefone : ", entrada, sizeof(entrada));
        somenteDigitos(entrada);

        if (!validarTelefone(entrada)) {
            printf("  [!] Telefone invalido. Use DDD + numero (ex.: 75988887777).\n");
            continue;
        }

        strcpy(destino, entrada);
        return;
    }
}

static void lerEmailCliente(char *destino)
{
    char entrada[TAM_EMAIL];

    while (1) {
        lerTexto("E-mail   : ", entrada, sizeof(entrada));

        if (!validarEmail(entrada)) {
            printf("  [!] E-mail invalido (formato esperado: nome@dominio.com).\n");
            continue;
        }

        strcpy(destino, entrada);
        return;
    }
}

static void lerCidadeCliente(char *destino)
{
    char entrada[TAM_CIDADE];

    while (1) {
        lerTexto("Cidade   : ", entrada, sizeof(entrada));

        if (strlen(entrada) < 2) {
            printf("  [!] Informe o nome da cidade.\n");
            continue;
        }

        strcpy(destino, entrada);
        return;
    }
}

/* Operacoes do CRUD */

void cadastrarCliente(ListaClientes *lista)
{
    Cliente novo;
    int codigo;
    int sugestao;

    limparTela();
    linha('=', 46);
    printf("            CADASTRAR CLIENTE\n");
    linha('=', 46);

    if (lista->total >= MAX_CLIENTES) {
        printf("\n  [!] Limite de %d clientes atingido.\n", MAX_CLIENTES);
        pausar();
        return;
    }

    sugestao = proximoCodigoCliente(lista);
    printf("\n(Codigo sugerido: %d)\n\n", sugestao);

    while (1) {
        codigo = lerInteiro("Codigo   : ", 1, 999999);

        if (buscarIndicePorCodigo(lista, codigo, 0) >= 0) {
            printf("  [!] Ja existe um cliente com o codigo %d.\n", codigo);
            continue;
        }
        break;
    }

    novo.codigo = codigo;
    lerNomeCliente(novo.nome);
    lerCPFCliente(lista, novo.cpf, -1);
    lerTelefoneCliente(novo.telefone);
    lerEmailCliente(novo.email);
    lerCidadeCliente(novo.cidade);

    printf("\nConfira os dados informados:\n");
    exibirFichaCliente(&novo);

    if (!confirmar("Confirmar o cadastro?")) {
        printf("\n  Cadastro cancelado.\n");
        pausar();
        return;
    }

    lista->itens[lista->total] = novo;
    lista->total++;

    if (salvarClientes(lista)) {
        printf("\n  [OK] Cliente cadastrado com sucesso!\n");
    }

    pausar();
}

void listarClientes(const ListaClientes *lista)
{
    int i;

    limparTela();
    linha('=', 86);
    printf("                          CLIENTES CADASTRADOS\n");
    linha('=', 86);

    if (lista->total == 0) {
        printf("\n  Nenhum cliente cadastrado ate o momento.\n");
        pausar();
        return;
    }

    printf("\n");
    exibirCabecalhoClientes();

    for (i = 0; i < lista->total; i++) {
        exibirLinhaCliente(&lista->itens[i]);
    }

    linha('-', 86);
    printf("Total: %d cliente(s).\n", lista->total);

    pausar();
}

void buscarCliente(const ListaClientes *lista)
{
    int opcao;
    int indice;
    int codigo;
    int i;
    int encontrados = 0;
    char termo[TAM_NOME];

    limparTela();
    linha('=', 46);
    printf("             BUSCAR CLIENTE\n");
    linha('=', 46);

    if (lista->total == 0) {
        printf("\n  Nenhum cliente cadastrado ate o momento.\n");
        pausar();
        return;
    }

    printf("\n1 - Por codigo\n");
    printf("2 - Por nome\n");
    printf("3 - Por CPF\n");
    printf("0 - Voltar\n\n");

    opcao = lerInteiro("Opcao: ", 0, 3);

    switch (opcao) {
        case 1:
            codigo = lerInteiro("\nCodigo: ", 1, 999999);
            indice = buscarIndicePorCodigo(lista, codigo, 0);

            if (indice < 0) {
                printf("\n  [!] Cliente de codigo %d nao encontrado.\n", codigo);
            } else {
                printf("\n");
                exibirFichaCliente(&lista->itens[indice]);
            }
            break;

        case 2:
            lerTexto("\nNome (ou parte dele): ", termo, sizeof(termo));
            printf("\n");
            exibirCabecalhoClientes();

            for (i = 0; i < lista->total; i++) {
                if (contemTexto(lista->itens[i].nome, termo)) {
                    exibirLinhaCliente(&lista->itens[i]);
                    encontrados++;
                }
            }

            linha('-', 86);
            printf("%d cliente(s) encontrado(s).\n", encontrados);
            break;

        case 3:
            lerTexto("\nCPF: ", termo, sizeof(termo));
            somenteDigitos(termo);
            indice = buscarIndicePorCPF(lista, termo);

            if (indice < 0) {
                printf("\n  [!] Nenhum cliente com esse CPF.\n");
            } else {
                printf("\n");
                exibirFichaCliente(&lista->itens[indice]);
            }
            break;

        default:
            return;
    }

    pausar();
}

void alterarCliente(ListaClientes *lista)
{
    Cliente *cliente;   /* ponteiro para o registro dentro do vetor */
    int codigo;
    int indice;
    int opcao;

    limparTela();
    linha('=', 46);
    printf("             ALTERAR CLIENTE\n");
    linha('=', 46);

    if (lista->total == 0) {
        printf("\n  Nenhum cliente cadastrado ate o momento.\n");
        pausar();
        return;
    }

    codigo = lerInteiro("\nCodigo do cliente: ", 1, 999999);
    indice = buscarIndicePorCodigo(lista, codigo, 0);

    if (indice < 0) {
        printf("\n  [!] Cliente de codigo %d nao encontrado.\n", codigo);
        pausar();
        return;
    }

    cliente = &lista->itens[indice];

    do {
        limparTela();
        printf("Dados atuais do cliente:\n");
        exibirFichaCliente(cliente);

        printf("\nO que deseja alterar?\n");
        printf("1 - Nome\n");
        printf("2 - CPF\n");
        printf("3 - Telefone\n");
        printf("4 - E-mail\n");
        printf("5 - Cidade\n");
        printf("0 - Concluir\n\n");

        opcao = lerInteiro("Opcao: ", 0, 5);

        switch (opcao) {
            case 1: lerNomeCliente(cliente->nome); break;
            case 2: lerCPFCliente(lista, cliente->cpf, indice); break;
            case 3: lerTelefoneCliente(cliente->telefone); break;
            case 4: lerEmailCliente(cliente->email); break;
            case 5: lerCidadeCliente(cliente->cidade); break;
            default: break;
        }

        if (opcao != 0) {
            if (salvarClientes(lista)) {
                printf("\n  [OK] Dado atualizado.\n");
            }
            pausar();
        }

    } while (opcao != 0);
}

void excluirCliente(ListaClientes *lista)
{
    int codigo;
    int indice;
    int i;

    limparTela();
    linha('=', 46);
    printf("             EXCLUIR CLIENTE\n");
    linha('=', 46);

    if (lista->total == 0) {
        printf("\n  Nenhum cliente cadastrado ate o momento.\n");
        pausar();
        return;
    }

    codigo = lerInteiro("\nCodigo do cliente: ", 1, 999999);
    indice = buscarIndicePorCodigo(lista, codigo, 0);

    if (indice < 0) {
        printf("\n  [!] Cliente de codigo %d nao encontrado.\n", codigo);
        pausar();
        return;
    }

    printf("\nCliente selecionado:\n");
    exibirFichaCliente(&lista->itens[indice]);

    if (!confirmar("Confirmar a EXCLUSAO deste cliente?")) {
        printf("\n  Exclusao cancelada.\n");
        pausar();
        return;
    }

    /* desloca os registros seguintes uma posicao para tras */
    for (i = indice; i < lista->total - 1; i++) {
        lista->itens[i] = lista->itens[i + 1];
    }
    lista->total--;

    if (salvarClientes(lista)) {
        printf("\n  [OK] Cliente excluido com sucesso.\n");
    }

    pausar();
}

/* Menu do modulo */

void menuClientes(ListaClientes *lista)
{
    int opcao;

    do {
        limparTela();
        linha('=', 46);
        printf("          GERENCIAR CLIENTES\n");
        linha('=', 46);
        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("3 - Buscar cliente\n");
        printf("4 - Alterar cliente\n");
        printf("5 - Excluir cliente\n");
        printf("0 - Voltar ao menu principal\n");
        linha('=', 46);
        printf("Clientes cadastrados: %d\n\n", lista->total);

        opcao = lerInteiro("Escolha uma opcao: ", 0, 5);

        switch (opcao) {
            case 1: cadastrarCliente(lista); break;
            case 2: listarClientes(lista);   break;
            case 3: buscarCliente(lista);    break;
            case 4: alterarCliente(lista);   break;
            case 5: excluirCliente(lista);   break;
            case 0: break;
        }

    } while (opcao != 0);
}
