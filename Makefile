CC = arm-linux-gnueabihf-gcc
CFLAGS = pthread


C_PATH= -I/path/to/arm/include

main.o :main.c fork_receive.o fork_dispose.o fork_usart.o 
	$(CC) -o $@  $^ $(C_PATH) -l$(CFLAGS)

fork_receive.o : fork_receive.c
	$(CC) -c $< -o $@
fork_usart.o : fork_usart.c
	$(CC) -c $< -o $@
fork_dispose.o : fork_dispose.c
	$(CC) -c $< -o $@
clean:
	rm -f *.o 
