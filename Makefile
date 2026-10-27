CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -Iinclude
SRC     = src/main.c src/cliente.c src/produto.c src/venda.c src/relatorio.c src/utils.c
ALVO    = sistema

all: $(ALVO)

$(ALVO): $(SRC) include/*.h
	$(CC) $(CFLAGS) $(SRC) -o $(ALVO)

clean:
	rm -f $(ALVO)

.PHONY: all clean
