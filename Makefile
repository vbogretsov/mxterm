NAME				= mxterm
CFLAGS				+= -Wall -Werror -pedantic -DMX_REND_SDL -I/usr/local/include -Ideps
LDFLAGS				+= -L/usr/local/lib -lSDL2 -lSDL2_ttf
HDR					= $(wildcard src/*.h) $(wildcard src/rend/*.h)
SRC					= $(wildcard src/*.c) $(wildcard src/rend/*.c)
OBJ					= $(SRC:.c=.o)
BIN					= bin
APP					= $(BIN)/$(NAME)


.PHONY: app
app: $(APP)


%.o: %.c %.h
	@echo cc $<
	@$(CC) $< -c -o $@ $(CFLAGS)


$(APP): $(BIN) $(HDR) $(OBJ)
	@echo ld $(APP)
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)
	@chmod +x $@


$(BIN):
	@mkdir -p $(BIN)


.PHONY: clean
clean:
	@$(foreach c, $(OBJ), echo rm $(c) && rm -f $(c);)
	@echo rm $(LIB) && rm -rf $(LIB)
	@echo rm $(BIN) && rm -rf $(BIN)
