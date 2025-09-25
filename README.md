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
- `src/` → implementações (.cpp)  
- `tests/` → testes de compilação e unitários  

---

## 📄 Licença
Este projeto é licenciado sob os termos da **MIT License**. Consulte o arquivo `LICENSE` na raiz do repositório para mais detalhes.

---

## 🚧 Status do Projeto
Etapa 1 (Design Inicial) concluída: estrutura base, cabeçalhos, CMake mínimo, UML preliminar e pipeline de CI adicionados. Próximas etapas incluirão regras de negócio, testes e persistência.

### Artefatos da Etapa 1
- Diagrama UML: `design/class_diagram.puml` (imagem exportada `design/class_diagram.png`)
- Relato técnico: `design/README_design.md` e [`docs/report_etapa1.pdf`](docs/report_etapa1.pdf)
- Headers principais: `include/Board.h`, `include/Column.h`, `include/Card.h`, `include/User.h`, `include/ActivityLog.h`
- Build mínimo: `CMakeLists.txt` + `src/CMakeLists.txt` + `src/test_headers.cpp`
- CI: `.github/workflows/ci.yml` (build e checagem de headers)
- Documentação: [`docs/documentation.pdf`](docs/documentation.pdf) (Doxygen)

### Como Compilar

**Comandos Básicos:**
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make test
```

**Pré-requisitos:** CMake ≥ 3.16, GCC ≥ 9.0 (C++17), Git

**📋 Guia Detalhado:** Para instruções completas, solução de problemas e validações adicionais, consulte [`docs/build_guide.md`](docs/build_guide.md).

---

