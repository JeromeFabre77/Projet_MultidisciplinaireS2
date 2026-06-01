CC = gcc
CFLAGS = -Wall -ansi -g
LDFLAGS = -lMLV -lm

BIN = projet_multidisciplinaireS2

OBJ = main.o \
      utils/utils.o \
      city/city.o \
			hospital/hospital.o \
      individual/individual.o \
			generation/generation.o \
      interface/interface.o \
      parser/csv_parser.o \
      file_writer/file_writer.o

CONST = constants.h

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

main.o: main.c $(CONST)
	$(CC) -c main.c $(CFLAGS)

utils/utils.o: utils/utils.c utils/utils.h $(CONST)
	$(CC) -c utils/utils.c -o utils/utils.o $(CFLAGS)

city/city.o: city/city.c city/city.h $(CONST)
	$(CC) -c city/city.c -o city/city.o $(CFLAGS)

hospital/hospital.o: hospital/hospital.c hospital/hospital.h $(CONST)
	$(CC) -c hospital/hospital.c -o hospital/hospital.o $(CFLAGS)

individual/individual.o: individual/individual.c individual/individual.h $(CONST)
	$(CC) -c individual/individual.c -o individual/individual.o $(CFLAGS)

generation/generation.o: generation/generation.c generation/generation.h $(CONST)
	$(CC) -c generation/generation.c -o generation/generation.o $(CFLAGS)

interface/interface.o: interface/interface.c interface/interface.h $(CONST)
	$(CC) -c interface/interface.c -o interface/interface.o $(CFLAGS)

parser/csv_parser.o: parser/csv_parser.c parser/parser.h $(CONST)
	$(CC) -c parser/csv_parser.c -o parser/csv_parser.o $(CFLAGS)

file_writer/file_writer.o: file_writer/json_file_writer.c file_writer/file_writer.h $(CONST)
	$(CC) -c file_writer/json_file_writer.c -o file_writer/file_writer.o $(CFLAGS)

clean:
	rm -f *.o */*.o *~ $(BIN)

re: clean $(BIN)