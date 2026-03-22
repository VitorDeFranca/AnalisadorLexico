/***************************************************************************
 * ANALISADOR LÉXICO DIDÁTICO — Implementação Completa em C
 * 
 * Arquivo: lexer.c
 * Descrição: Analisador léxico simples para um subconjunto de C
 * Objetivo: Material didático para disciplina de Compiladores
 * 
 * Características:
 * - Reconhece identificadores, números inteiros e reais, strings
 * - Palavras reservadas: if, else, while, int, float, return
 * - Operadores: +, -, *, /, =, ==, !=, <, <=
 * - Delimitadores: ( ) { } ;
 * - Ignora espaços, tabs, quebras de linha
 * - Ignora comentários de linha (//) e bloco
 * 
 * Compilação:
 *   gcc -Wall -Wextra -std=c99 -o lexer lexer.c
 * 
 * Execução:
 *   ./lexer                    # usa código-fonte embutido
 *   ./lexer arquivo.c          # analisa arquivo externo
 * 
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/***************************************************************************
 * DEFINIÇÕES DE TIPOS
 ***************************************************************************/

/* ── Enumeração de todos os tipos de token ───────────────────────────
   Cada constante representa uma classe léxica que o analisador pode
   reconhecer. Esta lista deve ser sincronizada com o array 'nomes[]'.
*/
typedef enum {
    TOK_ID,         /* Identificador: variáveis, funções, etc. */
    TOK_NUM_INT,    /* Número inteiro literal: 42, 0, 123 */
    TOK_NUM_FLOAT,  /* Número real literal: 3.14, 0.5, 2.0 */
    TOK_STRING,     /* String literal: "hello world" */
    
    /* Palavras reservadas */
    TOK_IF,         /* if */
    TOK_ELSE,       /* else */
    TOK_WHILE,      /* while */
    TOK_INT,        /* int (tipo de dado) */
    TOK_FLOAT,      /* float (tipo de dado) */
    TOK_RETURN,     /* return */
    
    /* Operadores aritméticos */
    TOK_PLUS,       /* + */
    TOK_MINUS,      /* - */
    TOK_STAR,       /* * */
    TOK_SLASH,      /* / */
    
    /* Operadores relacionais e lógicos */
    TOK_EQ,         /* == (igualdade) */
    TOK_ASSIGN,     /* = (atribuição) */
    TOK_NEQ,        /* != (diferente) */
    TOK_LT,         /* < (menor que) */
    TOK_LE,         /* <= (menor ou igual) */
    TOK_GT,         /* > (maior que) */
    TOK_GE,         /* >= (maior ou igual) */
    
    /* Delimitadores */
    TOK_SEMICOL,    /* ; */
    TOK_COMMA,      /* , */
    TOK_LPAREN,     /* ( */
    TOK_RPAREN,     /* ) */
    TOK_LBRACE,     /* { */
    TOK_RBRACE,     /* } */
    
    /* Especiais */
    TOK_EOF,        /* Fim de arquivo */
    TOK_ERROR       /* Token inválido / erro léxico */
} TokenType;

/* ── Tamanho máximo do lexema ──────────────────────────────────────── */
#define MAX_LEXEME 256

/* ── Estrutura de um token ─────────────────────────────────────────── 
   Representa um token com seu tipo e o texto literal (lexema).
*/
typedef struct {
    TokenType tipo;
    char      lexema[MAX_LEXEME];
    int       linha;    /* linha onde o token foi encontrado */
    int       coluna;   /* coluna onde o token começa */
} Token;

/* ── Par palavra-reservada / tipo de token ───────────────────────────
   Usado para construir a tabela de palavras reservadas.
*/
typedef struct {
    const char *palavra;
    TokenType   tipo;
} Reservada;

/***************************************************************************
 * VARIÁVEIS GLOBAIS DO LEXER
 ***************************************************************************/

/* ── Ponteiro para o código-fonte ────────────────────────────────────
   Aponta para o caractere atual sendo analisado. À medida que o lexer
   avança, este ponteiro é incrementado.
*/
static const char *src = NULL;

