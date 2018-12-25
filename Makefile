CFLAGS				+= -Ideps -Wall -Werror -pedantic
LDFLAGS				+= -Llib -framework Cocoa

# DEPSLIST			= toml.a

# DEPS				= $(addprefix deps/,$(DEPSLIST))

NAME				= mxterm
SRC					= $(wildcard src/*.c)
OBJ					= $(SRC:.m=.o)
LIB					= lib
BIN					= bin
APP					= $(BIN)/$(NAME)


.PHONY: app
app: $(APP)



%.a: $(LIB)
	+$(MAKE) -C $(@:.a=) TARGET=../../$(LIB)


%.o: %.c
	@echo cc $<
	@$(CC) $< -c -o $@ $(CFLAGS)


$(APP): $(DEPS) $(BIN) $(LIB) $(OBJ)
	@echo ld $(APP)
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)
	@chmod +x $@


$(BIN):
	@mkdir -p $(BIN)


$(LIB):
	@mkdir -p $(LIB)


.PHONY: clean
clean:
	@$(foreach c, $(DEPS), echo clean $(c) && $(MAKE) -C $(c:.a=) clean)
	@$(foreach c, $(OBJ), echo rm $(c) && rm -f $(c);)
	@echo rm $(LIB) && rm -rf $(LIB)
	@echo rm $(BIN) && rm -rf $(BIN)
