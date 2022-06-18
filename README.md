# mctrac

cd mctrac
flex instrument.l
bison -d instrument.y 
gcc lex.yy.c instrument.tab.c symtab.c memory.c coords.c cogen.c cexp.c file.c debug.c list.c -DMCCODE_PROJECT=1 -lm -o mcstas
