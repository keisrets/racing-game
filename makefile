all:	#target name
	gcc -pthread headers/common.c client/client.c headers/errcheck.c headers/game.c headers/packet.c -Wall -o client_exec -lSDL2 -lSDL2_image -lm
	gcc -pthread headers/common.c server/server.c headers/errcheck.c headers/packet.c -Wall -o server_exec -lm
	
