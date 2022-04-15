#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h> 
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

typedef unsigned short u_short;

// packet size 
#define PACKET_SIZE 4096
// The data's size of ICMP packet
#define DATA_SIZE 56
// Max number of packets
#define MAX_NO_PACKETS 6
// delay time
#define MAX_WAIT_TIME 5

/*
    Auther: goudan
    Time:   Fri 15 Apr 2022 15:33:10 -4:00
    FileName: icmp_ping.c
*/


/*
	ping命令
	1. 创建socket
	2. 打包 
			创建结构体
			给结构体赋值（校验和）
	3. 发多个包到目的机
	4. 收包
		4.1 解包
		4.2 要在特定时间段之内收到

*/

int nsend = 0; // The number of packet which had been send 
int nrecv = 0; // The number of packet which had been recv

char sendPacket[PACKET_SIZE];  // The Packet which will be send  ping packet
char recvPacket[PACKET_SIZE];  // The Packet which will be recv  ping packet

struct sockaddr_in fromAddr = {0};

struct timeval trecv; // receive time

int pack(int nsend);  // pack network packet function -> return the length of packet

int send_packet(int socketFd, struct sockaddr_in* pDestaddr, int len); // send packet

u_short cal_cksum(u_short* icmp, int packsize); //The ICMP message checksum method

void recv_packet(int socketFd);   // recv packet

int unpacking(int len, int uid); //unpacking

int tv_sub(struct timeval* out,struct timeval* in); //cal time sub

// Signal processing function of timer signal
void sigalrmHand(int signo);

int main(int argc, char *argv[]){
    // create raw socket
    struct protoent* ptotocal = getprotobyname("icmp");
    if(ptotocal==NULL) printf("don't find right protoent!\n"), exit(-1);
    int socketFd = socket(AF_INET, SOCK_RAW, ptotocal->p_proto);
    if(-1 == socketFd) printf("create socket failed!:%m\n"),exit(-1);
    printf("create socket successfully\n");
    // set buffer of recv
    int size = 50*1024;
    setsockopt(socketFd, SOL_SOCKET,SO_RCVBUF,&size, sizeof(size));
    // recv'machine ip address
    struct sockaddr_in destaddr = {0};
    destaddr.sin_family = AF_INET;

    struct hostent* host = NULL;
    in_addr_t iAddr = inet_addr(argv[1]);
    if(INADDR_NONE == iAddr){
        host = gethostbyname(argv[1]);
        if(host == NULL){
            printf("error ip address!\n");
            exit(-1);
        }
        memcpy(&(destaddr.sin_addr), host->h_addr, host->h_length);
    }else{
        destaddr.sin_addr.s_addr = iAddr;
    }

    // prepare send packet pack packet -> function: pack(int nsend) -> function: send_packet(int socketFd, struct sockaddr_in* pDestaddr, int len)
    // send packet
    send_packet(socketFd, &destaddr, sizeof(destaddr));

    // recv packet
    // unpacking            function: recv_packet(int socketFd) -> call function: unpacking(int len, int uid)
    recv_packet(socketFd);

    printf("\n--------------------ping-------------------------\n");
	printf("%d packets send success  %d packets receive success %%%f loss!\n",nsend,nrecv,((nsend-nrecv)*1.0)/nsend*100 );
    return 0;
}

int pack(int nsend){
    struct icmp* pIcmp = (struct icmp*)sendPacket;
    pIcmp->icmp_type = ICMP_ECHO;
	pIcmp->icmp_code = 0;
	pIcmp->icmp_cksum = 0;
	pIcmp->icmp_id = getuid();
	pIcmp->icmp_seq = nsend;

    int packetsize = 8 + DATA_SIZE;

    // set send time 
    struct timeval* tval = (struct timeval*)pIcmp->icmp_data;
    gettimeofday(tval, NULL);

    // set the CheckSum   use crc32 CheckSum method -> function: u_short cal_cksum(u_short icmp, int packsize);
    pIcmp->icmp_cksum =  cal_cksum((u_short*)pIcmp, packetsize);
    return packetsize;
}

