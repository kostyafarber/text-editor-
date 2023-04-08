all: kilo helper_kilo

kilo: kilo.c
	$(CC) kilo.c -g -o kilo -Wall -Wextra -pedantic -std=c99

helper_kilo: helper_kilo.c
	$(CC) helper_kilo.c -g -o helper_kilo -Wall -Wextra -pedantic -std=c99