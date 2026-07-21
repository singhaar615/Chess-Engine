# ONLY MODIFY LINE 2 - LIST YOUR SOURCE FILES
STUDENT_SRCS = chess.c

# ------------- DO NOT MODIFY --------------------
GCC = gcc -std=c11 -g -Wall -Wshadow --pedantic -Wvla -Werror
OUR_SRCS = main.c utils.c
SRCS = $(STUDENT_SRCS) $(OUR_SRCS)
OBJS = $(SRCS:%.c=%.o)
EXECUTABLE = chess

chess_engine: $(OBJS) 
	$(GCC) $(OBJS) -o $(EXECUTABLE)
	./$(EXECUTABLE)

%.o: %.c
	$(GCC) -c $< -o $@

# Use this recipe to test that your program compiles before submitting
submit: $(SRCS)
	gcc -std=c11 -O3 -Wall -Wshadow --pedantic -Wvla -Werror -o $(EXECUTABLE) $(SRCS)

clean:
	rm -f $(EXECUTABLE) $(OBJS)

.PHONY: clean