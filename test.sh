lex main.l
gcc lex.yy.c -lfl -o l.out

python3 code_generator.py > code.txt

./l.out code.txt
