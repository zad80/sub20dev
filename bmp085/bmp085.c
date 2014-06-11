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
#include <math.h>

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

/*
*-------------------------------------------------------------------------------
* Constants
*-------------------------------------------------------------------------------
*/
#define I2C_ADDRESS 0x77 //77?

const unsigned short oversampling_setting = 3; //oversamplig for measurement
const unsigned char pressure_waittime[4] = { 5, 8, 14, 26 };

//just taken from the BMP085 datasheet
int16_t ac1;
int16_t ac2;
int16_t ac3;
uint16_t ac4;
uint16_t ac5;
uint16_t ac6;
int16_t b1;
int16_t b2;
int16_t mb;
int16_t mc;
int16_t md;


/*
*-------------------------------------------------------------------------------
* Types
*-------------------------------------------------------------------------------
*/

/*
*-------------------------------------------------------------------------------
* Global Variables
*-------------------------------------------------------------------------------
*/
sub_handle* fd = NULL ;

/*
*-------------------------------------------------------------------------------
* Local Function Definition
*-------------------------------------------------------------------------------
*/

void bmp085_get_cal_data(void);
void bmp085_read_temperature_and_pressure(long int * temperature, long int* pressure);
int32_t bmp085_read_ut(void);
long bmp085_read_up(void);
int16_t read_int_register(unsigned char r);
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

	/* Open USB device */
	fd = sub_open( dev );

	if( !fd )
	{
		printf("sub_open: %s\n", sub_strerror(sub_errno));
		return -1;
	}else{

		printf("sub_open: OK\n");
	}

	char loop = 1;
	long int temperature=0;
	long int pressure=0;
	int16_t idVersion = read_int_register(0xD0);
	printf("Id = 0x%x Version = 0x%x\n",((idVersion&0xff00)>>8),idVersion&0xff);
	bmp085_get_cal_data();
	while(loop){
		bmp085_read_temperature_and_pressure(&temperature,&pressure);
		printf("Temperature %.1f[%ld] , pressure %.2lf[%ld]\n",temperature*0.1f,temperature,pressure*0.01f,pressure);
		sleep(1);
	}
/*
if( rc )
    {
        printf("Failed(rc=%d): %s\n", rc,sub_strerror(sub_errno));
        if( rc == SE_I2C )
            printf("I2C Status: %02x\n", sub_i2c_status );
    }
*/
    sub_close( fd );
	return rc;
}

void write_register(unsigned char r, unsigned char v)
{
	config.buf[0]=v;
	int rc = sub_i2c_write( fd, I2C_ADDRESS,r,1, config.buf, 1 );
	if(rc)
		printf("write_register error \n");

}

char read_register(unsigned char r)
{
	int rc = sub_i2c_read( fd,I2C_ADDRESS ,r, 1, config.buf, 1 );
	if(!rc)
		return config.buf[1];
	else
		printf("read_register error \n");
	return 0;
}
long read_long_register(unsigned char r)
{
	unsigned char msb=0, lsb=0;
	int rc = sub_i2c_read( fd,I2C_ADDRESS ,r, 1, config.buf, 2 );
	//printf("sub_i2c_read sa=0x%x, config.ma=0x%x, config.ma_sz=%d, config.sz=%d\n",
                       //I2C_ADDRESS, r, 1,  2);

	if(!rc ){
		msb = 0xff & config.buf[0];
		lsb = 0xff & config.buf[1];
	//	printf("read_int_register buf[0]=0x%x buf[1]=0x%x \n", config.buf[0] ,config.buf[1]);
		return (((long)msb<<8) + ((long)lsb));
	}
	else
		printf("read_int_register error \n");
	return 0;
}

int16_t read_int_register(unsigned char r)
{
	int16_t data=0;
	int rc = sub_i2c_read( fd,I2C_ADDRESS ,r, 1, config.buf, 2 );
	//printf("sub_i2c_read sa=0x%x, config.ma=0x%x, config.ma_sz=%d, config.sz=%d\n",
                       //I2C_ADDRESS, r, 1,  2);

	if(!rc ){
		///msb = 0xff & config.buf[0];
		//lsb = 0xff & config.buf[1];
		printf("read_int_register buf[0]=0x%x buf[1]=0x%x \n", config.buf[0] ,config.buf[1]);
		data= (int16_t)(((config.buf[0])<<8) + config.buf[1]);
		return data;
	}
	else
		printf("read_int_register error \n");
	return 0;
}

