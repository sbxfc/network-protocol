
#include <stdio.h>

/***
*
*/
struct ip_hdr
{
	unsigned int		hl:4;		  /* 4 bit header length */
	unsigned int		ver:4;		/* 4 bit version */
	unsigned char		tos;		  /* type of service */
	unsigned short  totl;		  /* total length of datagram */
	unsigned short	id;		    /* identifier */
	unsigned short 	notused;	/* this is were flags and fragment offset would go */
	unsigned char 	ttl;		  /* time to live */
	unsigned char		prot;		  /* protocol */
	unsigned short	checksum;	/* our checksum */
	unsigned long 	saddr;		/* source address */
	unsigned long 	daddr;		/* destination address */
};

int main(int argc, char **argv) {

  return 0;
}