int send_packet(int socketFd, struct sockaddr_in* pDestaddr, int len){
    int packsize = 0;
    while(nsend < MAX_NO_PACKETS){
        nsend++;
        packsize = pack(nsend);
        if(sendto(socketFd, sendPacket, packsize, 0, (struct sockaddr*)pDestaddr, len)<0){
            printf("The %dth of send failed!:%m\n", nsend);
            continue;
        }
        printf("The %dth of send successful!\n", nsend);
        // sleep(1);
    }
}
// use cal crc32 CheckSum method
u_short cal_cksum(u_short* icmp, int packsize){
    int nleft = packsize;
	int sum = 0;
	u_short* w = icmp;
	u_short answer = 0;

	while(nleft>1){
		sum += *w++;
		nleft-=2;
	}

	if(1 == nleft){
		*(unsigned char*)(&answer) = *(unsigned char*)w;
		sum +=answer;
	}

	sum = (sum>>16) + (sum&0xffff);
	sum += (sum>>16);
	answer =~sum;

	return answer;
}

void recv_packet(int socketFd){
    int r;
    int fromLen=0;
    signal(SIGALRM, sigalrmHand);
    while(nrecv<nsend){
        // timer
        alarm(MAX_WAIT_TIME);
        
        r = recvfrom(socketFd, recvPacket, PACKET_SIZE-1, 0, 
            (struct sockaddr*)&fromAddr, &fromLen);
        if(r<0){
            printf("receive failed!\n");
            continue;
        }
        gettimeofday(&trecv, NULL);
        //  recvfrom return the len of message
        if(unpacking(r, getuid())==-1) continue;
        
        nrecv++;
    }
}

// unpack function
int unpacking(int len, int uid){
    // time of send packet
    struct timeval* tvsend;
    // recv buffer
    char* buffer = (char *)recvPacket;
    // Time of receive delay
    double rtt = 0;
    // get ip header length
    struct ip* ipd;
    ipd = (struct ip*) recvPacket;
    int ipheaderLen = ipd->ip_hl << 2; // byte = bit / 4
    
    // get icmp header
    struct icmp* icmpheader = (struct icmp*)(buffer+ipheaderLen);
    len -= ipheaderLen;

    if(len < 8){
        printf("icmp message less than 8 bytes!\n");
        return -1;
    }
    if(icmpheader->icmp_type==ICMP_ECHOREPLY&&icmpheader->icmp_id == uid){
        tvsend = (struct timeval*)icmpheader->icmp_data;
        rtt = trecv.tv_sec*1000 + trecv.tv_usec/1000;
        tv_sub(&trecv, tvsend);
		// printf("%d byte from %s: icmp_seq:%u ttl=%d rtt=%.3f ms\n",
		// 	len , inet_ntoa(fromAddr->sin_addr) ,icmpheader->icmp_seq ,ipd->ip_ttl, rtt);
        printf("%d byte from %s: icmp_seq:%u ttl=%d rtt=%.3f ms\n",
			len ,inet_ntoa(fromAddr.sin_addr),icmpheader->icmp_seq ,ipd->ip_ttl, rtt);
    }else{
		return -1;
	}
}

// Signal processing function of timer signal
void sigalrmHand(int signo){
	printf("\n--------------------ping-------------------------\n");
	printf("%d packets send success  %d packets receive success %%%f loss!\n",nsend,nrecv,((nsend-nrecv)*1.0)/nsend*100 );
	exit(1);
}

// cal time sub
int tv_sub(struct timeval* out,struct timeval* in){
	if((out->tv_usec -= in->tv_usec) < 0 ){
		--out->tv_sec;
		out->tv_usec+=1000000;
	}
	out->tv_sec-=in->tv_sec;
}