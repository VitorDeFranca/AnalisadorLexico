# ANALISADOR LÉXICO EM C — MATERIAL DIDÁTICO

## Descrição

Este é um analisador léxico (lexer/scanner) completo e didático implementado em C para a disciplina de Compiladores. O código está amplamente comentado para fins educacionais.

## Características

### Tokens Reconhecidos

#### 1. Identificadores e Palavras Reservadas
- **Identificadores:** `x`, `contador`, `soma`, `_temp`, etc.
- **Palavras-chave:** `if`, `else`, `while`, `int`, `float`, `return`

#### 2. Literais
- **Inteiros:** `42`, `0`, `123`, `-5`
- **Reais:** `3.14`, `0.5`, `2.0`
- **Strings:** `"hello world"`, `"teste"`

#### 3. Operadores
- **Aritméticos:** `+`, `-`, `*`, `/`
- **Relacionais:** `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Atribuição:** `=`

#### 4. Delimitadores
- **Parênteses:** `(`, `)`
- **Chaves:** `{`, `}`
- **Outros:** `;`, `,`

#### 5. Comentários (ignorados)
- **Linha:** `// comentário até o fim da linha`
- **Bloco:** `/* comentário de múltiplas linhas */`

---

## Compilação

### Requisitos
- Compilador GCC (ou Clang)
- Sistema Linux, macOS ou Windows (com MinGW/Cygwin)

### Compilar o Lexer

```bash
# Compilação básica
gcc -o lexer lexer.c

# Compilação com warnings (recomendado)
gcc -Wall -Wextra -std=c99 -o lexer lexer.c

# Compilação otimizada para produção
gcc -O2 -Wall -Wextra -std=c99 -o lexer lexer.c
```

### Verificar Compilação

```bash
# Deve criar o executável 'lexer'
ls -lh lexer

# No Windows
dir lexer.exe
```

---

## Execução

### Modo 1: Código Embutido (Exemplo Didático)

Execute sem argumentos para analisar o código de exemplo que está embutido no programa:

```bash
./lexer
```

**Saída esperada:**
```
═══════════════════════════════════════════════════════════
 ANÁLISE LÉXICA — Código de Exemplo Embutido
═══════════════════════════════════════════════════════════

CÓDIGO-FONTE:
───────────────────────────────────────────────────────────
/* Exemplo de código em C */
int fatorial(int n) {
    if (n <= 1) {
        return 1;  // caso base
    ...
───────────────────────────────────────────────────────────

TOKENS RECONHECIDOS:
───────────────────────────────────────────────────────────
TIPO          LEXEMA                POSIÇÃO
────────────  ────────────────────  ─────────────
int           int                   [linha  2, col  1]
ID            fatorial              [linha  2, col  5]
LPAREN        (                     [linha  2, col 13]
int           int                   [linha  2, col 14]
ID            n                     [linha  2, col 18]
RPAREN        )                     [linha  2, col 19]
LBRACE        {                     [linha  2, col 21]
if            if                    [linha  3, col  5]
...
```

### Modo 2: Análise de Arquivo Externo

Passe o nome do arquivo como argumento:

```bash
# Analisar teste1.c
./lexer teste1.c

# Analisar teste2.c
./lexer teste2.c

# Analisar seu próprio código
./lexer meu_codigo.c
```

**Exemplo com teste1.c:**
```bash
./lexer teste1.c
```

**Saída:**
```
═══════════════════════════════════════════════════════════
 ANÁLISE LÉXICA DO ARQUIVO: teste1.c
═══════════════════════════════════════════════════════════

TOKENS RECONHECIDOS:
───────────────────────────────────────────────────────────
TIPO          LEXEMA                POSIÇÃO
────────────  ────────────────────  ─────────────
int           int                   [linha  5, col  1]
ID            soma                  [linha  5, col  5]
LPAREN        (                     [linha  5, col  9]
int           int                   [linha  5, col 10]
ID            a                     [linha  5, col 14]
COMMA         ,                     [linha  5, col 15]
int           int                   [linha  5, col 17]
ID            b                     [linha  5, col 21]
RPAREN        )                     [linha  5, col 22]
LBRACE        {                     [linha  5, col 24]
return        return                [linha  6, col  5]
ID            a                     [linha  6, col 12]
PLUS          +                     [linha  6, col 14]
ID            b                     [linha  6, col 16]
SEMICOL       ;                     [linha  6, col 17]
RBRACE        }                     [linha  7, col  1]
...
```

---

## Testes Fornecidos

### teste1.c — Exemplo Simples
Funções básicas com inteiros e floats.

```bash
./lexer teste1.c
```

### teste2.c — Exemplo Completo
Código mais complexo com:
- Comentários de linha e bloco
- Strings literais
- Operadores relacionais
- Estruturas de controle (if, while)

```bash
./lexer teste2.c
```

### teste3_erros.c — Testes de Erro
Arquivo preparado para testar detecção de erros léxicos.
**IMPORTANTE:** Os erros estão comentados. Descomente uma linha por vez para testar.

```bash
./lexer teste3_erros.c
```

Para testar um erro específico, edite o arquivo e descomente uma das linhas marcadas.

---

## Criando Seus Próprios Testes

### Passo 1: Criar arquivo de teste

```bash
# Crie um novo arquivo .c
nano meu_teste.c
```

### Passo 2: Escrever código de teste

```c
int main() {
    int x = 42;
    float pi = 3.14;
    
    if (x > 0) {
        return x + 1;
    }
    
    return 0;
}
```

### Passo 3: Executar análise

```bash
./lexer meu_teste.c
```

---

## Entendendo a Saída

