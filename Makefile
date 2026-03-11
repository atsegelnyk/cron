CC = clang
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -I.

SRC = main.c \
      cron/cron.c \
      parser/parser.c \
	  task/task.c

OBJ = $(SRC:.c=.o)

cron: $(OBJ)
	$(CC) $(OBJ) -o cron

clean:
	rm -f $(OBJ) cron
