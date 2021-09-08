# Instructions to the compiler and options
CC = g++
RM = rm
CFLAGS = -Wall
# Macro definition
DEFS = -D OS_UNIX
CFLAGS += $(DEFS)
# The target file  
TARGET = piguard
# The source file
DIR_SRC = ../../source
SRCS = $(wildcard ${DIR_SRC}/piguard.cpp)
# The header file search path
INC = -I../../source
# Dependent libraries,Select library file According to the operating system you using
OS_TYPE = x86_32
ifeq ($(OS_TYPE), x86_64)
LIBS = -L./libs/linux/x86_64 -lUSB2XXX -lusb-1.0 -lpthread
endif
ifeq ($(OS_TYPE), x86_32)
LIBS = -L./libs/linux/x86 -lUSB2XXX -lusb-1.0 -lpthread
endif
ifeq ($(findstring armv6,$(OS_TYPE)), armv6)
LIBS = -L./libs/linux/armv7 -lUSB2XXX -lusb-1.0 -lpthread
endif
ifeq ($(findstring armv7,$(OS_TYPE)), armv7)
LIBS = -L./libs/linux/armv7 -lUSB2XXX -lusb-1.0 -lpthread
endif
ifeq ($(findstring armv8,$(OS_TYPE)), armv8)
LIBS = -L./libs/linux/arm64 -lUSB2XXX -lusb-1.0 -lpthread
endif
ifeq ($(findstring aarch64,$(OS_TYPE)), aarch64)
LIBS = -L./libs/linux/aarch64 -lUSB2XXX -lusb-1.0 -lpthread
endif
ifeq ($(findstring mips64,$(OS_TYPE)), mips64)
LIBS = -L./libs/linux/mips64 -lUSB2XXX -lusb-1.0 -lpthread
endif
# The target file
OBJS = $(SRCS:.cpp=.o)
# Link for the executable file
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(LIBS)
	$(RM) -rf $(OBJS)
clean:
	$(RM) -rf $(TARGET) $(OBJS)

%.o:%.cpp
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<
