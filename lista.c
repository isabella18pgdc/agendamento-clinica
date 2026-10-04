/* lista.c                                                    */
/* Implementacao das listas do sistema da clinica             */
/* Trabalho Pratico I - Estrutura de Dados I (DS130)          */
/* Aluna: Isabella Pinheiro Galuski da Cruz                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lista.h"


/* ================= FUNCOES AUXILIARES ================= */

/* Descarta o que sobrou no buffer do teclado (o '\n' do scanf) */
/* Autoria: Isabella Pinheiro                                   */
void limpa_buffer(void)
{
    int c;

    c = getchar();
    while (c != '\n' && c != EOF)
        c = getchar();
}

/* Troca o '\n' deixado pelo fgets pelo fim de string */
/* Autoria: Isabella Pinheiro                         */
void remove_quebra(char s[])
{
    int i;

    i = 0;
    while (s[i] != '\0') {
        if (s[i] == '\n')
            s[i] = '\0';
        else
            i++;
    }
}

/* Le uma linha inteira com fgets e tira o '\n' do final */
/* Se digitar mais do que cabe, descarta o resto         */
/* Autoria: Isabella Pinheiro                            */
void le_texto(char s[], int tamanho)
{
    if (fgets(s, tamanho, stdin) == NULL) {
        s[0] = '\0';
        return;
    }

    /* Sem '\n' na string: a pessoa digitou demais */
    if (strchr(s, '\n') == NULL)
        limpa_buffer();
    else
        remove_quebra(s);
}

/* Le um numero inteiro; se digitar letra devolve -1 */
/* Autoria: Isabella Pinheiro                        */
int le_inteiro(void)
{
    int numero;
    int lidos;

    lidos = scanf("%d", &numero);

    /* Acabou a entrada (Ctrl+Z / Ctrl+D): trata como sair */
    if (lidos == EOF)
        return 0;

    if (lidos != 1)
        numero = -1;

    limpa_buffer();

    return numero;
}

/* Confere se a data existe (inclusive 29/02 em ano bissexto) */
/* Devolve 1 se valida e 0 se invalida                        */
/* Autoria: Isabella Pinheiro                                 */
int valida_data(Data d)
{
    int dias_no_mes[12] = {31, 28, 31, 30, 31, 30,
                           31, 31, 30, 31, 30, 31};

    if (d.ano < 2025 || d.ano > 2100)
        return 0;

    if (d.mes < 1 || d.mes > 12)
        return 0;

    /* Bissexto: divisivel por 4 e nao por 100, ou por 400 */
    if ((d.ano % 4 == 0 && d.ano % 100 != 0) || d.ano % 400 == 0)
        dias_no_mes[1] = 29;

    if (d.dia < 1 || d.dia > dias_no_mes[d.mes - 1])
        return 0;

    return 1;
}

/* Devolve 1 se as duas datas forem iguais */
/* Autoria: Isabella Pinheiro              */
int mesma_data(Data a, Data b)
{
    return a.dia == b.dia && a.mes == b.mes && a.ano == b.ano;
}

/* Confere se a hora esta no atendimento (8h a 17h, sem 12h) */
/* Autoria: Isabella Pinheiro                                */
int horario_valido(int hora)
{
    if (hora < HORA_INICIO || hora > HORA_FIM)
        return 0;

    if (hora == HORA_ALMOCO)
        return 0;

    return 1;
}

/* Compara dois nomes sem diferenciar maiuscula de minuscula  */
/* (o strcmp colocava "ana" depois de "Bruno")                */
/* Negativo: a vem antes | 0: iguais | positivo: a vem depois */
/* Autoria: Isabella Pinheiro                                 */
int compara_nomes(char a[], char b[])
{
    int i;
    int ca;
    int cb;

    i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        ca = toupper((unsigned char)a[i]);
        cb = toupper((unsigned char)b[i]);

        if (ca != cb)
            return ca - cb;

        i++;
    }

    return toupper((unsigned char)a[i]) - toupper((unsigned char)b[i]);
}

