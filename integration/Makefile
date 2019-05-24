DIR_INC = ./include
DIR_SRC = ./src
DIR_BIN = ./bin
DIR_OBJ = ./obj

CC  = gcc
MCC = mpicc
LD  = gcc
MLD = mpicc
CFLAGS = -Wall -I$(DIR_INC)

serial_0   = $(DIR_BIN)/main
mpi_0      = $(DIR_BIN)/main-mpi1
serial_nr  = $(DIR_BIN)/main_nr
mpi_nr1     = $(DIR_BIN)/main_nr-mpi1
mpi_nr2     = $(DIR_BIN)/main_nr-mpi2

_SRCS = main.c integration.c function.c main-mpi1.c main_nr.c integration_nr.c \
		main_nr-mpi1.c
SRCS  = $(patsubst %,$(DIR_SRC)/%,$(_SRCS))
#$(warning $(SRCS))

_OBJS = $(patsubst %.c,%.o,$(_SRCS))
#$(warning $(_OBJS))
OBJS  = $(patsubst %,$(DIR_OBJ)/%,$(_OBJS))
#$(warning $(OBJS))

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c include/*.h
	$(MCC) -c $(CFLAGS) $< -o $@

all : $(OBJS)

.PHONY: clean serial_0 mpi_0 serial_nr mpi_nr
clean:
	rm -f $(DIR_BIN)/* $(DIR_OBJ)/*

serial_0 : $(DIR_OBJ)/main.o $(DIR_OBJ)/integration.o $(DIR_OBJ)/function.o
	$(MLD) -o $(serial_0) $^ 

mpi_0: $(DIR_OBJ)/main-mpi1.o $(DIR_OBJ)/integration.o $(DIR_OBJ)/function.o
	$(MLD) -o $(mpi_0) $^

serial_nr: $(DIR_OBJ)/main_nr.o $(DIR_OBJ)/integration_nr.o $(DIR_OBJ)/function.o
	$(MLD) -o $(serial_nr) $^

mpi_nr1: $(DIR_OBJ)/main_nr-mpi1.o $(DIR_OBJ)/integration_nr.o $(DIR_OBJ)/function.o
	$(MLD) -o $(mpi_nr1) $^

mpi_nr2: $(DIR_OBJ)/main_nr-mpi2.o $(DIR_OBJ)/integration_nr.o $(DIR_OBJ)/function.o
	$(MLD) -o $(mpi_nr2) $^
