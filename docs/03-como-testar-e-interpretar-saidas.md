# Como testar e interpretar saídas (outputs)
Para utilizar o programa, abra o seu terminal e insira os comandos conforme o desejado:
## Compilar
- Para gerar o executável lexer:
```bash
   $ make
```
## Testes pré-definidos
- Para executar os testes pré-definidos nesse projeto:
```bash
   $ make test1        # Analisa o arquivo teste1.c
   $ make test2        # Analisa o arquivo teste2.c
   $ make test3        # Analisa o arquivo teste3_erros.c
```

## Executar em arquivo específico
- Para executar em um arquivo específico existente no projeto
```bash
   $ ./lexer arquivo.c
```

## Interpretação de saída
- A saída é uma tabela com três colunas:
```


   TIPO        LEXEMA                POSIÇÃO
   ──────────  ────────────────────  ─────────────
   ID          fatorial              [linha 1, col 5]
   LPAREN      (                     [linha 1, col 13]
   ...
```
  - TIPO: classe do token (ex.: ID, NUM_INT, if, PLUS, LPAREN, EOF, ERROR).
  - LEXEMA: texto exato extraído do código‑fonte.
  - POSIÇÃO: linha e coluna onde o token começa (linha 1 = primeira linha).

## Erros léxicos
- Quando um caractere inválido é encontrado (ex.: @, #, $) ou uma string não é fechada, o programa exibe uma mensagem de erro e um token do tipo ERROR.
- Exemplo:
```bash
     ERRO LÉXICO linha 5, coluna 8: caractere inesperado '@'
     ERROR        @                     [linha 5, col 8]
```