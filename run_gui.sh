#!/bin/bash
# Script para executar a GUI do Kanban-Lite
# Data: 08/10/2025
# Plataforma: Linux/macOS apenas (Bash script)
# Windows: Execute diretamente build\ui\kanban_lite_gui.exe

cd "$(dirname "$0")/build/ui"

echo "==================================="
echo "  Kanban-Lite GUI"
echo "==================================="
echo ""
echo "Para testar o sistema:"
echo "1. Menu: File > Open"
echo "2. Navegar para: ../../data/kanban_data.json ou ../../data/demo.json"
echo "3. Criar cards, adicionar tags, filtrar por tags"
echo "4. Testar drag & drop entre colunas"
echo ""
echo "Iniciando GUI..."
echo ""

env -i \
  HOME="$HOME" \
  DISPLAY="$DISPLAY" \
  XDG_RUNTIME_DIR="$XDG_RUNTIME_DIR" \
  PATH="/usr/local/bin:/usr/bin:/bin" \
  QT_QPA_PLATFORM_PLUGIN_PATH="/usr/lib/x86_64-linux-gnu/qt6/plugins/platforms" \
  ./kanban_lite_gui
