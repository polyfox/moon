BASEDIR = .
BUILD_DIR = ./build
CC = g++
ALL_CFLAGS = -g -Wall -std=c++11
OBJECT = $(patsubst %.cxx,%.o,$(wildcard $(BASEDIR)/src/*.cxx)) $(patsubst %.cxx,%.o,$(wildcard $(BASEDIR)/src/**/*.cxx))
INCLUDES = -I$(BASEDIR)/src -I$(BASEDIR)/include -I$(BASEDIR)/lib -I$(BASEDIR)/vendor/mruby/include -I$(BASEDIR)/vendor/gorilla-audio/include
RM_F := rm -f
ECHO := echo
MSG_BEGIN = @for line in
MSG_END = ; do echo "$$line"; done

.PHONY : all
all: engine game

.PHONY : engine
engine: $(BUILD_DIR)/libmoon.a

.PHONY : game
game: engine
	$(MAKE) -C game

$(BUILD_DIR)/libmoon.a : mruby gorilla-audio $(OBJECT)
	$(AR) r $@ $(OBJECT)

$(OBJECT) : %.o : %.cxx
	$(CC) $(ALL_CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY : mruby
mruby:
	$(MAKE) -C vendor/mruby

.PHONY : gorilla-audio
gorilla-audio:
	cd vendor/gorilla-audio/build; cmake .;	make

.PHONY : clean
clean :
	-$(RM_F) $(BUILD_DIR)/libmoon.a
	-$(RM_F) $(TARGET) $(OBJECT)
	-$(RM_F) $(OBJECT:.o=.d)
	$(MAKE) -C game clean

distclean : clean
	$(MAKE) -C vendor/mruby clean

.PHONY : help
help:
	$(MSG_BEGIN) \
		"" \
		"	Moon Makefile" \
		"" \
		"targets:" \
		"	all (default) - builds mruby, Moon engine, and game demo" \
		"	engine - builds Moon engine static library" \
		"	game - builds game demo using libmoon.a" \
		"	clean - clean Moon engine and game artifacts and demo files" \
		"	distclean - clean mruby artifacts as well as Moon artifacts/demo files" \
		$(MSG_END)