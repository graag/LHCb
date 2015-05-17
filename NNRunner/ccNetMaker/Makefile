CC = gcc

CPPFLAGS = $(shell ${ROOTSYS}/bin/root-config --cflags)
LDFLAGS = $(shell ${ROOTSYS}/bin/root-config --libs) -lXMLParser

INCS = GVector.h NeuralNetReader.h

OBJS = cmdNeuralNet.o NeuralNetReader.o

#all: eventview

run_net: $(OBJS)
	$(CC) -o run_net $(OBJS) $(CPPFLAGS) $(LDFLAGS)

cmdNeuralNet.cc: $(INCS)

clean:
	rm -f .*~ *~ *.bak *.o run_net.* .depend run_net