/* Diz qual agendamento vem primeiro no tempo: compara ano, */
/* depois mes, dia, hora e, se empatar, a sala              */
/* Funciona igual ao strcmp: negativo, zero ou positivo     */
/* Autoria: Isabella Pinheiro                               */
int compara_agendamentos(Agendamento a, Agendamento b)
{
    if (a.data.ano != b.data.ano)
        return a.data.ano - b.data.ano;

    if (a.data.mes != b.data.mes)
        return a.data.mes - b.data.mes;

    if (a.data.dia != b.data.dia)
        return a.data.dia - b.data.dia;

    if (a.hora != b.hora)
        return a.hora - b.hora;

    return a.sala - b.sala;
}


/* ============ LISTA ESTATICA - SALAS (vetor) ============ */

/* Insere uma sala no final do vetor                   */
/* Devolve 0 se a lista estiver cheia e 1 se deu certo */
/* Autoria: Isabella Pinheiro                          */
int insere_sala(ListaSalas *ls, int numero, char especialidade[])
{
    if (ls->qtd == MAX_SALAS)
        return 0;

    ls->itens[ls->qtd].numero = numero;
    strcpy(ls->itens[ls->qtd].especialidade, especialidade);
    ls->qtd++;

    return 1;
}

/* Zera a lista e cadastra as salas que a clinica ja tem */
/* Autoria: Isabella Pinheiro                            */
void inicializa_salas(ListaSalas *ls)
{
    ls->qtd = 0;

    insere_sala(ls, 1, "Clinica Geral");
    insere_sala(ls, 2, "Odontologia");
    insere_sala(ls, 3, "Psicologia");
    insere_sala(ls, 4, "Fisioterapia");
    insere_sala(ls, 5, "Nutricao");
}

/* Procura a sala pelo numero e devolve a posicao no vetor */
/* Se nao achar, devolve -1                                */
/* Autoria: Isabella Pinheiro                              */
int busca_sala(ListaSalas *ls, int numero)
{
    int i;

    for (i = 0; i < ls->qtd; i++) {
        if (ls->itens[i].numero == numero)
            return i;
    }

    return -1;
}

/* Mostra todas as salas da clinica */
/* Autoria: Isabella Pinheiro       */
void imprime_salas(ListaSalas *ls)
{
    int i;

    printf("\nSalas da clinica:\n");
    for (i = 0; i < ls->qtd; i++)
        printf("  Sala %d - %s\n", ls->itens[i].numero,
               ls->itens[i].especialidade);
}


/* ======= LISTA ENCADEADA SIMPLES - PACIENTES (A a Z) ======= */

/* Insere o paciente ja na posicao certa da ordem alfabetica. */
/* Recebe ponteiro para ponteiro porque, quando insere no     */
/* inicio, o primeiro no da lista muda.                       */
/* Devolve 1 se inseriu e 0 se faltou memoria                 */
/* Autoria: Isabella Pinheiro                                 */
int insere_paciente_ordenado(NoPaciente **inicio, Paciente p)
{
    NoPaciente *novo;
    NoPaciente *atual;

    novo = (NoPaciente *)malloc(sizeof(NoPaciente));
    if (novo == NULL)
        return 0;

    novo->dados = p;
    novo->prox = NULL;

    /* Lista vazia ou nome vem antes do primeiro: entra no inicio */
    if (*inicio == NULL ||
        compara_nomes(p.nome, (*inicio)->dados.nome) < 0) {
        novo->prox = *inicio;
        *inicio = novo;
        return 1;
    }

    /* Anda ate o no que vai ficar ANTES do novo */
    atual = *inicio;
    while (atual->prox != NULL &&
           compara_nomes(atual->prox->dados.nome, p.nome) < 0)
        atual = atual->prox;

    novo->prox = atual->prox;
    atual->prox = novo;

    return 1;
}

/* Percorre a lista procurando o CPF                 */
/* Devolve o no do paciente ou NULL se nao encontrar */
/* Autoria: Isabella Pinheiro                        */
NoPaciente *busca_paciente(NoPaciente *inicio, char cpf[])
{
    NoPaciente *atual;

    atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->dados.cpf, cpf) == 0)
            return atual;

        atual = atual->prox;
    }

    return NULL;
}

