GCC = gcc -Wall -Wextra -Werror -O2
GCC_RELEASE = gcc -Wall -Wextra -Werror -O3

PREREQS = src/init/init.o src/markers/markers.o

help:
	@echo "Args:"
	@echo "    build      Builds unoptimized main binary in ./target/main"
	@echo "    release    Builds   optimized main binary in ./target/main"
	@echo "    run        Builds and runs unoptimized main binary in ./target/main"
	@echo "    debug      Builds and runs gdb on binary in ./target/main_debug"
	@echo "    keys       Builds and runs key detection binary in ./target/keys"

build: src/main.c $(PREREQS)
	$(GCC) $^ -o target/main

debug: src/main.c $(PREREQS)
	$(GCC) -ggdb $^ -o target/main

release: src/main.c $(PREREQS)
	$(GCC_RELEASE) $^ -o target/main

run: build
	./target/main -m 400


%.o: %.c
	$(GCC) -ggdb -c $< -o $*.o

src/init/init.o : src/init/init.h src/markers/markers.h
src/markers/markers.o : src/markers/markers.h


#keys: ./src/identify_keystroke.c
#	$(GCC) $? -o target/keys
#	./target/keys
#
#debug: ./src/main.c
#	$(GCC) -ggdb main.c -o target/main_debug
#	gdb main_debug
#
#./target/build/init.o: src/init.c src/init.h target/build/markers.o
#	$(GCC) -c $^ -o target/build/init.o
#
#./target/build/interface.o: src/interface.c src/interface.h src/markers.c src/markers.h
#	$(GCC) -c $^ -o target/build/interface.o
#
#./target/build/markers.o: src/markers.c src/markers.h
#	$(GCC) -c src/markers.c -o target/build/markers.o
