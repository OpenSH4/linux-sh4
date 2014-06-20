CC = sh4-linux-gcc
LD = sh4-linux-ld
CFLAGS+= -I ../

flash_rdonly : flash_setrdonly.c 
	$(CC) $(CFLAGS) flash_setrdonly.c -o flash_setrdonly 	

flash_rdwr : flash_setrdwr.c  
	$(CC) $(CFLAGS) flash_setrdwr.c  -o flash_setrdwr

all  : flash_rdonly flash_rdwr

#install :
#	cp flash_setrdonly  flash_setrdwr ~/workspace/box/
	
clean :
	-rm flash_setrdonly  flash_setrdwr
