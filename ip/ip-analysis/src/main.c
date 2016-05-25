
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 获取ip第一段的十进制值
*/
int get_first_section_of_ip(char* ip){
	char character = '.';
	char* tmp = strchr(ip,character);
	if(tmp == NULL){
		return -1;
	}
	
	int idx = strlen(ip) - strlen(tmp);
	char str[4];
	strncpy(str,ip,idx);
	str[idx] = '\0';
	int val = strtol(str,NULL,10);
	return val;
}

int main(int argc, char **argv) {
	
	if(argc != 2){
		printf("Invalid Args!!!!! Interface {ip} \n");
		exit(-1);
	}

	printf("输入的IP为: %s 该地址为 ",argv[1]);
	int val = get_first_section_of_ip(argv[1]);

	if(val <= 127){
		printf("A 类地址!\n");
	}
	else if(val <= 191){
		printf("B 类地址!\n");
	}
	else if(val <= 223){
		printf("C 类地址!\n");
	}
	else if(val <= 239){
		printf("D 类地址!\n");
	}
	else if(val <= 247){
		printf("E 类地址!\n");
	}
	else{
		printf("Invalid Args!!!!!\n");
		exit(-1);
	}

	return 0;
}


