#include <stdio.h>

int main()
{
    int k = 0x01020304;
    printf("%d\n", k);
    char *c = (char *)(&k);
    if (c[0] == 0x01 && c[3] == 0x04 ){
        printf("bigger order\n");
    }
    else if (c[0] == 0x04 && c[3] == 0x01){
        printf("small order\n");
    }
    else{
        printf("error order\n");
    }
}