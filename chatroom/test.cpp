/*测试splice零拷贝
**将标准输入转移到管道，再由管道转移到标准输出
*/
#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<fcntl.h>
#include<iostream>
#include<string.h>

int main()
{
    int fd_in[2];
    int ret = pipe(fd_in);//定义管道
    assert(ret == 0);
    while(1)
    {
        ssize_t w_len = splice(0, NULL, fd_in[1], NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);//将标准输入(fd=0)移动到管道
        std::cout << "写入字符数：" << w_len << std::endl; 
        ssize_t r_len = splice(fd_in[0], NULL, 1, NULL, 32768, SPLICE_F_MOVE|SPLICE_F_MORE);//将管道数据移动到标准输出(fd=1)
        std::cout << "读出字符数：" << r_len << std::endl; 
    }
    return 0;
}

