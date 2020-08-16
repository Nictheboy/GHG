cc = g++
prom = hackgame
header = hackgame.h
src = $(shell find ./ -maxdepth 1 -name "*.cpp")
obj = $(src:%.cpp=%.o) 

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)
	cp $(prom) bin/$(prom) 
%.o: %.c $(header)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)

