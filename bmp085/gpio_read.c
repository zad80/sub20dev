#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "libsub.h"
#include "utils.h"
#include <unistd.h>
#include <stdint.h>
//#include "cmd_pars.h"
#include "sub_app.h"


#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

/*
*-------------------------------------------------------------------------------
* Constants
*-------------------------------------------------------------------------------
*/


/*
*-------------------------------------------------------------------------------
* Global Variables
*-------------------------------------------------------------------------------
*/
sub_handle* fd = NULL ;
char read_gpio(unsigned char ngpio);

/*
*-------------------------------------------------------------------------------
* START
*-------------------------------------------------------------------------------
*/
int main( int argc, char* argv[] )
{
    int     rc=0,i;//,tmp,cmd;
//	sub_int32_t tmp1,tmp2;
	struct usb_device* dev;

    /* Init */
    config.fpwm_duty[0]=config.fpwm_duty[1]=config.fpwm_duty[2]=-1;
    for( i=0; i<8; i++ )
        config.pwm_duty[i] = -1;

	config.serno=-1;	/* by default open first available device */
    config.bb_i2c_ch=-1;
    config.bb_i2c_mode=BB_I2C_STD;


	dev = NULL;
	if( config.serno != -1 )
	{
		char sn_buf[20];
		int found = 0 ;
		while( ( dev =  sub_find_devices( dev ) ) )
		{
			fd = sub_open( dev );
			if(!fd)
			{
				printf("ERROR: Cannot open handle to device\n" );
				return -1;
			}

            rc = sub_get_serial_number( fd, sn_buf, sizeof(sn_buf) );

			sub_close( fd );

			if(rc<0)
			{
				printf("ERROR: Cannot obtain seral number: %d\n", rc );
				return -1;
			}

			if( config.serno == strtoul(sn_buf, NULL, 16 ) )
			{
				/* device found */
				found = 1;
				break;
			}
		}

		if( !found )
		{
			printf("ERROR: Device with SN=0x%04x not found\n", config.serno );
			return -1;
		}else{

			printf("Device found\n" );
		}
	}
	if(argc<=1){
		printf("gpio number to read needed\n");
		return 0;
	}

	/* Open USB device */
	fd = sub_open( dev );

	if( !fd )
	{
		printf("sub_open: %s\n", sub_strerror(sub_errno));
		return -1;
	}else{

		printf("sub_open: OK\n");
	}
	int gpio= atoi(argv[1]);
	printf("GPio %d=%d\n",gpio,read_gpio(gpio));
	sub_close( fd );
	return 0;
}

char read_gpio(unsigned char ngpio)
{
	int data=0;

	int rc=0;
	if(ngpio>=31)
		return -1;
	rc= sub_gpio_read( fd,&data);
	if(!rc)
		return ((0x1<<ngpio)&data)>>ngpio ;
	else
		printf("read_register error \n");
	return 0;
}

