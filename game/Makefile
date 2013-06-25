BASEDIR = ..
BUILD_DIR = ../build
CC = g++
ALL_CFLAGS = -Wall
OBJECT = $(wildcard $(BASEDIR)/game/*.cxx) $(wildcard $(BASEDIR)/game/**/*.cxx)
INCLUDES = -I$(BASEDIR)/src -I$(BASEDIR)/include -I$(BASEDIR)/vendor/mruby/include -L$(BUILD_DIR)
LIBS = -lmoon -lglfw -lGL -lIL -lILU
CP_R := cp -r
RM_RF := rm -rf

.PHONY : all
all: 
	$(CC) $(ALL_CFLAGS) $(OBJECT) $(INCLUDES) $(LIBS) -o $(BUILD_DIR)/game
	$(CP_R) $(BASEDIR)/game/scripts $(BUILD_DIR)
	$(CP_R) $(BASEDIR)/core $(BUILD_DIR)

.PHONY : clean
clean:
	-$(RM_RF) $(BUILD_DIR)/game
	-$(RM_RF) $(BUILD_DIR)/scripts
	-$(RM_RF) $(BUILD_DIR)/core