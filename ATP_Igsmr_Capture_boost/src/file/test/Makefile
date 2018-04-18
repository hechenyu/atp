# Makefile

RM = rm -f
CXX = g++
CXXFLAGS = -Wall -g -std=c++98
INCLUDE = -I..
LDFLAGS = -lboost_chrono -lboost_system
LDPATH =
VPATH = ..

PROGS =	test_utc_to_string file_write file_write2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

test_utc_to_string: utc_to_string.o test_utc_to_string.o 
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

file_write: file_write.o auto_timestamp_ofile.o utc_to_string.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

file_write2: file_write2.o  auto_timestamp_ofile.o utc_to_string.o
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDPATH)

