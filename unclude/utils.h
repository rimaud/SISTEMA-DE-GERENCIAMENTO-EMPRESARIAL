#ifndef UTILS_H
#define UTILS_H

/*
 * utils.h - Funcoes auxiliares de entrada, validacao e formatacao.
 * Centraliza a leitura segura de dados do teclado para que os modulos
 * de clientes e produtos nao precisem tratar buffer sujo do scanf.
 */

/* Limpa o que sobrou no buffer do teclado (evita loop infinito no scanf). */
void limparBuffer(void);

/* Limpa a tela do terminal. */
void limparTela(void);

/* Aguarda o usuario pressionar ENTER. */
void pausar(void);

/* Desenha uma linha com o caractere informado. */
void linha(char c, int tamanho);

/* Le um texto do teclado removendo o '\n' final.
 * Retorna 1 se leu algo, 0 se a string ficou vazia.
 * Se a entrada for encerrada (Ctrl+D / fim de arquivo), o programa e
 * finalizado para nao entrar em laco infinito de leitura. */
int lerTexto(const char *rotulo, char *destino, int tamanho);

/* Le um inteiro validado dentro do intervalo [minimo, maximo]. */
int lerInteiro(const char *rotulo, int minimo, int maximo);

/* Le um numero real validado dentro do intervalo [minimo, maximo]. */
float lerReal(const char *rotulo, float minimo, float maximo);

/* Pergunta algo ao usuario e devolve 1 para 'S' e 0 para 'N'. */
int confirmar(const char *pergunta);

/* Remove espacos em branco do inicio e do fim da string (altera no lugar). */
void removerEspacos(char *texto);

/* Converte a string para letras maiusculas (altera no lugar). */
void paraMaiusculas(char *texto);

/* Compara duas strings ignorando maiusculas/minusculas.
 * Retorna 0 quando sao iguais (mesma logica de strcmp). */
int compararSemCase(const char *a, const char *b);

/* Verifica se 'trecho' aparece dentro de 'texto' ignorando o case. */
int contemTexto(const char *texto, const char *trecho);

/* Mantem apenas os digitos da string (usado no CPF e no telefone). */
void somenteDigitos(char *texto);

/* Validacoes de formato. Retornam 1 quando o valor e aceitavel. */
int validarNome(const char *nome);
int validarCPF(const char *cpf);
int validarTelefone(const char *telefone);
int validarEmail(const char *email);

/* Formata um CPF de 11 digitos como 000.000.000-00. */
void formatarCPF(const char *cpf, char *destino);

#endif /* UTILS_H */
