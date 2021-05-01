.PHONY: clean

OBJS = sobel.o main.o
CFLAGS = -c -g -Wall

main: $(OBJS)
	nvcc -o  $@ $^

%.o: %.cu %.h
	nvcc -c $<

%.o : %.cpp
	g++ $(CFLAGS) $^

clean:
	rm -f *.o *~
