# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -DUSE_GLOG -DDEBUG
INCLUDE = -I../common -I../conf -I../net -I../tty -I../file -I../log
LDFLAGS = -lboost_program_options -lboost_chrono -lboost_thread -lboost_system -lglog
LDPATH = 
VPATH = ../conf:../common:../tty:../net:../file:../log

OBJS = config_parser.o error.o wrapsock.o wraptermios.o tty_reader.o \
	    wrapsignal.o wrapglog.o wraplib.o udp_sender.o utc_to_string.o auto_timestamp_ofile.o \
		collection_data.o igsmr_config.o igsmr_serial_port.o \
		igsmr_net_serializer.o igsmr_udp_sender.o \
		igsmr_file_serializer.o igsmr_file_writer.o \
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

