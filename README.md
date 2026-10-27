# SISTEMA DE GERENCIAMENTO EMPRESARIAL

Trabalho prático da disciplina **Algoritmos e Programação II** — UFRB / CETENS
(Bacharelado em Sistemas de Informação). Sistema em linguagem **C** para
controle de clientes, produtos e vendas de uma pequena empresa.

Acesse [sistema-de-gerenciamento-empresaria.vercel.app](https://sistema-de-gerenciamento-empresaria.vercel.app/)

## Definição da equipe

- Houemakou Rimaud Djidonou
- Nayara Andrade de Oliveira
- Raimon Rios da Silva

Enunciado completo: `docs/Modelo_Trabalho_Sistema_Gerenciamento_Empresarial_APII_2.pdf`

## Organização do projeto

```
include/          cabeçalhos (.h) com structs e assinaturas das funções
  cliente.h       estrutura Cliente e operações do módulo
  produto.h       estrutura Produto e operações do módulo
  venda.h         estruturas Venda / ItemVenda e operações do módulo
  relatorio.h     consultas e relatórios
  utils.h         leitura validada do teclado, strings e formatação
src/              implementações (.c)
  main.c          menu principal
  cliente.c       CRUD de clientes + gravação em arquivo
  produto.c       CRUD de produtos + estoque + gravação em arquivo
  venda.c         registro de vendas com vários produtos + baixa no estoque
  relatorio.c     relatório geral, rankings de produtos e clientes
  utils.c         funções auxiliares
dados/            arquivos binários gerados em tempo de execução
  clientes.dat
  produtos.dat
  vendas.dat
Makefile          compilação do sistema
```

## Como compilar e executar

```
make
./sistema
```

Sem `make`:

```
gcc -Wall -Wextra -std=c99 -Iinclude src/main.c src/cliente.c src/produto.c src/venda.c src/relatorio.c src/utils.c -o sistema
```

A pasta `dados/` é criada automaticamente na primeira execução.

## Módulos

- **Clientes**: cadastrar, listar, buscar (código, nome ou CPF), alterar e excluir.
- **Produtos**: cadastrar, listar, buscar, alterar, excluir, consultar estoque e
  listar produtos com estoque baixo (≤ 5 unidades).
- **Vendas**: registrar venda para cliente cadastrado com até 10 produtos
  diferentes, calcular o valor total, dar baixa no estoque e impedir venda acima
  do estoque disponível; listar vendas e buscar por código da venda ou do cliente.
- **Consultas e relatórios**: relatório geral (total de clientes, total de
  produtos, valor do estoque, quantidade de vendas, valor vendido, produto mais
  vendido, cliente com maior volume de compras, produtos com estoque baixo),
  ranking de produtos mais vendidos, ranking de clientes por compras, estoque
  baixo e consulta de estoque.

## Conteúdos da disciplina aplicados

| Conteúdo                   | Onde aparece                                                                                                                          |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| Funções / modularização | todo o projeto está dividido em módulos;`main()` só coordena o menu                                                              |
| Estrutura                   | `Cliente` (`include/cliente.h`), `Produto` (`include/produto.h`), `Venda` e `ItemVenda` (`include/venda.h`)                    |
| Ponteiros                   | `ListaClientes`, `ListaProdutos` passados por referência; `Cliente cliente = &lista->itens[i]` em `alterarCliente`           |
| Arquivos                    | `carregarClientes` / `salvarClientes` e equivalentes de produto e venda (`fopen`, `fread`, `fwrite`)                        |
| Strings e caracteres        | `utils.c`: `removerEspacos`, `paraMaiusculas`, `somenteDigitos`, `contemTexto`, `formatarCPF`, validações de CPF/e-mail |
| Recursão                   | `buscarIndicePorCodigo` (cliente.c), `buscarIndiceProduto` e `valorTotalEstoque` (produto.c), `buscarIndiceVenda`, `valorTotalVendido` e `valorCompradoPorCliente` (venda.c) |
| Validações                | códigos duplicados, CPF duplicado/inválido, e-mail, telefone, preço > 0, quantidade > 0, entradas não numéricas                  |

## Validações implementadas

- Código de cliente e de produto não pode se repetir.
- CPF precisa ter 11 dígitos e não pode estar cadastrado para outro cliente.
- E-mail precisa ter `@` e domínio com ponto.
- Telefone aceita de 8 a 11 dígitos (somente números).
- Preço mínimo de R$ 0,01 (não aceita valor negativo ou zero).
- Quantidade mínima de 1 no cadastro.
- Texto digitado onde se espera número é recusado sem travar o programa.
- Toda exclusão pede confirmação.
- Venda só aceita cliente e produtos cadastrados.
- Quantidade vendida não pode passar do estoque disponível (considerando o que
  já está no carrinho da mesma venda).
- O estoque só é alterado depois que a venda é confirmada.

## Plano de desenvolvimento (preencher com a equipe)

| Etapa                       | Responsável |
| --------------------------- | ------------ |
| Modelagem das estruturas    |              |
| Módulo de clientes         |              |
| Módulo de produtos         |              |
| Módulo de vendas           |              |
| Arquivos                    |              |
| Relatórios e estatísticas |              |
| Recursão                   |              |
| Testes e correções        |              |
| Relatório e apresentação |              |
