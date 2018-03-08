package propositionallogiccalculator2;

public class Stack
{
    private int top;
    private String[] stack;
    private final int MAXOPS = 100;
    
    public Stack()
    {
        top = -1;
        stack = new String[MAXOPS];
    }//end Stack
    
    public String getTop()
    {
        return stack[top];
    }//end getTop
    
    public void push(String op)
    {
        stack[++top] = op;
    }//end addOp
    
    public String pop()
    {
        return stack[top--];
    }//end pop
    
    public boolean isEmpty()
    {
        return (top == -1);
    }//end isEmpty
    
    public boolean isFull()
    {
        return (top == MAXOPS);
    }//end isFull
}//end class Stack
