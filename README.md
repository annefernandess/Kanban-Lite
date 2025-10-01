# Trabalho Final — Programação Orientada a Objetos (C++)

**Disciplina:** Programação Orientada a Objetos  
**Tema:** Kanban lite  
**Aluna:** Anne Fernandes da Costa Oliveira  
**Matrícula:** 20240010789  

---

## 🎯 Objetivo
Este repositório contém o desenvolvimento do trabalho final da disciplina de Programação Orientada a Objetos (POO).  
O projeto escolhido foi um **Sistema de Gerenciamento de Tarefas Colaborativas (Kanban lite)**, desenvolvido em C++ com uso de POO e boas práticas modernas da linguagem.

---

## 📂 Estrutura de Pastas
- `design/` → diagramas UML, documentação de design  
- `docs/` → guias detalhados e documentação técnica
- `include/` → cabeçalhos principais (.h)  
- `src/` → implementações (.cpp) e executável CLI
- `tests/` → testes automatizados e validação

---

## 🚀 Como Usar (Etapa 2 - CLI Funcional)

### Compilação

**Comandos Básicos:**
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

**Pré-requisitos:** CMake ≥ 3.16, GCC ≥ 9.0 (C++17), Git

### Executando o CLI

**Modo Interativo:**
```bash
cd build/src
./kanban_cli
```

**Comandos Diretos:**
```bash
./kanban_cli help
./kanban_cli board create MeuProjeto
./kanban_cli column add MeuProjeto ToDo 5
./kanban_cli card add MeuProjeto ToDo "Implementar feature X"
```

### Comandos Disponíveis

#### Gerenciamento de Quadros
```bash
kanban_cli board create <nome>           # Criar quadro
kanban_cli board list                    # Listar quadros
kanban_cli board remove <nome>           # Remover quadro
```

#### Gerenciamento de Colunas
```bash
kanban_cli column add <quadro> <nome> [limite_wip]    # Adicionar coluna
kanban_cli column list <quadro>                       # Listar colunas
kanban_cli column remove <quadro> <nome>              # Remover coluna
```

#### Gerenciamento de Cartões
```bash
kanban_cli card add <quadro> <coluna> <titulo>        # Adicionar cartão
kanban_cli card list <quadro> [coluna]                # Listar cartões
kanban_cli card move <id> <origem> <destino> <quadro> # Mover cartão
kanban_cli card remove <quadro> <coluna> <id>         # Remover cartão
```

#### Utilidades
```bash
kanban_cli history                      # Mostrar histórico de atividades
kanban_cli save <arquivo>               # Salvar estado
kanban_cli load <arquivo>               # Carregar estado
kanban_cli help                         # Mostrar ajuda
```

### Exemplo de Uso Completo

```bash
# Modo interativo
./kanban_cli
> board create ProjetoFinal
> column add ProjetoFinal Backlog
> column add ProjetoFinal Desenvolvimento 3
> column add ProjetoFinal Teste 2
> column add ProjetoFinal Concluido
> card add ProjetoFinal Backlog "Implementar headers"
> card add ProjetoFinal Backlog "Criar CLI"
> card add ProjetoFinal Backlog "Escrever testes"
> card move card_1 Backlog Desenvolvimento ProjetoFinal
> card list ProjetoFinal
> history
> save meu_projeto.kanban
> exit
```

