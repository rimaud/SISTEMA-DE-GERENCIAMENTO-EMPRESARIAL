#ifndef RELATORIO_H
#define RELATORIO_H

#include "cliente.h"
#include "produto.h"
#include "venda.h"

/* Menu de consultas e relatorios. */
void menuRelatorios(const ListaClientes *clientes, const ListaProdutos *produtos,
                    const ListaVendas *vendas);

/* Resumo com todos os indicadores pedidos no enunciado. */
void relatorioGeral(const ListaClientes *clientes, const ListaProdutos *produtos,
                    const ListaVendas *vendas);

/* Ranking dos produtos pela quantidade vendida. */
void relatorioProdutosMaisVendidos(const ListaVendas *vendas);

/* Ranking dos clientes pelo valor total comprado. */
void relatorioClientesPorCompras(const ListaVendas *vendas);

#endif /* RELATORIO_H */
