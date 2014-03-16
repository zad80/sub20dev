/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  collections of utility functions
 *
 *        Version:  1.0
 *        Created:  03/15/14 11:24:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  e.placidi (), zad360@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

/*
* Convert HEX string to byte array 
*/
void hex_scan( char* buf_in, char* buf_out, int* sz )
{
    int     out_sz=0;

    while( *buf_in )
    {
        sscanf( buf_in, "%02hhx", &buf_out[out_sz++] );
        buf_in+=2;
        if( *buf_in != ' ' )
            break;
        buf_in++;
    }        

    if( sz )
        *sz = out_sz;
}

/* Dump buffer in Hex and ASCII */
void hex_ascii_dump( char* buf, int sz )
{
#define HEX_DUMP_WIDTH  16
    int             i;
    unsigned char   ascii[HEX_DUMP_WIDTH+1];

    for( i=0; i<sz; i++ )
    {
        printf("%02X ", ((unsigned int)buf[i])&0x0FF );
        if( (buf[i] >= 0x20) && ((unsigned char)buf[i] < 0x80) )
            ascii[i%HEX_DUMP_WIDTH] = buf[i];
        else
            ascii[i%HEX_DUMP_WIDTH] = '.';

        if( i%HEX_DUMP_WIDTH == HEX_DUMP_WIDTH-1 )
        {
            ascii[HEX_DUMP_WIDTH] = 0x00;
            printf( "| %s\n", ascii );
        }
    }

    ascii[i%HEX_DUMP_WIDTH] = 0x00;
    if( ascii[0] )
    {
        for( ; i%HEX_DUMP_WIDTH ; i++ )
            printf("   ");
        printf( "| %s\n", ascii );
    }
}

/*
* Print A/D Result
*/
void adc_printf( int adc, int mux )
{
    int gain=1,scale=1023;

    switch( mux )
    {
    case ADC_S0:
        printf("ADC0\t");
        break;
    case ADC_S1:
        printf("ADC1\t");
        break;
    case ADC_S2:
        printf("ADC2\t ");
        break;
    case ADC_S3:
        printf("ADC3           ");
        break;
    case ADC_S4:
        printf("ADC4           ");
        break;
    case ADC_S5:
        printf("ADC5           ");
        break;
    case ADC_S6:
        printf("ADC6           ");
        break;
    case ADC_S7:
        printf("ADC7           ");
        break;
    case ADC_D10_10X:
        printf("+ADC1-ADC0x10  ");
        gain = 10;
        scale= 512;
        break;
    case ADC_D10_200X:
        printf("+ADC1-ADC0x200 ");
        gain = 200;
        scale= 512;
        break;
    case ADC_D32_10X: 
        printf("+ADC3-ADC2x10  ");
        gain = 10;
        scale= 512;
        break;
    case ADC_D32_200X:
        printf("+ADC3-ADC2x200 ");
        gain = 200;
        scale= 512;
        break;
    case ADC_D01:     
        printf("+ADC0-ADC1     ");
        scale= 512;
        break;
    case ADC_D21:     
        printf("+ADC2-ADC1     ");
        scale= 512;
        break;
    case ADC_D31:     
        printf("+ADC3-ADC1     ");
        scale= 512;
        break;
    case ADC_D41:     
        printf("+ADC4-ADC1     ");
        scale= 512;
        break;
    case ADC_D51:     
        printf("+ADC5-ADC1     ");
        scale= 512;
        break;
    case ADC_D61:     
        printf("+ADC6-ADC1     ");
        scale= 512;
        break;
    case ADC_D71:     
        printf("+ADC7-ADC1     ");
        scale= 512;
        break;
    case ADC_D02:     
        printf("+ADC0-ADC2     ");
        scale= 512;
        break;
    case ADC_D12:     
        printf("+ADC1-ADC2     ");
        scale= 512;
        break;
    case ADC_D32:     
        printf("+ADC3-ADC2     ");
        scale= 512;
        break;
    case ADC_D42:     
        printf("+ADC4-ADC2     ");
        scale= 512;
        break;
    case ADC_D52:     
        printf("+ADC5-ADC2     ");
        scale= 512;
        break;
    default:
        printf("MUX=%02d         ", mux);
    }    

    printf( "%04d(0x%04x)\t%1.4fV\t%1.4fV\t%1.4fV\n", 
                adc, adc&0xFFFF,
                5.00*adc/scale/gain,
                3.30*adc/scale/gain,
                2.56*adc/scale/gain
    );
}

