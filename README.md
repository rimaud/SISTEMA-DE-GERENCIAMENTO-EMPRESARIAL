# SISTEMA DE GERENCIAMENTO EMPRESARIAL

Trabalho prático da disciplina **Algoritmos e Programação II** — UFRB / CETENS
(Bacharelado em Sistemas de Informação). Sistema em linguagem **C** para
controle de clientes, produtos e vendas de uma pequena empresa.

Acesse [sistema-de-gerenciamento-empresaria.vercel.app](https://sistema-de-gerenciamento-empresaria.vercel.app/)

## Definição da equipe

- Ausiane de Oliveira Costa
- Houemakou Rimaud Djidonou
- Nayara Andrade de Oliveira
- Raimon Rios da Silva

Enunciado completo: `docs/Modelo_Trabalho_Sistema_Gerenciamento_Empresarial_APII_2.pdf`

## Organização do projeto

```
include/          cabeçalhos (.h) com structs e assinaturas das funções
  cliente.h       estrutura Cliente e operações do módulo
  produto.h       estrutura Produto e operações do módulo
  utils.h         leitura validada do teclado, strings e formatação
src/              implementações (.c)
  main.c          menu principal e estatísticas
  cliente.c       CRUD de clientes + gravação em arquivo
  produto.c       CRUD de produtos + estoque + gravação em arquivo
  utils.c         funções auxiliares
dados/            arquivos binários gerados em tempo de execução
  clientes.dat
  produtos.dat
```

## Conteúdos da disciplina aplicados

| Conteúdo                   | Onde aparece                                                                                                                          |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| Funções / modularização | todo o projeto está dividido em módulos;`main()` só coordena o menu                                                              |
| Estrutura                   | `Cliente` (`include/cliente.h`), `Produto` (`include/produto.h`)                                                              |
| Ponteiros                   | `ListaClientes`, `ListaProdutos` passados por referência; `Cliente cliente = &lista->itens[i]` em `alterarCliente`           |
| Arquivos                    | `carregarClientes` / `salvarClientes` e equivalentes de produto (`fopen`, `fread`, `fwrite`)                                |
| Strings e caracteres        | `utils.c`: `removerEspacos`, `paraMaiusculas`, `somenteDigitos`, `contemTexto`, `formatarCPF`, validações de CPF/e-mail |
| Recursão                   | `buscarIndicePorCodigo` (cliente.c), `buscarIndiceProduto` e `valorTotalEstoque` (produto.c)                                    |
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
