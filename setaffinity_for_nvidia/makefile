
LINK = mpicc -g
CC = mpicc
CFLAGS = -D_GNU_SOURCE -g

OBJECTS = setaffinity_for_nvidia.o setaffinity_mpi.o 
SOBJECTS = setaffinity_for_nvidia.o setaffinity_serial.o 

all: setaffinity_mpi setaffinity_serial

setaffinity_mpi: ${OBJECTS} 
	${LINK} -o $@ $+

setaffinity_serial: ${SOBJECTS} 
	${LINK} -o $@ $+

setaffinity.o: setaffinity.c
	${CC} ${CFLAGS} -c $+ -o $@
setaffinity_mpi.o: setaffinity_mpi.c
	${CC} ${CFLAGS} -c $+ -o $@

clean:
	rm -f ${OBJECTS} ${SOBJECTS} setaffinity_mpi setaffinity_serial

