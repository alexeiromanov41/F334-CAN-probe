#include <stdio.h>  
#include <stdint.h>

int main()  
{  
    uint32_t ID1 = 0x0101;
    uint32_t ID2 = 0x0102;
    uint32_t ID3 = 0x0105;
    uint32_t mask = 0x0000;
    uint32_t idr = 0x0000;
    uint32_t countRef = 8;
    uint32_t numCount = 0;

    for (idr = 0; idr < 0x0800; idr++) {
        for (mask = 0; mask < 0x0800; mask++) {
            if(((ID1 & mask) == idr) && ((ID2 & mask) == idr) && ((ID3 & mask) == idr)) {
                    for(uint32_t i = 0; i < 0x0800; i++) {
                        if((i & mask) == idr) {
                            numCount ++;
                            
                        }
                    }
                    
                    if(numCount <= countRef) {
                        printf("*******begin********\n");
                        printf("0x%04X\n", mask);
                        printf("0x%04X\n", idr);
                        printf("********end********\n");
                    }
                    
                    numCount = 0;
            }
        }
    }

    return 0;
}
