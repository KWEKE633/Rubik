# CXX = c++
# CXXFLAGS = -Wall -Wextra -Werror -std=c++20
# NAME = Rubik
# SRC = main.cpp cube.cpp opengl_main.cpp
# OBJ = $(SRC:.cpp=.o)

# all: $(NAME)

# $(NAME): $(OBJ)
# 	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re



CXX = c++
CC = cc
CXXFLAGS = -Wall -Wextra -Werror -std=c++20
CFLAGS = -Wall -Wextra -Werror

# ===== Homebrewのパスを自動検出 (macOS用) =====
BREW_PREFIX = $(shell brew --prefix)

# ヘッダーファイルの検索パス (-I)
CPPFLAGS = -I$(BREW_PREFIX)/include -I./include

# ライブラリファイルの検索パス (-L)
LDFLAGS = -L$(BREW_PREFIX)/lib

NAME = rubik_viewer

SRC = main.cpp cube.cpp glad.c
OBJ = main.o cube.o glad.o
HEADERS = cube.hpp

# ===== OpenGL連携用のライブラリフラグ (-l) =====
LDLIBS = -lglfw

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Linking executable: $@"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp $(HEADERS)
	@echo "Compiling C++: $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	@echo "Compiling C: $<"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	rm -f $(OBJ)

fclean: clean
	@echo "Cleaning executable..."
	rm -f $(NAME)

re: fclean all