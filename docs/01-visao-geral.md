# Papel do Scanner no Front-End e Conceitos de Token, Lexema e Padrão

O **scanner**, também chamado de **analisador léxico**, é a primeira fase do processo de compilação e pertence ao bloco conhecido como **front-end**. Sua função principal é ler o código‑fonte caractere por caractere e agrupá‑los em **tokens**. Esses tokens são então fornecidos ao **parser** (analisador sintático), que constrói a árvore sintática do programa.

## Diferença entre Token, Lexema e Padrão

- **Lexema** – é a sequência de caracteres escrita no código‑fonte.
  *Exemplo:* Na linha `int x = 5;`, os lexemas são `"int"`, `"x"`, `"="`, `"5"` e `";"`.

- **Padrão** – é a regra (geralmente descrita por expressões regulares) que define como um lexema deve ser estruturado para pertencer a uma determinada categoria.
  *Exemplo:* O padrão para um **identificador** geralmente é `[a-zA-Z_][a-zA-Z0-9_]*` e o padrão para um **número inteiro** é `[0-9]+`.
- **Token** – é a estrutura de dados gerada pelo scanner que combina um **tipo** (que representa o padrão identificado) com o **lexema** correspondente. Além disso, o token pode carregar informações como linha e coluna como metadados para gerar mensagens de aviso/erro.
  *Exemplo:* Para o lexema `"x"` que casa com o padrão de identificador, o scanner produz um token do tipo `IDENTIFICADOR` com o valor `"x"`.

Em suma: o scanner aplica **padrões** sobre o texto, extrai **lexemas** e os empacota em **tokens** para as fases seguintes do compilador.

---

## Exemplo com uma Linha de Código

Considere a seguinte linha de código em C:

```c
int x = 5;
```

Possui os seguintes tokens:
```
TOKENS RECONHECIDOS:
───────────────────────────────────────────────────────────
TIPO          LEXEMA                POSIÇÃO
────────────  ────────────────────  ─────────────
int           int                   [linha  1, col  1]
ID            x                     [linha  1, col  5]
ASSIGN        =                     [linha  1, col  7]
NUM_INT       5                     [linha  1, col  9]
SEMICOL       ;                     [linha  1, col 10]
EOF           <EOF>                 [linha  1, col 11]
───────────────────────────────────────────────────────────
Total de tokens: 6
═══════════════════════════════════════════════════════════
```
