//mnemônicos: opcodes
//A tabela será uma matriz de chars com 25 instrunções por 2: mnemonico, opcode.

#define NOP "NOP", "0x0"
#define STA "STA", "0x10"
#define LDA "LDA", "0x20"
#define ADD "ADD", "0x30"
#define OR  "OR", "0x40"
#define AND "AND", "0x50"
#define NOT "NOT", "0x60"
#define SUB "SUB", "0x70"
#define JMP "JMP", "0x80"
#define JN "JN", "0x90"
#define JP "JP", "0x94"
#define JV "JV", "0X98"
#define JNV "JNV", "0x9C"
#define JZ "JZ", "0xA0"
#define JNZ "JNZ", "0xA4"
#define JC "JC", "0xB0"
#define JNC "JNC", "0xB4"
#define JB "JB", "0xB8"
#define JNB "JNB", "0xBC"
#define SHR "SHR", "0xE0"
#define SHL "SHL", "0xE1"
#define ROR "ROR", "0xE2"
#define ROL "ROL", "0xE3"
#define HLT "HLT", "0xF0"
//cabeçalho
#define header {0x03, 0x41, 0x48, 0x4D}

//dicionario macro que contém um mnemonico associado ao seu opcode
#define dictionary { NOP, STA, LDA, ADD, OR, AND, NOT, SUB, JMP, JN, JP, JV, JNV, JZ, JNZ, JC, JNC, JB, JNB, SHR, SHL, ROR, ROL, HLT }

#define HELP "Author: Manoel Vilela | Date: 12/04/2015\nEng. Comp [7DE] - UFPA - CAMTUC \n\nTranslator: Ahmes Instrunctions -> Opcodes.\n\nDescription:\nProcess text file and generate binary archive where is possibile to load in Ahmes software.\n\n---->How to use:\n\nExample1:\n\n>>>%s code_input.ahm\n->Output: code_input.mem \n\nExample2:\n\n>>>%s code_input.ahm -o output_name.mem\n->Output: output_name.mem\n\n Arguments for the program:\n\n\t-o | --output <file> \t Especific the name of output file\n\t-d | --debug \t\t Print some operations of translation\n", argv[0], argv[0]
