# @brief Makefile author : Axel LABARRE

CXX= g++
CXXFLAGS= -Wall -Wextra -Wpedantic -std=c++17 -I/opt/homebrew/Cellar/googletest/1.14.0/include
LEAK= -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address -fsanitize=undefined
GTEST_LIBS= -L/opt/homebrew/Cellar/googletest/1.14.0/lib -lgtest -lgtest_main -pthread


PROG=tree-ftp
VERSION=0.7.0


SRCDIR= src
INCDIR= include
TESTDIR= test
BUILDDIR = build
BINDIR= bin

SRC= $(wildcard $(SRCDIR)/*.cpp)
HSRC= $(wildcard $(INCDIR)/*.hpp)
OBJ= $(addprefix $(BUILDDIR)/,$(SRC:.cpp=.o))
DEP = $(OBJ:.o=.d)
TEST_SRC = $(wildcard $(TESTDIR)/*.cpp)
TEST_OBJ = $(TEST_SRC:.cpp=.o)
TEST_EXEC = test_${PROG}

# Récupérer tous les fichiers .cpp dans SRC_DIR
SRC_FILES = $(wildcard $(SRCDIR)/*.cpp)
# Convertir les fichiers .cpp en .o
SRC_OBJ = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/src/%.o,$(SRC_FILES))
# Exclure main.o
SRC_OBJ := $(filter-out $(BUILDDIR)/src/main.o, $(SRC_OBJ))




all: $(BUILDDIR) $(BINDIR) $(PROG)

dev: $(SRC)
	$(CXX) $(CXXFLAGS) -g $^ -o $(BINDIR)/$(PROG)

test: $(TEST_OBJ) $(SRC_OBJ)
	$(CXX) $(CXXFLAGS) $(GTEST_LIBS) $^ -o $(BINDIR)/$(TEST_EXEC)
	./$(BINDIR)/$(TEST_EXEC)

leak:
	$(CXX) $(CXXFLAGS) $(LEAK) -g $(SRC) -o $(BINDIR)/$(PROG)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(PROG): $(OBJ)
	$(CXX) $^ -o bin/$(PROG)

$(OBJ): $(BUILDDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour compiler les fichiers sources
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour compiler les objets de test
$(TESTDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILDDIR) $(TESTDIR)/*.o *.o *.out $(BINDIR)

version:
	@echo "$(PROG) version $(VERSION)"

.PHONY: all dev leak clean test test_memory_leak version