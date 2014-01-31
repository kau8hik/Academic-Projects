1.	Name : Kaushik Nadikuditi


2.	USC ID : 6682132628


3.	Execution


Phase 1
•	make    - Use make to compile all the source(.cpp) files. 
Run the following commands one after the other in different terminals.
•	./Server - Run the Server first before running any of the peers or mirror.
•	./Peer_1 - to run peer1
•	./Peer_2 - to run peer2
•	./Peer_3 - to run peer3
•	./Peer_4 - to run peer4

Phase 2
•	Server will be in phase 2 – no need to run server again.
•	./mirror – to run mirror

Note: The static TCP ports from 21100,22100,23100,24100 have been changed to 31100,32100,33100,34100, since I am unable to connect using the previous ports however I am able to connect with latter ports.


4.	Peers read their username and password form respective files and send their username and password to the server. The server sends back the reply ‘success’ if username and password of the peer are found in the server, else it sends ‘not found’. If a peer gets the success message, it sends its details peer#, ip, tcp port, operating system which server stores in respective files namely content_win.txt, content_mac.txt based on operating system.

Mirror reads username and password from mirror.txt and sends to server, server checks for username and password from the server.txt file and returns ‘success’ or ‘not found’ to mirror. On successful authentication the mirror sends ‘request’, the server sends back the filename containing the information about the mac clients.

The for loop has i=4, so run peer1 to peer4 only once even though one of the peer’s authentication fails(if you run more than 4 times in total for peers, then code doesn’t work. This strategy is used because after phase 1 the server needs to go to phase 2). 

In phase 3 the server reads content_win.txt file and passes second to last line to the peer in first line as a message in socket, similarly does peers. If the file is empty then the server will print “Nothing to send”.  If there is no second line the server/peers send “hi“ to the peer in first line. And the last peer prints “The file is empty”.
No delays are used.
 
Hard coded: 
All the character arrays are of the size 256, maximum length of characters that can be sent between server and peers in 256.

In server: 
•	server input file server.txt ,server output files content_win.txt, content_mac.txt.
•	Server UDP port. 
•	Return message to peer, mirror upon successful validation : ‘success’, upon unsuccessful validation : ‘not found’.

In mirror:
•	mirror input file mirror.txt
•	Server UDP port.

In Peers : 
•	Peer input files peer_1.txt, peer_2.txt, peer_3.txt, peer_4.txt.
•	Server UDP port in peers and TCP port of each peer.

The peer input files peer_1.txt, peer_2.txt, peer_3.txt, peer_4.txt should have a single line with username, operating system and password separated with space between them. The mirror input file mirror.txt should have mirror username and password separated with space.
The server file server.txt should have username and password of each peer and mirror separated by space, in different lines.




5.	The following materials have been referred 

•	Beej’s socket programming guide
•	http://www.cplusplus.com/ 
•	http://www.linuxhowtos.org/manpages/2/socket.htm
•	http://www.freebsd.org/doc/en/books/developers-handbook/sockets-essential-functions.html
•	http://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
•	http://mrbook.org/tutorials/make/
•	http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/udp-client.c
•	http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/udp-server.c
•	http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/tcp-server.c
•	http://www.cs.ucsb.edu/~almeroth/classes/W01.176B/hw2/examples/tcp-client.c

