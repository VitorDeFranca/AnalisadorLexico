#!/bin/bash
# ═══════════════════════════════════════════════════════════
# Script de Demonstração do Analisador Léxico
# ═══════════════════════════════════════════════════════════

echo "╔════════════════════════════════════════════════════════════╗"
echo "║     DEMONSTRAÇÃO DO ANALISADOR LÉXICO EM C                 ║"
echo "║     Disciplina: Compiladores                               ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Verifica se o lexer está compilado
if [ ! -f "lexer" ]; then
    echo "⚠️  Lexer não encontrado. Compilando..."
    gcc -Wall -Wextra -std=c99 -o lexer lexer.c 2>&1 | grep -v "warning: 'retroceder'"
    
    if [ ! -f "lexer" ]; then
        echo "❌ Erro na compilação!"
        exit 1
    fi
    echo "✅ Compilação bem-sucedida!"
    echo ""
fi

# Função para pausar entre testes
pausar() {
    echo ""
    echo "Pressione ENTER para continuar..."
    read
    echo ""
}

# ═══════════════════════════════════════════════════════════
# TESTE 1: Código embutido
# ═══════════════════════════════════════════════════════════
echo "════════════════════════════════════════════════════════════"
echo "  TESTE 1: Análise do código embutido (fatorial)"
echo "════════════════════════════════════════════════════════════"
echo ""

./lexer | head -45

pausar

# ═══════════════════════════════════════════════════════════
# TESTE 2: Arquivo simples
# ═══════════════════════════════════════════════════════════
if [ -f "teste1.c" ]; then
    echo "════════════════════════════════════════════════════════════"
    echo "  TESTE 2: Análise de teste1.c (funções simples)"
    echo "════════════════════════════════════════════════════════════"
    echo ""
    
    echo "▶ Conteúdo do arquivo:"
    echo "────────────────────────────────────────────────────────────"
    cat teste1.c
    echo "────────────────────────────────────────────────────────────"
    echo ""
    
    echo "▶ Tokens extraídos:"
    echo ""
    ./lexer teste1.c
    
    pausar
fi

# ═══════════════════════════════════════════════════════════
# TESTE 3: Arquivo complexo
# ═══════════════════════════════════════════════════════════
if [ -f "teste2.c" ]; then
    echo "════════════════════════════════════════════════════════════"
    echo "  TESTE 3: Análise de teste2.c (comentários e strings)"
    echo "════════════════════════════════════════════════════════════"
    echo ""
    
    echo "▶ Características do teste:"
    echo "  • Comentários de linha (//) e bloco (/* */)"
    echo "  • Strings literais"
    echo "  • Operadores relacionais"
    echo "  • Estruturas de controle"
    echo ""
    
    ./lexer teste2.c | head -50
    
    pausar
fi

# ═══════════════════════════════════════════════════════════
# RESUMO
# ═══════════════════════════════════════════════════════════
echo "════════════════════════════════════════════════════════════"
echo "  RESUMO DA DEMONSTRAÇÃO"
echo "════════════════════════════════════════════════════════════"
echo ""
echo "✅ O analisador léxico foi capaz de:"
echo "   • Reconhecer identificadores e palavras reservadas"
echo "   • Distinguir números inteiros e reais"
echo "   • Processar strings literais"
echo "   • Identificar operadores (aritméticos e relacionais)"
echo "   • Detectar delimitadores (, ), {, }, ;"
echo "   • Ignorar espaços em branco"
echo "   • Ignorar comentários de linha e bloco"
echo "   • Reportar linha e coluna de cada token"
echo ""
echo "📚 Correspondência com os slides:"
echo "   • Implementa o AFD apresentado no Slide 8"
echo "   • Código comentado conforme Slides 9-11"
echo "   • Usa as técnicas descritas na apresentação"
echo ""
echo "🎓 Próximos passos sugeridos:"
echo "   1. Adicionar novos tipos de token (%, for, etc.)"
echo "   2. Implementar tratamento de erros mais robusto"
echo "   3. Adicionar suporte para números hexadecimais"
echo "   4. Tratar caracteres de escape em strings"
echo ""
echo "════════════════════════════════════════════════════════════"
echo ""
echo "Para mais informações, consulte README.md"
echo ""
