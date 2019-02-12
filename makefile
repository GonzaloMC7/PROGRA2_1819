CC = gcc -ansi -pedantic
CFLAGS = -Wall
EXE = p1_e1

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o node.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o node.o

node.o : node.c node.h
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $<

	
p1_e1_test:
	@echo Ejecutando p1_e1
	@./p1_e1

	
# memoria.pdf: memoria.md Makefile
	# pandoc --template eisvogel -s -o memoria.pdf memoria.md --listings

