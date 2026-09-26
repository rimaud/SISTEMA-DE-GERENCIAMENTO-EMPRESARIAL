#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

void limparBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta */
    }
}

void limparTela(void)
{
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[2J");
    fflush(stdout);
#endif
}

void pausar(void)
{
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
}

void linha(char c, int tamanho)
{
    int i;
    for (i = 0; i < tamanho; i++) {
        putchar(c);
    }
    putchar('\n');
}

int lerTexto(const char *rotulo, char *destino, int tamanho)
{
    if (rotulo != NULL) {
        printf("%s", rotulo);
    }

    if (fgets(destino, tamanho, stdin) == NULL) {
        /* Entrada encerrada (Ctrl+D ou fim do arquivo redirecionado).
         * Sem isso, os lacos de validacao repetiriam a pergunta para sempre.
         * Os dados ja foram gravados a cada operacao, entao e seguro sair. */
        destino[0] = '\0';
        printf("\n\n[!] Entrada encerrada. Finalizando o sistema.\n");
        exit(0);
    }

    /* fgets guarda o '\n'; se ele nao veio, o restante ficou no buffer. */
    if (strchr(destino, '\n') == NULL) {
        limparBuffer();
    } else {
        destino[strcspn(destino, "\n")] = '\0';
    }

    removerEspacos(destino);
    return 1;
}

int lerInteiro(const char *rotulo, int minimo, int maximo)
{
    char entrada[64];
    char *fim;
    long valor;

    while (1) {
        lerTexto(rotulo, entrada, sizeof(entrada));

        if (entrada[0] == '\0') {
            printf("  [!] Valor obrigatorio. Tente novamente.\n");
            continue;
        }

        valor = strtol(entrada, &fim, 10);
        if (*fim != '\0') {
            printf("  [!] Digite apenas numeros inteiros.\n");
            continue;
        }

        if (valor < minimo || valor > maximo) {
            printf("  [!] Informe um valor entre %d e %d.\n", minimo, maximo);
            continue;
        }

        return (int) valor;
    }
}

float lerReal(const char *rotulo, float minimo, float maximo)
{
    char entrada[64];
    char *fim;
    double valor;
    int i;

    while (1) {
        lerTexto(rotulo, entrada, sizeof(entrada));

        if (entrada[0] == '\0') {
            printf("  [!] Valor obrigatorio. Tente novamente.\n");
            continue;
        }

        /* aceita virgula como separador decimal */
        for (i = 0; entrada[i] != '\0'; i++) {
            if (entrada[i] == ',') {
                entrada[i] = '.';
            }
        }

        valor = strtod(entrada, &fim);
        if (*fim != '\0') {
            printf("  [!] Digite um numero valido (ex.: 19.90).\n");
            continue;
        }

        if (valor < minimo || valor > maximo) {
            printf("  [!] Informe um valor entre %.2f e %.2f.\n", minimo, maximo);
            continue;
        }

        return (float) valor;
    }
}

int confirmar(const char *pergunta)
{
    char resposta[16];

    while (1) {
        printf("%s (S/N): ", pergunta);
        lerTexto(NULL, resposta, sizeof(resposta));

        if (resposta[0] == 'S' || resposta[0] == 's') {
            return 1;
        }
        if (resposta[0] == 'N' || resposta[0] == 'n') {
            return 0;
        }

        printf("  [!] Responda com S ou N.\n");
    }
}

void removerEspacos(char *texto)
{
    int inicio = 0;
    int fim;

    if (texto == NULL) {
        return;
    }

    while (texto[inicio] != '\0' && isspace((unsigned char) texto[inicio])) {
        inicio++;
    }

    if (inicio > 0) {
        memmove(texto, texto + inicio, strlen(texto + inicio) + 1);
    }

    fim = (int) strlen(texto) - 1;
    while (fim >= 0 && isspace((unsigned char) texto[fim])) {
        texto[fim] = '\0';
        fim--;
    }
}

void paraMaiusculas(char *texto)
{
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        texto[i] = (char) toupper((unsigned char) texto[i]);
    }
}

int compararSemCase(const char *a, const char *b)
{
    while (*a != '\0' && *b != '\0') {
        int ca = toupper((unsigned char) *a);
        int cb = toupper((unsigned char) *b);

        if (ca != cb) {
            return ca - cb;
        }
        a++;
        b++;
    }
    return toupper((unsigned char) *a) - toupper((unsigned char) *b);
}

int contemTexto(const char *texto, const char *trecho)
{
    int i, j;

    if (trecho[0] == '\0') {
        return 1;
    }

    for (i = 0; texto[i] != '\0'; i++) {
        j = 0;
        while (trecho[j] != '\0' &&
               toupper((unsigned char) texto[i + j]) == toupper((unsigned char) trecho[j])) {
            j++;
        }
        if (trecho[j] == '\0') {
            return 1;
        }
    }
    return 0;
}

void somenteDigitos(char *texto)
{
    int i = 0;
    int j = 0;

    while (texto[i] != '\0') {
        if (isdigit((unsigned char) texto[i])) {
            texto[j] = texto[i];
            j++;
        }
        i++;
    }
    texto[j] = '\0';
}

int validarNome(const char *nome)
{
    int i;
    int letras = 0;

    if (strlen(nome) < 3) {
        return 0;
    }

    for (i = 0; nome[i] != '\0'; i++) {
        unsigned char c = (unsigned char) nome[i];
        if (isalpha(c)) {
            letras++;
        } else if (!isspace(c) && c != '.' && c != '\'' && c != '-' && !isdigit(c) && c < 128) {
            return 0;
        }
    }

    return letras >= 3;
}

int validarCPF(const char *cpf)
{
    int i;
    int iguais = 1;

    if (strlen(cpf) != 11) {
        return 0;
    }

    for (i = 0; i < 11; i++) {
        if (!isdigit((unsigned char) cpf[i])) {
            return 0;
        }
        if (cpf[i] != cpf[0]) {
            iguais = 0;
        }
    }

    /* 00000000000, 11111111111 etc. nao sao CPFs validos */
    return !iguais;
}

int validarTelefone(const char *telefone)
{
    size_t tamanho = strlen(telefone);
    size_t i;

    if (tamanho < 8 || tamanho > 11) {
        return 0;
    }

    for (i = 0; i < tamanho; i++) {
        if (!isdigit((unsigned char) telefone[i])) {
            return 0;
        }
    }
    return 1;
}

int validarEmail(const char *email)
{
    const char *arroba = strchr(email, '@');
    const char *ponto;

    if (arroba == NULL || arroba == email) {
        return 0;
    }

    if (strchr(arroba + 1, '@') != NULL) {
        return 0;
    }

    ponto = strchr(arroba + 1, '.');
    if (ponto == NULL) {
        return 0;
    }

    /* precisa existir algo depois do ultimo ponto */
    return strlen(ponto + 1) >= 2 && strchr(email, ' ') == NULL;
}

void formatarCPF(const char *cpf, char *destino)
{
    if (strlen(cpf) != 11) {
        strcpy(destino, cpf);
        return;
    }

    sprintf(destino, "%c%c%c.%c%c%c.%c%c%c-%c%c",
            cpf[0], cpf[1], cpf[2],
            cpf[3], cpf[4], cpf[5],
            cpf[6], cpf[7], cpf[8],
            cpf[9], cpf[10]);
}
