cc = g++
prom = hackgame
header = $(shell find ./src-utf8 -maxdepth 1 -name "*.h")
src = $(shell find ./src-utf8 -maxdepth 1 -name "*.cpp")
object_code = code.cpp
$(prom): FORCE
	cat $(header) $(src) > $(object_code)
	$(cc) -o $(prom) $(object_code)
FORCE:;
