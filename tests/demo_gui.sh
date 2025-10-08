#!/bin/bash
# Script de demonstração - cria dados de exemplo para a GUI

echo "🎨 Kanban-Lite GUI - Setup de Demonstração"
echo "=========================================="
echo ""

# Navegar para o diretório do projeto
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
cd "$PROJECT_ROOT"

# Verificar se build/ existe
if [ ! -d "build" ]; then
    echo "❌ Diretório build/ não encontrado!"
    echo "Execute primeiro:"
    echo "  mkdir -p build && cd build && cmake .. && make"
    exit 1
fi

# Verificar se GUI foi compilada
if [ ! -f "build/ui/kanban_lite_gui" ]; then
    echo "❌ GUI não compilada!"
    echo "Certifique-se de ter Qt instalado:"
    echo "  sudo apt install qt6-base-dev"
    echo "Depois recompile:"
    echo "  cd build && cmake .. && make"
    exit 1
fi

# Criar pasta data/ se não existir
mkdir -p data

# Criar JSON de demonstração
echo "📝 Criando board de demonstração..."

cat > data/kanban_data.json << 'EOF'
{
  "boards": [
    {
      "name": "Projeto POO",
      "columns": [
        {
          "name": "Backlog",
          "wipLimit": 0,
          "cards": [
            {
              "id": "card_1",
              "title": "Estudar diagramas UML",
              "description": "Revisar notação de classes, associações e herança",
              "priority": "baixa",
              "assignee": "Anne",
              "dueDate": ""
            },
            {
              "id": "card_2",
              "title": "Implementar persistência JSON",
              "description": "Usar nlohmann/json para salvar estado",
              "priority": "média",
              "assignee": "",
              "dueDate": ""
            }
          ]
        },
        {
          "name": "Em Progresso",
          "wipLimit": 3,
          "cards": [
            {
              "id": "card_3",
              "title": "Desenvolver interface Qt",
              "description": "Criar MainWindow, BoardView e ColumnView",
              "priority": "alta",
              "assignee": "Anne",
              "dueDate": "2024-12-15"
            },
            {
              "id": "card_4",
              "title": "Escrever testes unitários",
              "description": "Cobrir classes principais com testes",
              "priority": "alta",
              "assignee": "Anne",
              "dueDate": ""
            }
          ]
        },
        {
          "name": "Revisão",
          "wipLimit": 2,
          "cards": [
            {
              "id": "card_5",
              "title": "Documentar arquitetura",
              "description": "Criar diagramas e report_etapa4.md",
              "priority": "média",
              "assignee": "",
              "dueDate": ""
            }
          ]
        },
        {
          "name": "Concluído",
          "wipLimit": 0,
          "cards": [
            {
              "id": "card_6",
              "title": "Configurar CMake",
              "description": "Setup de build system com Qt integration",
              "priority": "baixa",
              "assignee": "Anne",
              "dueDate": ""
            },
            {
              "id": "card_7",
              "title": "Implementar classes do domínio",
              "description": "Board, Column, Card, User, ActivityLog",
              "priority": "baixa",
              "assignee": "Anne",
              "dueDate": ""
            }
          ]
        }
      ]
    }
  ],
  "users": [
    {
      "id": "user_1",
      "username": "Anne",
      "role": "developer"
    }
  ],
  "activity_log": {
    "entries": [
      {
        "timestamp": "2024-01-15T10:30:00",
        "action": "Board 'Projeto POO' created"
      },
      {
        "timestamp": "2024-01-15T10:31:00",
        "action": "Column 'Backlog' added to board 'Projeto POO'"
      },
      {
        "timestamp": "2024-01-15T10:32:00",
        "action": "Card 'card_3' moved to 'Em Progresso'"
      }
    ]
  }
}
EOF

echo "✓ Arquivo criado: data/kanban_data.json"
echo ""
echo "🚀 Executando GUI..."
echo ""

# Executar GUI
cd build/ui
./kanban_lite_gui

echo ""
echo "👋 GUI fechada. Obrigado por testar!"
