/* agendamento.c                                              */
/* Controle dos agendamentos: marcar, listar, horarios livres */
/* e desmarcar consultas                                      */
/* Trabalho Pratico I - Estrutura de Dados I (DS130)          */
/* Aluna: Isabella Pinheiro Galuski da Cruz                   */

#include <stdio.h>
#include <string.h>
#include "agendamento.h"

/* Le uma data no formato dd/mm/aaaa e ja valida */
/* Devolve 1 se a data for valida e 0 se nao     */
/* Autoria: Isabella Pinheiro                    */
int le_data(Data *d)
{
    if (scanf("%d/%d/%d", &d->dia, &d->mes, &d->ano) != 3) {
        limpa_buffer();
        return 0;
    }

    limpa_buffer();

    return valida_data(*d);
}

/* Mostra uma consulta numa linha, com o nome do paciente e a */
/* especialidade da sala (buscados nas outras listas)         */
/* Autoria: Isabella Pinheiro                                 */
void imprime_agendamento(Agendamento a, NoPaciente *pacientes,
                         ListaSalas *salas)
{
    NoPaciente *pac;
    int pos;

    pac = busca_paciente(pacientes, a.cpf);
    pos = busca_sala(salas, a.sala);

    printf("  %02d/%02d/%04d as %02d:00 | Sala %d (%s) | %s\n",
           a.data.dia, a.data.mes, a.data.ano, a.hora, a.sala,
           pos != -1 ? salas->itens[pos].especialidade : "?",
           pac != NULL ? pac->dados.nome : "?");
}

/* Mostra os horarios do dia de uma sala, dizendo se cada um */
/* esta livre ou ocupado (consulta a lista a cada hora)      */
/* Autoria: Isabella Pinheiro                                */
void imprime_grade_horarios(NoAgendamento *lista, int sala, Data d)
{
    int h;
    int livres;

    printf("\nHorarios da sala %d em %02d/%02d/%04d:\n",
           sala, d.dia, d.mes, d.ano);

    livres = 0;
    for (h = HORA_INICIO; h <= HORA_FIM; h++) {
        if (!horario_valido(h)) {
            printf("  %02d:00  -- almoco --\n", h);
            continue;
        }

        if (horario_ocupado(lista, sala, d, h)) {
            printf("  %02d:00  OCUPADO\n", h);
        } else {
            printf("  %02d:00  livre\n", h);
            livres++;
        }
    }

    printf("Horarios livres nesse dia: %d\n", livres);
}

/* Marca uma consulta. Confere: paciente cadastrado, sala que */
/* existe, data valida, uma consulta por dia e horario livre. */
/* Se der tudo certo, insere na lista dupla (ordenada) e      */
/* registra AGENDADO no historico.                            */
/* Autoria: Isabella Pinheiro                                 */
void cadastra_agendamento(NoAgendamento **agendamentos,
                          NoPaciente *pacientes, ListaSalas *salas,
                          Cabecalho *historico)
{
    Agendamento a;
    NoPaciente *pac;

    printf("\n============ NOVO AGENDAMENTO ============\n");

    if (pacientes == NULL) {
        printf("Nenhum paciente cadastrado. Cadastre o paciente antes.\n");
        return;
    }

    printf("CPF do paciente: ");
    if (!le_cpf(a.cpf)) {
        printf("CPF invalido! Digite so os 11 numeros.\n");
        return;
    }

    pac = busca_paciente(pacientes, a.cpf);
    if (pac == NULL) {
        printf("Paciente nao encontrado. Faca o cadastro antes.\n");
        return;
    }

    printf("Paciente: %s\n", pac->dados.nome);

    imprime_salas(salas);
    printf("Numero da sala: ");
    a.sala = le_inteiro();

    if (busca_sala(salas, a.sala) == -1) {
        printf("Essa sala nao existe.\n");
        return;
    }

    printf("Data da consulta (dd/mm/aaaa): ");
    if (!le_data(&a.data)) {
        printf("Data invalida!\n");
        return;
    }

    /* Regra do sistema: no maximo uma consulta por dia */
    if (busca_agendamento(*agendamentos, a.cpf, a.data) != NULL) {
        printf("Esse paciente ja tem consulta marcada nesse dia.\n");
        printf("(regra: no maximo uma consulta por dia por paciente)\n");
        return;
    }

    imprime_grade_horarios(*agendamentos, a.sala, a.data);

    printf("Horario desejado (so a hora, ex: 14): ");
    a.hora = le_inteiro();

    if (!horario_valido(a.hora)) {
        printf("Horario fora do atendimento (8h as 17h, sem 12h).\n");
        return;
    }

    if (horario_ocupado(*agendamentos, a.sala, a.data, a.hora)) {
        printf("Esse horario ja esta ocupado nessa sala.\n");
        return;
    }

    if (!insere_agendamento_ordenado(agendamentos, a)) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return;
    }

    insere_historico(historico, "AGENDADO", a, pac->dados.nome);

    printf("\nConsulta agendada com sucesso!\n");
    imprime_agendamento(a, pacientes, salas);
}

