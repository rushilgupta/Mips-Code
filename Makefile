SRCS 		= add.cpp alu.cpp aluctrl.cpp and.cpp ctrl.cpp main.cpp \
		mux.cpp ram.cpp register.cpp registerfile.cpp rom.cpp \
		shift.cpp signextend.cpp decoder.cpp signextend2.cpp andpc.cpp shiftj.cpp

OBJS 		= $(SRCS:%.cpp=%.o)
DEPS		= $(SRCS:%.cpp=%.d)

SYSTEMC 	= /usr/local/systemc-2.2.0

CXX		= g++
CXXFLAGS	= -Wall -ansi -g -O0 -Di386
INCLUDES 	= -I $(SYSTEMC)/include

LD		= $(CXX)
LDFLAGS 	= $(CXXFLAGS)

LIBS		= -L $(SYSTEMC)/lib-linux -lsystemc

PROGRAM 	= mips

default:	$(PROGRAM)

$(PROGRAM):	$(OBJS) $(DEPS)
		$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o ./$@
		
test:		$(PROGRAM)
		./$<
		
clean:; 	rm -rf $(PROGRAM) *.o core* *.d *.vcd *.dump

# Cancel implicit rule
%.o:		%.cpp

%.o:		%.cpp %.d
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.d: 		%.cpp
		@ echo Making dependencies for $<
		@ g++ -MM $(INCLUDES) $< | sed 's#: # $*.d : #1' > $@

# Include dependency files .d
ifneq ("$(wildcard *.d)","")
  include $(wildcard *.d)
endif
