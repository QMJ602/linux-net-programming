#include <iostream>
#include <stdio.h>
#include <string.h>


void test(char* str)
{
    std::cout << sizeof(str) << std::endl;
}
int main()
{
    //定义字符串的几种方式
    //字符串和字符数组的区别：最后一位是否是空字符
    char names1[] = {'j', 'a', 'c', 'k', '\0'};
    char names2[50] = "jack";
    char * names3 = "jackdfgsfsfsfsfsfs";
    // test(names3);
    int* p = new int(2);
    std::cout << *p << std::endl;


    // printf("数组所占空间大小：%d\n", sizeof(names1));
    // printf("数组所占空间大小：%d\n", sizeof(names2));
    // printf("数组所占空间大小：%d\n", strlen(names3));
    return 0;
}

