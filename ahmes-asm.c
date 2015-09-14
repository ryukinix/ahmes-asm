/*
 * =====================================================================================
 *
 *                      Copyright 2015 Manoel Vilela
 *
 *
 *       Filename: ahmes-asm.c
 *
 *    Description: Trabalho da disciplina Arquitetura e Organização de Computadores
 *                 Lecionada pelo professor Alexandre Beletti
 *           
 *           Data: 12/04/2015 (última modificação antes de entregar)
 *
 *         Author: Manoel Vilela
 *        Contact: manoel_vilela@engineer.com
 *   Organization: Universidade Federal do Pará
 *
 * =====================================================================================
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"

// macros
#define SPACE 0x20
#define ISA_LENGHT 24 // tamanho do set de instrunções da máquina hipotética
#define MAX_BUFFER 5 // tamanho de buffer maximo para uma mnemonico ou operando
#define MAX_LINE 200 // tamanho do buffer maximo para uma linha de comando
#define MAX_MEMORY 512 // memoria total do arquivo binário (ahmes*2)
#define string_to_num(string, base)\
    (unsigned int)strtol(string, NULL, base)

/* ======================================================================================
*   [Metainfo]   
*
*   Escolhido unsigned char como tipo de muitas funções e memória
*   devido a propriedade de ser 1 byte no intervalo 0..255
*   Do qual escolho por convenção de me auxiliar na hora de escrever no arquivo binário.
*   A maioria das funções possui comentários que definem seu propósito. 
*   Operações intuitivas ou não tão importantes, foram inferidos.
*
*=========================================================================================*/

// Variável global para controlar o argumento -d | --debug
unsigned char debug_control = 0;

// Aqui está definida a tabela de de mnemonicos:opcodes como um vetor de chars.
const char table[ISA_LENGHT][2][25] = dictionary;

// mnem -> opcode
int translate_mnem(char* mnem){
    int i;
    for(i = 0; i < ISA_LENGHT; i++)
        if (strcmp(mnem, table[i][0]) == 0)
            return string_to_num(table[i][1], 16);
    return -1;
}

// "82h" ou "82d" -> unsigned char
int base_operand(char* operand){
    int last = strlen(operand) - 1;
    int base;
    switch(operand[last]){
        case 'h': case 'H':
            base = 16;
            break;
        case 'd': case 'D':
            base = 10;
            break;
        default:
            return -1;
            break;
    }
    return string_to_num(operand, base);
}

// dependendo do contador de instrunção para uma linha, tentar traduzir mnemonico ou endereço
// considera-se que o primeiro argumento é operador, resto operandos
int translate_key(char* token, int instr_count){
    int translated;
    if(instr_count == 0)
        translated = translate_mnem(token);
    else
        translated = base_operand(token);

    if(translated != -1)
        return translated;
    else
        return -1;
}

// corrigir quantidade de espaços entre operandos (evitar erros de sintaxe do usuário)
void fix_spaces(char* string){
    char new_string[MAX_LINE];
    int i, j = 0;
    int space_count = 0;
    for(i = 0; string[i] != '\0'; i++){
        if (string[i] == SPACE)
            space_count++;
        if (space_count > 1 && string[i] == SPACE)
            continue;
        else{
            space_count = 0; // zerar apenas o espaço entre entre instrunções
            new_string[j] = string[i];
            j += 1;
        }
    }

    for(i = 0; i < MAX_BUFFER; i++)
        string[i] = '\0';
    for(i = 0; i < j; i++)
        string[i] = new_string[i];
    string[i] = '\0';

}

// função extra: instrunção SET para inserir valores diretos na memoria do ahmes
void set_instrunction(unsigned char* memory, const char* line){
    unsigned char translated;
    int i, index_instr = 0;
    int value = 0, end = 0, instr_count = 0;
    char instr[MAX_BUFFER];
    for(i = 0; line[i] != '\0'; i++){
        // operadores delimitados por espaços até próximo newline
        if (line[i] == SPACE || line[i] == '\n'){
            instr[index_instr] = '\0';
            translated = translate_key(instr, instr_count);
            // primeira instrunção de SET, endereço. Segunda é o valor. Assim como algumas instrunções CISC.
            if (instr_count == 1)
                end = translated;
            else if (instr_count == 2)
                value = translated;
            instr_count++;
            index_instr = 0;
        }
        else{
            instr[index_instr] = line[i];
            index_instr++;
        }
    }
    if (debug_control)
        printf("#SET_INSTRUNCTION\nAdress: %dd -> Value: %dd!\n", end, value);
    memory[end*2] = value;
}


