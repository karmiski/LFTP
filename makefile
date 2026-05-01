CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

CLIENT = cliv
SERVER = serv

CLIENT_SRC = cliv.cpp config_module.cpp command_module.cpp network_module.cpp
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

SERVER_SRC = serv.cpp network_module.cpp
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)

# =========================
# BUILD ALL
# =========================
all: $(CLIENT) $(SERVER)

# =========================
# CLIENT TARGET
# =========================
$(CLIENT): $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# =========================
# SERVER TARGET
# =========================
$(SERVER): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# =========================
# COMPILE RULE
# =========================
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# =========================
# CLEAN
# =========================
clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(CLIENT) $(SERVER)

.PHONY: all clean