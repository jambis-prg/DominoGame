# Jogo de Dominó

Este é um projeto de um jogo de dominó desenvolvido em C++ utilizando a biblioteca SFML. O objetivo é criar um jogo completo com uma interface gráfica intuitiva e mecânicas de jogo funcionais.

## Funcionalidades

- **Jogo de dominó clássico**: Implementação das regras tradicionais do dominó.
- **Interface gráfica**: Interface de usuário construída usando SFML.
- **Multijogador online**: Suporte para jogar com outros jogadores pela internet (em desenvolvimento).
- **IA básica**: Jogador controlado pelo computador (planejado).

## Instalação

Para compilar e executar o jogo, você precisará do seguinte:

- GCC 13.1.0 ou superior (necessário para compatibilidade com a versão mais recente do SFML)
- SFML 2.5 ou superior
- CMake (opcional, se você preferir um Makefile)

### Passos para Compilação

1. Clone o repositório:
    ```bash
    git clone https://github.com/usuario/DominoGame.git
    cd DominoGame
    ```
2. Certifique-se de que você está usando o GCC 13.1.0 ou superior:
    ```bash
    g++ --version
    ```
3. Compile o projeto usando o Makefile:
    ```bash
    make
    ```

## Progresso

### Checklist de Implementação

- [x] **Interface de Usuário (UI)**
  - Criação da interface gráfica utilizando SFML.
  - A UI básica está funcionando, sendo possível interagir com o jogo e navegar por vários menus diferentes.

- [ ] **Lógica do Jogo**
  - Implementação das regras do dominó.
  - Desenvolvimento das interações entre as peças e o tabuleiro.

- [ ] **Multijogador Online**
  - Implementação do suporte para jogar com outros jogadores pela internet.

- [ ] **Inteligência Artificial (IA)**
  - Desenvolvimento de uma IA básica para jogar contra o jogador humano.

## Como Jogar
O Dominó possui 28 peças, dentre essas, 24 peças serão aleatoriamente distribuídas para 2 duplas, tendo cada jogador 6 peças.

![image](https://github.com/user-attachments/assets/f2499319-e0ab-4641-bc4b-461a6b543837)
O jogo começa selecionando a maior peça de um jogador e colocando na mesa, passando a vez para o jogador imediatamente após ele.

![image](https://github.com/user-attachments/assets/2fa06cae-acc3-4ce0-8cf2-4a17009db56a)

O jogador da vez só pode selecionar as peças que pelo menos um dos lados se encaixa com alguma cabeça na mesa.

Se o jogador da vez não possuir nenhuma peça válida para jogar sua vez é pulada automaticamente para o próximo jogador. O jogo é jogado em duplas, logo caso um membro da dupla tenha jogado todas as suas peças a dupla vence a partida. Caso em algum momento nenhum jogador tenha alguma peça válida para jogar o jogo apenas termina em empate.

## Contribuições

Contribuições são bem-vindas! Se você encontrar algum problema ou tiver sugestões, sinta-se à vontade para abrir uma issue ou enviar um pull request.

## Licença

Este projeto está licenciado sob a Apache-2.0 License. Veja o arquivo `LICENSE` para mais detalhes.
