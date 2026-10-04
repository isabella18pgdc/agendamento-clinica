/* lista.h                                                    */
/* Structs do sistema e prototipos das funcoes das listas     */
/* Trabalho Pratico I - Estrutura de Dados I (DS130)          */
/* Aluna: Isabella Pinheiro Galuski da Cruz                   */

#ifndef LISTA_H
#define LISTA_H

#define MAX_SALAS 10

/* Atendimento de hora em hora, das 8h as 17h, sem 12h (almoco) */
#define HORA_INICIO 8
#define HORA_FIM 17
#define HORA_ALMOCO 12

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    char nome[100];
    char cpf[12];          /* 11 digitos + '\0' */
    char matricula[20];
    char curso[60];
    char vinculo[15];      /* "Estudante" ou "Professor" */
} Paciente;

typedef struct {
    char cpf[12];
    int sala;
    Data data;
    int hora;              /* so a hora cheia: 14 = 14:00 */
} Agendamento;

typedef struct {
    int numero;
    char especialidade[40];
} Sala;

/* Lista estatica: vetor de salas + quantidade usada */
typedef struct {
    Sala itens[MAX_SALAS];
    int qtd;
} ListaSalas;

/* Lista encadeada simples: no de paciente */
typedef struct no_paciente {
    Paciente dados;
    struct no_paciente *prox;
} NoPaciente;

/* Lista duplamente encadeada: no de agendamento */
typedef struct no_agendamento {
    Agendamento dados;
    struct no_agendamento *ant;
    struct no_agendamento *prox;
} NoAgendamento;

/* Lista com cabecalho: no do historico */
typedef struct no_historico {
    int numero;
    char acao[12];         /* "AGENDADO" ou "CANCELADO" */
    char nome_paciente[100];
    Agendamento dados;
    struct no_historico *prox;
} NoHistorico;

/* Cabecalho: guarda o inicio, o fim e os contadores */
typedef struct {
    NoHistorico *inicio;
    NoHistorico *fim;
    int qtd;
    int total_agendados;
    int total_cancelados;
} Cabecalho;

/* Prototipos: funcoes auxiliares */
void limpa_buffer(void);
void remove_quebra(char s[]);
void le_texto(char s[], int tamanho);
int le_inteiro(void);
int valida_data(Data d);
int mesma_data(Data a, Data b);
int horario_valido(int hora);
int compara_nomes(char a[], char b[]);
int compara_agendamentos(Agendamento a, Agendamento b);

/* Prototipos: lista estatica de salas */
int insere_sala(ListaSalas *ls, int numero, char especialidade[]);
void inicializa_salas(ListaSalas *ls);
int busca_sala(ListaSalas *ls, int numero);
void imprime_salas(ListaSalas *ls);

/* Prototipos: lista encadeada simples de pacientes */
int insere_paciente_ordenado(NoPaciente **inicio, Paciente p);
NoPaciente *busca_paciente(NoPaciente *inicio, char cpf[]);
void imprime_pacientes(NoPaciente *inicio);
void libera_pacientes(NoPaciente **inicio);

/* Prototipos: lista duplamente encadeada de agendamentos */
int insere_agendamento_ordenado(NoAgendamento **inicio, Agendamento a);
NoAgendamento *busca_agendamento(NoAgendamento *inicio, char cpf[],
                                 Data d);
int horario_ocupado(NoAgendamento *inicio, int sala, Data d, int hora);
int remove_no_agendamento(NoAgendamento **inicio, char cpf[], Data d,
                          Agendamento *removido);
void libera_agendamentos(NoAgendamento **inicio);

/* Prototipos: lista com cabecalho (historico) */
Cabecalho *cria_historico(void);
int insere_historico(Cabecalho *h, char acao[], Agendamento a,
                     char nome[]);
void imprime_historico(Cabecalho *h, ListaSalas *salas);
void libera_historico(Cabecalho *h);

#endif
