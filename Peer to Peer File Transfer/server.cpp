	/* UDP server */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#define udp_port 5728
using namespace std;


int check_in_file(char check[256]){			// function to verify the username and password of the client in server.txt
	string line;
	char sendline[256];
	int flag;
	fstream myfile;
	myfile.open ("server.txt");
   	if (myfile.is_open())
   	{
     		while ( getline (myfile,line) )
     		{
			strcpy(sendline, line.c_str());
			
			flag=strcmp(sendline, check);
			if(flag==0) return flag;

     		}
     	myfile.close();
   	} 
	return flag;
}



int main(){
			
	int sockfd,n;								//socket and other variable initializations
   	struct sockaddr_in servaddr,servaddr_c,cliaddr;
   	socklen_t len;
   	char rec1[256],recu[256],rec2[256];
	
  	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	struct hostent *he;
	struct in_addr **addr_list;
	char hostname1[256];
    	gethostname(hostname1, sizeof hostname1);	
	if ((he = gethostbyname(hostname1)) == NULL) {  // get the host info
        	perror("gethostbyname");
        	return 2;
    	}

	int i=0;
	int some_f=0;
	int k=0;
	char machine123[100];int j=0;

	char *ipaddr1;
	addr_list = (struct in_addr **)he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) ipaddr1=inet_ntoa(*addr_list[i]);

	cout<<"Server Phase 1: The server has static UDP port number "<<udp_port<<" and IP address "<< ipaddr1<<endl<<endl;
  	memset(&servaddr,0,sizeof(servaddr));
  	servaddr.sin_family = AF_INET;
  	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  	servaddr.sin_port=htons(udp_port);
  	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	
   	for (int p=0;p<4;p++)									//continiously listening
   	{
		fstream content_w,content_m;
		content_w.open("content_win.txt",ios::out | ios::app );		//opeing files content_win/content_mac in append mode
		content_m.open("content_mac.txt",ios::out | ios::app );

  	   	len = sizeof(cliaddr);
		
		n = recvfrom(sockfd,recu,256,0,(struct sockaddr *)&cliaddr,&len); //recieving username and passwd from peers
  	    	recu[n] = 0;
		//cout<<"recu:"<<recu<<endl;
		char username[100];
		int l=0;
  	    	//cout<<"rec1:"<<rec1<<endl;;
		while(recu[l]){
			username[l]=recu[l];
			if(recu[l]==' ')
				break;			
			l++;
		}
		username[l]='\0';l++;
		//cout<<"username:"<<username<<endl;
		int u=0;
		while(l<n)
		{
			rec1[u]=recu[l];u++;l++;
		}
		//cout<<"rec1:"<<rec1<<endl;
		char *ch ;		
		int flag=check_in_file(rec1);
		
		
		//cout<<"username:"<<username<<endl;
		if(flag==0){											// if username and passwd of peer found in file server.txt

			cout<<"Server Phase 1: "<<username<<" authentication successful, sending ACK \n";
			ch=(char *)"success";

			sendto(sockfd,ch,strlen(ch),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));  //sending success for successful authentication
			
			n = recvfrom(sockfd,rec2,256,0,(struct sockaddr *)&cliaddr,&len);		//reciving client details <userid, ipaddress, tcp port, win/mac>
			rec2[n] = 0;
			
			string recievd=rec2;			
			cout<<"Server Phase 1: "<<username<<" information received <"<<rec2 <<">"<<endl;
			
			if(recievd[n-7]=='W'){								//checking for windows and writing to content_win.txt
						
				if (content_w.is_open())
  				{
    					content_w<< rec2 <<endl;
    					content_w.close();
  				}
 				 else cout << "Unable to open file";
			}
			if(recievd[n-6]=='M'){								//checking and writing for mac
						
				if (content_m.is_open())
  				{
    					content_m<< rec2 <<endl;
    					content_m.close();
  				}
 				 else cout << "Unable to open file";
			}
			cout<<"Server Phase 1: Information written to the respective files\n"<<endl;	
			char ret[100]="w";
			sendto(sockfd,ret,strlen(ret),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));			
		}
     
		else {												//on unsuccessful authentication
			cout<<"Server Phase 1: "<<username<<" authentication unsuccessful, sending NACK "<<endl<<endl;
			ch=(char *)"not found";
			sendto(sockfd,ch,strlen(ch),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr)); 
		}      
   	}
	cout<<"Server Phase 1: End of Phase 1\n"<<endl;
		cout<<"Server Phase 2: The server has static UDP port number "<<udp_port<<" and IP address "<< ipaddr1<<endl<<endl;
  	   	len = sizeof(cliaddr);
		
		n = recvfrom(sockfd,rec1,256,0,(struct sockaddr *)&cliaddr,&len); //recieving username and passwd from peers
  	    	rec1[n] = 0;
		char *ch ;		
		int flag=check_in_file(rec1);
		char username[100];
	        int l=0;
		while(rec1[l]){
			username[l]=rec1[l];
			if(rec1[l]==' '){
				username[l]='\0';
				break;
			}			
			l++;
		}
		if(flag==0){											// if username and passwd of peer found in file server.txt
			cout<<"Server Phase 2: Mirror authentication successful, sending ACK \n";
			ch=(char *)"success";

			sendto(sockfd,ch,strlen(ch),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));  //sending success for successful authentication
			
			n = recvfrom(sockfd,rec2,256,0,(struct sockaddr *)&cliaddr,&len);		// recieving request from the mirror
			rec2[n] = 0;
			string recievd=rec2;
			if(strcmp(rec2,"request")==0){			
				cout<<"Server Phase 2: Mirror request received, transferring the filename"<<endl;
			
				char ret[100]="content_mac.txt";
				sendto(sockfd,ret,strlen(ret),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
				cout<<"Server Phase 2: Filename was sent to the Mirror" << endl;
			}	
					
		}
     
		else {												//on unsuccessful authentication
			cout<<"Server Phase 2: "<<username<<" authentication unsuccessful, sending NACK "<<endl<<endl;
			ch=(char *)"not found";
			sendto(sockfd,ch,strlen(ch),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr)); 
		}      
			
   	close(sockfd);
	cout<<endl<<"Server Phase 2: End of Phase 2\n";

	

 

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
	
	fstream f;
	f.open("content_win.txt");
	string line,outa;
	int no_cli=0;
	if(f.is_open())
	{
		while(getline(f,line))
			{outa=outa+" "+line;no_cli++;}
	}
	//cout<<no_cli;
	if(no_cli==0)
	cout<<"Server Phase 3: Nothing to send "<<endl;
	else{
	  //cout<<"outa:"<<outa<<endl;
	  char sendl[outa.length()-1];
	  i=0;j=1;
	  while(i<outa.length()-1){
		sendl[i]=outa.at(j);i++;j++;
	  }sendl[i]='\0';
	  //cout<<"sendl:"<<sendl<<endl;
	  int no=i;
	  //cout<<"no'''''''''''''''"<<no<<endl;
	  i=0;char ipaddr[1000],port[1000];k=0;
	  if(no>5)
	  {	
		char name[20];
		while(sendl[i]!=' '){
			name[i]=sendl[i];i++;
		}
		i++;
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
		
		cout<<"\nServer Phase 3: Dynamic TCP port number "<<ntohs(servaddr.sin_port)<<" and IP address "<<ipaddr1<<endl;
		cout<<"Server Phase 3: There are "<<no_cli<<" Windows clients" <<endl;
		char *end;
		double p=strtod(port,&end);
		cout<<"Server Phase 3: Sending file to "<<name<<" having IP address "<<ipaddr<<" and static TCP port number "<<p<<endl;
		servaddr.sin_port=htons(p);
			
		//cout<<"mesg_send:"<<mesg_send<<endl;
   		int c=connect(sockfd1, (struct sockaddr *)&servaddr, sizeof(servaddr));
		if(c==-1){
			cout<<"unable to connect to peers\n";
			exit(EXIT_FAILURE);
		}

		//cout<<"connect:"<<c<<endl;


		if(sizeof(mesg_send)<5){
			char mesg_s[9]={'S', 'e','r','v','e','r',' ','h','i'};
			mesg_s[9]='\0';
			cout<<"mesg_s:"<<mesg_s<<endl;			
			int t=send(sockfd1,mesg_s,sizeof mesg_s,0);
			//send(sockfd1,"server",sizeof "server",0);

       		//cout<<"st:"<<t<<endl;  			
		}
		else{
			int m_s=sizeof mesg_send;
			char mesg_se1[m_s+7];
			mesg_se1[0]='S';mesg_se1[1]='e';mesg_se1[2]='r';mesg_se1[3]='v';mesg_se1[4]='e';mesg_se1[5]='r';mesg_se1[6]=' ';
			
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
   		cout<<"Server Phase 3: File transfer successful"<<endl;
			
		
	  }close(sockfd1);
	}
	cout<<"Server Phase 3: End of Phase3\n";	
	return 1;

}

