#include <stdio.h>  
#include <stdint.h>

int main()  
{  
    uint32_t ID = 0x0011;
    uint32_t Mask = 0x0011;
 
    for(uint32_t i = 0; i < 0x0800; i++)
    {
        if((i & Mask) == ID) 
        {
            printf("0x%04X\n", i);
        }
    }

    return 0;
}
