#include  <stdio.h>
#define MEM_SZ 64

// Assume we have a task like this
void task1(void) {
    int i;
    int m[64];
    i=0;
INC: ++i;
    printf("%d\n",i);
    m[i]=i;
    if (i==5) goto END;
    goto INC;
END: printf("Done\n");
}
// Assume we have a task like this
void task2(void) {
    int i2;
    int m2[MEM_SZ];
    i2=0;
INC: ++i2;
    printf("%d\n",i2);
    m2[i2]=i2;
    if (i2==5) goto END;
    goto INC;
END: printf("Done\n");
}

void task3(void) {
    int i3;
    int m3[MEM_SZ];
    i3=0;
INC: if (m3[0]==0) goto END;
    printf("%d\n",i2);
    m2[i2]=i2;
    if (i2==5) goto END;
    goto INC;
END: printf("Done\n");
}


