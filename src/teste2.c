/* ============================================================
   TESTE 2: Exemplo com comentários, strings e operadores
   ============================================================ */

// Esta é uma função de busca
int buscar(int arr[], int tamanho, int valor) {
    int i = 0;
    
    /* Loop principal de busca */
    while (i < tamanho) {
        if (arr[i] == valor) {
            return i;  // encontrado!
        }
        i = i + 1;
    }
    
    return -1;  /* não encontrado */
}

int main() {
    int numeros[] = {10, 20, 30, 40, 50};
    char mensagem[] = "Teste de strings";
    float temperatura = 25.5;
    
    // Testa operadores relacionais
    if (temperatura >= 25.0 && temperatura <= 30.0) {
        // temperatura confortável
    }
    
    int resultado = buscar(numeros, 5, 30);
    
    if (resultado != -1) {
        // valor encontrado
    } else {
        // valor não encontrado
    }
    
    return 0;
}
