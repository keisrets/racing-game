// initializing server struct on client side
void initialize_client(struct sockaddr_in *serverStruct, int port);

// initializing server struct on server side
void initialize_server(struct sockaddr_in *serverStruct, int port);

// client - check if start command received
bool c_game_start(char *s_msg);