CFLAGS				+= -Ideps -Wall -pedantic
LDFLAGS				+= -Llib

# DEPSLIST			= toml.a

# DEPS				= $(addprefix deps/,$(DEPSLIST))

NAME				= mxterm
SRC					= $(wildcard src/*.c) $(wildcard src/rend/*.c)
OBJ					= $(SRC:.c=.o)
LIB					= lib
BIN					= bin
APP					= $(BIN)/$(NAME)

MX_REND				= opengl

SRC_SIZE			= $(words $(SRC))
SRC_DONE			= 0


define showprogress
	$(eval $(1)=$(shell echo $$(($($(1))+1))))
	@printf $(3) $($(1)) $(2) $<
endef


define rmall
	$(eval done=0)
	$(eval size=$(words $(1)))
	$(foreach c, $(1), \
		$(call showprogress,done,$(size),'[% 4d/%d] rm $(c)%s\n') && \
		rm -f $(c))
endef

define cleanall
	$(eval done=0)
	$(eval size=$(words $(1)))
	$(foreach c, $(1), \
		$(call showprogress,done,$(size),'[% 4d/%d] clean $(c)%s\n') && \
		$(MAKE) -C $(c:.a=) clean TARGET=$(2))
endef


app: $(APP)


%.a: $(LIB)
	@echo make $@
	@+$(MAKE) -C $(@:.a=) TARGET=../../$(LIB)


%.o: %.c
	$(call showprogress,SRC_DONE,$(SRC_SIZE),'[% 4d/%d] cc %s\n')
	@$(CC) $< -c -o $@ $(CFLAGS)


$(APP): $(DEPS) $(BIN) $(LIB) $(OBJ)
	@echo ld $(APP)
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)
	@chmod +x $@
	@echo done


$(BIN):
	@mkdir -p $(BIN)


$(LIB):
	@mkdir -p $(LIB)


clean:
	$(call cleanall,$(DEPS),../../$(LIB))
	$(call rmall,$(OBJ))
	@rm -rf $(LIB)
	@rm -rf $(BIN)
