#include <stdio.h>
 
int main()
{
    int i,j,n;
    scanf("%d",&n);
    for(i=1;i<n;i++) {
        for(j=1;j<=i-1+n;j++)
            //打印*的位置为n-i+1和n+i-1
            if(j==n-i+1||j==n+i-1)
                printf("*");
            //其他的位置打印空格
            else
                printf(" ");
            //打印回车
            printf("\n");
    }
    //打印三角形最后一行*，个数为2n-1个
    for(i=1;i<=2*n-1;i++)
        printf("*");
    printf("\n");
    return 0;
}
