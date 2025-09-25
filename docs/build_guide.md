# 🔧 Guia Detalhado de Compilação - Kanban-Lite

Este guia fornece instruções completas para compilar e testar o projeto **Kanban-Lite** do zero, incluindo solução de problemas comuns.

---

## # Documentação disponível
ls -la docs/documentation.pdf

## 📋 Pré-requisitos

Antes de compilar, certifique-se de ter instalado:

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y build-essential cmake g++ git

# Verificar versões mínimas
cmake --version    # ≥ 3.16
g++ --version      # ≥ 9.0 (suporte C++17)
```

**Versões testadas:**
- **CMake:** 3.22.1 ou superior
- **GCC:** 11.4.0 ou superior  
- **C++ Standard:** 17

---

## 📥 Clonando o Repositório

```bash
# Clone do repositório
git clone https://github.com/annefernandess/Kanban-Lite.git
cd Kanban-Lite

# Verificar branch (deve estar em 'develop' ou 'main')
git branch
```

---

## 🏗️ Compilação Passo a Passo

### **Passo 1: Preparar Diretório de Build**
```bash
# Criar diretório de build (fora do source)
mkdir -p build
cd build
```

### **Passo 2: Configurar com CMake**
```bash
# Configurar projeto (Release mode)
cmake .. -DCMAKE_BUILD_TYPE=Release

# Saída esperada:
# -- The CXX compiler identification is GNU X.X.X
# -- Configuring done
# -- Generating done
# -- Build files have been written to: /path/to/Kanban-Lite/build
```

### **Passo 3: Compilar**
```bash
# Compilar o projeto
make

# Ou alternativamente:
cmake --build . --target headers_check

# Saída esperada:
# [50%] Building CXX object src/CMakeFiles/headers_check.dir/test_headers.cpp.o
# [100%] Linking CXX executable headers_check
# [100%] Built target headers_check
```

### **Passo 4: Executar Testes**
```bash
# Executar suite de testes
make test

# Ou com mais detalhes:
ctest --output-on-failure -V

# Saída esperada:
# Running tests...
# Test project /path/to/Kanban-Lite/build
#     Start 1: headers_check
# 1/1 Test #1: headers_check ....................   Passed    0.00 sec
# 100% tests passed, 0 tests failed out of 1
```

---

## ✅ Verificação de Sucesso

### **Compilação Bem-sucedida:**
```bash
# Verificar executável gerado
ls -la src/headers_check

# Executar manualmente
./src/headers_check
echo $?  # Deve retornar: 0 (sucesso)
```

### **Estrutura Final no Build:**
```
build/
├── src/headers_check          # Executável de teste
├── CMakeCache.txt             # Cache do CMake  
├── Makefile                   # Makefile gerado
└── Testing/                   # Resultados CTest
```

---

## 🚨 Solução de Problemas

### **Erro: "cmake: command not found"**
```bash
# Ubuntu/Debian
sudo apt install cmake

# Verificar instalação
which cmake
```

### **Erro: "No suitable C++ compiler found"**
```bash
# Instalar compilador C++
sudo apt install build-essential g++

# Verificar versão C++17
g++ -std=c++17 --version
```

### **Erro: "CMake version too old"**
```bash
# Verificar versão atual
cmake --version

# Se < 3.16, atualizar via snap (Ubuntu)
sudo snap install cmake --classic
```

### **Build Limpo (caso de problemas):**
```bash
# Limpar e reconstruir
cd build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

### **Problemas de Permissão:**
```bash
# Se erro de permissão ao instalar dependências
sudo chown -R $USER:$USER ~/.cmake
```

### **Erro de Headers não Encontrados:**
```bash
# Verificar se todos os headers estão presentes
find ../include -name "*.h" -ls

# Deve listar: ActivityLog.h, Board.h, Card.h, Column.h, User.h
```

---

## 📊 Validação Completa

Para confirmar que tudo está funcionando perfeitamente:

```bash
# 1. Compilação sem warnings
make VERBOSE=1

# 2. Testes passando  
make test

# 3. Headers válidos individualmente
find ../include -name "*.h" -exec echo "Checking {}" \; \
    -exec g++ -I ../include -std=c++17 -fsyntax-only {} \;

# 4. Teste de integração (todos headers juntos)
echo '#include "ActivityLog.h"
#include "Board.h" 
#include "Card.h"
#include "Column.h"
#include "User.h"
int main(){return 0;}' > test_all.cpp
g++ -I ../include -std=c++17 -fsyntax-only test_all.cpp
rm test_all.cpp

# 5. Documentação disponível
ls -la ../documentation.pdf

# 6. Estrutura do projeto
tree .. -I 'build|latex|.git'
```

**✅ Status Esperado:** Todos os comandos devem executar sem erros.

---

## 🎯 Informações Adicionais

### **Configurações Avançadas:**
```bash
# Build com debug symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build com warnings extras desabilitados
cmake .. -DKANBAN_LITE_ENABLE_WARNINGS=OFF

# Compilação paralela (mais rápida)
make -j$(nproc)
```

### **Limpeza Completa:**
```bash
# Remover todos os arquivos de build
cd ..
rm -rf build/
rm -rf latex/     # Se existir (Doxygen temporário)
rm -f Doxyfile    # Se existir (Doxygen temporário)
```

### **Verificação de Sistema:**
```bash
# Informações do sistema
uname -a
lsb_release -a    # Ubuntu/Debian
cat /proc/cpuinfo | grep "model name" | head -1
free -h
```

---

## 📝 Notas Técnicas

- **Etapa Atual:** 1 (Design e Headers)
- **Funcionalidade:** Verificação de integridade dos headers
- **Padrão C++:** C++17 (requerido para std::chrono e features modernas)
- **Build System:** CMake 3.16+ com CTest integrado
- **Próximas Etapas:** Implementação das classes, testes unitários, persistência
- **Documentação API:** Disponível em `docs/documentation.pdf` na raiz do projeto
- **CI/CD:** GitHub Actions configurado (`.github/workflows/ci.yml`)

---

## 📞 Suporte

Se encontrar problemas não cobertos neste guia:

1. Verifique a versão dos pré-requisitos
2. Confirme que está na branch correta (`develop` ou `main`)
3. Tente um build limpo
4. Consulte os logs detalhados com `make VERBOSE=1`
5. Verifique a documentação no arquivo `README.md` principal