// traduzir uma linha de código para a memoria
int translate_to_memory(unsigned char* memory, int* index_mem, char* line, int line_count){
    int translated;
    int index_line = 0;
    int index_token = 0;
    int instr_count = 0;

    char token[MAX_BUFFER];

    // corrigir quantidade de espaços
    fix_spaces(line);

    if (debug_control)
        printf("\nLine %d: %s\n", line_count, line);
    
    // percorrer linha
    // se foi possível traduzir, acrescentar na memória, do contário, return -1.
    while(line[index_line] != '\0'){
        if (line[index_line] == SPACE || line[index_line] == '\n'){
            // finalizar string \/.
            token[index_token] = '\0';
            translated = translate_key(token, instr_count);
            // função especial SET: inserir dado na memória e pular linha.
            if (strcmp(token, "SET") == 0){
                set_instrunction(memory, line);
                return 0;
            }   
            else if (translated != -1){ // acrescentar na memoria
                memory[*index_mem] = (unsigned char) translated;
                if (debug_control)
                    printf("#TRANSLATING\n%s -> %dd \n", token, translated);
            }
            else { // retornar error e finalizar
                printf("***Syntax Error %d:%d:\n-> %s\n",\
                    line_count + 1, index_line, line);
                exit(-1);
            }
            index_token = 0;
            instr_count++;
            *index_mem = *index_mem + 2;
        }
        else{
            token[index_token] = line[index_line];
            index_token++;
        }
        index_line++;
    }
    return 0;
}

// traduzir código-fonte ahmes
// Recebe uma memoria(vetor) e o nome do codigo-fonte
int translate_file(unsigned char* memory, const char* file_input){
    int index_mem = 0;
    int line_count = 0;
    char line[MAX_LINE];

    FILE *f;
    f = fopen(file_input, "r");

    // somente executar se conseguir abrir o arquivo, 
    // se não for possível abrir arquivo, sair e retornar erro.
    if(f != NULL){
        // 'puxar' linhas do arquivo enquanto não retornar null
        for(line_count = 0; fgets(line, sizeof(line), f) != NULL; line_count++) {
            if (line[0] != '\n' && line[0] != '#')
                translate_to_memory(memory, &index_mem, line, line_count);
        }
    }
    else {
        printf("No such file o archive with name %s\n", file_input);
        exit(1);
    }

    fclose(f);
    return 0;
}

// a partir do código traduzido, gravar em disco.
void write_bin_file(unsigned char* memory, char* file_output){
    FILE* f;
    f = fopen(file_output, "wb");
    char head_of_file[] = header;

    // header (.AHM)
    fwrite(head_of_file, 1, sizeof(head_of_file), f);

    // code (Todas as instrunções)
    fwrite(memory, 1, MAX_MEMORY, f);

    // fechar arquivo
    fclose(f);
}

// gerar nome genérico de saída com base no nome de entrada.
void output_name(char* output, char* input_name){
    int i;
    for(i = 0; input_name[i] != '.' && input_name[i] != '\0'; i++)
        output[i] = input_name[i];
    // finalizar nome por segurança
    output[i] = '\0';
    strcat(output, ".mem");
}

int main(int argc, char *argv[]){
    // alocar memoria do ahmes.
    unsigned char* memory;
    memory = (unsigned char*) calloc(MAX_MEMORY, (sizeof(unsigned char))); // alocando memoria ahmes (256*2 blocos)

    char* output;
    // alocar nome output se existe algum argumento para receber como
    if (argc > 1){
        output = (char*) calloc(sizeof(argv[1])*sizeof(char) + 1, sizeof(char));
        output_name(output, argv[1]);
    }
    // CLI(Command-Line-Interface)
    int i;
    for(i = 0; i < argc; i++){
        // se há algum argumento -h ou --help, ou não há argumentos, imprimir ajuda e sair.
        if(argc == 1 || strcmp(argv[i], "-h") == 0 ||  strcmp(argv[i], "/?") == 0  || strcmp(argv[i], "--help") == 0){
            printf(HELP);
            exit(1);
        }
        // modo debug --
        // imprime algumas saidas do processamento de tradução para 
        // auxílio de debug e verificação de operações
        if (strcmp(argv[i], "-d") == 0 || strcmp (argv[i], "--debug") == 0)
            debug_control = 1;
        // especifica arquivo de saída, do contrário, usar padrão.
        if (strcmp(argv[i], "-o") == 0 || strcmp (argv[i], "--output") == 0){
            strcpy(output, argv[i + 1]);
        }
    }

    // traduzir arquivo para memória
    translate_file(memory, argv[1]);
    // escrever memória traduzida para o arquivo de saída
    write_bin_file(memory, output);

    if (debug_control)
        printf("Success! Output file translated: %s\n", output);


    // disponibilizar memória
    free(memory);
    // apontar memory para o nada (medida de segurança)
    
    memory=NULL;
    return 0;
}
