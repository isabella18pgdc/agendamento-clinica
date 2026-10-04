/* paciente.c                                                   */
/* Cadastro de pacientes: le os dados, valida e manda pra lista */
/* Trabalho Pratico I - Estrutura de Dados I (DS130)            */
/* Aluna: Isabella Pinheiro Galuski da Cruz                     */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "paciente.h"

/* Confere se o CPF tem 11 caracteres e se todos sao numeros */
/* (nao calcula os digitos verificadores, so o formato)      */
/* Devolve 1 se esta certo e 0 se nao                        */
/* Autoria: Isabella Pinheiro                                */
int valida_cpf(char cpf[])
{
    int i;

    if (strlen(cpf) != 11)
        return 0;

    for (i = 0; i < 11; i++) {
        if (!isdigit((unsigned char)cpf[i]))
            return 0;
    }

    return 1;
}

/* Le o CPF numa string maior (para nao cortar se digitar a */
/* mais) e so copia para o destino se estiver valido        */
/* Autoria: Isabella Pinheiro                               */
int le_cpf(char cpf[])
{
    char entrada[50];

    le_texto(entrada, 50);

    if (!valida_cpf(entrada))
        return 0;

    strcpy(cpf, entrada);

    return 1;
}

/* Pede os dados do paciente, nao deixa repetir CPF e insere */
/* na lista de pacientes                                     */
/* Autoria: Isabella Pinheiro                                */
void cadastra_paciente(NoPaciente **lista)
{
    Paciente p;
    int opcao;

    printf("\n========== CADASTRO DE PACIENTE ==========\n");

    printf("CPF (somente os 11 numeros): ");
    if (!le_cpf(p.cpf)) {
        printf("CPF invalido! Digite so os numeros, sem ponto e traco.\n");
        return;
    }

    if (busca_paciente(*lista, p.cpf) != NULL) {
        printf("Ja existe um paciente cadastrado com esse CPF.\n");
        return;
    }

    printf("Nome completo: ");
    le_texto(p.nome, 100);

    if (strlen(p.nome) == 0) {
        printf("O nome nao pode ficar vazio.\n");
        return;
    }

    printf("Matricula: ");
    le_texto(p.matricula, 20);

    printf("Curso / Departamento: ");
    le_texto(p.curso, 60);

    printf("Vinculo (1 - Estudante | 2 - Professor): ");
    opcao = le_inteiro();

    if (opcao == 2)
        strcpy(p.vinculo, "Professor");
    else
        strcpy(p.vinculo, "Estudante");   /* qualquer outro valor */

    /* Passa o endereco da lista: o inicio pode mudar */
    if (insere_paciente_ordenado(lista, p))
        printf("\nPaciente %s cadastrado com sucesso!\n", p.nome);
    else
        printf("\nErro: nao foi possivel alocar memoria.\n");
}

/* Mostra os dados de um paciente em forma de ficha */
/* Autoria: Isabella Pinheiro                       */
void imprime_paciente(Paciente p)
{
    printf("Nome.......: %s\n", p.nome);
    printf("CPF........: %s\n", p.cpf);
    printf("Matricula..: %s\n", p.matricula);
    printf("Curso......: %s\n", p.curso);
    printf("Vinculo....: %s\n", p.vinculo);
}