/* ── Ponteiro para o início do código-fonte ──────────────────────────
   Mantido para poder calcular colunas e fazer retrocessos seguros.
*/
static const char *src_inicio = NULL;

/* ── Contador de linha atual ─────────────────────────────────────────
   Incrementado a cada '\n' encontrado. Começa em 1.
*/
static int linha_atual = 1;

/* ── Posição da coluna atual ─────────────────────────────────────────
   Distância desde o início da linha. Usado para reportar erros.
*/
static int coluna_atual = 1;

/* ── Tabela de palavras reservadas ───────────────────────────────────
   Array estático que mapeia strings para seus tipos de token.
   Terminado por sentinela {NULL, TOK_ID}.
*/
static Reservada reservadas[] = {
    {"if",     TOK_IF    },
    {"else",   TOK_ELSE  },
    {"while",  TOK_WHILE },
    {"int",    TOK_INT   },
    {"float",  TOK_FLOAT },
    {"return", TOK_RETURN},
    {NULL,     TOK_ID    }  /* sentinela: fim da tabela */
};

/* ── Nomes legíveis para cada tipo de token ──────────────────────────
   Usado para imprimir a saída em formato legível. A ordem DEVE
   corresponder exatamente à ordem do enum TokenType.
*/
static const char *nomes_tokens[] = {
    "ID", "NUM_INT", "NUM_FLOAT", "STRING",
    "if", "else", "while", "int", "float", "return",
    "PLUS", "MINUS", "STAR", "SLASH",
    "EQ", "ASSIGN", "NEQ", "LT", "LE", "GT", "GE",
    "SEMICOL", "COMMA", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
    "EOF", "ERROR"
};

/***************************************************************************
 * FUNÇÕES AUXILIARES DO LEXER
 ***************************************************************************/

/* ── Verifica se chegamos ao fim da entrada ──────────────────────────
   Retorna true se o ponteiro src está em '\0' (fim de string).
*/
static inline bool fim_de_arquivo(void) {
    return *src == '\0';
}

/* ── Retorna o caractere atual sem consumi-lo ────────────────────────
   Operação de "lookahead" — permite ver o próximo caractere sem
   avançar o ponteiro. Essencial para decidir entre '=' e '=='.
*/
static inline char espiar(void) {
    return *src;
}

/* ── Retorna o caractere seguinte sem consumir o atual ───────────────
   Lookahead de 2 caracteres. Usado para detectar comentários
*/
static inline char espiar_proximo(void) {
    if (fim_de_arquivo()) return '\0';
    return *(src + 1);
}

/* ── Consome e retorna o caractere atual ─────────────────────────────
   Avança o ponteiro src em 1 posição. Atualiza contadores de linha
   e coluna. Esta é a operação fundamental do lexer.
*/
static char avancar(void) {
    char c = *src;
    
    if (c == '\0') return c;  /* não avança além do fim */
    
    /* Atualiza contadores de posição */
    if (c == '\n') {
        linha_atual++;
        coluna_atual = 1;
    } else {
        coluna_atual++;
    }
    
    src++;  /* avança o ponteiro */
    return c;
}

/* ── Devolve um caractere ao buffer ──────────────────────────────────
   Operação de "pushback". Usado quando lemos um caractere a mais
   e precisamos devolvê-lo. Por exemplo, ao ler '=' seguido de 'x',
   devemos devolver o 'x' após reconhecer o token ASSIGN.
   
   IMPORTANTE: Esta implementação simples só funciona para 1 caractere.
   Lexers reais usam um buffer circular que permite múltiplos pushbacks.
*/
static void retroceder(void) {
    if (src > src_inicio) {
        src--;
        /* Ajusta contadores (simplificado — não trata \n corretamente) */
        if (coluna_atual > 1) {
            coluna_atual--;
        }
    }
}