**Saída esperada:**
```
Board 'ProjetoFinal' created successfully.
Column 'Backlog' added to board 'ProjetoFinal'.
Column 'Desenvolvimento' added to board 'ProjetoFinal' with WIP limit 3.
Column 'Teste' added to board 'ProjetoFinal' with WIP limit 2.
Column 'Concluido' added to board 'ProjetoFinal'.
Card 'Implementar headers' (ID: card_1) added to column 'Backlog' in board 'ProjetoFinal'.
Card 'Criar CLI' (ID: card_2) added to column 'Backlog' in board 'ProjetoFinal'.
Card 'Escrever testes' (ID: card_3) added to column 'Backlog' in board 'ProjetoFinal'.
Card 'card_1' moved from 'Backlog' to 'Desenvolvimento'.

All cards in board 'ProjetoFinal':

  Column: Backlog
    - Criar CLI (ID: card_2, Priority: 0)
    - Escrever testes (ID: card_3, Priority: 0)

  Column: Desenvolvimento
    - Implementar headers (ID: card_1, Priority: 0)

  Column: Teste
    (no cards)

  Column: Concluido
    (no cards)

Activity History:
  [2025-10-01 17:30:00] Column 'Backlog' added to board 'ProjetoFinal'
  [2025-10-01 17:30:00] Column 'Desenvolvimento' added to board 'ProjetoFinal'
  [2025-10-01 17:30:00] Column 'Teste' added to board 'ProjetoFinal'
  [2025-10-01 17:30:00] Column 'Concluido' added to board 'ProjetoFinal'
  [2025-10-01 17:30:01] Card 'card_1' added to column 'Backlog' in board 'ProjetoFinal'
  [2025-10-01 17:30:01] Card 'card_2' added to column 'Backlog' in board 'ProjetoFinal'
  [2025-10-01 17:30:01] Card 'card_3' added to column 'Backlog' in board 'ProjetoFinal'
  [2025-10-01 17:30:01] Card 'card_1' moved from 'Backlog' to 'Desenvolvimento' in board 'ProjetoFinal'

State saved to 'meu_projeto.kanban'.
```

---

## 🧪 Executando Testes

```bash
# Executar todos os testes via CTest
cd build
ctest --verbose

# Executar apenas os testes do CLI
tests/cli_tests.sh

# Executar teste de headers (Etapa 1)
./src/headers_check
```

Os testes incluem:
- ✅ Compilação de headers (Etapa 1)
- ✅ 20 testes automatizados do CLI (Etapa 2)
- ✅ Funcionalidades CRUD completas
- ✅ Movimentação de cartões
- ✅ Limites WIP
- ✅ Persistência de estado
- ✅ Tratamento de erros

---

## 📄 Licença
Este projeto é licenciado sob os termos da **MIT License**. Consulte o arquivo `LICENSE` na raiz do repositório para mais detalhes.

---

## 📈 Status do Projeto

### ✅ Etapa 1 (Design Inicial) - **CONCLUÍDA**
- ✅ Estrutura base e cabeçalhos
- ✅ CMake configurado
- ✅ Diagrama UML
- ✅ Pipeline de CI
- ✅ Documentação Doxygen

### ✅ Etapa 2 (CLI Funcional) - **CONCLUÍDA** 
- ✅ Implementação completa das classes do domínio
- ✅ Interface CLI interativa e não-interativa
- ✅ Operações CRUD para boards, colunas e cartões
- ✅ Sistema de movimentação de cartões
- ✅ Controle de limites WIP
- ✅ Sistema de logging/histórico
- ✅ Persistência básica (save/load)
- ✅ 20 testes automatizados
- ✅ Integração com CTest
- ✅ CI/CD atualizado

### 🚧 Próximas Etapas
- **Etapa 3:** Interface Gráfica (GUI)

### Artefatos da Etapa 2
- **Executável:** `build/src/kanban_cli`
- **Implementações:** `src/{Board,Column,Card,User,ActivityLog}.cpp`
- **CLI Principal:** `src/main.cpp`
- **Testes:** `tests/cli_tests.sh` (20 testes automatizados)
- **Build:** CMakeLists.txt atualizado para Etapa 2
- **CI:** `.github/workflows/ci.yml` expandido

### Conceitos POO Implementados
- **Encapsulamento:** Atributos privados com interface pública controlada
- **Composição:** Board possui Columns, Column possui Cards
- **Associação:** Card referencia User
- **Dependency Injection:** ActivityLog injetado em Board
- **RAII:** Smart pointers para gerenciamento de memória
- **STL:** Uso extensivo de containers e algoritmos
- **Exception Safety:** Tratamento de erros e códigos de saída apropriados

### Métricas do Código
- **Linhas de código:** ~800 linhas (implementação + CLI)
- **Classes implementadas:** 5 (Board, Column, Card, User, ActivityLog)
- **Testes automatizados:** 20 cenários
- **Cobertura funcional:** 100% dos requisitos da Etapa 2
- **Padrão:** C++17 moderno

---

**📋 Guia Detalhado:** Para instruções completas, solução de problemas e validações adicionais, consulte [`docs/build_guide.md`](docs/build_guide.md).