/* Mostra todos os pacientes (ja saem em ordem alfabetica) */
/* Autoria: Isabella Pinheiro                              */
void imprime_pacientes(NoPaciente *inicio)
{
    NoPaciente *atual;
    int cont;

    printf("\n========== PACIENTES CADASTRADOS ==========\n");

    if (inicio == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    cont = 0;
    atual = inicio;
    while (atual != NULL) {
        cont++;
        printf("%d) %s | CPF: %s | %s - %s\n", cont,
               atual->dados.nome, atual->dados.cpf,
               atual->dados.vinculo, atual->dados.curso);
        atual = atual->prox;
    }

    printf("Total: %d paciente(s)\n", cont);
}

/* Da free em todos os nos da lista de pacientes */
/* Autoria: Isabella Pinheiro                    */
void libera_pacientes(NoPaciente **inicio)
{
    NoPaciente *atual;
    NoPaciente *aux;

    atual = *inicio;
    while (atual != NULL) {
        aux = atual->prox;     /* guarda o proximo antes do free */
        free(atual);
        atual = aux;
    }

    *inicio = NULL;
}


/* === LISTA DUPLAMENTE ENCADEADA - AGENDAMENTOS (data/hora) === */

/* Insere o agendamento mantendo a ordem por data e hora. */
/* Tres casos: lista vazia, antes do primeiro e meio/fim. */
/* Devolve 1 se inseriu e 0 se faltou memoria             */
/* Autoria: Isabella Pinheiro                             */
int insere_agendamento_ordenado(NoAgendamento **inicio, Agendamento a)
{
    NoAgendamento *novo;
    NoAgendamento *atual;

    novo = (NoAgendamento *)malloc(sizeof(NoAgendamento));
    if (novo == NULL)
        return 0;

    novo->dados = a;
    novo->ant = NULL;
    novo->prox = NULL;

    /* Caso 1: lista vazia */
    if (*inicio == NULL) {
        *inicio = novo;
        return 1;
    }

    /* Caso 2: o novo vem antes do primeiro */
    if (compara_agendamentos(a, (*inicio)->dados) < 0) {
        novo->prox = *inicio;
        (*inicio)->ant = novo;
        *inicio = novo;
        return 1;
    }

    /* Caso 3: meio ou fim. Para no ultimo no que vem antes */
    atual = *inicio;
    while (atual->prox != NULL &&
           compara_agendamentos(atual->prox->dados, a) < 0)
        atual = atual->prox;

    novo->prox = atual->prox;
    novo->ant = atual;

    if (atual->prox != NULL)       /* se nao for o ultimo */
        atual->prox->ant = novo;

    atual->prox = novo;

    return 1;
}

/* Procura a consulta de um CPF numa data. Como cada paciente  */
/* so pode ter uma consulta por dia, CPF + data ja identifica. */
/* Devolve o no ou NULL se nao achar                           */
/* Autoria: Isabella Pinheiro                                  */
NoAgendamento *busca_agendamento(NoAgendamento *inicio, char cpf[],
                                 Data d)
{
    NoAgendamento *atual;

    atual = inicio;
    while (atual != NULL) {
        if (strcmp(atual->dados.cpf, cpf) == 0 &&
            mesma_data(atual->dados.data, d))
            return atual;

        atual = atual->prox;
    }

    return NULL;
}

/* Verifica se ja tem consulta na sala, na data e na hora */
/* Devolve 1 se esta ocupado e 0 se esta livre            */
/* Autoria: Isabella Pinheiro                             */
int horario_ocupado(NoAgendamento *inicio, int sala, Data d, int hora)
{
    NoAgendamento *atual;

    atual = inicio;
    while (atual != NULL) {
        if (atual->dados.sala == sala && atual->dados.hora == hora &&
            mesma_data(atual->dados.data, d))
            return 1;

        atual = atual->prox;
    }

    return 0;
}

/* Acha a consulta pelo CPF + data e tira o no da lista dupla. */
/* Antes do free copia os dados para "removido" (historico).   */
/* Como o no conhece o anterior (ant), nao precisa guardar o   */
/* anterior enquanto percorre, como na lista simples.          */
/* Devolve 1 se removeu e 0 se nao encontrou                   */
/* Autoria: Isabella Pinheiro                                  */
int remove_no_agendamento(NoAgendamento **inicio, char cpf[], Data d,
                          Agendamento *removido)
{
    NoAgendamento *no;

    no = busca_agendamento(*inicio, cpf, d);
    if (no == NULL)
        return 0;

    *removido = no->dados;

    if (no->ant == NULL)           /* era o primeiro da lista */
        *inicio = no->prox;
    else
        no->ant->prox = no->prox;

    if (no->prox != NULL)          /* nao era o ultimo */
        no->prox->ant = no->ant;

    free(no);

    return 1;
}

/* Libera todos os nos da lista de agendamentos */
/* Autoria: Isabella Pinheiro                   */
void libera_agendamentos(NoAgendamento **inicio)
{
    NoAgendamento *atual;
    NoAgendamento *aux;

    atual = *inicio;
    while (atual != NULL) {
        aux = atual->prox;
        free(atual);
        atual = aux;
    }

    *inicio = NULL;
}


/* ======= LISTA COM CABECALHO - HISTORICO DE AGENDAMENTOS ======= */

/* Aloca o cabecalho da lista e deixa tudo zerado */
/* Autoria: Isabella Pinheiro                     */
Cabecalho *cria_historico(void)
{
    Cabecalho *h;

    h = (Cabecalho *)malloc(sizeof(Cabecalho));
    if (h != NULL) {
        h->inicio = NULL;
        h->fim = NULL;
        h->qtd = 0;
        h->total_agendados = 0;
        h->total_cancelados = 0;
    }

    return h;
}

/* Registra AGENDADO ou CANCELADO no final do historico.       */
/* Com o ponteiro "fim" do cabecalho nao precisa percorrer a   */
/* lista para inserir no final. Tambem atualiza os contadores. */
/* Devolve 1 se inseriu e 0 se faltou memoria                  */
/* Autoria: Isabella Pinheiro                                  */
int insere_historico(Cabecalho *h, char acao[], Agendamento a,
                     char nome[])
{
    NoHistorico *novo;

    novo = (NoHistorico *)malloc(sizeof(NoHistorico));
    if (novo == NULL)
        return 0;

    /* Guarda uma COPIA dos dados: o agendamento original pode */
    /* ser apagado depois, quando a consulta for desmarcada    */
    novo->numero = h->qtd + 1;
    strcpy(novo->acao, acao);
    strcpy(novo->nome_paciente, nome);
    novo->dados = a;
    novo->prox = NULL;

    if (h->inicio == NULL)         /* historico vazio */
        h->inicio = novo;
    else
        h->fim->prox = novo;       /* liga depois do ultimo */

    h->fim = novo;
    h->qtd++;

    if (strcmp(acao, "AGENDADO") == 0)
        h->total_agendados++;
    else
        h->total_cancelados++;

    return 1;
}

/* Mostra o resumo guardado no cabecalho e depois todos os */
/* registros, na ordem em que aconteceram                  */
/* Autoria: Isabella Pinheiro                              */
void imprime_historico(Cabecalho *h, ListaSalas *salas)
{
    NoHistorico *atual;
    int pos;

    printf("\n============ HISTORICO DE AGENDAMENTOS ============\n");
    printf("Registros: %d | Agendados: %d | Cancelados: %d\n",
           h->qtd, h->total_agendados, h->total_cancelados);
    printf("---------------------------------------------------\n");

    if (h->inicio == NULL) {
        printf("Nenhum registro no historico ainda.\n");
        return;
    }

    atual = h->inicio;
    while (atual != NULL) {
        pos = busca_sala(salas, atual->dados.sala);

        printf("#%d [%s] %02d/%02d/%04d %02d:00 | Sala %d (%s)\n",
               atual->numero, atual->acao,
               atual->dados.data.dia, atual->dados.data.mes,
               atual->dados.data.ano, atual->dados.hora,
               atual->dados.sala,
               pos != -1 ? salas->itens[pos].especialidade : "?");
        printf("    Paciente: %s - CPF %s\n",
               atual->nome_paciente, atual->dados.cpf);

        atual = atual->prox;
    }
}

/* Libera todos os nos do historico e depois o cabecalho */
/* Autoria: Isabella Pinheiro                            */
void libera_historico(Cabecalho *h)
{
    NoHistorico *atual;
    NoHistorico *aux;

    if (h == NULL)
        return;

    atual = h->inicio;
    while (atual != NULL) {
        aux = atual->prox;
        free(atual);
        atual = aux;
    }

    free(h);
}
