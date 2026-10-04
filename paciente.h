/* paciente.h                                                 */
/* Prototipos do cadastro de pacientes                        */
/* A lista de pacientes em si fica em lista.c                 */
/* Aluna: Isabella Pinheiro Galuski da Cruz                   */

#ifndef PACIENTE_H
#define PACIENTE_H

#include "lista.h"

/* Prototipos das funcoes */
int valida_cpf(char cpf[]);
int le_cpf(char cpf[]);
void cadastra_paciente(NoPaciente **lista);
void imprime_paciente(Paciente p);

#endif
