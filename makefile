cc = g++ -std=c++11 -fsanitize=address
prom = hackgame
header = $(shell find ./src -maxdepth 1 -name "*.h")
src = $(shell find ./src -maxdepth 1 -name "*.cpp")
object_code = one.cpp
$(prom): FORCE
	cat $(header) $(src) > $(object_code)
	iconv -f UTF-16 -t UTF-8 $(object_code) > $(object_code).temp
	mv $(object_code).temp $(object_code)
	$(cc) -o $(prom) $(object_code)
clean:
	rm $(prom) $(object_code)
	rm -rf ./src-utf8
FORCE:;
