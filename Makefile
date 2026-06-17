CXX      := g++
STD      := -std=c++17
WARNINGS := -Wall -Wextra -Wpedantic
#DEBUG    := -g -fsanitize=address,undefined
#DEBUG    := -gmake clean
DEBUG    := -g
OPT      := -O2
INC      := -Iinclude




.PHONY: all ex1.1 ex1.2 ex1.3 ex1.4 ex2.1 ex2.2 ex2.3 ex3.1 ex3.2 ex3.3 ex3.4 ex4.1 ex4.2 ex4.3 ex5.1 ex5.3 ex5.4 projet clean

all: bin/ex1.1 \
     bin/ex1.2 \
     bin/ex1.3 \
     bin/ex1.4 \
     bin/ex2.1 \
     bin/ex2.2 \
     bin/ex2.3 \
	 bin/ex3.1 \
     bin/ex3.2 \
     bin/ex3.3 \
     bin/ex3.4 \
	 bin/ex4.1 \
     bin/ex4.2 \
     bin/ex4.3 \
	 bin/ex5.1 \
	 bin/ex5.3 \
	 bin/ex5.4 \
	 bin/projet

bin/:
	mkdir -p bin

bin/ex1.1: src/ex1.1/main.cpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex1.1/main.cpp

bin/ex1.2: src/ex1.2/main.cpp src/ex1.2/order_book.hpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex1.2/main.cpp

bin/ex1.3: src/ex1.3/main.cpp src/ex1.3/order_manager.hpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex1.3/main.cpp

bin/ex1.4: src/ex1.4/main.cpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex1.4/main.cpp

# ============================================================
# Exercice 2.1 - OrderBook avec std::map
# ============================================================

bin/ex2.1: src/ex2.1/main.cpp src/ex2.1/order_book_map.hpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex2.1/main.cpp

# ============================================================
# Exercice 2.2 - Statistiques STL
# ============================================================

bin/ex2.2: src/ex2.2/main.cpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex2.2/main.cpp

# ============================================================
# Exercice 2.3 - erase-remove idiom
# ============================================================

bin/ex2.3: src/ex2.3/main.cpp include/mmfx/types.hpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex2.3/main.cpp

bin/ex3.1: src/ex3.1/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex3.1/main.cpp

bin/ex3.2: src/ex3.2/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex3.2/main.cpp

bin/ex3.3: src/ex3.3/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex3.3/main.cpp

bin/ex3.4: src/ex3.4/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex3.4/main.cpp



bin/ex4.1: src/ex4.1/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(DEBUG) $(INC) -o $@ src/ex4.1/main.cpp

bin/ex4.2: src/ex4.2/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex4.2/main.cpp

bin/ex4.3: src/ex4.3/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex4.3/main.cpp	

bin/ex5.1: src/ex5.1/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex5.1/main.cpp

bin/ex5.3: src/ex5.3/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex5.3/main.cpp	

bin/ex5.4: src/ex5.4/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/ex5.4/main.cpp	

bin/projet: src/projet/main.cpp | bin/
	$(CXX) $(STD) $(WARNINGS) $(OPT) $(INC) -o $@ src/projet/main.cpp	



ex1.1: bin/ex1.1
	./bin/ex1.1

ex1.2: bin/ex1.2
	./bin/ex1.2

ex1.3: bin/ex1.3
	./bin/ex1.3

ex1.4: bin/ex1.4
	./bin/ex1.4

ex2.1: bin/ex2.1
	./bin/ex2.1

ex2.2: bin/ex2.2
	./bin/ex2.2

ex2.3: bin/ex2.3
	./bin/ex2.3

ex3.1: bin/ex3.1
	./bin/ex3.1

ex3.2: bin/ex3.2
	./bin/ex3.2

ex3.3: bin/ex3.3
	./bin/ex3.3

ex3.4: bin/ex3.4
	./bin/ex3.4

ex4.1: bin/ex4.1
	./bin/ex4.1

ex4.2: bin/ex4.2
	./bin/ex4.2

ex4.3: bin/ex4.3
	./bin/ex4.3	

ex5.1: bin/ex5.1
	./bin/ex5.1	

ex5.3: bin/ex5.3
	./bin/ex5.3	

ex5.4: bin/ex5.4
	./bin/ex5.4	

projet: bin/projet
	./bin/projet	


clean:
	rm -rf bin/
