
BUILD_DIR=./build
SOURCE=$(wildcard ./src/*.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SOURCE))
INCLUDES=-I./include

# CC := g++
# TARGET = client
# $(info SOURCE = $(SOURCE))
# $(TARGET):$(OBJ)
# 	$(CC) $(OBJ) -o $(TARGET)
# %.o:%.cpp
# 	$(CC) $(INCLUDES) -c $(SOURCE)

.PHONY:clean
clean:
	@echo $(SOURCE)
	@echo $(OBJ)
