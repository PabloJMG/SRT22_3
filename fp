CC=gcc
LIBS=-lm -fopenmp
SDIR=source
ODIR=obj
_OBJ=openf.o
OBJ=$(patsubst %, $(ODIR)/%, $(_OBJ))


$(ODIR)/%.o : $(SDIR)/%.c
	$(CC) -c -o $@ $< $(LIBS)

openf: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
