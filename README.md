# Sistema de Agendamento de Consultas em Clínica Universitária

Trabalho Prático I da disciplina **Estrutura de Dados I (DS130)**
Tecnologia em Análise e Desenvolvimento de Sistemas – UFPR
Professor: Helcio Soares Padilha Junior

## Autoria

Trabalho feito individualmente por **Isabella Pinheiro Galuski da Cruz** (única integrante do grupo). Todas as funções foram desenvolvidas por ela, e cada uma tem no topo um comentário explicando o que faz e a autoria.

## Sobre o projeto

A clínica universitária atende estudantes e professores somente com agendamento prévio. Cada sala atende uma especialidade e tem horários fixos durante o dia. O sistema permite cadastrar pacientes, marcar e desmarcar consultas, ver os horários livres de cada sala e consultar o histórico de tudo que foi agendado ou cancelado.

Os dados ficam só na memória enquanto o programa está rodando (não salvamos em arquivo).

## Funcionalidades

1. Cadastrar paciente (nome, CPF, matrícula, curso e vínculo: estudante ou professor)
2. Listar pacientes (em ordem alfabética)
3. Agendar consulta (CPF, sala, data e hora)
4. Consultar agendamentos por CPF
5. Listar agendamentos por sala
6. Ver horários disponíveis de uma sala em uma data
7. Desmarcar consulta (por CPF + data)
8. Histórico completo de agendamentos (lista com cabeçalho)

## Estruturas de dados utilizadas

| Dado | Estrutura | Por que escolhemos |
|---|---|---|
| Salas | Lista estática (vetor) | A clínica tem um número fixo e pequeno de salas, que não muda durante a execução. Com vetor o acesso é simples e não precisa de `malloc`. |
| Pacientes | Lista encadeada simples, ordenada por nome | Não sabemos quantos pacientes vão ser cadastrados, então a lista dinâmica cresce conforme precisa. Inserindo já em ordem alfabética, a listagem sai pronta sem precisar ordenar depois. |
| Agendamentos | Lista duplamente encadeada, ordenada por data e hora | Agendamentos entram e saem o tempo todo. Na lista dupla cada nó conhece o anterior, então para remover basta achar o nó e religar `ant` e `prox`, sem ficar guardando o anterior durante a busca. A ordem por data/hora faz as listagens saírem em ordem cronológica. |
| Histórico | Lista com cabeçalho | O cabeçalho guarda ponteiro para o início, ponteiro para o fim e os contadores (total de registros, agendados e cancelados). Com o ponteiro `fim` a inserção no final é direta, e o resumo do histórico já fica pronto no cabeçalho sem precisar percorrer a lista. |

## Regras de funcionamento

- O CPF deve ser digitado só com os 11 números (sem ponto e traço) e não pode repetir.
- Só pode agendar consulta para paciente que já está cadastrado.
- Horário de atendimento: de hora em hora, das 8h às 17h, sem atendimento às 12h (almoço).
- Não pode ter duas consultas na mesma sala, na mesma data e hora.
- Cada paciente pode ter **no máximo uma consulta por dia**. Por isso o CPF + data identificam uma consulta só, e é assim que fazemos a remoção.
- Todo agendamento e todo cancelamento ficam registrados no histórico (o histórico nunca é apagado).

## Organização dos arquivos

```
main.c          -> menu principal
paciente.c/.h   -> cadastro e exibição de pacientes, validação de CPF
agendamento.c/.h-> controle dos agendamentos (marcar, listar, horários, desmarcar)
lista.c/.h      -> structs e implementação das 4 listas + funções auxiliares
README.md       -> este arquivo
diario_de_bordo.pdf -> relato do desenvolvimento e divisão de tarefas
```

## Como compilar e executar

Precisa ter o `gcc` instalado.

**Linux / WSL / macOS:**

```bash
gcc main.c paciente.c agendamento.c lista.c -o clinica
./clinica
```

**Windows (MinGW / prompt de comando):**

```bash
gcc main.c paciente.c agendamento.c lista.c -o clinica.exe
clinica.exe
```

No Code::Blocks dá para criar um projeto "Console application" em C e adicionar todos os arquivos `.c` e `.h`.

## Exemplo de uso

```
1 - Cadastrar paciente
CPF (somente os 11 numeros): 12345678901
Nome completo: Maria Souza
Matricula: GRR20240001
Curso / Departamento: Analise e Desenvolvimento de Sistemas
Vinculo (1 - Estudante | 2 - Professor): 1

3 - Agendar consulta
CPF do paciente: 12345678901
Numero da sala: 2
Data da consulta (dd/mm/aaaa): 10/11/2026
Horario desejado (so a hora, ex: 14): 14

Consulta agendada com sucesso!
  10/11/2026 as 14:00 | Sala 2 (Odontologia) | Maria Souza
```
