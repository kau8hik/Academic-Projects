/* Client peer_3 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>

#define udp_port 5728
#define tcp_port "33728"
#define static_tcp_port 33728
using namespace std;


int main(){	
														//socket and other variable initializations
  	int sockfd,cli_sockfd,n;
 	int i=0,j=0,k=0;
	char* username= (char*) malloc(256);
	char* user= (char*) malloc(256);  	
 	char* sendline = (char*) malloc(256);
	char* sendudp = (char*) malloc(256);

 	char* recvline= (char*) malloc(256);
	char* recvline1= (char*) malloc(256);

 	char* machine= (char*) malloc(256); 
	string machine1;
 	string line;	

	struct sockaddr_in cli,cliaddr;
	cli_sockfd=socket(AF_INET,SOCK_DGRAM,0);
	socklen_t len = sizeof(cli);
	if (getsockname(cli_sockfd, (struct sockaddr *)&cli, &len) == -1)
    		perror("ERROR IN GETTING SOCKNAME");	
	short unsigned int d_port;
	cli.sin_family = AF_INET;
	cli.sin_addr.s_addr = htonl(INADDR_ANY);
	cli.sin_port = 0;
	bind(cli_sockfd, (struct sockaddr *)&cli, sizeof(cli));
	len = sizeof(cli);
	getsockname(cli_sockfd, (struct sockaddr *)&cli, &len);	

	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	struct hostent *he;
	struct in_addr **addr_list;
	char hostname1[256];
    	gethostname(hostname1, sizeof hostname1);

	if ((he = gethostbyname(hostname1)) == NULL) {  							// get the host info
        	perror("gethostbyname");
        	return 2;
    	}
														
	char *ipaddr1;
	addr_list = (struct in_addr **)he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) ipaddr1=inet_ntoa(*addr_list[i]);			//ipaddress in ipaddr1 variable
	
	i=0;
	char peer[256];
	servaddr.sin_addr.s_addr=inet_addr(he->h_name);
  	servaddr.sin_port=htons(udp_port);	
														//file opening and reading from file
	fstream myfile;
	myfile.open ("peer_3.txt");
	if (myfile.is_open())
   	{
     		while ( getline (myfile,line) )
     		{
			strcpy(sendline, line.c_str());
			while(sendline[i]){
				username[j]=sendline[i];j++;						//getting only username and password
				if(sendline[i]==' '){
					i++;
					while(sendline[i]!=' ') {
						machine[k]=sendline[i];k++;					//getting windows or mac
						i++;
					}
				}
			i++;
			}
		}	
     	}
	i=0;j=0;
	while(sendline[i]){											//getting only username to identify the client
		user[i]=sendline[i];i++;				
		if(sendline[i]==' ')break;
	}
	

	if(*machine=='w') machine1="Windows";
	else machine1="Mac_OS";
	
	cout<<"\nPeer3 Phase 1: The dynamic UDP port number "<<ntohs(cli.sin_port)<<" and IP address "<<ipaddr1<<endl; //dynamic port 0
	
     	myfile.close();
	sendudp[0]='P';sendudp[1]='e';sendudp[2]='e';sendudp[3]='r';sendudp[4]='1';sendudp[5]=' ';
	int u=0,v=6;
	while(u<sizeof(username)+7)
	{
		sendudp[v]=username[u];u++;v++;
	}
	//cout<<"sendudp:"<<sendudp<<endl;
	cout<<"Peer3 Phase 1: Sending the authentication information to the server < "<<username<<" >"<<endl;
	sendto(sockfd,sendudp,strlen(sendudp),0,(struct sockaddr *)&servaddr,sizeof(servaddr));	//sending username and password to the server
	
	n=recvfrom(sockfd,recvline,256,0,(struct sockaddr *)&servaddr,&len);				//storing the return message from server to recvline
       recvline[n]=0;
	
	string se="Peer3 "+(string) ipaddr1+" "+(string) tcp_port+" "+machine1;	// compiling message <username, ipaddress, tcp port, windows/mac>
	char send_ch[100];strcpy(send_ch,se.c_str());	

	if(strcmp(recvline,"success")==0){
		cout<<"Peer3 Phase 1: Authentication successful\n";
		cout<<"Peer3 Phase 1: Sending the file transfer information to the server < "<<send_ch <<" >"<<endl;
		sendto(sockfd,send_ch,strlen(send_ch),0,(struct sockaddr *)&servaddr,sizeof(servaddr));//upon successful authentication sending compiled message
		n=recvfrom(sockfd,recvline1,256,0,(struct sockaddr *)&servaddr,&len);//cout<<recvline1;
		recvline[n]=0;

		//string a;strcpy(a,recvline1.c_str());
		if(recvline1[0]=='w')cout<<"Peer3 Phase 1: Information received by the server\n";
	} 
	else {
		cout<<"Peer3 Phase 1: Authentication failed\n";
		cout<<"Peer3 Phase 1: End of Phase 1\n";
	
		exit(EXIT_FAILURE);					//upon usuccessful authentication
	}
	cout<<"Peer3 Phase 1: End of Phase 1\n"<<endl<<endl;
	close(sockfd);	
	

	int listenfd,connfd,sockfd1;
	struct sockaddr_in tcp_client, tcp_server,cliaddr_tcp;

   	socklen_t clilen;
   	pid_t     childpid;

   	listenfd=socket(AF_INET,SOCK_STREAM,0);
	

	cout<<"Peer3 Phase 3: The Peer3 has static TCP port number "<<static_tcp_port<<" and IP address "<<ipaddr1<<endl;
 	memset(&tcp_server,0,sizeof(tcp_server));     
    	tcp_server.sin_family = AF_INET;

       tcp_server.sin_addr.s_addr=inet_addr(ipaddr1);
       tcp_server.sin_port=htons(static_tcp_port);
	//cout<<"where am i\n";
  
   
   	bind(listenfd,(struct sockaddr *)&tcp_server,sizeof(tcp_server));

   	listen(listenfd,1024);
	
  	char mesg1[1000];
	char mesgn[1000];
	char from[10];int m=0,fr=0;
      	//int clilen=sizeof(cliaddr);
      	connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
	
      	if ((childpid = fork()) == 0)
      	{
         	
              m = recv(connfd,mesgn,sizeof mesgn,0);
		mesgn[m]='\0';
		//cout<<"mesgn:"<<mesgn<<m<<endl;
		while(mesgn[fr]!=' '){
			from[fr]=mesgn[fr];
			fr++;
		}
		from[fr]='\0';
		//cout<<"from:"<<from<<endl;
		fr++;
		n=m-fr;int so=0;
		while(fr<m){
			mesg1[so]=mesgn[fr];
			fr++;so++;
		}
		mesg1[so]='\0';
		//cout<<"mesg1:"<<mesg1<<endl;
		
		cout<<"Peer3 Phase 3: Received the file from " <<from<<endl;
		char mesg_a[n];//cout<<"nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn:"<<n<<endl;
		int ii=0,i1=0,cou=0,ik=0;;
		char for_print[n+20];
		//cout<<"mesg1:"<<mesg1<<endl;
		while(i1<n){
			mesg_a[i1]=mesg1[i1];i1++;
		}
		mesg_a[i1]='\0';
		//cout<<"'''''''''''''''''''''''''''''''''''''''''mesg_a:"<<mesg_a<<endl;
		while(ii<n){
			
			for_print[ik]=mesg1[ii];
			if(mesg1[ii]==' '){
				cou++;
				if(cou%4==0){
					for_print[ik]='"';ik++;
					for_print[ik]='\n';ik++;
					for_print[ik]='"';
				}

			}
			ik++;	ii++;		
		}
		for_print[ik]='\0';
		cout<<"Peer3 Phase 3: The contents of the file are as follows: "<<endl;
		if(for_print[0]=='h'&&for_print[1]=='i')
			cout<<"The file is empty\n";
		else{
			string pri1(for_print);//=(string)'\"'+(string)for_print+(string)'\"';
			pri1="\""+pri1+"\"";
			cout<<pri1<<endl;
		}

		//cout<<"---recieved-------------------"<<mesg_a<<endl;
		char ch[1]={'h'};
            	send(connfd,ch,sizeof ch,0);
		close (listenfd);
            	//close(connfd);   
		//close(sockfd);
            	//mesg[n] = 0;
		
            	//cout<<"---recieved-------------------"<<mesg_a<<endl;
		//cout<<"this is what i have recieved:--------------------------"<<mesg1<<endl;
		char ipaddr[50],port[50];
		i=0;k=0;
		
		if(n>5)
		{
			//cout<<"mesg_a:"<<mesg_a<<endl;
			char peer_no[10];
			while(mesg_a[i]!=' '){
				peer_no[i]=mesg_a[i];i++;
			}peer_no[i]='\0';i++;
			while(mesg_a[i]!=' '){
				//cout<<mesg[i]<<endl;
			 	ipaddr[k]=mesg_a[i];
				i++;k++;
			}i++;
			ipaddr[k]='\0';
			//cout<<"ipaddr:"<<ipaddr<<endl;
			k=0;
			while(mesg_a[i]!=' '){
				port[k]=mesg_a[i];i++;k++;
			}i++;
			port[k]='\0';
			//cout<<"port:"<<port<<endl;
			//cout<<"sizeof mesg:"<<sizeof(mesg)<<endl;
			while(mesg_a[i]!=' ' && i<(sizeof(mesg_a)))i++;i++;
			//cout<<"i:"<<i<<endl;
			int ij=0;
			if((n-i)>5) {ij=n-i;}
			else {ij=2;}
			char mesg_send[ij];
			//cout<<"mesg_send size:"<<sizeof(mesg_send)<<endl;
			j=0;
			while(i<n){
				mesg_send[j]=mesg_a[i];i++;j++;
			}
			//cout<<"jjjjjjjjjjjjjjjjjjjjjjjj="<<j<<endl;
			mesg_send[j]='\0';
			
			//cout<<"mesg_send:"<<mesg_send;
			char *end;
			double p=strtod(port,&end);
			
			memset(&tcp_client,0,sizeof(tcp_client));     
    			tcp_client.sin_family = AF_INET;
			int ss=sockfd1=socket(AF_INET,SOCK_STREAM,0);

      			tcp_client.sin_addr.s_addr=inet_addr(ipaddr);
			bind(sockfd1, (struct sockaddr *)&tcp_client, sizeof(tcp_client));
			getsockname(sockfd1, (struct sockaddr *)&tcp_client, &len);
			cout<<"\nPeer3 Phase 3: The peer has dynamic TCP port number "<<ntohs(tcp_client.sin_port)<<" and IP address "<<ipaddr1<<endl;
			cout<<"Peer3 Phase 3: Sending file to "<<peer_no<<" having IP address "<<ipaddr<<" and static TCP port number "<<p<<endl;

			tcp_client.sin_port=htons(p);
			//cout<<"ipaddr:"<<ipaddr<<p<<endl;	
			
			//int s=sockfd1=socket(AF_INET,SOCK_STREAM,0);
			
   			//cout<<"sockfd1:"<<s<<endl;
			
   			int c=connect(sockfd1, (struct sockaddr *)&tcp_client, sizeof(tcp_client));
			//cout<<"connect:"<<c<<endl;
			if(c==-1){
				cout<<"unable to connect to peers\n";
				exit(EXIT_FAILURE);
			}

			if(j<5){
				
				char mesg_s[8]={'P', 'e','e','r','3',' ','h','i'};
				mesg_s[8]='\0';
				int t=send(sockfd1,mesg_s,sizeof mesg_s,0);
				//cout<<"T:"<<t<<endl;
			}
			
			else{
				//cout<<"mesg_send:"<<mesg_send<<endl;

				int m_s=sizeof mesg_send;
				char mesg_se1[m_s+6];
				mesg_se1[0]='P';mesg_se1[1]='e';mesg_se1[2]='e';mesg_se1[3]='r';mesg_se1[4]='3';mesg_se1[5]=' ';
			
				int ms=6;i=0;
				while(ms<sizeof mesg_send+8){
					mesg_se1[ms]=mesg_send[i];ms++;i++;
				}
				int t=send(sockfd1,mesg_se1,sizeof mesg_se1,0);


				//cout<<"mesg_se1:"<<mesg_se1	<<endl;
   				//int t=send(sockfd1,mesg_send,sizeof mesg_send,0);
				//cout<<"T:"<<t<<endl;
			}
       		//cout<<"st:"<<t<<endl;    
			char rec3[10];
			int sockfd4;
			int s=sockfd4=socket(AF_INET,SOCK_STREAM,0);

			n=recv(sockfd1,rec3,sizeof rec3,0);
			//rec3[0]='h';

			rec3[n]='\0';
			//cout<<"N:"<<n<<endl;
			if(rec3[0]=='h')
   			cout<<"Peer3 Phase 3: File transfer successful"<<endl;
			
      			//fputs(rec3,stdout);*/
			
		}
         	close(sockfd1);
		cout<<"Peer3 Phase 3: End of Phase 3\n";
      	}
      	
	//close(listenfd);		
    	return 1; 
}

