tar   = threadpool_main
src   = $(wildcard ./*.cpp)
obj   = $(patsubst %.cpp, %.o, $(src))
CLAFS = -g -Wall

all: $(obj)
	g++ -o $(tar) $^ -pthread -lrt

%.o: %.c
	g++ $(CLAFS) -c $< $@

.PHONY: clean
clean:
	rm -rf $(obj) $(tar)
