#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

// Função auxiliar para rodar um ciclo completo de testes
void executarTesteCicloCompleto(const char *id, const char *expressaoPosfixaOriginal)
{
    printf("----------------------------------------\n");
    printf("Teste %s\n", id);
    printf("1. Pós-fixa Original : %s\n", expressaoPosfixaOriginal);

    // É importante criar cópias, pois as funções podem modificar as strings
    char copiaParaInfixa[512];
    strcpy(copiaParaInfixa, expressaoPosfixaOriginal);

    char copiaParaValor[512];
    strcpy(copiaParaValor, expressaoPosfixaOriginal);

    // 2. Converte de Pós-fixa para Infixa
    char *resultadoInfixa = getFormaInFixa(copiaParaInfixa);
    printf("2. Infixa Convertida : %s\n", resultadoInfixa);

    // 3. Pega a Infixa gerada e converte de volta para Pós-fixa
    char *resultadoPosfixaReconvertido = getFormaPosFixa(resultadoInfixa);
    printf("3. Pós-fixa Reconvertida: %s\n", resultadoPosfixaReconvertido);

    // 4. Calcula o valor final a partir da Pós-fixa original
    float valorFinal = getValorPosFixa(copiaParaValor);
    printf("4. Valor Calculado   : %.4f\n\n", valorFinal);
}

int main()
{
    printf("AVALIADOR DE EXPRESSOES NUMERICAS - TESTE DE CICLO COMPLETO\n\n");

    // Array com o novo conjunto de expressões pós-fixadas para testar
    const char *testes[] = {
        "100 200 10 / + 3 2 * -",
        "10 20 + 100 log *",
        "9 raiz 2 3 ^ +",
        "90 sen 0 cos + 4 *",
        "15 4 % 5 +",
        "2 1000 10 10 * / log *"};
    const char *ids[] = {"1", "2", "3", "4", "5", "6"};
    int numTestes = sizeof(testes) / sizeof(testes[0]);

    // Loop que executa o ciclo completo para cada novo teste
    for (int i = 0; i < numTestes; i++)
    {
        executarTesteCicloCompleto(ids[i], testes[i]);
    }

    printf("----------------------------------------\n");
    printf("Fim dos testes.\n");

    return 0;
}
