int main() {
    // Esta é uma linha de comentário (line comment)
    int idade = 25;

    /* Comentário de bloco:
       verifica se a idade é maior ou igual a 18 */
    if (idade >= 18) {
        char *mensagem = "Você é maior de idade."; // string literal
        printf("%s\n", mensagem);
    }

    // Loop de exemplo para testar a nova palavra reservada for
    for (int i = 0; i < 5; i++) {
        printf("Contador: %d\n", i);
    }

    return 0;
}