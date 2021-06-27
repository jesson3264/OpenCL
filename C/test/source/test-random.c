#include "test-random.h"
#include "uuid.h"

void show(uuid_t src,char *out)
{
    unsigned char* p = (unsigned char*)src;
    int i = 0; 
    int j = 0;
    for (i = 0; i < sizeof(uuid_t); ++i)
    {
        int g = (*(p + i))%16;
        int s = (*(p + i))/16;
        out[j++] = (s >= 10) ? ('a' + s - 10) : ('0' + s);
        out[j++] = (g >= 10) ? ('a' + g - 10) : ('0' + g);
        
        printf("%02x ", *(p + i));
    }
        
    printf("\n");
}
void gen_random()
{
    uuid_t out;
    char dst[33] = {0};
    int i = 0;
    for (i = 0; i < 20; ++i)
    {
        uuid_generate(out);
        //printf("uuid:%s\n", out);
        show(out, dst);
        printf("dst:%s\n", dst);
    }
}