#ifndef CLIENTE_H
#define CLIENTE_H

#define MAX_CLIENTES      200
#define TAM_NOME           61
#define TAM_CPF            12
#define TAM_TELEFONE       12
#define TAM_EMAIL          61
#define TAM_CIDADE         41

#define ARQUIVO_CLIENTES  "dados/clientes.dat"

/* Estrutura que representa um cliente da empresa. */
typedef struct {
    int  codigo;
    char nome[TAM_NOME];
    char cpf[TAM_CPF];
    char telefone[TAM_TELEFONE];
    char email[TAM_EMAIL];
    char cidade[TAM_CIDADE];
} Cliente;

/* Colecao de clientes carregada em memoria. */
typedef struct {
    Cliente itens[MAX_CLIENTES];
    int     total;
} ListaClientes;

/* Persistencia em arquivo */
int carregarClientes(ListaClientes *lista);
int salvarClientes(const ListaClientes *lista);

/* Menu do modulo */
void menuClientes(ListaClientes *lista);

/* Operacoes (CRUD) */
void cadastrarCliente(ListaClientes *lista);
void listarClientes(const ListaClientes *lista);
void buscarCliente(const ListaClientes *lista);
void alterarCliente(ListaClientes *lista);
void excluirCliente(ListaClientes *lista);

/* Apoio */

/* Busca RECURSIVA pelo codigo. Devolve o indice ou -1 se nao encontrar.
 * Chame com posicao = 0. */
int buscarIndicePorCodigo(const ListaClientes *lista, int codigo, int posicao);

/* Busca pelo CPF (retorna indice ou -1). */
int buscarIndicePorCPF(const ListaClientes *lista, const char *cpf);

/* Sugere o proximo codigo livre (maior codigo + 1). */
int proximoCodigoCliente(const ListaClientes *lista);

/* Mostra um cliente em formato de ficha e em formato de linha de tabela. */
void exibirFichaCliente(const Cliente *cliente);
void exibirLinhaCliente(const Cliente *cliente);

/* Cabecalho da tabela de listagem. */
void exibirCabecalhoClientes(void);

#endif /* CLIENTE_H */
