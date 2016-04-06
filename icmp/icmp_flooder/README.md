一个简单的ICMP/TCP Flood攻击程序,在类unix系统上编译运行

#程序原理

- ICMP ECHO Flood 大量、高频率的IP协议数据请求致使接收方缓冲区溢出。
- TCP SYN Flood 发送大量伪造的TCP连接请求，从而使得被攻击方资源耗尽（CPU满负荷或内存不足）的攻击方式。

#编译

使用root权限运行shell脚本:

	sudo sh install.sh

#运行

	sudo icmp_flooder <ip> <port> <source ip>

