#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef unsigned int u32;
typedef unsigned char u8;
typedef int s32;
typedef char s8;
typedef short int u16;

extern void uart0_init(u32);
extern void uart0_tx(u8);
extern u8 uart0_rx(void);
extern void uart0_tx_string(char *);
extern void uart0_rx_string(char *, int);
extern void uart0_tx_integer(s32);
extern void uart0_tx_float(float);
extern void uart0_hex(int);
extern void gsm_rx(u8 *, s32);

#define EXP 100 //Maximum size of expression
#define TOK 50 //Maximum number of tokens

extern int isOperator(char);
extern int precedence(char);
extern int isValid(char *);
extern int tokenize(char *exp);
extern void infix_to_postfix(int num_tokens);
extern float evaluate(int num_tokens);
