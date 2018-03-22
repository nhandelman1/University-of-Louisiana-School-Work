/*
 * Broken Link Checker
 * Nick Handelman
 * CSCE513 Spring 2013
 * 
 * You will develop a program that reports URL errors. The program should have
 * three input parameters: web link (U), depth (D), and type (T).
 *       Executable U D T
 * U represents an URL of a web page. D is an integer number that limits the
 * search depth. T indicates the type of the broken links (such as http, ftp, 
 * file, mailto, or local links). Please properly encode T such that the program
 * can check one or multiple types of links.
 * 
 * The program should check all Type T URLs contained in the given webpage (U).
 * If an URL is invalid, the program reports an error message; otherwise it accesses the
 * corresponding webpage and checks its URLs recursively to a depth of D.
 * The output of the program should be a tree of the links (sitemap), with U as the
 * root. The broken links and their ancestors should be highlighted. The output sitemap may
 * follow the XML format (http://www.sitemaps.org/protocol.html).
 * 
 * 
 * FtpURLConnection
 *  test working link - "ftp://ftp.bls.gov/pub/special.requests/cpi/cpiai.txt"
 *  test broken link - "ftp://my.ftpserver.com/"
 * 
 * 
 * Local Links
 *  file://C:/Users/Nick/Desktop/CMPS 513/project/Socket Programming And Test webpage/CSCE513_Test_Webpage.htm
 * 
 */
package BrokenLinkChecker;

import java.net.*;
import java.io.*;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
        
public class Main 
{
    static final String INIT_URL = "file://C:/Users/Nick/Desktop/CMPS 513/project/Socket Programming And Test webpage/CSCE513_Test_Webpage.htm";
    static final int DEPTH = 2;
    static final boolean HTTP_URL = true; //set to true to check HTTP links
    static final boolean FILE_URL = true; //set to true to check HTTP links
    static final boolean FTP_URL = true; //set to true to check HTTP links
    static final boolean MAILTO_URL = true; //set to true to check HTTP links
    static final String OUTPUT_FILE = "output.xml";
    
    public static void main(String[] args) 
    {
        Main m = new Main();
        
        try
        {
            //setup output file
            BufferedWriter bw = new BufferedWriter(new FileWriter(OUTPUT_FILE));
            bw.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            bw.newLine();
            bw.write("<urlset xmlns=\"http://www.sitemaps.org/schemas/sitemap/0.9\">");
            bw.newLine();
            
            //begin link checking
            m.checkLink(INIT_URL, 0, bw);
            
            //finish writing to output file
            bw.write("</urlset>");
            bw.newLine();
            bw.close();
        }
        catch(IOException e)
        {
           System.out.println("Error creating output file");
        }
        
    }//end main
    
