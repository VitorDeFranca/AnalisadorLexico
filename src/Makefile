# ═══════════════════════════════════════════════════════════
# Makefile para o Analisador Léxico
# Disciplina: Compiladores
# ═══════════════════════════════════════════════════════════

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
CFLAGS_DEBUG = -Wall -Wextra -std=c99 -g -DDEBUG

# Arquivos
TARGET = lexer
SOURCE = lexer.c
TESTS = teste1.c teste2.c teste3_erros.c

# Regra padrão
all: $(TARGET)
	@echo "✅ Compilação concluída com sucesso!"
	@echo "Execute: ./$(TARGET)"
	@echo "Ou:      ./$(TARGET) arquivo.c"

# Compilação do lexer
$(TARGET): $(SOURCE)
	@echo "🔨 Compilando $(SOURCE)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

# Compilação com símbolos de debug
debug: $(SOURCE)
	@echo "🔨 Compilando em modo debug..."
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET) $(SOURCE)
	@echo "✅ Versão debug pronta"
	@echo "Execute com: gdb ./$(TARGET)"

# Teste rápido com código embutido
test: $(TARGET)
	@echo "════════════════════════════════════════════════════════════"
	@echo " TESTE RÁPIDO: Código Embutido"
	@echo "════════════════════════════════════════════════════════════"
	@./$(TARGET) | head -50

# Testa todos os arquivos de teste
test-all: $(TARGET)
	@echo "════════════════════════════════════════════════════════════"
	@echo " EXECUTANDO TODOS OS TESTES"
	@echo "════════════════════════════════════════════════════════════"
	@for file in $(TESTS); do \
		if [ -f $$file ]; then \
			echo ""; \
			echo "▶ Testando $$file:"; \
			echo "────────────────────────────────────────────────────────────"; \
			./$(TARGET) $$file | head -30; \
			echo ""; \
		fi; \
	done

# Teste individual
test1: $(TARGET) teste1.c
	@echo "▶ Analisando teste1.c (funções simples):"
	@./$(TARGET) teste1.c

test2: $(TARGET) teste2.c
	@echo "▶ Analisando teste2.c (comentários e strings):"
	@./$(TARGET) teste2.c

test3: $(TARGET) teste3_erros.c
	@echo "▶ Analisando teste3_erros.c (casos de erro):"
	@./$(TARGET) teste3_erros.c

# Demonstração interativa
demo: $(TARGET)
	@if [ -f demo.sh ]; then \
		chmod +x demo.sh; \
		./demo.sh; \
	else \
		echo "❌ demo.sh não encontrado"; \
	fi

# Verifica sintaxe sem compilar
check:
	@echo "🔍 Verificando sintaxe..."
	$(CC) $(CFLAGS) -fsyntax-only $(SOURCE)
	@echo "✅ Sintaxe OK"

# Análise estática com lint
lint:
	@echo "🔍 Executando análise estática..."
	@which splint > /dev/null 2>&1 && splint $(SOURCE) || echo "⚠️  splint não instalado"
	@which cppcheck > /dev/null 2>&1 && cppcheck --enable=all $(SOURCE) || echo "⚠️  cppcheck não instalado"

# Benchmark simples
benchmark: $(TARGET)
	@echo "⏱️  Executando benchmark..."
	@time -p ./$(TARGET) > /dev/null

# Limpeza
clean:
	@echo "🧹 Limpando arquivos gerados..."
	rm -f $(TARGET)
	rm -f *.o
	rm -f core
	rm -f vgcore.*
	@echo "✅ Limpeza concluída"

# Limpeza completa (inclui backups)
distclean: clean
	@echo "🧹 Limpeza completa..."
	rm -f *~
	rm -f *.bak
	@echo "✅ Limpeza completa concluída"

# Ajuda
help:
	@echo "════════════════════════════════════════════════════════════"
	@echo " MAKEFILE DO ANALISADOR LÉXICO"
	@echo "════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo ""
	@echo "  make              - Compila o lexer (padrão)"
	@echo "  make debug        - Compila com símbolos de debug"
	@echo "  make test         - Teste rápido com código embutido"
	@echo "  make test-all     - Testa todos os arquivos .c"
	@echo "  make test1        - Testa apenas teste1.c"
	@echo "  make test2        - Testa apenas teste2.c"
	@echo "  make test3        - Testa apenas teste3_erros.c"
	@echo "  make demo         - Demonstração interativa"
	@echo "  make check        - Verifica sintaxe"
	@echo "  make lint         - Análise estática"
	@echo "  make benchmark    - Mede tempo de execução"
	@echo "  make clean        - Remove arquivos compilados"
	@echo "  make distclean    - Limpeza completa"
	@echo "  make help         - Mostra esta ajuda"
	@echo ""
	@echo "Exemplos:"
	@echo "  make && ./lexer              # Compila e executa"
	@echo "  make test1                   # Compila e testa arquivo 1"
	@echo "  make clean && make test-all  # Recompila e testa tudo"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"

# Declaração de alvos que não são arquivos
.PHONY: all debug test test-all test1 test2 test3 demo check lint benchmark clean distclean help
