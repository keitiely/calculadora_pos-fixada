#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

#define MAX 512
#define M_PI 3.14159265358979323846

// ----- DEFINIÇÃO DAS STRUCTS -----
typedef struct
{
    char dados[MAX];
    int topo;
} OPERADORES;

typedef struct
{
    float dados[MAX];
    int topo;
} NUMEROS;

// ----- FUNÇÕES DE INICIALIZAÇÃO -----
void inicializarOperadores(OPERADORES *p) { p->topo = -1; }
void inicializarNumeros(NUMEROS *n) { n->topo = -1; }

// ----- FUNÇÕES DE MANIPULAÇÃO DA PILHA DE OPERADORES -----
char topoOperador(OPERADORES *p)
{
    if (p->topo != -1)
        return p->dados[p->topo];
    return '\0';
}
void empilharOperador(OPERADORES *p, char c)
{
    if (p->topo < MAX - 1)
        p->dados[++(p->topo)] = c;
}
char desempilharOperador(OPERADORES *p)
{
    if (p->topo >= 0)
        return p->dados[(p->topo)--];
    return '\0';
}

// ----- FUNÇÕES DE MANIPULAÇÃO DA PILHA DE NÚMEROS -----
void empilharNumero(NUMEROS *p, float num)
{
    if (p->topo < MAX - 1)
        p->dados[++(p->topo)] = num;
}
float desempilharNumero(NUMEROS *p)
{
    if (p->topo >= 0)
        return p->dados[(p->topo)--];
    return 0.0;
}

// ----- FUNÇÕES AUXILIARES -----
int funcoes(const char *str)
{
    return (strncmp(str, "sen", 3) == 0 || strncmp(str, "cos", 3) == 0 ||
            strncmp(str, "tg", 2) == 0 || strncmp(str, "log", 3) == 0 ||
            strncmp(str, "raiz", 4) == 0);
}

