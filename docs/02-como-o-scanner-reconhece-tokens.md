# Reconhecimento de Três Classes de Tokens no Analisador Léxico

O processo de reconhecimento de tokens no analisador léxico se baseia na implementação de um autômato finito determinístico **(AFD)** que pode representar os padrões que descrevem os tipos de tokens na forma de **expressões regulares**.
Iremos analisar o reconhecimento de três classes: **identificadores e palavras-reservadas**, **números inteiros e reais** e **operadores e delimitadores**.

## 1. Identificadores e palavras-reservadas
- **Regra**:
  - [a-zA-Z_][a-zA-Z0-9_]*
- **AFD**:
  - q0 --- [ letra | _ ] --> q1 (aceitação)
  - q1 --- [ letra | dígito | _ ] --> q1 (loop)
- **Descrição**:
   1. Primeiro o scanner avalia se o primeiro caractere é uma letra do alfabeto ou um underline ('_').
   2. Caso positivo, o scanner consome os próximos caracteres desde que sejam letra, número ou underline.
   3. É utilizado o lookahead (espiar()) para checar se a cadeia de caracteres chegou ao fim.
   4. Uma vez identificado o fim da string, é verificado se a palavra formada é uma das palavras reservadas pré-determinadas daquela linguagem. Caso contrário, é um identificador.
- **Exemplo**
  - <ID, _context>

## 2. Números inteiros e reais
- **Regra**:
  - Inteiro: [0-9]+
  - Real: [0-9]+\.[0-9]+
- **AFD**:
  - q0 --[dígito]--> q2 (aceitação para inteiro)
  - q2 --[dígito]--> q2 (loop)
  - q2 --[.]--> q3
  - q3 --[dígito]--> q4 (aceitação para real)
  - q4 --[dígito]--> q4 (loop)
- **Descrição**:
  1. Primeiro o scanner consome dígitos.
  2. Caso encontre '.', o sistema irá continuar consumindo dígitos e classificará o token como número real (float).
  3. Se não encontrar '.', o token é classificado como inteiro (int).
  4. É utilizado o lookahead (espiar()) para determinar quando a cadeia de caracteres chegará ao fim.
- **Exemplo**:
  - <NUM_INT, 5>

  ## 3. Operadores e delimitadores
- **Regra**:
  - Operadores: ==|!=|<=|>=|[+\-*/=<>]
  - Delimitadores: [\;(){},]
- **AFD**:
    Exemplo para '='
  - q0 --[=]--> qEQ (estado intermediário)
  - qEQ --[=]--> qEQUAL (aceitação: ==)
  - qEQ --[outros]--> qASSIGN (aceitação: =)
- **Descrição**:
  1. O scanner verifica se é um operador de dois caracteres (ex.: ==, !=, <=, >=) usando lookahead (espiar()). Se sim, consome ambos e retorna o token correspondente.
  2. Se não for um operador de dois caracteres, verifica a possibilidade de ser um operador de um caractere (+, -, *, /, =, <, >) ou um delimitador (;, ,, (, ), {, }). Consome o caractere e retorna o token respectivo.
  3. Caso não corresponda a nenhum, gera um token de erro.
- **Exemplo**:
  - <ASSIGN, =>