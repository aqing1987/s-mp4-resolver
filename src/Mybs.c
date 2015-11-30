#include "Mybs.h"

void bs_init(bs_t *s, void *p_data, int i_data)
{
    s->p_start = (unsigned char *) p_data; //
    s->p = (unsigned char *) p_data; //
    s->p_end = s->p + i_data; //
    s->i_left = 8; //
}

int bs_read( bs_t *s, int i_count )
{
	 static int i_mask[33] ={0x00,
                             0x01,      0x03,      0x07,      0x0f,
                             0x1f,      0x3f,      0x7f,      0xff,
                             0x1ff,     0x3ff,     0x7ff,     0xfff,
                             0x1fff,    0x3fff,    0x7fff,    0xffff,
                             0x1ffff,   0x3ffff,   0x7ffff,   0xfffff,
                             0x1fffff,  0x3fffff,  0x7fffff,  0xffffff,
                             0x1ffffff, 0x3ffffff, 0x7ffffff, 0xfffffff,
                             0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};
/*

			  
			  byte:		1		2		3		4
				   00000000 00000000 00000000 00000000		index

			  0x00:							  00000000		x[0]

			  0x01:							  00000001		x[1]
			  0x03:							  00000011		x[2]
			  0x07:							  00000111		x[3]
			  0x0f:							  00001111		x[4]

			  0x1f:							  00011111		x[5]
			  0x3f:							  00111111		x[6]
			  0x7f:							  01111111		x[7]
			  0xff:							  11111111		x[8]	1 byte

			 0x1ff:						 0001 11111111		x[9]
			 0x3ff:						 0011 11111111		x[10]	i_mask[s->i_left]
			 0x7ff:						 0111 11111111		x[11]
			 0xfff:						 1111 11111111		x[12]	1.5 bytes

			0x1fff:					 00011111 11111111		x[13]
			0x3fff:					 00111111 11111111		x[14]
			0x7fff:					 01111111 11111111		x[15]
			0xffff:					 11111111 11111111		x[16]	2 bytes

		   0x1ffff:				0001 11111111 11111111		x[17]
		   0x3ffff:				0011 11111111 11111111		x[18]
		   0x7ffff:				0111 11111111 11111111		x[19]
		   0xfffff:				1111 11111111 11111111		x[20]	2.5 bytes

		  0x1fffff:			00011111 11111111 11111111		x[21]
		  0x3fffff:			00111111 11111111 11111111		x[22]
		  0x7fffff:			01111111 11111111 11111111		x[23]
		  0xffffff:			11111111 11111111 11111111		x[24]	3 bytes

		 0x1ffffff:	   0001 11111111 11111111 11111111		x[25]
		 0x3ffffff:	   0011 11111111 11111111 11111111		x[26]
		 0x7ffffff:    0111 11111111 11111111 11111111		x[27]
		 0xfffffff:    1111 11111111 11111111 11111111		x[28]	3.5 bytes

		0x1fffffff:00011111 11111111 11111111 11111111		x[29]
		0x3fffffff:00111111 11111111 11111111 11111111		x[30]
		0x7fffffff:01111111 11111111 11111111 11111111		x[31]
		0xffffffff:11111111 11111111 11111111 11111111		x[32]	4 bytes

*/
    int i_shr; //
    int i_result = 0; //

    while (i_count > 0) {
        if (s->p >= s->p_end) { //
            break;
        }

        if ((i_shr = s->i_left - i_count) >= 0) { //
            // more in the buffer than requested
            i_result |= (*s->p >> i_shr) & i_mask[i_count]; //
            s->i_left -= i_count; //
            if (s->i_left == 0) //
                    {
                s->p++; //
                s->i_left = 8; //
            }
            return (i_result); //
        }
        else {
            i_result |= (*s->p & i_mask[s->i_left]) << -i_shr; //"-i_shr"相当于取了绝对值
            i_count -= s->i_left; //
            s->p++; //
            s->i_left = 8; //
        }
    }

    return (i_result); //
}

int bs_read1(bs_t *s)
{
    if (s->p < s->p_end) {
        unsigned int i_result;

        s->i_left--; //
        i_result = (*s->p >> s->i_left) & 0x01; //
        if (s->i_left == 0) {
            s->p++; //
            s->i_left = 8; //
        }
        
        return i_result; //unsigned int
    }

    return 0; //
}

int bs_read_ue(bs_t *s)
{
    int i = 0;

    while (bs_read1(s) == 0 && s->p < s->p_end && i < 32) {
        i++;
    }
    
    return ((1 << i) - 1 + bs_read(s, i));
}

