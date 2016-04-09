# arp protocol

ARP是Address Resolution Protocol的简称,即地址解析协议。

ARP 是一个局域网协议,在IP数据包经由一个又一个router到达另一台主机的过程里,每当到达一个router,router都会通过ARP来查询该IP数据包包含的目的地地址是否为局域网内的一台主机。

ARP协议的工作方式如下:主机发出一个ARP包，该ARP包中包含有自己的IP地址和MAC地址。通过ARP包，主机以广播的形式询问局域网上所有的主机和路由：我是IP地址xxxx，我的MAC地址是xxxx，有人知道199.165.146.4的MAC地址吗？拥有该IP地址的主机会回复发出请求的主机：哦，我知道，这个IP地址属于我的一个NIC(网卡)，它的MAC地址是xxxxxx。由于发送ARP请求的主机采取的是广播形式，并附带有自己的IP地址和MAC地址，其他的主机和路由会同时检查自己的ARP cache，如果不符合，则更新自己的ARP cache。

这样，经过几次ARP请求之后，ARP cache会达到稳定。如果局域网上设备发生变动，ARP重复上面过程。

#运行

代码在Mac OSX上编译通过。