int precedencia(char op)
{
    switch (op)
    {
    case 's':
    case 'c':
    case 't':
    case 'l':
    case 'r':
        return 4;
    case '^':
        return 3;
    case '*':
    case '/':
    case '%':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}

// ----- FUNÇÕES PRINCIPAIS -----

char *getFormaPosFixa(char *Str)
{
    static char posFixa[MAX * 2];
    OPERADORES pilhaOperadores;
    inicializarOperadores(&pilhaOperadores);
    int i = 0, j = 0;
    posFixa[0] = '\0';

    while (Str[i] != '\0')
    {
        if (isspace(Str[i]))
        {
            i++;
            continue;
        }

        if (isdigit(Str[i]) || (Str[i] == '.' && isdigit(Str[i + 1])))
        {
            while (isdigit(Str[i]) || Str[i] == '.')
            {
                posFixa[j++] = Str[i++];
            }
            posFixa[j++] = ' ';
            continue;
        }

        if (funcoes(&Str[i]))
        {
            char funcToken;
            if (strncmp(&Str[i], "raiz", 4) == 0)
            {
                funcToken = 'r';
                i += 4;
            }
            else if (strncmp(&Str[i], "sen", 3) == 0)
            {
                funcToken = 's';
                i += 3;
            }
            else if (strncmp(&Str[i], "cos", 3) == 0)
            {
                funcToken = 'c';
                i += 3;
            }
            else if (strncmp(&Str[i], "tg", 2) == 0)
            {
                funcToken = 't';
                i += 2;
            }
            else if (strncmp(&Str[i], "log", 3) == 0)
            {
                funcToken = 'l';
                i += 3;
            }
            empilharOperador(&pilhaOperadores, funcToken);
            continue;
        }

        if (Str[i] == '(')
        {
            empilharOperador(&pilhaOperadores, Str[i]);
            i++;
            continue;
        }

        if (Str[i] == ')')
        {
            while (topoOperador(&pilhaOperadores) != '\0' && topoOperador(&pilhaOperadores) != '(')
            {
                char op = desempilharOperador(&pilhaOperadores);
                posFixa[j++] = op;
                posFixa[j++] = ' ';
            }
            desempilharOperador(&pilhaOperadores); // Pop '('
            if (strchr("sctlr", topoOperador(&pilhaOperadores)))
            {
                char op = desempilharOperador(&pilhaOperadores);
                if (op == 's')
                    j += sprintf(&posFixa[j], "sen ");
                else if (op == 'c')
                    j += sprintf(&posFixa[j], "cos ");
                else if (op == 't')
                    j += sprintf(&posFixa[j], "tg ");
                else if (op == 'l')
                    j += sprintf(&posFixa[j], "log ");
                else if (op == 'r')
                    j += sprintf(&posFixa[j], "raiz ");
            }
            i++;
            continue;
        }

        // Se for um operador binário
        while (topoOperador(&pilhaOperadores) != '\0' && topoOperador(&pilhaOperadores) != '(' && precedencia(topoOperador(&pilhaOperadores)) >= precedencia(Str[i]))
        {
            posFixa[j++] = desempilharOperador(&pilhaOperadores);
            posFixa[j++] = ' ';
        }
        empilharOperador(&pilhaOperadores, Str[i]);
        i++;
    }

    while (topoOperador(&pilhaOperadores) != '\0')
    {
        char op = desempilharOperador(&pilhaOperadores);
        if (strchr("sctlr", op))
        {
            if (op == 's')
                j += sprintf(&posFixa[j], "sen ");
            else if (op == 'c')
                j += sprintf(&posFixa[j], "cos ");
            else if (op == 't')
                j += sprintf(&posFixa[j], "tg ");
            else if (op == 'l')
                j += sprintf(&posFixa[j], "log ");
            else if (op == 'r')
                j += sprintf(&posFixa[j], "raiz ");
        }
        else
        {
            posFixa[j++] = op;
            posFixa[j++] = ' ';
        }
    }

    if (j > 0)
        posFixa[j - 1] = '\0';
    else
        posFixa[j] = '\0';
    return posFixa;
}

char *getFormaInFixa(char *StrPosFixa)
{
    char inFixa[30][100];
    int topo = -1;
    static char retorno[MAX];
    char copiaStr[MAX];
    strcpy(copiaStr, StrPosFixa);
    char *token = strtok(copiaStr, " ");

    while (token != NULL)
    {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        {
            strcpy(inFixa[++topo], token);
        }
        else
        {
            char novaExpressao[256];
            if (funcoes(token))
            {
                char op1[100];
                strcpy(op1, inFixa[topo]);
                sprintf(novaExpressao, "%s(%s)", token, op1);
                strcpy(inFixa[topo], novaExpressao);
            }
            else
            {
                char op2[100], op1[100];
                strcpy(op2, inFixa[topo--]);
                strcpy(op1, inFixa[topo]);
                sprintf(novaExpressao, "(%s %s %s)", op1, token, op2);
                strcpy(inFixa[topo], novaExpressao);
            }
        }
        token = strtok(NULL, " ");
    }
    if (topo == 0)
    {
        char *resultadoFinal = inFixa[0];
        int tamanho = strlen(resultadoFinal);
        if (tamanho > 1 && resultadoFinal[0] == '(' && resultadoFinal[tamanho - 1] == ')')
        {
            resultadoFinal[tamanho - 1] = '\0';
            strcpy(retorno, resultadoFinal + 1);
        }
        else
        {
            strcpy(retorno, resultadoFinal);
        }
    }
    else
    {
        strcpy(retorno, "Erro na expressao");
    }
    return retorno;
}

float getValorPosFixa(char *StrPosFixa)
{
    NUMEROS pilhaNumeros;
    inicializarNumeros(&pilhaNumeros);
    char copiaStr[MAX];
    strcpy(copiaStr, StrPosFixa);
    char *token = strtok(copiaStr, " ");

    while (token != NULL)
    {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        {
            empilharNumero(&pilhaNumeros, atof(token));
        }
        else
        {
            float op1, op2, resultado = 0.0f;
            if (funcoes(token))
            {
                op1 = desempilharNumero(&pilhaNumeros);
                if (strcmp(token, "raiz") == 0)
                    resultado = sqrt(op1);
                else if (strcmp(token, "sen") == 0)
                    resultado = sin(op1 * M_PI / 180.0);
                else if (strcmp(token, "cos") == 0)
                    resultado = cos(op1 * M_PI / 180.0);
                else if (strcmp(token, "tg") == 0)
                    resultado = tan(op1 * M_PI / 180.0);
                else if (strcmp(token, "log") == 0)
                    resultado = log10(op1);
            }
            else
            {
                op2 = desempilharNumero(&pilhaNumeros);
                op1 = desempilharNumero(&pilhaNumeros);
                switch (token[0])
                {
                case '+':
                    resultado = op1 + op2;
                    break;
                case '-':
                    resultado = op1 - op2;
                    break;
                case '*':
                    resultado = op1 * op2;
                    break;
                case '/':
                    resultado = op1 / op2;
                    break;
                case '%':
                    resultado = fmod(op1, op2);
                    break;
                case '^':
                    resultado = pow(op1, op2);
                    break;
                }
            }
            empilharNumero(&pilhaNumeros, resultado);
        }
        token = strtok(NULL, " ");
    }
    return desempilharNumero(&pilhaNumeros);
}

float getValorInFixa(char *StrInFixa)
{
    char *posFixa = getFormaPosFixa(StrInFixa);
    float valor = getValorPosFixa(posFixa);
    return valor;
}

