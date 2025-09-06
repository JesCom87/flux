CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC = core/compiler.cpp
OUT = fluxc

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
