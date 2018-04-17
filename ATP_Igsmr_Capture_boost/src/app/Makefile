# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DDEBUG
INCLUDE = -I../common -I../conf -I../net -I../tty
LDFLAGS = -lboost_program_options -lboost_chrono -lboost_thread -lboost_system
LDPATH = 
VPATH = ../conf:../common:../tty:../net:../file

OBJS = config_parser.o error.o wrapsock.o wraptermios.o tty_reader.o \
	    wraplib.o udp_sender.o \
		collection_data.o igsmr_config.o igsmr_serial_port.o \
		igsmr_net_serializer.o igsmr_udp_sender.o \
		igsmr_file_serializer.o \
		igsmr_monitor.o igsmr_monitor_app.o \
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

