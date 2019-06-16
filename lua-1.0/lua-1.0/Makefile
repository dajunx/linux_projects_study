OBJS= hash.o inout.o lex_yy.o opcode.o table.o y_tab.o lua.o iolib.o mathlib.o strlib.o
DEPS= hash.h inout.h lex_yy.h opcode.h table.h y_tab.h lua.h iolib.h mathlib.h strlib.h

CFLAGS= -I.

lua: lua.o
	gcc -g -o lua.o -lm
$(OBJS): $(OBJS) $(DEPS)
	gcc -g -c -o $@ $< $(CFLAGS)

A=--------------------------------------------------------------------------
test:	$T
	@echo "$A"
	./$T sort.lua main
	@echo "$A"
	./$T globals.lua | sort | column
	@echo "$A"
	./$T array.lua
	@echo "$A"
	./$T save.lua
	@echo "$A"
	./$T test.lua retorno_multiplo norma

clean:
	rm -f $T $(OBJS) core core.*

diff:
	diff . fixed | grep -v ^Only
