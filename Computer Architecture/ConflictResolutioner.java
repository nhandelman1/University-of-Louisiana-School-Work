package distributedsharedbufferapp;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
/**
 * @author Arthur, Nick, & Xiangbo
 */
public class ConflictResolutioner
{
    final private int THRESHOLD = 1;        // Minimum amount of free buffering
                                            // at a host to determine if
                                            // there is enough room for one more
                                            // flit.
    private MiddleMemoryReservationTable MMRT;
    private int buffersathostA,             // How many buffers at host A
                buffersathostB,             // How many buffers at host B
                buffersathostC,             // How many buffers at host C
                buffersathostD,             // How many buffers at host D
                buffersathostE;             // How many buffers at host E
    private Buffer[] inputbuffers;
    private List<Flit> winningflits;

    public ConflictResolutioner(MiddleMemoryReservationTable mmrt, Buffer[] buffers,
            List<Flit> listofwinningflits)
    {
        MMRT = mmrt;
        inputbuffers = buffers;
        winningflits = listofwinningflits;
        buffersathostA = buffersathostB = buffersathostC = buffersathostD = buffersathostE = 8;
    }

    /**
     * This method checks MMRT for each flit at the head of each input buffer
     * (virtual channels). If there is a MM spot available, it sets the
     * row and column for the flit. If not, nothing is set and the flit will
     * re-enter the TS stage.
     */
    public void CheckMiddleMemoryForFlits()
    {
        // Check all flits that are timestamped
        List<Flit> flits = sortFlits(); // Get timestamped flits.
        for (int i = flits.size() - 1; i >= 0; i--)
        {
            // Get next-hop destination
            Flit requestingflit = flits.get(i);
            if (requestingflit.getTimeStamp() < 0)
            {
                continue;   // Flit was not a winning flit. Skip to next flit
            }
            int departure = requestingflit.getDeparturePort();

            // Check if there is enough buffering at next-hop router
            switch(departure)
            {
                case 0:
                    if (buffersathostA > THRESHOLD)
                    {
                        boolean success = findConflictFreeMM(requestingflit);
                        if (success)
                        {
                            buffersathostA--;
                        }
                    }
                    else
                    {
                        // Not enough buffering at next-hop router
                        requestingflit.setTimeStamp(-1);// Set to null value.
                    }
                    break;
                case 1:
                    if (buffersathostB > THRESHOLD)
                    {
                        boolean success = findConflictFreeMM(requestingflit);
                        if (success)
                        {
                            buffersathostB--;
                        }
                    }
                    else
                    {
                        // Not enough buffering at next-hop router
                        requestingflit.setTimeStamp(-1);// Set to null value.
                    }
                    break;
                case 2:
                    if (buffersathostC > THRESHOLD)
                    {
                        boolean success = findConflictFreeMM(requestingflit);
                        if (success)
                        {
                            buffersathostC--;
                        }
                    }
                    else
                    {
                        // Not enough buffering at next-hop router
                        requestingflit.setTimeStamp(-1);// Set to null value.
                    }
                    break;
                case 3:
                    if (buffersathostD > THRESHOLD)
                    {
                        boolean success = findConflictFreeMM(requestingflit);
                        if (success)
                        {
                            buffersathostD--;
                        }
                    }
                    else
                    {
                        // Not enough buffering at next-hop router
                        requestingflit.setTimeStamp(-1);// Set to null value
                    }
                    break;
                default:
                    if (buffersathostE > THRESHOLD)
                    {
                        boolean success = findConflictFreeMM(requestingflit);
                        if (success)
                        {
                            buffersathostE--;
                        }
                    }
                    else
                    {
                        // Not enough buffering at next-hop router
                        requestingflit.setTimeStamp(-1);// Set to null value
                    }
                    break;
            }
        }

        // Set which flits have won a spot in MM
        setWinningFlits();
    }

    /**
     * This method updates how many buffers are available at a next-hop host.
     * @param hostID Determines which host.
     * @param amountmoreavailable Determines how many available buffers became available.
     */
    public void acknowledgeMoreBuffers(int hostID, int amountmoreavailable)
    {
        switch(hostID)
        {
            case 0:
                buffersathostA += amountmoreavailable;
                break;
            case 1:
                buffersathostB += amountmoreavailable;
                break;
            case 2:
                buffersathostC += amountmoreavailable;
                break;
            case 3:
                buffersathostD += amountmoreavailable;
                break;
            case 4:
                buffersathostE += amountmoreavailable;
                break;
            default:
                break;
        }
    }

    /**
     * This method checks MMRT for a flit. If there is a MM spot available, it
     * sets the row and column for the flit. If not, nothing is set and the
     * flit will re-enter the TS stage.
     * @param requestflit The flit that will be checked via MMRT
     */
    private boolean findConflictFreeMM(Flit requestflit)
    {
        int timestamp = requestflit.getTimeStamp();
        int priority = requestflit.getPriority();
        boolean success = !MMRT.checkTable(priority, timestamp);
        if (success)
        {
            requestflit.setMMRow(priority);
            requestflit.setMMCol(timestamp);
            MMRT.setTable(priority, timestamp, true);
            return true;
        }
        // Cannot find a Middle Memory spot for flir. It must re-enter TS stage.
        requestflit.setTimeStamp(-1);       // Set to a null value
        return false;
    }

    /**
     * This method will set which flits have won a spot in MM, and allocate them
     * to the list "winningflits" for XBar1 to choose from.
     */
    private void setWinningFlits()
    {
        for (int i = 0; i < inputbuffers.length; i++)
        {
            if (!inputbuffers[i].isEmpty())
            {
                if (inputbuffers[i].peekHead().getMMCol() > -1 &&
                    inputbuffers[i].peekHead().getMMRow() > -1)
                {
                    winningflits.add(inputbuffers[i].popHead());
                }
            }
        }
    }


    /**
     * This method sorts flits based on priority using insertion sort.
     * @param flits The flit-array to be sorted.
     * @return The sorted flit-array.
     */
    private List<Flit> sortFlits()
    {
        // First obtain all flits that were timestamped
        List<Flit> flits = new ArrayList();
        for (int i = 0; i < inputbuffers.length; i++)
        {
            Flit temp = inputbuffers[i].peekHead();
            if (temp != null)
            {
                if (temp.getPriority() >= 0)
                {
                    flits.add(temp);
                }
            }
        }

        // Copy flits into an array
        Flit[] cflits = new Flit[flits.size()];
        for (int i = 0; i < flits.size(); i++)
            cflits[i] = flits.get(i);

        for (int i = 1; i < flits.size(); i++)
        {
            Flit temp = cflits[i];
            int j = i - 1;
            while ( j >= 0 && cflits[j].getPriority() > temp.getPriority())
            {
                cflits[j+1] = cflits[j];
                j--;
            }
            cflits[j+1] = temp;
        }

        // Convert back to list
        flits.clear();
        flits.addAll(Arrays.asList(cflits));

        return flits;
    }
}

/*
 * Conflict Resolution
 *      find conflict-free middle memory assignment for flits that were
        assigned timestamps in the TS stage.
 *      If a flit cannot be granted a middle memory slot, then it re-enteres
        the TS stage.
 *      max timestamp = current time + B(buffering at input ports) - 1
 *      Use Middle Memory reservation Table (MMRT). For N middle memories, and
        B flits per buffering. If MMRT[i][j] is set, then memory bank j holds
        a flit with timestamp i and vice versa
 */