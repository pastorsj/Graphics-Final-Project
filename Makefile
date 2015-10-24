# This make file will compile all .cpp and .c files in the directory. Be aware, the C++ compiler is used for both C and C++ files. Resulting objects are then linked into a single executable.

# make settings
NEWLINE = @echo ""

# compile settings
CXX?=g++
LINK=g++
PROGRAM=a.out
DEL=rm -rf
LIBS=-lm -ldl -lsfml-graphics -lsfml-window -lsfml-system -lGLU -lGL
WARN=-w
OP=-O0
FLAGS= $(WARN) $(OP)

# setup object list
LOCAL_CPP_OBJECTS= $(patsubst %.cpp, %.o, $(wildcard ./src/*.cpp))
LOCAL_C_OBJECTS= $(patsubst %.c, %.o, $(wildcard ./src/*.c))
GLEW_OBJECTS= $(patsubst %.c, %.o, $(wildcard ./src/glew/*.c)) # glew objects which we will use later
LOAD_OBJECTS= $(patsubst %.cpp, %.o, $(wildcard ./src/objload/*.cpp)) # load objects which we will use later
GL3W_OBJECTS= $(patsubst %.c, %.o, $(wildcard ./src/gl3w/*.c)) # glew objects which we will use later
OBJECTS=$(LOCAL_C_OBJECTS) $(LOCAL_CPP_OBJECTS) $(GLEW_OBJECTS) $(GL3W_OBJECTS) $(LOAD_OBJECTS)

# make targets
makeall:
	make $(PROGRAM)
	$(NEWLINE)

$(PROGRAM): $(OBJECTS) 
	@echo "--$(PROGRAM)--"
	$(LINK) $(FLAGS) $(OBJECTS) $(LIBS) -o $(PROGRAM)

%.o: %.cpp
	@echo "--$@--"
	$(CXX) -c $(FLAGS) $< $(LIBS) -o $@
	$(NEWLINE)

%.o: %.c
	@echo "--$@--"
	$(CXX) -c $(FLAGS) $< $(LIBS) -o $@
	$(NEWLINE)

# clean
.PHONEY: clean
clean:
	$(DEL) $(OBJECTS)

.PHONEY: cleanall
cleanall:
	$(DEL) $(OBJECTS)
	$(DEL) $(PROGRAM)
