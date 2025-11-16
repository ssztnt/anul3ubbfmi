CXX = g++
CXXFLAGS = -std=c++11 -pthread -O2 -Wall
TARGET = main
TARGET2 = main2
TARGET_ENHANCED = main_enhanced
SOURCE = main.cpp
SOURCE2 = main2.cpp
SOURCE_ENHANCED = main_enhanced.cpp

all: $(TARGET) $(TARGET2) $(TARGET_ENHANCED)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

$(TARGET2): $(SOURCE2)
	$(CXX) $(CXXFLAGS) $(SOURCE2) -o $(TARGET2)

$(TARGET_ENHANCED): $(SOURCE_ENHANCED)
	$(CXX) $(CXXFLAGS) $(SOURCE_ENHANCED) -o $(TARGET_ENHANCED)

clean:
	rm -f $(TARGET) $(TARGET2) $(TARGET_ENHANCED)

run: $(TARGET)
	./$(TARGET) 4

sequential: $(TARGET)
	./$(TARGET) 0

# Test cu exemplul din laborator
test_example: $(TARGET_ENHANCED)
	@echo "=== Test cu exemplul din laborator (3x3 kernel pe 5x5 matrice) ==="
	./$(TARGET_ENHANCED) 0 test_example.txt

# Test de performanta cu matrici mari
test_performance: $(TARGET_ENHANCED)
	@echo "=== Test de performanta ==="
	@echo "Secvential:"
	./$(TARGET_ENHANCED) 0
	@echo "\nCu 2 thread-uri:"
	./$(TARGET_ENHANCED) 2
	@echo "\nCu 4 thread-uri:"
	./$(TARGET_ENHANCED) 4
	@echo "\nCu 8 thread-uri:"
	./$(TARGET_ENHANCED) 8

.PHONY: all clean run sequential test_example test_performance