/* Consulta por CPF: mostra a ficha do paciente e todas as */
/* consultas dele (percorre a lista filtrando pelo CPF)    */
/* Autoria: Isabella Pinheiro                              */
void consulta_por_cpf(NoAgendamento *agendamentos, NoPaciente *pacientes,
                      ListaSalas *salas)
{
    char cpf[12];
    NoPaciente *pac;
    NoAgendamento *atual;
    int cont;

    printf("\n============ CONSULTA POR CPF ============\n");
    printf("CPF: ");
    if (!le_cpf(cpf)) {
        printf("CPF invalido!\n");
        return;
    }

    pac = busca_paciente(pacientes, cpf);
    if (pac == NULL) {
        printf("Nenhum paciente cadastrado com esse CPF.\n");
        return;
    }

    imprime_paciente(pac->dados);
    printf("\nConsultas marcadas:\n");

    cont = 0;
    atual = agendamentos;
    while (atual != NULL) {
        if (strcmp(atual->dados.cpf, cpf) == 0) {
            imprime_agendamento(atual->dados, pacientes, salas);
            cont++;
        }
        atual = atual->prox;
    }

    if (cont == 0)
        printf("  Nenhuma consulta marcada.\n");
    else
        printf("Total: %d consulta(s)\n", cont);
}

/* Mostra todas as consultas de uma sala. Como a lista ja esta */
/* ordenada por data e hora, saem em ordem cronologica         */
/* Autoria: Isabella Pinheiro                                  */
void lista_por_sala(NoAgendamento *agendamentos, NoPaciente *pacientes,
                    ListaSalas *salas)
{
    NoAgendamento *atual;
    int sala;
    int pos;
    int cont;

    printf("\n========= AGENDAMENTOS POR SALA =========\n");
    imprime_salas(salas);
    printf("Numero da sala: ");
    sala = le_inteiro();

    pos = busca_sala(salas, sala);
    if (pos == -1) {
        printf("Essa sala nao existe.\n");
        return;
    }

    printf("\nConsultas da sala %d (%s):\n", sala,
           salas->itens[pos].especialidade);

    cont = 0;
    atual = agendamentos;
    while (atual != NULL) {
        if (atual->dados.sala == sala) {
            imprime_agendamento(atual->dados, pacientes, salas);
            cont++;
        }
        atual = atual->prox;
    }

    if (cont == 0)
        printf("  Nenhuma consulta marcada nessa sala.\n");
    else
        printf("Total: %d consulta(s)\n", cont);
}

/* Pede a sala e a data e mostra a grade de horarios livres */
/* Autoria: Isabella Pinheiro                               */
void mostra_horarios_livres(NoAgendamento *agendamentos,
                            ListaSalas *salas)
{
    int sala;
    Data d;

    printf("\n========== HORARIOS DISPONIVEIS ==========\n");
    imprime_salas(salas);
    printf("Numero da sala: ");
    sala = le_inteiro();

    if (busca_sala(salas, sala) == -1) {
        printf("Essa sala nao existe.\n");
        return;
    }

    printf("Data (dd/mm/aaaa): ");
    if (!le_data(&d)) {
        printf("Data invalida!\n");
        return;
    }

    imprime_grade_horarios(agendamentos, sala, d);
}

/* Desmarca uma consulta pelo CPF + data: mostra a consulta,  */
/* pede confirmacao, tira da lista dupla e registra CANCELADO */
/* no historico                                               */
/* Autoria: Isabella Pinheiro                                 */
void remove_agendamento(NoAgendamento **agendamentos,
                        NoPaciente *pacientes, ListaSalas *salas,
                        Cabecalho *historico)
{
    char cpf[12];
    char resposta[10];
    Data d;
    NoAgendamento *no;
    NoPaciente *pac;
    Agendamento removido;

    printf("\n=========== DESMARCAR CONSULTA ===========\n");

    if (*agendamentos == NULL) {
        printf("Nao ha nenhuma consulta marcada.\n");
        return;
    }

    printf("CPF do paciente: ");
    if (!le_cpf(cpf)) {
        printf("CPF invalido!\n");
        return;
    }

    printf("Data da consulta (dd/mm/aaaa): ");
    if (!le_data(&d)) {
        printf("Data invalida!\n");
        return;
    }

    no = busca_agendamento(*agendamentos, cpf, d);
    if (no == NULL) {
        printf("Nenhuma consulta desse CPF nessa data.\n");
        return;
    }

    printf("\nConsulta encontrada:\n");
    imprime_agendamento(no->dados, pacientes, salas);

    printf("Confirma o cancelamento? (S/N): ");
    le_texto(resposta, 10);

    if (resposta[0] != 'S' && resposta[0] != 's') {
        printf("Cancelamento abortado.\n");
        return;
    }

    /* Passa o endereco da lista: se for o primeiro, o inicio muda */
    if (remove_no_agendamento(agendamentos, cpf, d, &removido)) {
        pac = busca_paciente(pacientes, cpf);
        insere_historico(historico, "CANCELADO", removido,
                         pac != NULL ? pac->dados.nome : "?");
        printf("Consulta desmarcada com sucesso!\n");
    }
}
