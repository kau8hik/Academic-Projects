#------------------------------------------------------------------------------

MYLIBRARIES=socket -lresolv -lnsl
CC=g++

#------------------------------------------------------------------------------

all: server peer_1 peer_2 peer_3 peer_4 mirror 
peer_1: peer_1.cpp
	$(CC) peer_1.cpp -o Peer_1 -l$(MYLIBRARIES)

peer_2: peer_2.cpp
	$(CC) peer_2.cpp -o Peer_2 -l$(MYLIBRARIES)

peer_3: peer_3.cpp
	$(CC) peer_3.cpp -o Peer_3 -l$(MYLIBRARIES)

peer_4: peer_4.cpp
	$(CC) peer_4.cpp -o Peer_4 -l$(MYLIBRARIES)

server: server.cpp
	$(CC) server.cpp -o Server -l$(MYLIBRARIES)

mirror: mirror.cpp
	$(CC) mirror.cpp -o Mirror -l$(MYLIBRARIES)



