# Trabalho Final - Kanban-Lite | Programação Orientada a Objetos (C++)

**Disciplina:** Programação Orientada a Objetos  
**Tema:** Kanban lite  
**Aluna:** Anne Fernandes da Costa Oliveira  
**Matrícula:** 20240010789  

## 🎥 Vídeo de Demonstração

> **📹 Assista à demonstração do sistema:**  
> 🔗 **[Demonstração do Kanban-Lite](media/demonstracao.webm)**
>
> O vídeo apresenta:
> - ✅ Execução do projeto
> - ✅ Interface gráfica (Qt6)
> - ✅ Funcionalidades principais (boards, colunas, cards)
> - ✅ Sistema de tags e filtros
> - ✅ Drag & drop e persistência JSON


## 🎯 Objetivo
Este repositório contém o desenvolvimento do trabalho final da disciplina de Programação Orientada a Objetos (POO).  
O projeto escolhido foi um **Sistema de Gerenciamento de Tarefas Colaborativas (Kanban lite)**, desenvolvido em C++ com uso de POO e boas práticas modernas da linguagem.

---

## 📂 Estrutura de Pastas
- `design/` → diagramas UML, documentação de design  
- `docs/` → guias detalhados e documentação técnica
- `include/` → cabeçalhos principais (.h)  
  - `external/` → bibliotecas de terceiros (nlohmann/json)
- `src/` → implementações (.cpp) e executável CLI
- `tests/` → testes automatizados e validação
- `data/` → arquivos JSON salvos (*.json ignorado pelo git)

---

## 🚀 Como Usar

### 🎨 Interface Gráfica (Etapa 4 - NOVO!)

**Pré-requisitos:** Qt6 ou Qt5

```bash
# Instalar Qt (Ubuntu/Debian)
sudo apt install qt6-base-dev
# OU
sudo apt install qtbase5-dev

# Compilar
mkdir -p build && cd build
cmake ..
make

# Executar GUI
# Opção 1: A partir do diretório build/ (onde você está após make)
cd ui && ./kanban_lite_gui

# Opção 2: A partir da raiz do projeto
./build/ui/kanban_lite_gui

# Opção 3: Usar script helper (Linux/macOS apenas)
./run_gui.sh
```

� **Relatório Final**: [`docs/final_report.pdf`](docs/final_report.pdf) (todas as etapas)  
📐 **Diagrama de Classes**: [`design/class_diagram.png`](design/class_diagram.png)  
� **Documentação de Design**: [`design/README_design.md`](design/README_design.md)

---

### 💻 Interface CLI (Etapas 2-3)

**Compilação:**
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

**Pré-requisitos:** CMake ≥ 3.16, GCC ≥ 9.0 (C++17), Git

**Executando o CLI:**
```bash
cd build/src
./kanban_cli
```

**Comandos Diretos (modo referência):**
```bash
./kanban_cli help                                      # Ver todos os comandos
./kanban_cli board create MeuProjeto                   # Criar board
./kanban_cli column add MeuProjeto ToDo 5              # Adicionar coluna
./kanban_cli card add MeuProjeto ToDo "Feature X"      # Adicionar cartão
```

