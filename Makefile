CXX = g++
CXXFLAGS = -std=c++11
SRC_FILES = Mesh.cpp main.cpp
OUT = my_program

all: $(OUT)

$(OUT): $(SRC_FILES)
    $(CXX) $(CXXFLAGS) -o $(OUT) $(SRC_FILES)

clean:
    rm -f $(OUT)
