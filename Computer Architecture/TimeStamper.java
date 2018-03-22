package distributedsharedbufferapp;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * @author Arthur, Nick, & Xiangbo
 */
public class TimeStamper
{
    private Buffer[] inputbuffers;
    private int[] lat;                      // Last timestamp for each port.
    private boolean[] isportselected;       // Determines if flit already
                                            // reserved output port.
    private final int B = 20;                // Max amount of flits in MM buffer

    public TimeStamper(Buffer[] inputbuffers)
    {
        // TimeStamper needs access to VCs, so we pass them as argument.
        this.inputbuffers = inputbuffers;
        lat = new int[inputbuffers.length >>> 1];
        for (int i = 0; i < lat.length; i++)    // set all a null value
            lat[i] = -1;
        isportselected = new boolean[inputbuffers.length >>> 1];
        for (int i = 0; i < isportselected.length; i++) // set all to false
            isportselected[i] = false;
    }

    public void timestampFlits(int clockcycle)
    {
        // Compute regular time to leave
        int Tp = clockcycle + 3;

        // Get the flits from the VC buffers
        List<Flit> flits = new ArrayList();
        for (int i = 0; i < inputbuffers.length; i += 2)
        {
            if (!inputbuffers[i].isEmpty() && !inputbuffers[i+1].isEmpty())
            {
                if (inputbuffers[i].peekHead().getPriority() >
                        inputbuffers[i+1].peekHead().getPriority())
                {
                    flits.add(inputbuffers[i].peekHead());
                    //[flitindex] = inputbuffers[i].peekHead();
                }
                else
                {
                    flits.add(inputbuffers[i+1].peekHead());
                    //[flitindex] = inputbuffers[i+1].peekHead();
                }
            }
            else if (!inputbuffers[i].isEmpty())
            {
                flits.add(inputbuffers[i].peekHead());
                //[flitindex] = inputbuffers[i].peekHead();
            }
            else if (!inputbuffers[i+1].isEmpty())
            {
                flits.add(inputbuffers[i+1].peekHead());
                //[flitindex] = inputbuffers[i].peekHead();
            }
            else
            {
                // VC buffer is empty
            }
        }

        // If not a head flit, then increase priority (body and tail flits).
        for (int i = 0; i < flits.size(); i++)
                if (!flits.get(i).isHead())//[i].isHead())
                    flits.get(i).setPriority(flits.get(i).getPriority() + 1);
        
        // Sort flits from lowest priority to highest priority.
        flits = sortFlits(flits);

        int[] numreqflits = new int[lat.length];  // Will track how many
                                                    // flits are requesting
                                                    // the same port.
        for (int i = 0; i < numreqflits.length; i++)
            numreqflits[i] = 0;

        // Timestamp the flits
        int[] templat = new int[lat.length];    // Will store lat's for this cycle
        System.arraycopy(lat, 0, templat, 0, templat.length);
        for (int i = flits.size() - 1; i >= 0; i--)
        {
            int departure = flits.get(i).getDeparturePort();
            flits.get(i).setPriority(flits.get(i).getPriority() + 1);//Increase priority
            if (isportselected[departure])
            {
                // Departure port is already selected.
                numreqflits[departure]++;
            }
            else
            {
                // First time departure port is selected
                isportselected[departure] = true;
            }
            
            // Compute the timestamp for the flit
            int offset = numreqflits[departure];
            int timestamp = max(lat[departure] + 1, Tp) + offset;

            // Store previously assigned timestamp for an output port
            templat[departure] = timestamp;

            // Make sure timestamp is bounded [0, B -1]
            timestamp = timestamp % B;

            // Stamp the flit
            flits.get(i).setTimeStamp(timestamp);
        }

        // Store LATs collected for this cycle in lat[]
        System.arraycopy(templat, 0, lat, 0, lat.length);
        
        // Reset the boolean[] for the next stage
        resetIsPortSelected();
    }

    /**
     * This method compares two ints and returns the maximum one.
     * @param num1 First number to be compared.
     * @param num2 Second number to be compared.
     * @return The maximum number of num1 and num2
     */
    private int max(int num1, int num2)
    {
        if (num1 < num2)
            return num2;
        else
            return num1;
    }
    
    private void resetIsPortSelected()
    {
        for (int i = 0; i < isportselected.length; i++)
            isportselected[i] = false;
    }

    /**
     * This method sorts flits based on priority using insertion sort.
     * @param flits The flit-array to be sorted.
     * @return The sorted flit-array.
     */
    private List<Flit> sortFlits(List<Flit> flits)
    {
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
 * Route Computation stage
 *      look ahead routing scheme
 * Timestamping stage
 */

/*
 *      Regular time to leave (T[p]) = current time + 3
 *      Timestamper remembers the value of the last timestamp
        assigned for each output port till the previous cycle.
 *      Last timestamp for output port p is LAT[p].
 *      If two incoming flits request the same output port, then one wins
        and the other must be given higher priority for later.
 *      Timestamp = max(LAT[p] + 1, T[p])
 *      Offset is computed as equal to the number of higher priority flits
        requesting a timestamp for the same output port.
 *      The final timestamp assigned to a flit at input i is the sum of the
        timestamp assigned to the highest priority flit requesting the same
        output port as the current flit and the computed offset.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      TSi = max(LAT[OPi] + 1, current time + 3) + offseti
 *      max timestamp = current time + B(buffering at input ports) - 1
 *      Time rolls over every B cycle
 *      Avoid rollover problem with timestamps
 *      If the timestamp computed using Equation 2 is greater than B, it is
        rolled over by subtracting B from the result.
 */