CXX= g++
CXXFLAGS= -Wall -Wextra -Wpedantic -std=c++17
LEAK= -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address -fsanitize=undefined

PROG=tree-ftp-${VERSION}
VERSION=0.1.0


SRCDIR= src
INCDIR= include
TESTDIR= tests
BUILDDIR = build
BINDIR= bin

SRC= $(wildcard $(SRCDIR)/*.cpp)
HSRC= $(wildcard $(INCDIR)/*.hpp)
OBJ= $(addprefix $(BUILDDIR)/,$(SRC:.cpp=.o))
DEP = $(OBJ:.o=.d)


all: $(BUILDDIR) $(BINDIR) $(PROG)

dev: $(SRC)
	$(CXX) $(CXXFLAGS) -g $^ -o bin/$(PROG)

leak:
	$(CXX) $(CXXFLAGS) $(LEAK) -g $(SRC) -o bin/$(PROG)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(PROG): $(OBJ)
	$(CXX) $^ -o bin/$(PROG)

$(OBJ): $(BUILDDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILDDIR) *.o *.out $(BINDIR)

version:
	@echo "$(PROG) version $(VERSION)"

.PHONY: all dev leak clean test test_memory_leak version