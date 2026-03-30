CC = gcc
CFLAGS = -Wall -ansi
LDFLAGS = -lMLV -lm

BIN = projet_multidisciplinaireS2

OBJ = main.o \
      utils/utils.o \
      city/city.o \
      individual/individual.o \
      genetic/genetic.o \
      interface/interface.o \
      parser/csv_parser.o \
      file_writer/file_writer.o

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

main.o: main.c genetic/genetic.h interface/interface.h parser/parser.h file_writer/file_writer.h city/city.h
	$(CC) -c main.c $(CFLAGS)

utils/utils.o: utils/utils.c utils/utils.h genetic/genetic.h individual/individual.h city/city.h
	$(CC) -c utils/utils.c -o utils/utils.o $(CFLAGS)

city/city.o: city/city.c city/city.h utils/utils.h
	$(CC) -c city/city.c -o city/city.o $(CFLAGS)

individual/individual.o: individual/individual.c individual/individual.h city/city.h utils/utils.h
	$(CC) -c individual/individual.c -o individual/individual.o $(CFLAGS)

genetic/genetic.o: genetic/genetic.c genetic/genetic.h individual/individual.h city/city.h utils/utils.h
	$(CC) -c genetic/genetic.c -o genetic/genetic.o $(CFLAGS)

interface/interface.o: interface/interface.c interface/interface.h genetic/genetic.h individual/individual.h city/city.h utils/utils.h
	$(CC) -c interface/interface.c -o interface/interface.o $(CFLAGS)

parser/csv_parser.o: parser/csv_parser.c parser/parser.h city/city.h utils/utils.h
	$(CC) -c parser/csv_parser.c -o parser/csv_parser.o $(CFLAGS)

file_writer/file_writer.o: file_writer/json_file_writer.c file_writer/file_writer.h genetic/genetic.h city/city.h
	$(CC) -c file_writer/json_file_writer.c -o file_writer/file_writer.o $(CFLAGS)

clean:
	rm -f *.o */*.o *~ $(BIN)

re: clean $(BIN)