> **💡 Nota:** Estes são exemplos de sintaxe. Para uso real, veja [Exemplo de Uso Completo](#exemplo-de-uso-completo) abaixo.

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

#### Persistência e Utilidades
```bash
kanban_cli save <arquivo>               # Salvar estado em JSON (recomendado: data/my_board.json)
kanban_cli load <arquivo>               # Carregar estado do JSON
kanban_cli history                      # Mostrar histórico de atividades
kanban_cli help                         # Mostrar ajuda
```

> **⚠️ IMPORTANTE:** O CLI não persiste automaticamente! Para salvar seu trabalho:
> 1. Use `save data/meu_board.json` para salvar
> 2. Use `load data/meu_board.json` para carregar
> 3. Ou use o **modo interativo** (execute `./kanban_cli` sem argumentos) para trabalhar e salvar ao final
>
> **💡 Recomendação:** Use a **Interface Gráfica** para persistência automática em JSON.

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
> save data/meu_projeto.json
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

State saved to 'data/meu_projeto.json'.
```

**Usando comandos diretos (em uma sessão):**
```bash
# Criar um board e trabalhar nele em sequência
./kanban_cli board create MeuBoard
./kanban_cli column add MeuBoard ToDo 5
./kanban_cli column add MeuBoard Doing 3
./kanban_cli column add MeuBoard Done
./kanban_cli card add MeuBoard ToDo "Tarefa 1"
./kanban_cli card add MeuBoard ToDo "Tarefa 2"
./kanban_cli card list MeuBoard
./kanban_cli save data/meu_board.json

# Carregar e continuar trabalhando
./kanban_cli load data/meu_board.json
./kanban_cli card move card_1 ToDo Doing MeuBoard
./kanban_cli save data/meu_board.json
```

> **⚠️ Nota:** Comandos diretos funcionam apenas dentro de uma mesma sessão. Use `save` para persistir!

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

### ✅ Etapa 3 (Persistência JSON e Interface Gráfica Qt) - **CONCLUÍDA**
- ✅ Serialização completa para JSON (nlohmann/json)
- ✅ Métodos `toJson()` e `fromJson()` em todas as classes
- ✅ Timestamps e metadados completos
- ✅ 24 testes de persistência automatizados
- ✅ Pasta `data/` para organização de arquivos
- ✅ CLI com feedback visual aprimorado
- ✅ 100% compatibilidade com Etapa 2
- ✅ Interface gráfica completa com Qt6/Qt5
- ✅ Widgets hierárquicos (MainWindow → BoardView → ColumnView → CardView)
- ✅ Sistema de menus (File, Help)
- ✅ Gestão visual de WIP limits com cores
- ✅ Persistência JSON integrada (save/load)
- ✅ Indicadores visuais de prioridade
- ✅ Carregamento automático de `data/kanban_data.json`
- ✅ Compatibilidade 100% com CLI
- ✅ CMake com Qt integration
- ✅ Documentação completa

### 🎯 Funcionalidades Principais

#### 🖥️ CLI (kanban_cli)
- Modo interativo e comandos diretos
- Operações CRUD completas
- Persistência JSON
- Histórico de atividades
- Validação de WIP limits

#### 🎨 GUI (kanban_lite_gui)
- Visualização moderna de boards Kanban
- Drag-ready interface (preparada para arrastar)
- Cores inteligentes de WIP (verde/amarelo/vermelho)
- Cards com prioridade visual (bordas coloridas)
- Salvamento automático ao fechar
- Menus e dialogs intuitivos

### Artefatos do Projeto

#### Executáveis
- **CLI:** `build/src/kanban_cli`
- **GUI:** `build/ui/kanban_lite_gui`
- **Testes:** `build/persistence_tests`, `build/headers_check`

#### Documentação
- **Relatório Final:** `docs/final_report.pdf` (completo - todas as etapas)
- **Relatórios por Etapa:** `docs/report_etapa{1,2,3}.pdf`
- **Diagrama UML:** `design/class_diagram.png`
- **Design:** `design/README_design.md`

#### Código
- **Backend:** `src/{Board,Column,Card,User,ActivityLog}.cpp`
- **GUI:** `ui/{MainWindow,BoardView,ColumnView,CardView}.{h,cpp}`
- **Testes:** `tests/{cli_tests.sh,persistence_tests.cpp}`

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

## � Documentação Adicional

- **📄 Relatório Final Completo:** [`docs/final_report.pdf`](docs/final_report.pdf)
- **📐 Diagrama de Classes (UML):** [`design/class_diagram.png`](design/class_diagram.png)
- **📋 Documentação de Design:** [`design/README_design.md`](design/README_design.md)
- **📦 Relatórios por Etapa:** 
  - [`docs/report_etapa1.pdf`](docs/report_etapa1.pdf) - Design Inicial
  - [`docs/report_etapa2.pdf`](docs/report_etapa2.pdf) - CLI Funcional
  - [`docs/report_etapa3.pdf`](docs/report_etapa3.pdf) - Persistência JSON e GUI

---

**Desenvolvido como trabalho final de Programação Orientada a Objetos - UFPB - 2025**

