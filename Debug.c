/*----- FILE CALC.C --------------------------------------------------*/
/*                                                                    */
/* A simple calculator that does operations on integers that          */
/* are pushed and popped on a stack                                   */
//--------------------------------------------------------------------/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "calc.h"
IntStack stack = { 0 };
int main()
{
  Token tok;
  char word[100];
  char buf_out[100];
  int num, num2;
  for(;;)
  {
    tok=read_token(word);
    switch(tok)
    {
      case T_STOP:
        break;
      case T_INTEGER:
        num = atoi(word);
        push(&stack,num);    
        break;
      case T_PLUS:
        push(&stack, pop(&stack)+pop(&stack) );
        break;
      case T_MINUS:
        num = pop(&stack);
        push(&stack, num-pop(&stack));
        break;
      case T_TIMES:
        push(&stack, pop(&stack)*pop(&stack));
        break;
      case T_DIVIDE:
        num2 = pop(&stack);
        num = pop(&stack);
        push(&stack, num/num2);   
        break;
      case T_EQUALS:
        num = pop(&stack);
        sprintf(buf_out,"= %d ",num);
        puts(buf_out);  // Debug - Just added this line and it's working
        push(&stack,num);
        break;
    }
    if (tok==T_STOP)
      break;
  }
  return 0;
}

//----- FILE PUSHPOP.C -----------------------------------------------/
/*                                                                    */
/* A push and pop function for a stack of integers                    */
/*--------------------------------------------------------------------/
/--------------------------------------------------------------------/
/* input:  stk - stack of integers                                    */
/*         num - value to push on the stack                           */
/* action: get a link to hold the pushed value, push link on stack    */
/*                                                                    */
extern void push(IntStack * stk, int num)
{
  IntLink * ptr;
  ptr       = (IntLink *) malloc( sizeof(IntLink));  //  PUSHPOP1   */
  ptr->i    = num;                /*  PUSHPOP2  statement */
  ptr->next = stk->top;
  stk->top  = ptr;

}
/*--------------------------------------------------------------------/
/* return: int value popped from stack                                */
/* action: pops top element from stack and gets return value from it  */
//--------------------------------------------------------------------/
extern int pop(IntStack * stk)
{
  IntLink * ptr;
  int num;
  ptr      = stk->top;
  num      = ptr->i;
  stk->top = ptr->next;
  free(ptr);
  return num;
}

//----- FILE READTOKN.C ----------------------------------------------/
/*                                                                    */
/* A function to read input and tokenize it for a simple calculator   */
//--------------------------------------------------------------------/
//--------------------------------------------------------------------/
/* action: get next input char, update index for next call            */
/* return: next input char                                            */
//--------------------------------------------------------------------/
static char nextchar(void)
{
//--------------------------------------------------------------------/
/* input  action:                                                     */
/*    2      push 2 on stack                                          */
/*    18     push 18                                                  */
/*    +      pop 2, pop 18, add, push result (20)                     */
/*    =      output value on the top of the stack (20)                */
/*    5      push 5                                                   */
/*    /      pop 5, pop 20, divide, push result (4)                   */
/*    =      output value on the top of the stack (4)                 */
//--------------------------------------------------------------------/
  char * buf_in  = "2 18 + = 5 / = ";
  //char * buf_in = " 3 27 + = 50 - = 2 / = 10 * = ";
  static int index;     /* starts at 0 */
  char ret;
  ret = buf_in[index];
  ++index;
  return ret;
}
//--------------------------------------------------------------------/
/* output: buf - null terminated token                                */
/* return: token type                                                 */
/* action: reads chars through nextchar() and tokenizes them          */
//--------------------------------------------------------------------/
Token read_token(char buf[])
{
  int i;
  char c;
  /* skip leading white space */
  for( c=nextchar();
       isspace(c);
       c=nextchar())
    ;
  buf[0] = c;  /* get ready to return single char e.g."+" */
  buf[1] = 0;
  switch(c)
  {
    case '+' : return T_PLUS;
    case '-' : return T_MINUS;
    case '*' : return T_TIMES;
    case '/' : return T_DIVIDE;
    case '=' : return T_EQUALS;
    default:
      i = 0;
      while (isdigit(c)) {
        buf[i++] = c;
        c = nextchar();
      }
      buf[i] = 0;
      if (i==0)
        return T_STOP;
      else
        return T_INTEGER;
  }
}