Cada token é mostrado com:

1. **TIPO:** Classificação do token (ID, int, PLUS, etc.)
2. **LEXEMA:** Texto literal do token
3. **POSIÇÃO:** Linha e coluna onde o token foi encontrado

**Exemplo:**
```
ID            contador              [linha  5, col 10]
```
- **Tipo:** `ID` (identificador)
- **Lexema:** `contador`
- **Posição:** Linha 5, coluna 10

---

## Detecção de Erros

O lexer detecta e reporta erros léxicos:

### Exemplos de Erros

```c
// 1. Caractere inválido
int $x = 10;
// ERRO: caractere '$' não pertence ao alfabeto

// 2. String não fechada
char msg[] = "texto incompleto;
// ERRO: string não fechada

// 3. Operador inválido
if (x ! 0) { }
// ERRO: '!' sozinho não é válido (use !=)

// 4. Comentário não fechado
/* comentário sem fechar
int x = 5;
// ERRO: comentário não fechado
```

**Formato da mensagem de erro:**
```
ERRO LÉXICO linha X, coluna Y: descrição do erro
```

---

## Estrutura do Código

### Principais Funções

```c
// Função pública principal
Token proximo_token(void);

// Inicialização
void inicializar_lexer(const char *codigo_fonte);

// Funções auxiliares (privadas)
static char avancar(void);
static char espiar(void);
static void pular_brancos(void);
static TokenType checar_reservada(const char *lexema);
```

### Fluxo de Execução

```
1. main()
   │
   ├─→ inicializar_lexer(codigo)
   │
   └─→ Loop:
       ├─→ proximo_token()
       │   ├─→ pular_brancos()
       │   ├─→ avancar()
       │   └─→ retorna Token
       │
       ├─→ imprimir_token()
       │
       └─→ até TOK_EOF
```

---

## Correspondência com os Slides

Este código implementa diretamente os conceitos apresentados nos slides:

### Slide 8: AFD Completo
O código em `proximo_token()` implementa o Autômato Finito Determinístico mostrado no slide. Cada bloco `if/switch` corresponde a uma transição do AFD.

```c
// Exemplo: Transição para identificadores
if (isalpha(c) || c == '_') {
    // Estado q0 → q1
    buf[i++] = c;
    
    // Loop em q1
    while (isalnum(espiar()) || espiar() == '_') {
        buf[i++] = avancar();
    }
    
    // q1 é estado de aceitação
    return criar_token(...);
}
```

### Slides 9-11: Código C Comentado
As funções auxiliares implementadas correspondem exatamente às descritas nos slides:
- `avancar()` — avança no buffer
- `espiar()` — lookahead
- `pular_brancos()` — ignora whitespace
- `checar_reservada()` — verifica palavras-chave

---

## Exercícios Sugeridos

### Nível 1: Básico
1. Adicione suporte para o operador `%` (módulo)
2. Adicione a palavra reservada `for`
3. Modifique para reconhecer números hexadecimais (0x...)

### Nível 2: Intermediário
4. Implemente suporte para caracteres literais: `'a'`, `'\n'`
5. Adicione detecção de números em notação científica: `1.5e-10`
6. Implemente uma tabela hash para palavras reservadas

### Nível 3: Avançado
7. Adicione suporte para caracteres de escape em strings: `\n`, `\"`, `\\`
8. Implemente um buffer circular para permitir múltiplos pushbacks
9. Adicione recuperação de erros (modo pânico)

---

## Referências

### Livros
- **Aho, Lam, Sethi, Ullman** — *Compiladores: Princípios, Técnicas e Ferramentas* (Livro do Dragão)
- **Appel, Andrew** — *Modern Compiler Implementation in C*
- **Cooper & Torczon** — *Engineering a Compiler*

### Online
- [Teoria de Autômatos](https://en.wikipedia.org/wiki/Finite-state_machine)
- [Expressões Regulares](https://en.wikipedia.org/wiki/Regular_expression)
- [Flex Manual](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_mono/flex.html)

---

## Notas Importantes

### Limitações Conhecidas
1. **Números negativos:** O lexer reconhece `-` como operador separado
   - Entrada: `-5` → Tokens: `MINUS`, `NUM_INT(5)`
   - Isso é resolvido na fase sintática
   
2. **Escape sequences:** Strings não processam `\n`, `\"`, etc.
   - Isso seria adicionado em um lexer completo

3. **Pushback limitado:** A função `retroceder()` só funciona para 1 caractere
   - Lexers reais usam buffer circular

4. **Encoding:** Assume ASCII/UTF-8 simples
   - Não trata caracteres multibyte adequadamente

### Por que essas limitações?
Este é um **lexer didático** focado em clareza pedagógica, não em completude. Um lexer de produção (como o do GCC) tem milhares de linhas e trata centenas de casos especiais.

---

## Solução de Problemas

### Erro: "command not found"
```bash
# Certifique-se de que compilou
gcc -o lexer lexer.c

# Execute com ./
./lexer
```

### Erro: "permission denied"
```bash
# Dê permissão de execução
chmod +x lexer
./lexer
```

### Arquivo não encontrado
```bash
# Certifique-se de que o arquivo existe
ls teste1.c

# Use caminho relativo ou absoluto
./lexer ./teste1.c
./lexer /caminho/completo/teste1.c
```

---


---

## Suporte

Para dúvidas sobre este material didático:
- Revise os comentários no código `lexer.c`
- Consulte os slides da apresentação
- Compare com a bibliografia recomendada

---

## Licença

Material didático para uso educacional em cursos de Ciências da Computação.

---

**Bons estudos! **
