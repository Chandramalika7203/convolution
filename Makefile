CPP = g++
CPPFLAGS = 
FINAL = conv

all: $(FINAL)

$(FINAL): conv.o
	$(CPP) $(CPPFLAGS) -o $(FINAL) conv.o

cov.o: conv.cpp
	$(CPP) $(CPPFLAGS) -c conv.cpp

clean:
	rm -rf *.o $(FINAL)