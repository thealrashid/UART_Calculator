#include<lpc21xx.h>
#include"header.h"

#define THRE ((U0LSR>>5)&1)
#define RDR (U0LSR&1)

void uart0_init(u32 baud)
{
	int arr[] = {15, 60, 30};
	u32 pclk, result = 0;
	if(VPBDIV%4 == 3) return;
	pclk = arr[VPBDIV%4]*1000000;
	result = pclk/(16*baud);
	PINSEL0 |= 0X05;
	U0LCR = 0X83;
	U0DLL = result & 0xFF;
	U0DLM = (result>>8) & 0xFF;
	U0LCR = 0X03;
}

void uart0_tx(u8 data)
{
	U0THR = data;
	while(THRE == 0);
}

u8 uart0_rx(void)
{
	while(RDR == 0);
	return U0RBR;
}

void uart0_tx_string(char *ptr)
{
	while(*ptr != 0)
	{
		U0THR = *ptr;
		while(THRE == 0);
		ptr++;
	}
}

void uart0_rx_string(char *s, int b)
{
	s32 i;
	for(i=0;i<b;i++)
	{
		while(RDR==0);
		s[i] = U0RBR;
		if(s[i] == '\r') break;
	}
	s[i] = '\0';
}

void uart0_tx_integer(s32 num)
{
	int arr[10], i;
	if(num==0)
	{
		uart0_tx('0');
		return;
	}
	if(num<0)
	{
		uart0_tx('-');
		num = -num;
	}

	i=0;
	while(num>0)
	{
		arr[i] = num%10;
		num = num/10;
		i++;
	}

	for(--i;i>=0;i--)
	{
		uart0_tx(arr[i]+'0');
	}
}

void uart0_tx_float(float f)
{
	int n = (int)f, p;
	float m = f-n;
	while(m != (int)m) m *= 10;
	p = (int)m;
	uart0_tx_integer(n);
	uart0_tx('.');
	uart0_tx_integer(p);
}

void gsm_rx(u8 *s, s32 b)
{
	s32 i;
	for(i=0;i<b;i++)
	{
		while(RDR==0);
		s[i] = U0RBR;
		if(s[i] == '\0') break;
	}
	s[i] = '\0';
}

#include<stdio.h>
void uart0_hex(int num)
{
    char buf[10];
    sprintf(buf, "%x", num);
    uart0_tx_string(buf);
}
