
BUILD_DIR=./build
SRC_DIR=./src
SOURCE=$(wildcard ./src/*.cpp)  #获取目录下所有cpp名
OBJ=$(addprefix $(BUILD_DIR)/, $(notdir $(patsubst %.cpp, %.o, $(SOURCE))))#.cpp替换为build下的.o

INCLUDES=-I./include # 头文件路径

CC := g++
TARGET = server

$(TARGET):$(OBJ)
	$(CC) $(OBJ) -o $(TARGET)
$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CC) $(INCLUDES) -c $< -o $@

.PHONY:clean
clean:
	rm -rf $(OBJ)