/* ── Ignora espaços em branco e comentários ──────────────────────────
   Chamada antes de tentar reconhecer cada token. Consome:
   - Espaços, tabs, quebras de linha
   - Comentários de linha: // até o fim da linha
   - Comentários de bloco: delimitados por barra-asterisco
   
   Esta função é crucial para manter o lexer simples: as funções de
   reconhecimento de token não precisam se preocupar com whitespace.
*/
static void pular_brancos(void) {
    while (true) {
        char c = espiar();
        
        /* Espaços em branco */
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            avancar();
            continue;
        }
        
        /* Comentário de linha: // ... \n */
        if (c == '/' && espiar_proximo() == '/') {
            avancar();  /* consome / */
            avancar();  /* consome / */
            /* Avança até encontrar \n ou EOF */
            while (espiar() != '\n' && !fim_de_arquivo()) {
                avancar();
            }
            continue;
        }
        
        /* Comentário de bloco: delimitado por barra-asterisco */
        if (c == '/' && espiar_proximo() == '*') {
            avancar();  /* consome / */
            avancar();  /* consome * */
            /* Avança até encontrar fim do bloco */
            while (true) {
                if (fim_de_arquivo()) {
                    fprintf(stderr, "ERRO linha %d: comentário não fechado\n",
                            linha_atual);
                    break;
                }
                if (espiar() == '*' && espiar_proximo() == '/') {
                    avancar();  /* consome * */
                    avancar();  /* consome / */
                    break;
                }
                avancar();
            }
            continue;
        }
        
        /* Não é whitespace nem comentário: sair do loop */
        break;
    }
}

/* ── Verifica se um lexema é palavra reservada ───────────────────────
   Percorre a tabela 'reservadas[]' comparando strings.
   Retorna o tipo correspondente se encontrar, TOK_ID caso contrário.
   
   OTIMIZAÇÃO: Em lexers reais, usa-se uma hash table para O(1).
   Aqui usamos busca linear O(n) para simplicidade didática.
*/
static TokenType checar_reservada(const char *lexema) {
    for (int i = 0; reservadas[i].palavra != NULL; i++) {
        if (strcmp(lexema, reservadas[i].palavra) == 0) {
            return reservadas[i].tipo;
        }
    }
    return TOK_ID;  /* não é reservada: é identificador */
}

/* ── Cria um token com tipo e lexema especificados ───────────────────
   Função auxiliar para construir tokens. Preenche linha e coluna.
*/
static Token criar_token(TokenType tipo, const char *lexema) {
    Token tok;
    tok.tipo = tipo;
    strncpy(tok.lexema, lexema, MAX_LEXEME - 1);
    tok.lexema[MAX_LEXEME - 1] = '\0';  /* garante terminação */
    tok.linha = linha_atual;
    tok.coluna = coluna_atual;
    return tok;
}

/* ── Cria um token de erro com mensagem ──────────────────────────────*/
static Token token_erro(const char *mensagem) {
    fprintf(stderr, "ERRO LÉXICO linha %d, coluna %d: %s\n",
            linha_atual, coluna_atual, mensagem);
    return criar_token(TOK_ERROR, mensagem);
}

/***************************************************************************
 * FUNÇÃO PRINCIPAL: RECONHECIMENTO DE TOKENS
 ***************************************************************************/

