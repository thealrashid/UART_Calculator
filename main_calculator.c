#include"header.h"

char tokens[TOK][EXP], postfix[TOK][EXP];

int main()
{
    char exp[EXP];
    int num_tokens;
    float result;

    uart0_init(9600);
    uart0_tx_string("UART CALCULATOR\r\n");

    while(1)
    {
        uart0_tx_string("\r\nEnter the expression: ");
        uart0_rx_string(exp, EXP);
		uart0_tx_string(exp);

        // Check of the expression is valid
        if(!isValid(exp))
        {
            uart0_tx_string("\r\nInvalid expression");
            continue;
        }

        // Tokenize the infix expression and get the number of tokens
        num_tokens = tokenize(exp);

        if(num_tokens == -1)
        {
            uart0_tx_string("\r\nInvalid expression");
            continue;
        }

        // Convert infix to postfix
        infix_to_postfix(num_tokens);

        // Evaluate the postfix expression and print the result
        result = evaluate(num_tokens);
        uart0_tx_string("\r\nResult: ");
        uart0_tx_float(result);
    }
}
