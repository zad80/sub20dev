/*
 * Demo application for libsub header file
 * Copyright (C) 2008-2009 Alex Kholodenko <sub20@xdimax.net>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2.0 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
*-------------------------------------------------------------------
* Version: $Id: sub_app.h,v 1.45 2011-01-19 13:02:07 avr32 Exp $
*-------------------------------------------------------------------
*/
#ifndef SUB_APP_H_INCLUDED
#define SUB_APP_H_INCLUDED


/*
*------------------------------------------------------------------------------
*                               types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
*                               defines
*------------------------------------------------------------------------------
*/

/* Version */
#define VERSION_STR     "XDimax LTD, V%s, %s %s\n"
#define VERSION_INFO    "0.1.9"
#define APP_NAME        "sub_app"
#define XPRINT_VERSION do{printf( VERSION_STR, VERSION_INFO, __DATE__, __TIME__ );}while(0);

/*
*------------------------------------------------------------------------------
*                               Global Variables
*------------------------------------------------------------------------------
*/
struct 
{
    int     usb_to_ms;      /* Default timeout */

#define MAX_CMD 256
    char    cmd[MAX_CMD];
    int     cmd_sz;
    int     resp_sz;

    char    todo[2000];
    int     todo_sz;

    int     sa;             /* Slave Address */
    sub_int32_t     ma;             /* Memory Address */
    int     ma_sz;
    sub_int32_t     i2c_freq;
    int     i2c_sa;
    int     i2c_dis;        /* Disable */
    int     bb_i2c_ch;      /* Bit-bang I2C channel */
    int     bb_i2c_mode;
    int     bb_i2c_stretch;

    int     spi_config;
    int     ss;
    int     spi_out_sz;
    int     spi_in_sz;
    char    spi_ess[300];
#if _MSC_VER == 800
    char    buf[0x1000];
#else
    char    buf[0x100000];
#endif
    int     sz;

    sub_int32_t		gpio_val;
    int     gpio_mask;
    int     gpiob_val;
    int     gpiob_mask;
    int     edge_config;

    double  fpwm_freq;
    double  fpwm_duty[3];
    int     fpwm_flags;

    int     pwm_res;
    int     pwm_limit;
    int     pwm_duty[8];    

    int     adc_cfg;
    int     adc_reads;
    int     adc_mux[30];

    sub_int32_t     rs_baud;
    int     rs_config;
    int     rs_flags;
    int     rs_tx_space;
    int     rs_rx_msg_to;
    int     rs_rx_byte_to;
    int     rs_tx_sz;
    int     rs_rx_sz;

    char*   lcd_str;

    int     fifo_cfg;

    int     mdio_n;
    int     mdio_channel;
    union   sub_mdio_frame mdios[20];

	int		serno;
}config = 
{
    10000,      /* usb_to_ms */
	"",         /* cmd */
    0,          /* cmd_sz */
    0,          /* resp_sz */
	"",         /* todo */
    0,          /* todo_sz */
};


/*
*------------------------------------------------------------------------------
*                               MACRO
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
*                               Functions
*------------------------------------------------------------------------------
*/

#endif /*H_INCLUDED*/

