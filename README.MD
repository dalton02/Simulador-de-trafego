# Simulador de Tráfego 🚦

Um simulador de tráfego rodoviário que demonstra conceitos de programação concorrente, onde cada veículo é uma thread independente e interage com semáforos e outros veículos através de mecanismos de sincronização.

## 🎯 Sobre o Projeto

Este simulador foi desenvolvido para demonstrar na prática conceitos fundamentais de programação concorrente:

- **Threads**: Cada carro é uma thread independente

- **Semáforos**: Controle de fluxo nos cruzamentos

- **Exclusão Mútua**: Acesso seguro a recursos compartilhados

- **Condições de Corrida**: Prevenção através de mecanismos de sincronização

- **Deadlocks**: Gerenciamento cuidadoso para evitar travamentos

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C++

- **Biblioteca Gráfica:** SFML

## 📋 Pré-requisitos

Antes de começar, você precisará ter instalado em sua máquina:

- **C++ Compiler** (GCC 10+ ou equivalente com suporte a C++20)

- **SFML 2.5+** (bibliotecas de desenvolvimento)

### Instalando as Dependências

#### Linux (Ubuntu/Debian)

```

sudo apt-get update

sudo apt-get install g++ libsfml-dev

```

#### Windows

- Baixe o MinGW em mingw-w64.org

- Baixe o SFML SDK em sfml-dev.org

- Configure as variáveis de ambiente apropriadamente

# Compilar o projeto

### Pelo terminal:

`g++ -std=c++20 $(find . -type f -name '\*.cpp') -lsfml-system -lsfml-window -lsfml-graphics -o simulador && ./simulador`

### Pelo Code Blocks

Abra o projeto com o code blocks e configure os linkers:

- sfml-system
- sfml-window
- sfml-graphics

Configure os diretórios de busca:

Na aba Search directories

- Em Compiler: adicione o caminho para os includes do SFML
- Em Linker: adicione o caminho para as bibliotecas do SFML

Agora pode compilar o projeto.

## Para brincar

- Em graphics.hpp, caso deseje olhar as hitbox de colisão de carros e detectores de semaforos, basta modificar a variavel SHOW_COLLISION para true.
- Em sycronization.hpp, caso deseje mudar o clock do relogio global, basta modificar o valor da variavel CLOCK
