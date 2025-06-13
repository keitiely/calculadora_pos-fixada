#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

// Função auxiliar para testar a partir de uma expressão pós-fixada
void executarTestePosfixo(const char *id, const char *expressaoPosfixa)
{
    printf("----------------------------------------\n");
    printf("Teste %s\n", id);
    printf("Pós-fixa Original  : %s\n", expressaoPosfixa);

    // É importante criar cópias, pois as funções podem modificar as strings
    char copiaParaInfixa[512];
    strcpy(copiaParaInfixa, expressaoPosfixa);

    char copiaParaValor[512];
    strcpy(copiaParaValor, expressaoPosfixa);

    // 1. Testando: Pós-fixa -> Infixa
    char *resultadoInfixa = getFormaInFixa(copiaParaInfixa);
    printf("Infixa Convertida  : %s\n", resultadoInfixa);

    // 2. Testando: Avaliar o Valor da Pós-fixa
    float valorFinal = getValorPosFixa(copiaParaValor);
    printf("Valor Calculado    : %.4f\n\n", valorFinal);
}

int main()
{
    printf("AVALIADOR DE EXPRESSOES NUMERICAS - NOVOS TESTES (A PARTIR DA POS-FIXA)\n\n");

    // Array com o novo conjunto de expressões pós-fixadas para testar
    const char *testes[] = {
        "100 200 10 / + 3 2 * -",
        "10 20 + 100 log *",
        "9 raiz 2 3 ^ +",
        "90 sen 0 cos + 4 *",
        "15 4 % 5 +",
        "2 1000 10 10 * / log *"
    };
    // Identificadores dos testes alterados para números
    const char *ids[] = {"1", "2", "3", "4", "5", "6"};
    int numTestes = sizeof(testes) / sizeof(testes[0]);

    // Loop que executa o ciclo completo para cada novo teste
    for (int i = 0; i < numTestes; i++)
    {
        executarTestePosfixo(ids[i], testes[i]);
    }

    printf("----------------------------------------\n");
    printf("Fim dos testes.\n");

    return 0;
}
