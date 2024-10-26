default:
	gcc -Wall -Wextra -fsanitize=address -ggdb main.c
	./a.out luac.out
debug:
	gcc -Wall -Wextra -ggdb main.c
	gf2 ./a.out
