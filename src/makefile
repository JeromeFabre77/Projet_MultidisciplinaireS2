CC=gcc
CFLAGS=-Wall -ansi
LDFLAGS=-lMLV -lm
BIN=projet_multidisciplinaireS2
OBJ=utils/utils.o genetic/genetic.o interface/interface.o parser/csv_parser.o main.o

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(CFLAGS) $(LDFLAGS)

main.o: main.c genetic/genetic.h interface/interface.h parser/parser.h
	$(CC) -c main.c $(CFLAGS)

genetic/genetic.o: genetic/genetic.c genetic/genetic.h utils/utils.h
	$(CC) -c genetic/genetic.c -o genetic/genetic.o $(CFLAGS)

interface/interface.o: interface/interface.c interface/interface.h utils/utils.h
	$(CC) -c interface/interface.c -o interface/interface.o $(CFLAGS)

utils/utils.o: utils/utils.c utils/utils.h
	$(CC) -c utils/utils.c -o utils/utils.o $(CFLAGS)

parser/parser.o: parser/csv_parser.c parser/parser.h utils/utils.h
	$(CC) -c parser/csv_parser.c -o parser/csv_parser.o $(CFLAGS)

clean: 
	rm -f *.o */*.o
	rm -f *~
	rm -f $(BIN)
