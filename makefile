cc = g++
prom = hackgame
header = hackgame.h mujs.h
src = $(shell find ./ -maxdepth 1 -name "*.cpp")
lib = lib/libmujs.o
obj = $(src:%.cpp=%.o) 

$(prom): $(obj) $(lib)
	$(cc) -o $(prom) $(obj) $(lib)

%.o: %.c $(header)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)

