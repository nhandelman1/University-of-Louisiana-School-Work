package distributedsharedbufferapp;
/**
 * @author Arthur, Nick, & Xiangbo
 */
public class Buffer
{
    private Flit flits[];
    private boolean empty;
    private int head, tail;
    private int amountinbuffer;

    public Buffer(int size)
    {
        flits = new Flit[size];
        for (int i = 0; i < size; i++)
            flits[i] = null;
        empty = true;
        head = tail = 0;
        amountinbuffer = 0;
    }

    //-----------------------Buffer Methods------------------------------

    /**
     * This method lets the caller view the head flit in the buffer.
     * @return head flit in buffer.
     */
    public Flit peekHead()
    {
        if (empty)
            return null;
        else
        {
            return flits[head];
        }
    }

    /**
     * This method removes the head flit in the buffer.
     * @return head flit in the buffer.
     */
    public Flit popHead()
    {
        if (empty)
            return null;
        else
        {
            amountinbuffer--;   // Decrement amount of flits in this buffer.
            int oldhead = head;
            head = (head + 1) % flits.length;
            if (head == tail)
                empty = true;
            return flits[oldhead];
        }
    }

    /**
     * Checks to see if the buffer is empty.
     */
    public boolean isEmpty()
    {
        return empty;
    }

    /**
     * Checks to see if the buffer is full.
     */
    public boolean isFull()
    {
        if (!empty && head == tail)
            return true;
        else
            return false;
    }

    /**
     * This method should be used when the buffer is used as a VC. It allows the
     * caller add a flit into the buffer if room is available.
     * @param newflit The new flit to be added to the buffer.
     */
    public void addInputFlit(Flit newflit)
    {
        empty = false;
        amountinbuffer++;
        flits[tail] = newflit;
        tail = (tail + 1) % flits.length;
    }

    /**
     * This method should be used when the buffer is used as an MM buffer.
     * @param newFlit
     */
    public void addMMFlit(Flit newFlit)
    {
        flits[newFlit.getMMCol()] = newFlit;
    }

    /**
     * This method should be used when the buffer is used as an output buffer.
     * @param newFlit
     */
    public void addOutputFlit(Flit newFlit)
    {
        empty = false;
        flits[head++] = newFlit;
        head = head % flits.length;
    }

    /**
     * This method returns the size of the buffer.
     * @return The size of the flit buffer.
     */
    public int getSize()
    {
        return flits.length;
    }

    /**
     * This method returns the number of flits in a buffer.
     * @return Number of flits in the buffer.
     */
    public int getAmountInBuffer()
    {
        return amountinbuffer;
    }

    public Flit getFlitAtIndex(int index)
    {
        if (flits[index] != null)
            return new Flit(flits[index]);
        else
            return null;
    }

    public void clearFlitAtIndex(int index)
    {
       flits[index] = null;
    }

    public void setHead(int index)
    {
        head = index;
    }

    /**
     * This method will set every object in buffer to null.
     */
    public void clear()
    {
        for (int i = 0; i < flits.length; i++)
        {
            flits[i] = null;
        }
        empty = true;
        head = tail = 0;
        amountinbuffer = 0;
    }
}