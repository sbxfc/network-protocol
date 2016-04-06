ICMP Echo Smurf攻击程序,在类unix系统上编译运行

#Smurf攻击原理



利用广播的特性，假造一个 ICMP 的封包（来源位址为受害者的 IP 位址），而目的位址为该网域的广播位址。当该封包送往网域的广播位址后（例如 10.4.1.255），该网域所有仍存活的主机即会回应封包给 ICMP 封包的来源位址（受害者的 IP），即可能使受害者的 IP 在短时间内无法承受这麽大的网路流量而瘫痪掉。从另一角度来看，由于网域内充满了回应的封包，也可能造成该网路拥塞的情况发生。此种攻击可怕的地方，在于 可利用少量的网路封包（难以被网管人员察觉）即可瞬间创造出极大量的封包流。举例来说，假设目标网段中有 200 台电脑，攻击者只需要送出 10K bps 的 ICMP 封包流至该网段的广播 (broadcast) 位址的，即可能瞬间产生高达 20 Mbps 的资料量回传到受害者的主机上，即使受害者主机有再大的频宽，Smurf 攻击都可以轻易地耗尽这样的频宽资源。除此之外，由于此种攻击手法是以 ICMP 通讯协定来完成，而 ICMP 通讯协定又常被大众所忽略，也因此成为安全上的一大漏洞。

查看广播地址(broadcast):
	
	$ifconfig
	$en0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
	options=10b<RXCSUM,TXCSUM,VLAN_HWTAGGING,AV>
	ether 3c:07:54:63:c5:61 
	inet6 fe80::3e07:54ff:fe63:c561%en0 prefixlen 64 scopeid 0x4 
	inet 192.168.1.49 netmask 0xffffff00 broadcast 192.168.1.255
	nd6 options=1<PERFORMNUD>
	media: autoselect (100baseTX <full-duplex,flow-control,energy-efficient-ethernet>)



ping一下广播地址

	$localhost:~ sbxfc$ ping 192.168.1.255
	$PING 192.168.1.255 (192.168.1.255): 56 data bytes
	$64 bytes from 192.168.1.49: icmp_seq=0 ttl=64 time=0.074 ms
	$64 bytes from 192.168.1.99: icmp_seq=0 ttl=64 time=0.897 ms
	$64 bytes from 192.168.1.96: icmp_seq=0 ttl=64 time=0.973 ms
	$64 bytes from 192.168.1.1: icmp_seq=0 ttl=255 time=1.432 ms
	$64 bytes from 192.168.1.201: icmp_seq=0 ttl=64 time=5.780 ms

或者使用tcpdump查看所有ICMP Echo Reply

	$sudo tcpdump 'icmp[icmptype] = icmp-echoreply' //接收echo
	$sudo tcpdump 'icmp[icmptype] = icmp-echo'	      //发出echo		

#编译

使用root权限运行shell脚本:

	sudo sh install.sh

#运行

	sudo icmp_smurf <broadcast ip> <port> <source ip>

