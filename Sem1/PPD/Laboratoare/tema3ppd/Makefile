CXX = mpic++
CXXFLAGS = -std=c++20 -Wall
TARGET = Tema_3

SOURCES = main.cpp \
          source/GenerateNumber.cpp \
          source/SequentialCalculation.cpp \
          source/StandardCalculation.cpp \
          source/ScatterCalculation.cpp \
          source/AsyncCalculation.cpp \
          source/OptimizedCalculation.cpp \
          source/Verification.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) *.txt resultAsync.txt resultOptimized.txt

.PHONY: all clean

