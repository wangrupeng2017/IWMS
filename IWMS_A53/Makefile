
CC    := gcc
FLAGS := -Wall -g
OBJS  := main

#LIBS=-L ./lib -lsqlite3
LIBS     := -lsqlite3
Includes := -I ../IWMS_SQLs
Others   := ../IWMS_SQLs/store.c ../IWMS_SQLs/common_time.c

all:$(OBJS)


$(OBJS):%:%.c
	$(CC) $(FLAGS) $< $(Others) -o $@ $(LIBS) $(Includes)
	cp $@ ~/IWMS/BOA/www/cgi-bin/

clean:
	rm $(OBJS)
