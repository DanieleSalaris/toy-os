CC := clang
OUTDIR := ../build/test
MAINDIR := ..
EXE := $(OUTDIR)/test

CFLAGS = -Wall -Wextra

C_APP_SRCS := formats.c string.c
C_APP_SRCS := $(patsubst %, $(MAINDIR)/%, $(C_APP_SRCS))
C_TEST_SRCS = main.c format_test.c test.c
C_SRCS := $(C_APP_SRCS) $(C_TEST_SRCS)

C_APP_OBJS = $(patsubst $(MAINDIR)/%.c, $(OUTDIR)/%.o, $(C_APP_SRCS)) 
C_TEST_OBJS = $(patsubst %.c, $(OUTDIR)/%.o, $(C_TEST_SRCS))
C_OBJS := $(C_APP_OBJS) $(C_TEST_OBJS)

run: $(EXE)
	$(EXE)
compile: $(EXE)

$(OUTDIR):
	mkdir -p $@

$(OUTDIR)/%.o: %.c | $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(MAINDIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(C_OBJS)
	$(CC) $(CFLAGS) $(C_OBJS) -o $(EXE)

clean:
	rm -rf $(OUTDIR)

.PHONY: run compile clean

