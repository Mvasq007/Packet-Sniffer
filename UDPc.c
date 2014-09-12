#include <stdio.h>          /*for printf() and fprintf() */
#include <sys/socket.h>     /*for socket(), connect(), send(), recv()*/
#include <arpa/inet.h>    /*for sockaddr_in and inet_addr()*/
#include <stdlib.h>    /*for atoi()*/
#include <string.h>        /*for memset()*/
#include <unistd.h>       /*for close()*/
 
#define RCVBUFFERSIZE 32    /* Size of the receive buffer*/

 
#define ECHOMAX 255 /*the max number of the string to be echoed*/
static void tvsub(register struct timeval *out, register struct timeval *in);

int ac;
char **av;

long count = 0; //count for number of pings sent
long average = 0; //this is actually the total, divides by  count to get average
int MAX = 0; //command line parameter, average will be output every MAX pings

void* client(void *thread_ID)
{
	long ID = (long)thread_ID;
   // printf("%d has connected", ID);
    int sock;
    char *servIP;
    int servPort;
    struct sockaddr_in UDPServ;
    struct sockaddr_in fromAddr;    /*the addres of the server that replies to the message send from the client*/
    int fromSize;          
    char sendmsg[1000];
    char rcvmsg[1000];
    int rv;
    int c;
 
struct timeval newtime;
struct timeval oldtime;
long ping;
    if((ac<4)||(ac>5))
    {
        fprintf(stderr,"%s <IP> <Port Number>, NUM_THREADS, #of Pings",av[0]);
        exit(1);
    }
 
    servIP=av[1];
    if(ac==5)
        servPort=atoi(av[2]);
    else
        servPort=7;
 
    /*creation of the UDP socket*/
    if((sock=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<=0)
    {
        perror("socket() failed");
        exit(1);
    }
 
    /* it is a UDP socket so we don't need to connect it*/
    /*send data through this socket*/
    memset(&UDPServ,0,sizeof(UDPServ));
    UDPServ.sin_family=AF_INET;
    UDPServ.sin_port=htons(servPort);
    UDPServ.sin_addr.s_addr=inet_addr(servIP);
    while(1) {
        memset(&sendmsg, '\0', sizeof(sendmsg));
        char s[RCVBUFFERSIZE];

        //UNCOMMENT THIS FOR MANUAL INPUT
	//system("/bin/stty raw");   
        //c = getchar();
        //system("/bin/stty cooked");
	//JUST PASSES IN NEW LINES AT THE MOMENT;
	
	c = 65+ID; //makes each client output a different letter based on its ID
	
	//printf("message: %c ", c); 
        //int r = rand()%5;
	//printf("%d is sleeping for %d ", ID, r);
	//sleep(r);
        //if(c == 27) exit(0);
        char q = c;
        s[0] = q;
        strcpy(sendmsg,s);
        //putchar(10);
        (void)gettimeofday(&oldtime, (struct timezone *)NULL); //gets current time
        if(sendto(sock,sendmsg,strlen(sendmsg),0,(struct sockaddr *)&UDPServ,sizeof(UDPServ))!=strlen(sendmsg))
        {
            perror("sendto() failed");
            exit(1);
        }

        fromSize=sizeof(fromAddr);
        //memset(&rcvmsg, '\0', sizeof(rcvmsg));   
        rv = recvfrom(sock,rcvmsg,strlen(rcvmsg),0,(struct sockaddr *)&fromAddr,&fromSize);

    //ping = difftime(newtime,oldtime);
    (void)gettimeofday(&newtime, (struct timezone *)NULL); //gets current time
    tvsub(&newtime,&oldtime); //subtracts oldtime from new time to get ping
    ping = newtime.tv_sec * 1000000 + (newtime.tv_usec);
    //tv_sec is seconds while tv_usec is microseconds, this line converts it
    //all to microseconds
    average+=ping; //ping time total
	count++;
    if(count>=MAX)
      {
	 
	 if(ID == 0){ printf ("%ld.%03ld average of the last %ld pings\n", (average/count)/1000,(average/count)%1000,count);}
	 count = 0;
	 average = 0;
      }

	//printf ("%ld.%03ld ms passed, msg sent by %d, count = %ld\n", ping/1000,ping%1000, ID, count);
        if(UDPServ.sin_addr.s_addr!=fromAddr.sin_addr.s_addr)
        {
            perror("The server that replied was not the one that was supposed to do so");
            exit(1);
        }
        rcvmsg[rv] = 0;
        //printf("%d \n",rv);

    }
    close(sock);
    exit(0);
}
             
static void
tvsub(register struct timeval *out, register struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0) {
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}     



int main(int argc,char *argv[])
{
	 int NUM_THREADS = atoi(argv[3]);
	 ac = argc;
	 av = argv;
	 int rc;
	 MAX = atoi(argv[4]);
	 pthread_t * thready = (pthread_t *)malloc(sizeof(pthread_t));
	 
    	void* status;
	int i;
 	pthread_t * thread = (pthread_t *)malloc(sizeof(pthread_t)*NUM_THREADS);
    for ( i = 0; i < NUM_THREADS; i++) { rc = pthread_create(&thread[i], NULL, client, (void*)(intptr_t)i);}
    for (i = 0; i < NUM_THREADS; i++)  {pthread_join(thread[i], &status);}	
    

}
