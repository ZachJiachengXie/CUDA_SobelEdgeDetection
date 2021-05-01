OBJS = sobel.o main.o
CFLAGS = -c -g -Wall

main: $(OBJS)
	nvcc -o  $@ $^

%.o: %.cu %.h
	nvcc -c $<

%.o : %.cpp
	g++ $(CFLAGS) $^

.PHONY: clean
clean:
	rm -rf $(OBJS)
