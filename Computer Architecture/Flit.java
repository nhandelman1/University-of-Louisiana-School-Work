package distributedsharedbufferapp;
/**
 *
 * @author Arthur, Nick, & Xiangbo
 */
public class Flit 
{
    private char source;                     //tracks what source came from.
    private int sequencenum;                //tracks packet order
    private boolean headflit;               //tells if flit is head flit or not
    private int timestamp;
    private int priority;
    private int departureport;
    private int mmrow, mmcol;               //tracks if mid mem location
                                            //was allocated to flit
    private int maxpriority;                //The maximum a priority can be
    private int createdcycle;               //The cycle the flit was created.

    /**
     * @param destport The departure port for the flit.
     * @param inport The port the flit arrives on.
     * @param atype "Head", "Body", or "Tail" flit.
     * @param apriority Priority for the flit.
     */
    public Flit(int destport, int apriority)
    {
        departureport = destport;
        priority = apriority;
        mmrow = mmcol = -1;         // Set to null value
        timestamp = -1;             // Set to null value
        createdcycle = 0;
    }

    /**
     * Copy constructor
     * @param f Da flit
     */
    public Flit(Flit f)
    {
        source = f.getSource();
        sequencenum = f.getSequenceNum();
        headflit = f.isHead();
        timestamp = f.getTimeStamp();
        priority = f.getPriority();
        departureport = f.getDeparturePort();
        mmrow = f.getMMRow();
        mmcol = f.getMMCol();
        createdcycle = f.getCreatedCycle();
    }

    //---------------------Get & Set Methods--------------------------------

    public char getSource()
    {
        return source;
    }

    public int getSequenceNum()
    {
        return sequencenum;
    }

    public boolean isHead()
    {
        return headflit;
    }

    public int getDeparturePort()
    {
        return departureport;
    }

    public int getTimeStamp()
    {
        return timestamp;
    }

    public int getPriority()
    {
        return priority;
    }

    public int getMMRow()
    {
        return mmrow;
    }

    public int getMMCol()
    {
        return mmcol;
    }

    public int getCreatedCycle()
    {
        return createdcycle;
    }

    public void setSource(char newsource)
    {
        source = newsource;
    }

    public void setSequenceNum(int newsequence)
    {
        sequencenum = newsequence;
    }

    public void setIsHead(boolean isheadflit)
    {
        headflit = isheadflit;
    }

    public void setDeparturePort(int newdepartport)
    {
        departureport = newdepartport;
    }

    public void setTimeStamp(int newstamp)
    {
        timestamp = newstamp;
    }

    public void setMaxPriority(int max)
    {
        maxpriority = max;
    }

    public void setPriority(int newpriority)
    {
        if (newpriority >= maxpriority)
        {
            priority = maxpriority;
        }
        else
        {
            priority = newpriority;
        }
    }

    public void setMMRow(int row)
    {
        mmrow = row;
    }

    public void setMMCol(int col)
    {
        mmcol = col;
    }

    public void setCreatedCycle(int cycle)
    {
        createdcycle = cycle;
    }
}