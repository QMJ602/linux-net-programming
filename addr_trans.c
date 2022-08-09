#include<stdio.h>
#include<arpa/inet.h>
//将ip地址由字符串转换为为无符号整数
int main()
{
	char* str = "192.163.1.11";
	unsigned int ip;
	inet_pton(AF_INET, str, &ip);
	printf("%d\n", ip);
	unsigned char* ipadd = (char*)&ip;
	printf("%d\n", &ip);
	printf("%d\n", ipadd);
	printf("%d.%d.%d.%d\n",*ipadd,*(ipadd+1), *(ipadd+2), *(ipadd+3) );
	return 0;
}
