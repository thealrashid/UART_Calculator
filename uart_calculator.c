#include"header.h"

extern char tokens[TOK][EXP], postfix[TOK][EXP];

/*Function to check if the character is an operator*/
int isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

/*Function to return precedence of operators*/
int precedence(char op)
{
    switch(op)
    {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

/*Function to check the validity of the expression*/
int isValid(char *exp)
{
    int i;

    if(isOperator(exp[0]))
    {
        if((exp[0] != '-') && (exp[0] != '+')) return 0;
        //If first character is an operator and it is something other than '+' or '-'
    }

    for(i=1;exp[i];i++)
    {
        if(isOperator(exp[i]) && isOperator(exp[i-1])) return 0; //If consecutive operators are present
    }

    if(isOperator(exp[strlen(exp)-1])) return 0; //If last character is an operator

    return 1;
}

/*Function to tokenize the expression and store the tokens in an array*/
int tokenize(char *exp)
{
    int token_index = 0; // Index to keep track of the current token
    int array_index = 0; // Index to keep track of the array of strings
    int i;

    for(i=0;exp[i];i++)
    {
        if(isdigit(exp[i]))
        {
            // If the character is a digit, append it to the current token
            tokens[array_index][token_index] = exp[i];
            token_index++;
        }
        else if(isOperator(exp[i]))
        {
            // If the character is an operator, store the current token and the operator
            if(token_index>0)
            {
                 // If there's a token, store it in the array
                tokens[array_index][token_index] = '\0'; // Null-terminate the token string
                array_index++;
                if(array_index >= TOK)
                {
                     // Check if the array is full
                    //uart0_tx_string("Too many inputs\r\n");
                    return -1;
                }
                token_index = 0; // Reset the token index
            }

            // Store the operator as a separate token
            tokens[array_index][0] = exp[i];
            tokens[array_index][1] = '\0'; // Null-terminate the operator string
            array_index++;
            if(array_index >= TOK)
            {
                 // Check if the array is full
                //uart0_tx_string("Too many inputs\r\n");
                return -1;
            }
        }
        else return -1; // Ignore other characters
    }

    // Store any remaining token at the end of the expression
    if(token_index > 0)
    {
        tokens[array_index][token_index] = '\0'; // Null-terminate the token string
        array_index++;
        if(array_index >= TOK)
        {
             // Check if the array is full
            //uart0_tx_string("Too many inputs\r\n");
            return -1;
        }
    }

    return array_index; // Return the number of tokens stored in the array
}

/*Function to convert infix expression to postfix expression*/
void infix_to_postfix(int num_tokens)
{
    char stack[TOK][EXP];
    int top = 0, index = 0, i;

    for(i=0;i<num_tokens;i++)
    {
        if(isdigit(tokens[i][0]))
        {
            // If the token is an operand, append it to the postfix expression
            strcpy(postfix[index], tokens[i]);
            index++;
        }
        else if(isOperator(tokens[i][0]))
        {
            /*If the token is an operator, pop operators from the stack and append them to postfix
              until the stack is empty or the top operator has lower precedence*/
            while(top > 0 && isOperator(stack[top-1][0]) && precedence(stack[top-1][0]) >= precedence(tokens[i][0]))
            {
                strcpy(postfix[index], stack[top-1]);
                index++;
                top--;
            }

             // Push the current operator onto the stack
            strcpy(stack[top], tokens[i]);
            top++;
        }
    }

     // Pop any remaining operators from the stack and append them to postfix
    while(top > 0)
    {
        strcpy(postfix[index], stack[top-1]);
        index++;
        top--;
    }

    postfix[index][0] = '\0';  // Null-terminate the postfix expression
}

/*Function to evaluate the postfix expression and return the result*/
float evaluate(int num_tokens)
{
    float stack[TOK];
    int top = 0, i, j;
    float op1, op2, ans;

    for(i=0;i<num_tokens;i++)
    {
        if(isdigit(postfix[i][0]))
        {
            // If the token is an operand, push it onto the stack
            stack[top++] = atof(postfix[i]);
        }
        else if(isOperator(postfix[i][0]))
        {
            //If the token is an operator, pop operands from the stack, perform the operation, and push the result back
            op2 = stack[--top];
            op1 = stack[--top];

            switch(postfix[i][0])
            {
                case '+': ans = op1+op2; break;
                case '-': ans = op1-op2; break;
                case '*': ans = op1*op2; break;
                case '/': 
                    if(op2 != 0) ans = op1/op2;
                    else
                    {
                        // Handle division by zero
                        uart0_tx_string("Error: Division by zero.\r\n");
                        return 0;
                    }    
                    break;
                case '^':
                    ans = 1;
                    for(j=0;j<op2;j++) ans *= op1;
                    break;
                default:
                    // Handle unknown operators
                    uart0_tx_string("Unknown operator\r\n");
                    return 0;
            }

            // Push the result onto the stack
            stack[top++] = ans;
        }
    }

    // The final result should be on the top of the stack
    return stack[top-1];
}
