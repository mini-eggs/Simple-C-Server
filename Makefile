CXX = gcc
CCoptions = -pedantic -Wall -Wextra
SERVER = server
SERVER_ENTRY = server.c
CLIENT = client
CLIENT_ENTRY = client.c
DEPS_SRC = err.c
DEPS = err.o

all: $(SERVER) $(CLIENT)

$(SERVER): $(DEPS) $(SERVER_ENTRY)
	$(CXX) $(CCoptions) $(DEPS) $(SERVER_ENTRY) -o $(SERVER)

$(CLIENT): $(DEPS) $(CLIENT_ENTRY)
	$(CXX) $(CCoptions) $(DEPS) $(CLIENT_ENTRY) -o $(CLIENT)

$(DEPS): $(DEPS_SRC)
	$(CXX) -c $(DEPS_SRC) -o $(DEPS)

clean: 
	rm $(SERVER) $(CLIENT) $(DEPS)
