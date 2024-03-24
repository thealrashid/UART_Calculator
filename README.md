# UART Calculator

The UART Calculator is a project that utilizes UART (Universal Asynchronous Receiver-Transmitter) protocol to take input and display output in a serial terminal. It reads an expression written on the serial terminal as a string, tokenizes the string based on operators and operands, converts the input from infix expression to postfix expression, evaluates the postfix expression, and sends the result back to the serial terminal via UART. The calculator supports the following operators: +, -, *, /, ^.

## Working

- **Input via UART:** Accepts input expressions from the serial terminal using UART communication.

- **Expression Parsing:** Parses the input expression and tokenizes it based on operators and operands.

- **Conversion to Postfix:** Converts the input expression from infix notation to postfix notation for evaluation.

- **Output via UART:** Sends the result of the evaluated expression back to the serial terminal via UART communication.

