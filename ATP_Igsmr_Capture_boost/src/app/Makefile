# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g
INCLUDE = -I../common -I../conf -I../net -I../tty
LDFLAGS = -lboost_program_options -lboost_chrono -lboost_system
LDPATH = 
VPATH = ../conf:../common:../tty:../net

OBJS = config_parser.o igsmr_config.o \
	   error.o wrapsock.o wraptermios.o tty_reader.o igsmr_serial_port.o \
		collection_data.o \
		main.o 
PROGS =	igsmr

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

$(OBJS):%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

igsmr: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