    public void checkLink(String nextURL, int curDepth, BufferedWriter bw)
    {
        Boolean[] linkBroken = new Boolean[1];
        linkBroken[0] = new Boolean(true);
        boolean print = false;
        Elements links = null;
        
        //stop checking deeper links if depth has been reached
        if(curDepth == DEPTH+1)
            return;
        
        //form URL from nextURL and check it
        System.out.println("Now checking url: " + nextURL);
        try
        {
            URL url = new URL(nextURL);

            //check mailto links
            if(url.getProtocol().contentEquals("mailto"))
            {
                if(!MAILTO_URL) System.out.println("Not checking mailto links.");               
                else
                {
                    print = true;
                    linkBroken[0] = new Boolean(mailtoLink(nextURL));
                }
            }//end if mailto
            
            //check ftp links
            if(url.getProtocol().contentEquals("ftp"))
            {
                if(!FTP_URL) System.out.println("Not checking ftp links.");              
                else
                {
                    print = true;
                    linkBroken[0] = new Boolean(ftpLink(url));
                }
            }//end if ftp

            //check http links
            if(url.getProtocol().contentEquals("http"))
            {
                if(!HTTP_URL) System.out.println("Not checking http links.");
                else
                {
                    print = true;
                    links = parseHTTPLink(nextURL, linkBroken);
                }
            }//end if http
            
            //check file links
            if(url.getProtocol().contentEquals("file"))
            {
                if(!FILE_URL) System.out.println("Not checking file links.");
                else
                {
                    print = true;
                    links = parseFileLink(nextURL, linkBroken);
                }
            }//end if file
            
            //print link to output xml file
            if(print)
            {
                bw.write("<url>");
                bw.newLine();
                String s = "<loc>" + nextURL + "</loc>";
                bw.write(s);
                bw.newLine();
                if(linkBroken[0].booleanValue()) //if the link is broken
                {
                    bw.write("<lastmod> link broken </lastmod>");
                    bw.newLine();
                }

                //recursively check links
                if(links != null)
                {
                    for(Element src : links)
                        checkLink(src.absUrl("href"), curDepth+1, bw);
                }

                bw.write("</url>");
                bw.newLine();
            }//end if
    
        }//end try
        catch(MalformedURLException e) //error creating URL data type given nextURL String
        { 
            System.out.println("Malformed URL");
            
            //print broken link to file
            try
            {
                bw.write("<url>");
                bw.newLine();
                String s = "<loc>" + nextURL + "</loc>";
                bw.write(s);
                bw.newLine();
                bw.write("<lastmod> link broken </lastmod>");
                bw.newLine();
                bw.write("</url>");
                bw.newLine();
            }
            catch(IOException f)
            {
                System.out.println("Input stream error or Error writing to file");
            }
        }
        catch(IOException e)
        {
            System.out.println("Input stream error or Error writing to file");
        }
    }//end checkLink()
    
    /**
     * Parse HTTP link using Jsoup
     * 
     * @param url - the url that is being parsed
     * @param linkBroken - Boolean array with 1 value, will be set to true if link is broken
     * @return links - Jsoup Elements data structure that holds links parsed from String url
     */
    public Elements parseHTTPLink(String url, Boolean[] linkBroken)
    {   
        Elements links = null;
        
        try
        {
            Document doc = Jsoup.connect(url).get();
            links = doc.select("a[href]");
            linkBroken[0] = new Boolean(false);
        }
        catch(IOException e)
        {
            System.out.println("Unable to Parse Broken Link");
            linkBroken[0] = new Boolean(true);
            links = null;
        }
        
        return links;
    }//end parseLink()
    
    
    /**
     * Parse file link using Jsoup
     * 
     * @param url - the url that is being parsed
     * @param linkBroken - Boolean array with 1 value, will be set to true if link is broken
     * @return links - Jsoup Elements data structure that holds links parsed from String url
     */
    public Elements parseFileLink(String url, Boolean[] linkBroken)
    {
        Elements links = null;
        
        try
        {
            File in = new File(url.substring(7));
            Document doc = Jsoup.parse(in, null, url);
            links = doc.select("a[href]"); 
            linkBroken[0] = new Boolean(false);
        }
        catch(IOException e)
        {
            System.out.println("Unable to Parse Broken Link");
            linkBroken[0] = new Boolean(true);
            links = null;
        }
        
        return links;
    }//end parseFileLink
    
    
    /**
     * Check mailto URLs
     * 
     * @param mailURL - URL String
     * @return isBroken - true if the link is broken
     */
    public boolean mailtoLink(String mailURL)
    {
        boolean isBroken = false;
        
        //remove the mailto: from the string
        String temp = mailURL.substring(7);
        
        //check that mail link is valid
        try
        {
            InternetAddress a = new InternetAddress(temp);
            a.validate(); //invalid will throw exception
        }
        catch(AddressException e)
        {
            System.out.println("Invalid email URL");
            isBroken = true;
        }
        
        return isBroken;
    }//end mailtoLink()

    /**
     * Check ftp URLs
     * 
     * @param ftpURL
     * @return isBroken - true if the link is broken
     */
    public boolean ftpLink(URL ftpURL)
    {
        boolean isBroken = false;
        InputStream is;

        try
        {
            URLConnection connection = ftpURL.openConnection();
            is = connection.getInputStream();
            is.close();
        }
        catch(IOException e)
        {
            System.out.println("Ftp connection error");
            isBroken = true;
        }
        
        return isBroken;
    }//end ftpLink()
      
}//end class Main
