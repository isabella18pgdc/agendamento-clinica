/* agendamento.h                                              */
/* Prototipos do controle de agendamentos                     */
/* A lista dupla e o historico ficam em lista.c               */
/* Aluna: Isabella Pinheiro Galuski da Cruz                   */

#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

#include "lista.h"
#include "paciente.h"

/* Prototipos das funcoes */
int le_data(Data *d);
void imprime_agendamento(Agendamento a, NoPaciente *pacientes,
                         ListaSalas *salas);
void imprime_grade_horarios(NoAgendamento *lista, int sala, Data d);
void cadastra_agendamento(NoAgendamento **agendamentos,
                          NoPaciente *pacientes, ListaSalas *salas,
                          Cabecalho *historico);
void consulta_por_cpf(NoAgendamento *agendamentos, NoPaciente *pacientes,
                      ListaSalas *salas);
void lista_por_sala(NoAgendamento *agendamentos, NoPaciente *pacientes,
                    ListaSalas *salas);
void mostra_horarios_livres(NoAgendamento *agendamentos,
                            ListaSalas *salas);
void remove_agendamento(NoAgendamento **agendamentos,
                        NoPaciente *pacientes, ListaSalas *salas,
                        Cabecalho *historico);

#endif
