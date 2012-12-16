OR_PATH = /home/mario/Scrivania/or-tools/
CC		= g++
PRG		= solver
SRC_DIR	= ./src
INC_DIR	= ./inc
OBJ_DIR = ./obj

CFLAGS=	-fPIC -O3 -DNDEBUG -I$(INC_DIR) \
		-I$(OR_PATH)src -I$(OR_PATH)src/gen  \
		-I$(OR_PATH)dependencies/install/include -DARCH_K8 \
		-I$(OR_PATH)dependencies/sources/TinyThread++-1.0/source \
		-I$(OR_PATH)dependencies/sources/Minisat \
		-Wno-deprecated -I$(OR_PATH)dependencies/install/include \
		-I$(OR_PATH)dependencies/install/include -DUSE_CBC \
		-I$(OR_PATH)dependencies/install/include -DUSE_CLP \
		-I$(OR_PATH)dependencies/install/include -DUSE_GLPK \
		-I$(OR_PATH)dependencies/install/include

LDFLAGS=-L'/usr/local/lib' -lgflags -lprotobuf -lpthread -lrt -lm \
		-Wl,-rpath $(OR_PATH)lib -L$(OR_PATH)lib -lconstraint_solver \
		-Wl,-rpath $(OR_PATH)lib -L$(OR_PATH)lib -llinear_solver \
		-Wl,-rpath $(OR_PATH)lib -L$(OR_PATH)lib -lutil \
		-Wl,-rpath $(OR_PATH)lib -L$(OR_PATH)lib -lbase \
		-Wl,-rpath $(OR_PATH)lib -L$(OR_PATH)dependencies/install/lib -lglpk \
		-Wl,-rpath $(OR_PATH)lib -L$(OR_PATH)dependencies/install/lib -lCbcSolver \
		-lCbc -lCgl -lOsi -lOsiCbc -lOsiClp \
		-Wl,-rpath $(OR_PATH)dependencies/install/lib -L$(OR_PATH)dependencies/install/lib -lClp -lCoinUtils

SRC_FILES := $(shell ls $(SRC_DIR)/*.cpp;)
OBJ_FILES := ${SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o}

all: proto hardware solver

solver: $(OBJ_DIR) $(OBJ_FILES)
	@$(CC) $(CFLAGS) -c prog/solver.cpp -o $(OBJ_DIR)/solver.o
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) $(OBJ_DIR)/solver.o -o $(PRG)

hardware: $(OBJ_DIR) $(OBJ_FILES)
	@$(CC) -I$(INC_DIR) -c prog/hardware.cpp $(CFLAGS) -o $(OBJ_DIR)/hardware.o
	@$(CC) -I$(INC_DIR) -c prog/hardware_viewer.cpp $(CFLAGS) -o $(OBJ_DIR)/hardware_viewer.o
	@$(CC) $(LDFLAGS) -o hardware $(OBJ_FILES) $(OBJ_DIR)/hardware.o
	@$(CC) $(LDFLAGS) -o hardware_viewer $(OBJ_FILES) $(OBJ_DIR)/hardware_viewer.o

proto:
	@protoc ./taskgraph.proto --cpp_out=.
	@mv taskgraph.pb.h inc/
	@mv taskgraph.pb.cc src/taskgraph.pb.cpp
	@protoc ./hardware.proto --cpp_out=.
	@mv hardware.pb.h inc/
	@mv hardware.pb.cc src/hardware.pb.cpp

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(PRG)
	@rm -f hardware
	@rm -f hardware_viewer
	@rm -Rf $(OBJ_DIR)