/* ══════════════════════════════════════════════════════════════════════
   proximo_token()
   
   Retorna o próximo token da entrada. Esta é a função pública do lexer,
   chamada repetidamente pelo parser até retornar TOK_EOF.
   
   ALGORITMO:
   1. Ignora espaços e comentários
   2. Lê o primeiro caractere
   3. Baseado no caractere, decide qual estado do AFD entrar
   4. Consome caracteres adicionais conforme necessário
   5. Retorna o token reconhecido
   
   IMPLEMENTAÇÃO DO AFD:
   O código abaixo é uma implementação direta do Autômato Finito
   Determinístico (AFD) apresentado nos slides. Cada ramificação
   do if/switch corresponde a uma transição do AFD.
   ══════════════════════════════════════════════════════════════════════
*/
Token proximo_token(void) {
    char buf[MAX_LEXEME];
    int i = 0;
    char c;
    
    /* Passo 1: Ignora whitespace e comentários */
    pular_brancos();
    
    /* Salva posição para reportar erros */
    int linha_token = linha_atual;
    int coluna_token = coluna_atual;
    
    /* Passo 2: Lê primeiro caractere */
    c = avancar();
    
    /* ─────────────────────────────────────────────────────────────────
       CASO 1: FIM DE ARQUIVO
       Se o caractere é '\0', retornamos TOK_EOF.
    */
    if (c == '\0') {
        return criar_token(TOK_EOF, "<EOF>");
    }
    
    /* ─────────────────────────────────────────────────────────────────
       CASO 2: IDENTIFICADORES E PALAVRAS RESERVADAS
       
       REGRA: [a-zA-Z_][a-zA-Z0-9_]*
       
       AFD:
         q0 --[letra|_]--> q1 (aceitação)
         q1 --[letra|dígito|_]--> q1 (loop)
       
       Ao encontrar letra ou '_', entramos no estado q1 e consumimos
       todos os caracteres alfanuméricos seguintes. Ao final, verificamos
       se o lexema é palavra reservada.
    */
    if (isalpha(c) || c == '_') {
        buf[i++] = c;  /* armazena primeiro caractere */
        
        /* Loop de q1: consome enquanto for letra, dígito ou underscore */
        while ((isalnum(espiar()) || espiar() == '_') && i < MAX_LEXEME - 1) {
            buf[i++] = avancar();
        }
        
        buf[i] = '\0';  /* termina a string */
        
        /* Verifica se é palavra reservada */
        TokenType tipo = checar_reservada(buf);
        Token tok = criar_token(tipo, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* ─────────────────────────────────────────────────────────────────
       CASO 3: NÚMEROS (INTEIROS E REAIS)
       
       REGRAS:
       - Inteiro: [0-9]+
       - Real:    [0-9]+\.[0-9]+
       
       AFD:
         q0 --[dígito]--> q2 (aceitação para inteiro)
         q2 --[dígito]--> q2 (loop)
         q2 --[.]--> q3
         q3 --[dígito]--> q4 (aceitação para real)
         q4 --[dígito]--> q4 (loop)
       
       Primeiro consumimos dígitos. Se encontrarmos '.', continuamos
       consumindo dígitos e reconhecemos como FLOAT. Caso contrário,
       reconhecemos como INT.
    */
    if (isdigit(c)) {
        buf[i++] = c;
        
        /* Consome parte inteira */
        while (isdigit(espiar()) && i < MAX_LEXEME - 1) {
            buf[i++] = avancar();
        }
        
        /* Verifica se há parte fracionária */
        if (espiar() == '.' && isdigit(espiar_proximo())) {
            buf[i++] = avancar();  /* consome o '.' */
            
            /* Consome parte fracionária */
            while (isdigit(espiar()) && i < MAX_LEXEME - 1) {
                buf[i++] = avancar();
            }
            
            buf[i] = '\0';
            Token tok = criar_token(TOK_NUM_FLOAT, buf);
            tok.linha = linha_token;
            tok.coluna = coluna_token;
            return tok;
        }
        
        /* Apenas inteiro */
        buf[i] = '\0';
        Token tok = criar_token(TOK_NUM_INT, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* ─────────────────────────────────────────────────────────────────
       CASO 4: STRINGS LITERAIS
       
       REGRA: "[^"]*"
       
       Consome todos os caracteres até encontrar '"' de fechamento.
       Não armazena as aspas no lexema (apenas o conteúdo).
       
       SIMPLIFICAÇÃO: Não trata caracteres de escape (\n, \", etc.)
    */
    if (c == '"') {
        /* Consome até " de fechamento */
        while (espiar() != '"' && !fim_de_arquivo() && i < MAX_LEXEME - 1) {
            buf[i++] = avancar();
        }
        
        if (espiar() != '"') {
            return token_erro("string não fechada");
        }
        
        avancar();  /* consome " de fechamento */
        buf[i] = '\0';
        Token tok = criar_token(TOK_STRING, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* ─────────────────────────────────────────────────────────────────
       CASO 5: OPERADORES E DELIMITADORES
       
       Aqui tratamos todos os símbolos de um ou dois caracteres.
       Para operadores de dois caracteres (==, !=, <=, >=), usamos
       LOOKAHEAD para decidir qual token reconhecer.
       
       AFD (exemplo para '='):
         q0 --[=]--> qEQ (estado intermediário)
         qEQ --[=]--> qEQUAL (aceitação: ==)
         qEQ --[outros]--> qASSIGN (aceitação: =)
       
       A implementação usa espiar() para o lookahead e retroceder()
       se necessário (embora neste caso, não retrocedemos pois cada
       branch do switch já consome corretamente).
    */
    
    /* Operador '=' ou '==' */
    if (c == '=') {
        buf[0] = c;
        if (espiar() == '=') {
            buf[1] = avancar();
            buf[2] = '\0';
            Token tok = criar_token(TOK_EQ, buf);
            tok.linha = linha_token;
            tok.coluna = coluna_token;
            return tok;
        }
        buf[1] = '\0';
        Token tok = criar_token(TOK_ASSIGN, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* Operador '!' ou '!=' */
    if (c == '!') {
        buf[0] = c;
        if (espiar() == '=') {
            buf[1] = avancar();
            buf[2] = '\0';
            Token tok = criar_token(TOK_NEQ, buf);
            tok.linha = linha_token;
            tok.coluna = coluna_token;
            return tok;
        }
        /* '!' sozinho não é válido nesta linguagem */
        buf[1] = '\0';
        return token_erro("caractere inesperado '!'");
    }
    
    /* Operador '<' ou '<=' */
    if (c == '<') {
        buf[0] = c;
        if (espiar() == '=') {
            buf[1] = avancar();
            buf[2] = '\0';
            Token tok = criar_token(TOK_LE, buf);
            tok.linha = linha_token;
            tok.coluna = coluna_token;
            return tok;
        }
        buf[1] = '\0';
        Token tok = criar_token(TOK_LT, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* Operador '>' ou '>=' */
    if (c == '>') {
        buf[0] = c;
        if (espiar() == '=') {
            buf[1] = avancar();
            buf[2] = '\0';
            Token tok = criar_token(TOK_GE, buf);
            tok.linha = linha_token;
            tok.coluna = coluna_token;
            return tok;
        }
        buf[1] = '\0';
        Token tok = criar_token(TOK_GT, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* Operadores aritméticos simples (um caractere) */
    if (c == '+') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_PLUS, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == '-') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_MINUS, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == '*') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_STAR, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == '/') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_SLASH, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* Delimitadores */
    if (c == ';') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_SEMICOL, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == ',') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_COMMA, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == '(') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_LPAREN, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == ')') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_RPAREN, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == '{') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_LBRACE, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    if (c == '}') {
        buf[0] = c; buf[1] = '\0';
        Token tok = criar_token(TOK_RBRACE, buf);
        tok.linha = linha_token;
        tok.coluna = coluna_token;
        return tok;
    }
    
    /* ─────────────────────────────────────────────────────────────────
       CASO 6: CARACTERE INVÁLIDO
       
       Se chegamos aqui, o caractere não pertence a nenhum padrão
       reconhecido. Geramos um token de erro.
    */
    buf[0] = c;
    buf[1] = '\0';
    return token_erro(buf);
}

/***************************************************************************
 * FUNÇÃO AUXILIAR: INICIALIZAR O LEXER
 ***************************************************************************/

/* ── Inicializa o lexer com um código-fonte ──────────────────────────
   Deve ser chamada antes de começar a análise.
*/
void inicializar_lexer(const char *codigo_fonte) {
    src = codigo_fonte;
    src_inicio = codigo_fonte;
    linha_atual = 1;
    coluna_atual = 1;
}

/***************************************************************************
 * FUNÇÃO PRINCIPAL: TESTES
 ***************************************************************************/

/* ── Imprime um token formatado ──────────────────────────────────────*/
void imprimir_token(Token tok) {
    printf("%-12s  %-20s  [linha %2d, col %2d]\n",
           nomes_tokens[tok.tipo],
           tok.lexema,
           tok.linha,
           tok.coluna);
}

/* ── Lê arquivo para string ──────────────────────────────────────────*/
char* ler_arquivo(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo '%s'\n", nome_arquivo);
        return NULL;
    }
    
    /* Descobre tamanho do arquivo */
    fseek(f, 0, SEEK_END);
    long tamanho = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    /* Aloca buffer */
    char *conteudo = malloc(tamanho + 1);
    if (!conteudo) {
        fprintf(stderr, "Erro de alocação de memória\n");
        fclose(f);
        return NULL;
    }
    
    /* Lê arquivo */
    fread(conteudo, 1, tamanho, f);
    conteudo[tamanho] = '\0';
    fclose(f);
    
    return conteudo;
}

/* ── Função main: testa o lexer ──────────────────────────────────────
   Aceita dois modos:
   1. Sem argumentos: usa código-fonte embutido (exemplo didático)
   2. Com argumento: lê arquivo especificado
*/
int main(int argc, char *argv[]) {
    const char *codigo_exemplo =
        "/* Exemplo de código em C */\n"
        "int fatorial(int n) {\n"
        "    if (n <= 1) {\n"
        "        return 1;  // caso base\n"
        "    } else {\n"
        "        return n * fatorial(n - 1);\n"
        "    }\n"
        "}\n"
        "\n"
        "int main() {\n"
        "    int x = 5;\n"
        "    float pi = 3.14159;\n"
        "    char msg[] = \"Resultado: \";\n"
        "    \n"
        "    while (x >= 0) {\n"
        "        x = x - 1;\n"
        "    }\n"
        "    \n"
        "    // Operadores relacionais\n"
        "    if (x == 0 || x != -1) {\n"
        "        return 0;\n"
        "    }\n"
        "}\n";
    
    const char *codigo = codigo_exemplo;
    bool deve_liberar = false;
    
    /* Se foi passado argumento, lê arquivo */
    if (argc > 1) {
        codigo = ler_arquivo(argv[1]);
        if (!codigo) {
            return 1;
        }
        deve_liberar = true;
        printf("═══════════════════════════════════════════════════════════\n");
        printf(" ANÁLISE LÉXICA DO ARQUIVO: %s\n", argv[1]);
        printf("═══════════════════════════════════════════════════════════\n\n");
    } else {
        printf("═══════════════════════════════════════════════════════════\n");
        printf(" ANÁLISE LÉXICA — Código de Exemplo Embutido\n");
        printf("═══════════════════════════════════════════════════════════\n\n");
        printf("CÓDIGO-FONTE:\n");
        printf("───────────────────────────────────────────────────────────\n");
        printf("%s", codigo);
        printf("───────────────────────────────────────────────────────────\n\n");
    }
    
    /* Inicializa o lexer */
    inicializar_lexer(codigo);
    
    /* Cabeçalho da tabela de tokens */
    printf("TOKENS RECONHECIDOS:\n");
    printf("───────────────────────────────────────────────────────────\n");
    printf("%-12s  %-20s  %s\n", "TIPO", "LEXEMA", "POSIÇÃO");
    printf("%-12s  %-20s  %s\n", "────────────", "────────────────────", "─────────────");
    
    /* Loop principal: extrai tokens até EOF ou ERROR */
    Token tok;
    int contador = 0;
    do {
        tok = proximo_token();
        imprimir_token(tok);
        contador++;
        
        /* Limite de segurança para evitar loop infinito em caso de bug */
        if (contador > 10000) {
            fprintf(stderr, "\nAVISO: Limite de 10000 tokens atingido. Abortando.\n");
            break;
        }
    } while (tok.tipo != TOK_EOF && tok.tipo != TOK_ERROR);
    
    printf("───────────────────────────────────────────────────────────\n");
    printf("Total de tokens: %d\n", contador);
    printf("═══════════════════════════════════════════════════════════\n");
    
    /* Libera memória se foi alocada */
    if (deve_liberar) {
        free((char*)codigo);
    }
    
    return (tok.tipo == TOK_ERROR) ? 1 : 0;
}
