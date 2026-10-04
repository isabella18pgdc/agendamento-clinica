/* main.c                                                       */
/* Sistema de Agendamento de Consultas em Clinica Universitaria */
/* Trabalho Pratico I - Estrutura de Dados I (DS130) - UFPR     */
/* Aluna: Isabella Pinheiro Galuski da Cruz                     */

#include <stdio.h>
#include "lista.h"
#include "paciente.h"
#include "agendamento.h"

/* Prototipo da funcao do menu */
void imprime_menu(void);

/* Cria as listas, mostra o menu em loop e chama a funcao de */
/* cada opcao. Ao sair, libera toda a memoria alocada.       */
/* Autoria: Isabella Pinheiro                                */
int main(void)
{
    NoPaciente *pacientes = NULL;          /* lista simples */
    NoAgendamento *agendamentos = NULL;    /* lista dupla */
    ListaSalas salas;                      /* lista estatica */
    Cabecalho *historico;                  /* lista com cabecalho */
    int opcao;

    inicializa_salas(&salas);

    historico = cria_historico();
    if (historico == NULL) {
        printf("Erro ao criar o historico.\n");
        return 1;
    }

    do {
        imprime_menu();
        opcao = le_inteiro();

        switch (opcao) {
        case 1:
            cadastra_paciente(&pacientes);
            break;
        case 2:
            imprime_pacientes(pacientes);
            break;
        case 3:
            cadastra_agendamento(&agendamentos, pacientes, &salas,
                                 historico);
            break;
        case 4:
            consulta_por_cpf(agendamentos, pacientes, &salas);
            break;
        case 5:
            lista_por_sala(agendamentos, pacientes, &salas);
            break;
        case 6:
            mostra_horarios_livres(agendamentos, &salas);
            break;
        case 7:
            remove_agendamento(&agendamentos, pacientes, &salas,
                               historico);
            break;
        case 8:
            imprime_historico(historico, &salas);
            break;
        case 0:
            printf("\nSaindo do sistema... ate mais!\n");
            break;
        default:
            printf("Opcao invalida! Tente de novo.\n");
        }
    } while (opcao != 0);

    /* Libera a memoria de todas as listas */
    libera_pacientes(&pacientes);
    libera_agendamentos(&agendamentos);
    libera_historico(historico);

    return 0;
}

/* Funcao void: so imprime as opcoes do menu principal */
/* Autoria: Isabella Pinheiro                          */
void imprime_menu(void)
{
    printf("\n==================================================\n");
    printf("     CLINICA UNIVERSITARIA - AGENDAMENTOS\n");
    printf("==================================================\n");
    printf(" 1 - Cadastrar paciente\n");
    printf(" 2 - Listar pacientes\n");
    printf(" 3 - Agendar consulta\n");
    printf(" 4 - Consultar agendamentos por CPF\n");
    printf(" 5 - Listar agendamentos por sala\n");
    printf(" 6 - Ver horarios disponiveis\n");
    printf(" 7 - Desmarcar consulta (CPF + data)\n");
    printf(" 8 - Historico completo de agendamentos\n");
    printf(" 0 - Sair\n");
    printf("Escolha uma opcao: ");
}
