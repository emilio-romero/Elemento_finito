CFLAGS=-c -std=c99 -lm 
CFLAGS1= -std=c99 -lm 
EXEC=Calor.bin 

CPPS:=$(wildcard src/*.c)
OBJS:=$(patsubst src/%.c,obj/%.o,$(CPPS))
OBJDIR:=obj

all: bin/$(EXEC)

bin/$(EXEC): $(OBJS) | bin
	gcc -o $@ $(OBJS) $(CFLAGS1)
bin: 
	mkdir -p $@

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	gcc $(CFLAGS) $< -o $@ 
$(OBJDIR): 
	mkdir -p $@
clean: 
	rm -f -r $(OBJDIR) bin
