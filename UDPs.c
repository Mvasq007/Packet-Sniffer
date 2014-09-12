#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 255
/* ALL THIS THING DOES IS ECHO BACK WHAT IT'S SENT */
int main(int argc,char *argv[])
{
	int sock;
	int servPort;
	struct sockaddr_in ServAddr;
	struct sockaddr_in ClntAddr;
	int sizeClntAddr;
	char msgin[1000];
	char msgout[1000];
	if(argc!=2)
	{
		fprintf(stderr,"%s <Port Number>\n",argv[0]);
		exit(1);
	}
	
	servPort=atoi(argv[1]);

	if((sock=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<=0)
	{
		perror("socket() failed");
		exit(1);
	}
	
	memset(&ServAddr,0,sizeof(ServAddr));
	ServAddr.sin_family=AF_INET;
	ServAddr.sin_port=htons(servPort);
	ServAddr.sin_addr.s_addr=htons(INADDR_ANY);
	
	if(bind(sock,(struct sockaddr *)&ServAddr,sizeof(ServAddr))<0)
	{
		perror("bind() failed");
		exit(1);
	}
	
	for(;;)
	{
		sizeClntAddr=sizeof(ClntAddr);
		/*recvfrom() and sendto()*/
		int n;
        n = recvfrom(sock,msgin,strlen(msgin),0,(struct sockaddr *)&ClntAddr,&sizeClntAddr);
		//printf("Handling client: %s\n",inet_ntoa(ClntAddr.sin_addr));
		msgin[n] = 0;
		strcpy(msgout,msgin);
		if(sendto(sock,msgout,strlen(msgout),0,(struct sockaddr *)&ClntAddr,sizeof(ClntAddr))!=strlen(msgout))
		{
			perror("sendto() failed");
			exit(1);
		}
		msgout[n] = 0;
		//printf("%d \n",n);
		printf("%s \n",msgout);
	}
} 	
