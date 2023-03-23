CC = g++
CFLAGS = -std=c++20 -Wall
OBJECTS = CInputParser.o CSimulatedAnnealingSolver.o main.o
BIN = ./mwsatsolver


all: $(OBJECTS)
	make $(BIN)

run:
	$(BIN)

clean:
	rm -rf $(BIN) $(OBJECTS)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

main.o: src/main.cpp src/CInputParser.h src/CFormula.h src/CClause.h src/CVariable.h src/CSimulatedAnnealingSolver.h
	$(CC) $(CFLAGS) -c src/main.cpp

CInputParser.o: src/CInputParser.cpp src/CInputParser.h src/CFormula.h src/CClause.h src/CVariable.h
	$(CC) $(CFLAGS) -c src/CInputParser.cpp

CSimulatedAnnealingSolver.o: src/CSimulatedAnnealingSolver.cpp  src/CSimulatedAnnealingSolver.h src/CFormula.h src/CClause.h  src/CVariable.h src/CRandomAssignmentGenerator.h
	$(CC) $(CFLAGS) -c src/CSimulatedAnnealingSolver.cpp
