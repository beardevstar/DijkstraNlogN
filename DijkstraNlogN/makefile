CC = g++
CFLAGS  = -g -Wall

TARGET = dijkstra

SRCS = DijkstraNlogN.cpp \
	edge.cpp \
	node.cpp \
	link_t.cpp \
	record.cpp \
	lsdb.cpp \
	nodeset.cpp
OBJS = $(SRCS:.cpp=.o)
all: $(TARGET)
	@echo  DijkstraNlogN Compiled Successfully.

$(TARGET) : $(OBJS)
	$(RM) $(TARGET)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	$(RM) *.o

clean:
	$(RM) *.o $(TARGET)