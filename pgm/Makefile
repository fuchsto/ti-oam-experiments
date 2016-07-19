
CPP         = g++
CPP_OPTS    = -O3 -Wall -Wextra
EXE					= pgm-test.bin
SOURCES     = main.cpp pgm.cpp gaussian.cpp
OBJS				= $(patsubst %.cpp, %.o, $(SOURCES))

$(EXE): $(OBJS)
	$(CPP) $(OBJS) -o $@

%.o: %.cpp
	$(CPP) $(CPP_OPTS) -c $<

clean:
	@rm -f $(EXE)
	@rm -f $(OBJS)
	@rm -f *out.pgm
	@rm -f gen.in.pgm


