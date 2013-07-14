BASEDIR = ..
BUILD_DIR = ../build
CC = g++
ALL_CFLAGS = -Wall -std=c++11
OBJECT = $(wildcard $(BASEDIR)/game/*.cxx) $(wildcard $(BASEDIR)/game/**/*.cxx)
INCLUDES = -I$(BASEDIR)/src -I$(BASEDIR)/include -I$(BASEDIR)/vendor/mruby/include -I$(BASEDIR)/vendor/gorilla-audio/include -L$(BUILD_DIR) -L$(BASEDIR)/vendor/gorilla-audio/bin/linux/Release -L$(BASEDIR)/vendor/mruby/build/host/lib
LIBS = -lmoon -lmruby -lglfw -lGL -lIL -lILU -lgorilla -lopenal
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