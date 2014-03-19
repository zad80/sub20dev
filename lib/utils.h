#ifndef __UTILS_H
#define __UTILS_H


void hex_scan( char* buf_in, char* buf_out, int* sz );
void hex_ascii_dump( char* buf, int sz );
void adc_printf( int adc, int mux );
#endif
