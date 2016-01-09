/*
  Author: Marie-Josee Blais
  Date: 02-12-2001

*/

package tetris;

import javax.swing.*;
import java.awt.*;
import java.awt.image.*;
import javax.swing.border.*;

class  DisplayPanel extends JPanel implements Display_I
{
  static final long serialVersionUID = 0;
  private int width;
  private int height;
  private int sqWidth;
  private int sqHeight;
  private boolean firstTime = true;
  private boolean imgCreated = false;
  private int[][] playZone;
  private int x1, y1, x2, y2;
  private Color[] palette = { null,
                              Color.magenta,
                              Color.yellow,
                              Color.red,
                              Color.cyan,
                              Color.darkGray,
                              Color.green,
                              Color.blue};
  private BufferedImage[] offScrImg = new BufferedImage[palette.length+1];
  private Graphics2D[] offScrG = new Graphics2D[palette.length+1];                            
  
  public DisplayPanel(int width, int height)
  {  
    this.width = width;
    this.height = height;
    setBackground(Color.blue);
    setForeground(Color.green);
    setBorder(new EtchedBorder());
    setBounds(10,10,3*height/8,3*height/4);
    setOpaque(true);           
  }                
        
  public void paintComponent(Graphics g)
  { 
    super.paintComponent(g);              
    Graphics2D g1 = (Graphics2D)g;
    if(imgCreated)
    {
      for(int j=0; j<playZone.length; j++)
        for(int i=0; i<playZone[0].length; i++)
        {
          synchronized(playZone)
          {
            g1.drawImage(offScrImg[playZone[j][i]], i*sqWidth, j*sqHeight, this);
          }
        }   
    }
    else if(playZone != null)
    { 
      sqHeight = (int)((3*height)/(4*playZone.length));
      sqWidth =  (int)(sqHeight);

      int boundsHeight = sqHeight*playZone.length;
      int boundsWidth =  sqWidth*playZone[0].length;
      setBounds(10,10,boundsWidth,boundsHeight);        
      
      offScrImg[0] = (BufferedImage) createImage(sqWidth,sqHeight);
      offScrG[0] = offScrImg[0].createGraphics();        
      for(int i=1; i<palette.length;i++)
      {
        offScrImg[i] = (BufferedImage) createImage(sqWidth,sqHeight);
        offScrG[i] = offScrImg[i].createGraphics();
        offScrG[i].setColor(palette[i]);
        offScrG[i].fill3DRect(0,0,sqWidth,sqHeight,true);
      }  
      imgCreated = true;
    }      
  }
  
  public synchronized void setPlayZone(int[][] dirtyRegion, int x1, int y1)
  {
    int tableWidth = dirtyRegion[0].length;
    int tableHeight = dirtyRegion.length;      
    x2 = x1 + tableWidth;
    y2 = y1 + tableHeight;          
    for(int j=y1; j<y2; j++)
      for(int i=x1; i<x2; i++)  
        playZone[j][i] = dirtyRegion[j-y1][i-x1];    
    repaint(x1*sqWidth,y1*sqHeight,tableWidth*sqWidth,tableHeight*sqHeight);       
  }
  
  public synchronized void setPlayZone(int[][] temp)
  {             
    playZone = new int[temp.length][temp[0].length];      
    for(int j=0; j<playZone.length; j++)
      for(int i=0; i<playZone[0].length; i++)        
        playZone[j][i] = temp[j][i];
    repaint();
  }        
        
}
