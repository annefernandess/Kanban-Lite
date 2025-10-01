# Relatório Técnico – Etapa 2

**Projeto:** Kanban-Lite (Trabalho Final POO – C++)  
**Aluna:** Anne Fernandes da Costa Oliveira  
**Matrícula:** 20240010789  
**Data:** 02/10/2025  

---

## 1. Objetivo da Etapa
A Etapa 2 teve como objetivo implementar um **protótipo funcional em linha de comando (CLI)** para validar o design definido na Etapa 1.  
O sistema agora permite a criação e manipulação de quadros, colunas e cartões diretamente no terminal, demonstrando os conceitos de Programação Orientada a Objetos aplicados em C++.  

---

## 2. Escopo Implementado
- **Boards**: criação, listagem e remoção  
- **Columns**: adição, listagem e remoção, com limite WIP  
- **Cards**: criação, movimentação, listagem e remoção  
- **ActivityLog**: registro de operações realizadas  
- **CLI**: suporte a execução por comandos (`kanban board create "Meu Board"`) e modo interativo  

---

## 3. Conceitos de POO Aplicados
- **Encapsulamento:** atributos privados e métodos de acesso controlados  
- **Composição:** Board contém Columns; Columns contêm Cards  
- **Associação:** Cards referenciam Users sem ownership  
- **RAII e Smart Pointers:** uso de `std::unique_ptr` para gestão automática de memória  
- **STL e Templates:** containers (`std::vector`) e algoritmos (`std::find_if`) aplicados  
- **Sobrecarga de operadores:** `Card::operator==` para comparação  
- **Exceções:** uso de `std::runtime_error` em operações inválidas  

---

## 4. Testes e Qualidade
- **CTest + cli_tests.sh**: conjunto de testes automatizados cobrindo criação de boards, manipulação de colunas e cartões  
- **CI no GitHub Actions:** compilação e execução dos testes em cada push  
- **Resultados:** todos os testes executados com sucesso, validando as operações essenciais do domínio  

---

## 5. Conclusão
A Etapa 2 entregou um **CLI funcional** que comprova a viabilidade do design do Kanban-Lite.  
As principais operações foram implementadas e validadas com testes automatizados, garantindo robustez e qualidade.  

**Próximos passos (Etapa 3):**  
- Implementar interface gráfica (Qt)  
- Adicionar persistência em arquivo/banco de dados  
- Expandir funcionalidades avançadas (tags, prazos, autenticação)