# Ahmes Asssembler

Montador `assembler` para a arquitetura (ISA) da máquina hipotética [ahmes](http://inf.ufrgs.br/~vbuaraujo/sw/neander/ahmes.html) proposta pelos professores da [UFRGS](http://www.ufrgs.br/) Raul Fernando Weber e Taisy Silva Webe.

## Compiling && Usage

  * Compilar:
    * `make`
  * Usar:
    * `./ahmes-asm.out <código-fonte>.txt`
  * Debug:
    * `./ahmes-asm.out <código-fonte>.txt --debug`

Um exemplo de código-fonte com mnemônicos está neste repositório. Uma operação de [multiplicação](multiplication-test.txt). Use sempre _line-endings_ _Unix_: `\n`.

## ISA Ahmes

Segue um resumo do conjunto de instrunções de arquitetura (ISA) da máquina Ahmes descrito na sua [wiki](https://pt.wikipedia.org/wiki/M%C3%A1quinas_hipot%C3%A9ticas_da_Universidade_Federal_do_Rio_Grande_do_Sul):

Código Binário |Instrunção | Descrição da operação                        | Comentário
---------------|-----------|----------------------------------|-------------------------------------------
  0000 0000    |  NOP      | nenhuma operação                 | nenhuma operação
  0001 0000    |  STA end  | MEM(end) ← AC                    | armazena acumulador no endereço de memória
  0010 0000    |  LDA end  | AC← MEM(end)                     | carrega acumulador com conteúdo da memória
  0011 0000    |  ADD end  | AC← MEM(end) + AC                | soma acumulador com conteúdo da memória
  0100 0000    |  OR end   | AC← MEM(end) OR AC               | operação 'ou' lógico
  0101 0000    |  AND end  | AC← MEM(end) AND AC              | operação 'e' lógico
  0110 0000    |  NOT      | AC← NOT AC                       | complemento a 1
  0111 0000    |  SUB end  | AC← MEM(end) -                   | AC subtrai acumulador do conteúdo da memória
  1000 0000    |  JMP end  | PC ← end                         | desvio incondicional para o endereço
  1001 0000    |  JN end   | IF N=1 THEN PC ← end             | desvio condicional se negativo
  1001 0100    |  JP end   | IF N=0 THEN PC ← end             | desvio condicional se positivo
  1001 1000    |  JV end   | IF V=1 THEN PC ← end             | desvio condicional se houve estouro
  1001 1100    |  JNV end  | IF V=0 THEN PC ← end             | desvio condicional se não houve estouro
  1010 0000    |  JZ end   | IF Z=1 THEN PC ← end             | desvio condicional se zero
  1010 0100    |  JNZ end  | IF Z=0 THEN PC ← end             | desvio condicional se diferente de zero
  1011 0000    |  JC end   | IF C=1 THEN PC ← end             | desvio condicional se foi-um
  1011 0100    |  JNC end  | IF C=0 THEN PC ← end             | desvio condicional se não foi-um
  1011 1000    |  JB end   | IF B=1 THEN PC ← end             | desvio condicional se emprestou-um
  1011 1100    |  JNB end  | IF B=0 THEN PC ← end             | desvio condicional se não emprestou-um
  1110 0000    |  SHR      | C←AC(0); AC(i-1)←AC(i); AC(7)←0  | deslocamento para direita
  1110 0001    |  SHL      | C←AC(7); AC(i)←AC(i-1); AC(0)←0  | deslocamento para esquerda
  1110 0010    |  ROR      | C←AC(0); AC(i-1)←AC(i); AC(7)←C  | rotação para direita
  1110 0011    |  ROL      | C←AC(7); AC(i)←AC(i-1); AC(0)←C  | rotação para esquerda
  1111 0000    |  HLT      | parada                           | término de execução



## History

Este programa foi feito originalmente como trabalho acadêmico da disciplina _Arquitetura e Organização de Computadores_,Lecionada pelo professor [Alexandre Beletti](https://github.com/rhiguita) na Universidade Federal do Pará, primeiro semestre do ano de 2015 no 2º período do curso de Engenharia da Computação.

## Credits

  * Eu (Manoel Vilela)

## License

[MIT](LICENSE.md)

## ROADMAP

  - [X] Finalizar trabalho e entregá-lo
  - [ ] Filtrar os warnings
  - [ ] Fazer documentação oficial
    - [ ] Traduzir todo o conteúdo para inglês
  