CC = arm-linux-gnueabihf-gcc
CC2 = gcc
CFLAGS = pthread

FREE=-I/home/Wn/File/tools/freetype/include/freetype2 -L/home/Wn/tools/free/lib \
						-lfreetype -L/home/Wn/tools/zlib/lib -lz -L/home/Wn/tools/png/lib -lpng -lm

C_PATH= -I/path/to/arm/include
all : main.o server.o

main.o :main.c fork_receive.o fork_dispose.o fork_usart.o  LCD_free.o
	@ $(CC) -o $@  $^ $(C_PATH) -l$(CFLAGS) $(FREE)

server.o : server.c
	$(CC2) -o $@  $^ $(C_PATH) 

fork_receive.o : fork_receive.c 
	$(CC) -c $< -o $@  $(FREE)
fork_usart.o : fork_usart.c
	$(CC) -c $< -o $@
fork_dispose.o : fork_dispose.c
	$(CC) -c $< -o $@
LCD_free.o : LCD_free.c
	$(CC) -c $< -o $@ $(FREE)
clean:
	rm -f *.o 
