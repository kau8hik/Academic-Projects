/* mirror */

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
//#define tcp_port "21728"
using namespace std;


int main(){	
														//socket and other variable initializations
  	int sockfd,cli_sockfd,n;
 	int i=0,j=0,k=0;
	char* username= (char*) malloc(256);
	char* user= (char*) malloc(256);  	
 	char* sendline = (char*) malloc(256);
 	char* recvline= (char*) malloc(256);
	char* recvline1= (char*) malloc(256);

 	char* machine= (char*) malloc(256); 
	string machine1;
 	string line;	

	struct sockaddr_in cli;
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
	myfile.open ("mirror.txt");
	if (myfile.is_open())
   	{
     		while ( getline (myfile,line) )
     		{
			strcpy(sendline, line.c_str());
			//cout<<sendline<<endl;
		}	
     	}
	i=0;j=0;
	while(sendline[i]){											//getting only username to identify the client
		user[i]=sendline[i];i++;				
		if(sendline[i]==' ')break;
	}
	
	
	cout<<"\nMirror Phase 2: The dynamic UDP port number "<<ntohs(cli.sin_port)<<" and IP address "<<ipaddr1<<endl; //dynamic port 
	
     	myfile.close();
	
	cout<<"Mirror Phase 2: Sending the authentication information to the server < "<<sendline<<" >"<<endl;
	sendto(sockfd,sendline,strlen(sendline),0,(struct sockaddr *)&servaddr,sizeof(servaddr));	
	n=recvfrom(sockfd,recvline,256,0,(struct sockaddr *)&servaddr,&len);				//storing the return message from server to recvline
       recvline[n]=0;
	
	char send_ch[100];strcpy(send_ch,"request");	

	if(strcmp(recvline,"success")==0){
		cout<<"Mirror Phase 2: Authentication successful\n";
		cout<<"Mirror Phase 2: seeking information about the mac clients"<<endl;
		sendto(sockfd,send_ch,strlen(send_ch),0,(struct sockaddr *)&servaddr,sizeof(servaddr));//upon successful authentication sending compiled message
		n=recvfrom(sockfd,recvline1,256,0,(struct sockaddr *)&servaddr,&len);//cout<<recvline1;
		recvline1[n]=0;

		//string a;strcpy(a,recvline1.c_str());
		cout<<"Mirror Phase 2: File name recieved form the server. The contents of the file as follows\n";
		myfile.open(recvline1);
		if(myfile.is_open())
		{
			while(getline(myfile,line))
				//cout<<line<<endl;
				cout<<"\""<<line<<"\""<<endl;
		}

	} 
	else {
		cout<<"Mirror Phase 2: Authentication failed\n";	
		exit(EXIT_FAILURE);
	}
				//upon usuccessful authentication
	cout<<"\nMirror Phase 2: End of Phase 2\n"<<endl;
	




	int sockfd1;
	int s=sockfd1=socket(AF_INET,SOCK_STREAM,0);
	if(s==-1){
		cout<<"unable to create socket\n";
		exit(EXIT_FAILURE);
	}
   	//cout<<s;
	
   	memset(&servaddr,0,sizeof(servaddr));
   	servaddr.sin_family = AF_INET;
   	servaddr.sin_addr.s_addr=inet_addr(ipaddr1);
	bind(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr));
	getsockname(sockfd1, (struct sockaddr *)&servaddr, &len);
	//cout<<"still in\n";
	fstream f;
	f.open(recvline1);
	string line1,outa;
	int no_cli=0;
	if(f.is_open())
	{
		while(getline(f,line1))
			{outa=outa+" "+line1;no_cli++;}
	}
	//cout<<"still in"<<endl;

	if(no_cli==0)
	cout<<"Mirror Phase 3 : Nothing to send\n";
	//cout<<"outa:"<<outa<<endl;
	//cout<<outa.length()<<endl;
	else{
	  char sendl[outa.length()-1];
	  i=0;j=1;
	
	  while(i<outa.length()-1){
	 	sendl[i]=outa.at(j);i++;j++;
	  }sendl[i]='\0';
	  //cout<<"sendl:"<<sendl<<endl;
	  int no=i;
	  //cout<<"no'''''''''''''''"<<no<<endl;
	  i=0;char ipaddr[1000],port[1000];k=0;
	  //cout<<"n:"<<no<<endl;
	  if(no>5)
	  {
		char name[20];
		while(sendl[i]!=' '){name[i]=sendl[i];i++;}name[i]='\0';i++;
		while(sendl[i]!=' '){
		 ipaddr[k]=sendl[i];
			i++;k++;
		}i++;
		ipaddr[k]='\0';
		//cout<<"ipaddr"<<ipaddr<<endl;
		k=0;
		while(sendl[i]!=' '){
			port[k]=sendl[i];i++;k++;
		}
		port[k]='\0';
		i++;
		
		//cout<<"port"<<port<<endl;
		while(sendl[i]!=' '&&i<outa.length()-1)i++;i++;
		int k=i;
		//cout<<"k"<<k<<endl;
		while(i<no)i++;
		//cout<<"i:"<<i<<endl;
		char mesg_send[i-k];
		//cout<<"i-k"<<i-k<<endl;
		j=0;
		while(k<outa.length()){
			mesg_send[j]=sendl[k];k++;j++;
		}
		mesg_send[j]='\0';
		
		cout<<"\nMirror Phase 3: Dynamic TCP port number "<<ntohs(servaddr.sin_port)<<" and IP address "<<ipaddr1<<endl;
		cout<<"Mirror Phase 3: There are "<<no_cli<<" Mac OS clients" <<endl;
		char *end;
		double p=strtod(port,&end);
		cout<<"Mirror Phase 3: Sending file to "<<name<<" having IP address "<<ipaddr<<" and static TCP port number "<<p<<endl;
		servaddr.sin_port=htons(p);
			
		//cout<<"mesg_send:"<<mesg_send<<endl;
   		int c=connect(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr));
		if(c==-1){
			cout<<"unable to connect to peers\n";
			exit(EXIT_FAILURE);
		}

		//cout<<"connect:"<<c<<endl;


		if(sizeof(mesg_send)<5){
			char mesg_s[9]={'M','i','r','r','o','r',' ','h','i'};
			mesg_s[9]='\0';
			//cout<<"mesg_s:"<<mesg_s<<endl;			
			int t=send(sockfd1,mesg_s,sizeof mesg_s,0);
			//send(sockfd1,"server",sizeof "server",0);

       		//cout<<"st:"<<t<<endl;  			
		}
		else{
			int m_s=sizeof mesg_send;
			char mesg_se1[m_s+7];
			mesg_se1[0]='M';mesg_se1[1]='i';mesg_se1[2]='r';mesg_se1[3]='r';mesg_se1[4]='o';mesg_se1[5]='r';mesg_se1[6]=' ';
			
			int ms=7;i=0;
			while(ms<sizeof mesg_send+8){
				mesg_se1[ms]=mesg_send[i];ms++;i++;
			}
			int t=send(sockfd1,mesg_se1,sizeof mesg_se1,0);
			//send(sockfd1,"server",sizeof "server",0);
       		//cout<<"st:"<<t<<endl;  
		}	
		char rec3[1000];
		n=recv(sockfd1,rec3,sizeof rec3,0);
		rec3[n]='\0';
      		if(rec3[0]=='h')
   		cout<<"Mirror Phase 3: File transfer successful"<<endl;
				
		
	  }
	}
	cout<<"Mirror Phase 3: End of Phase3\n";
	close(sockfd1);
	
	return 1;

}

