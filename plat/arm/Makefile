SRCDIR ?= .
SRCS = hooks.c
OBJS = $(OBJDIR)/$(SRCS:.c=.o)
OUTLIB = platlib.a
CFLAGS += -Wall

default: $(OUTLIB)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTLIB): $(OBJS)
	ar rcs $(OUTDIR)/$(OUTLIB) $(OBJS)

clean:
	rm -f *.a
	rm -f *.o


