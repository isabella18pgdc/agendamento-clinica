# Sistema de Agendamento de Consultas em Clínica Universitária

**Trabalho Prático I – Estrutura de Dados I (DS130)**\
Análise e Desenvolvimento de Sistemas – UFPR\
Professor: Helcio Soares Padilha Junior\
Aluna: Isabella Pinheiro Galuski da Cruz (trabalho individual)

## Sobre o projeto

Sistema em C para organizar as consultas de uma clínica universitária. Estudantes e professores são cadastrados como pacientes e podem marcar, consultar e desmarcar consultas nas salas da clínica. Tudo o que é agendado ou cancelado fica registrado em um histórico.

## Funcionalidades

1. Cadastrar paciente (nome, CPF, matrícula, curso e vínculo)
2. Listar pacientes em ordem alfabética
3. Agendar consulta (CPF, sala, data e hora)
4. Consultar agendamentos por CPF
5. Listar agendamentos por sala
6. Ver horários disponíveis de uma sala
7. Desmarcar consulta (CPF + data)
8. Ver histórico completo de agendamentos

## Listas utilizadas

- **Salas → lista estática (vetor)**
  - A clínica tem poucas salas e esse número não muda durante a execução.
- **Pacientes → lista encadeada simples, em ordem alfabética**
  - Os pacientes só são inseridos e buscados, nunca removidos.
- **Agendamentos → lista duplamente encadeada, em ordem de data e hora**
  - As consultas entram e saem o tempo todo, e a lista dupla facilita a remoção.
- **Histórico → lista com cabeçalho**
  - O cabeçalho guarda o início, o fim e os totais, o que facilita inserir no final.

## Regras do sistema

- CPF com 11 números, sem ponto e traço, e sem repetir
- Só é possível agendar para paciente já cadastrado
- Atendimento de hora em hora, das 8h às 17h (sem atendimento às 12h)
- Não pode haver duas consultas na mesma sala, data e hora
- Cada paciente pode ter no máximo uma consulta por dia

## Arquivos

- `main.c` — menu principal
- `paciente.c` / `paciente.h` — cadastro de pacientes
- `agendamento.c` / `agendamento.h` — controle dos agendamentos
- `lista.c` / `lista.h` — structs e implementação das listas
- Diário de bordo (PDF) — relato do desenvolvimento

## Como compilar e executar

```
gcc main.c paciente.c agendamento.c lista.c -o clinica
./clinica
```

No Windows, o executável fica como `clinica.exe` e roda com `.\clinica.exe`.