uint16_t read_uint_register(unsigned char r){
	int16_t data=(uint16_t)read_int_register(r);
	return data;
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

void bmp085_read_temperature_and_pressure(long int* temperature, long int* pressure) {
  int32_t UT, X1, X2, B5;     // following ds convention
  UT= bmp085_read_ut();
  float t;


  // step 1
  X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) / pow(2,15);
  X2 = ((int32_t)mc * pow(2,11)) / (X1+(int32_t)md);
  B5 = X1 + X2;
  t = (B5+8)/pow(2,4);
  t /= 10;

  *temperature = t;
  return;
#if 0
  long int up = bmp085_read_up();
   long x1, x2, x3, b3, b5, b6, p;
   unsigned long b4, b7;

   //calculate the temperature
   x1 = ((long)ut - ac6) * (ac5 >> 15);
   x2 = ((long) mc << 11) / (x1 + md);
   b5 = x1 + x2;
   *temperature = (b5 + 8) >> 4;
   //calculate the pressure
   b6 = b5 - 4000;
   x1 = (b2 * (b6 * b6 >> 12)) >> 11; 
   x2 = ac2 * (b6 >> 11);
   x3 = x1 + x2;
   b3 = (( ac1 * 4 + x3)<< (oversampling_setting + 2) ) >> 2;
   x1 = ac3 * (b6 >> 13);
   x2 = (b1 * (b6 * b6 >> 12)) >> 16;
   x3 = ((x1 + x2) + 2) >> 2;
   b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
   b7 = ((unsigned long) up - b3) * (50000 >> oversampling_setting);
   p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
   x1 = (p >> 8) * (p >> 8);
   x1 = (x1 * 3038) >> 16;
   x2 = (-7357 * p) >> 16;
   *pressure = p + ((x1 + x2 + 3791) >> 4);
#endif
}


int32_t bmp085_read_ut() {
	write_register(0xf4,0x2e);
	while(read_gpio(5)==0)
		usleep(500);
	//usleep(5*1000); //longer than 4.5 ms
	uint16_t t=0;
	t=read_int_register(0xf6);
	return (int32_t)t;
}

void  bmp085_get_cal_data() {
	printf("Reading Calibration Data\n");
	ac1 = read_int_register(0xAA);
	printf("AC1: %d\n",ac1);
	ac2 = read_int_register(0xAC);
	printf("AC2: %d\n",ac2);
	ac3 = read_int_register(0xAE);
	printf("AC3: %d\n",ac3);
	ac4 = read_uint_register(0xB0);
	printf("AC4: %d\n",ac4);
	ac5 = read_uint_register(0xB2);
	printf("AC5: %d\n",ac5);
	ac6 = read_uint_register(0xB4);
	printf("AC6: %d\n",ac6);
	b1 = read_int_register(0xB6);
	printf("B1: %d\n",b1);
	b2 = read_int_register(0xB8);
	printf("B2: %d\n",b2);
	mb = read_int_register(0xBA);
	printf("MB: %d\n",mb);
	mc = read_int_register(0xBC);
	printf("MC: %d\n",mc);
	md = read_int_register(0xBE);
	printf("MD: %d\n",md);
}


long bmp085_read_up() {
	int rc =0;
	write_register(0xf4,0x34+(oversampling_setting<<6));
	while(read_gpio(5)==0)
		usleep(500);
	//usleep(pressure_waittime[oversampling_setting]*1000);

	rc = sub_i2c_read( fd,I2C_ADDRESS ,0xf6, 1, config.buf, 3 );
	if(!rc)
		return (((long)((0xff&config.buf[0])<<16) | ((long)(0xff&config.buf[1])<<8) | ((long)((0xff&config.buf[2]))))) >>(8-oversampling_setting);
	else
		printf("bmp085_read_up error \n");
	return 0;
}



