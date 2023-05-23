#makefile
CC = g++
CPPFLAGS = -g -lncurses -std=c++14 

TARGET = snake-game #실행 파일의 이름
OBJS = src/input-manager.o src/render-manager.o src/stage-manager.o src/snake-game.o src/map-manager.o


$(TARGET) : $(OBJS)
	$(CC) $^ -o $@ $(CPPFLAGS)
	mv snake-game ../snake-game
	
%.o : src/%.cpp
	$(CC) $(CPPFLAGS) -c $^

clean :
	-rm -f $(OBJS)



# #makefile
# CC = g++
# CPPFLAGS = -g -lncurses -std=c++14 

# TARGET = snack-game #실행 파일의 이름
# OBJS = src/input-manager.o src/render-manager.o src/stage-manager.o src/snake-game.o


# $(TARGET) : $(OBJS)
# 	$(CC) $^ -o $@ $(CPPFLAGS)
# 	mv snack-game ../snack-game
	
# %.o : src/%.cpp
# 	$(CC) $(CPPFLAGS) -c $^

# clean :
# 	-rm -f $(OBJS)