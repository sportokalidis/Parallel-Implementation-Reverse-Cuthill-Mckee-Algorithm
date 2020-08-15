CC = gcc
CFLAGS = -O3 -g -Wall -std=c99 -fopenmp -lm
SDIR = ./src
LDIR = ./lib
IDIR = ./inc
TDIR = ./tester
INCLUDES = -I$(IDIR)
LIBRARIES = -L$(LDIR)
CLEAN = clean
EXEC = $(SDIR)/v0 $(SDIR)/v1 $(SDIR)/v2 $(SDIR)/v3
LIBS = $(LDIR)/v0_seq.a $(LDIR)/v1_omp.a $(LDIR)/v2_omp.a $(LDIR)/v3_omp.a
MAIN = v0_seq v1_omp v2_omp v3_omp


all: $(CLEAN) $(EXEC)

# Executables
$(SDIR)/v0: $(SDIR)/v0_seq.c $(LDIR)/v0_seq.a
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBRARIES) -o $@ $^

$(SDIR)/v1: $(SDIR)/v1_omp.c $(LDIR)/v1_omp.a
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBRARIES) -o $@ $^

$(SDIR)/v2: $(SDIR)/v2_omp.c $(LDIR)/v2_omp.a
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBRARIES) -o $@ $^

$(SDIR)/v3: $(SDIR)/v3_omp.c $(LDIR)/v3_omp.a
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBRARIES) -o $@ $^



# LIBRARIES
$(LDIR)/%.a: $(SDIR)/%.o $(SDIR)/functions.o $(SDIR)/queue.o
	ar rcs $@ $^



$(SDIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $^

run:
	$(SDIR)/v0
	$(SDIR)/v1
	$(SDIR)/v2
	$(SDIR)/v3
	python3 $(TDIR)/python_tester.py
	python3 $(TDIR)/tester.py

clean:
	rm -f $(EXEC) $(LIBS)
