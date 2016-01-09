/*
  Author: Marie-Josee Blais
  Date: 02-12-2001

*/

package tetris;

public class TetrisManager extends Thread
{
            
  private int[][] playZone;
  private int height;
  private int width;
  private Display_I display;
  private Sprite tetrinoid;
  private int xpos;
  private int ypos;
  private boolean tetrisRunning;
  private int currentSprite = 0;
     
  public TetrisManager( Display_I display)
  {
    this.display = display;      
    playZone = new int[30][10];
    height = 30;
    width = 10;
    tetrisRunning = true;
    setPriority(Thread.NORM_PRIORITY-1);
    start();
  }
  
  public TetrisManager(Display_I display, int width, int height)
  {
    this.display = display;      
    playZone = new int[height][width];
    this.height = playZone.length;
    this.width = playZone[0].length;
    display.setPlayZone(playZone);
    tetrisRunning = true;
    setPriority(Thread.NORM_PRIORITY-1);
    start();
  }

  private int getWidth()
  {
    return width;        
  }        
  
  private int getHeight()
  {
    return height;      
  }
     
  public void run()
  {     
    while( tetrisRunning)
    { 
      synchronized(this)
      {        
        if (tetrinoid == null)
        {
          tetrinoid = new Sprite(); // random sprite
          xpos = (int)(width/2);
          ypos = -tetrinoid.getUMost();
          if( isCollision(tetrinoid,xpos,ypos) )
            stopThread();
          else
          {
            int x2 = xpos+tetrinoid.width-1;
            int y2 = ypos+tetrinoid.height-1;                   
            sendPlayZone(xpos,0,x2,y2);
          }  
        }    
        else
        {
          if ( !moveDown() )
          {      
            for (int j = ypos; j < (ypos+tetrinoid.height); j++ )
              for (int i = xpos; i < (xpos+tetrinoid.width); i++ )
              {
                int shapeUnit = tetrinoid.getShapeUnit(j-ypos,i-xpos);
                if (shapeUnit != 0)
                {
                  playZone[j][i] = shapeUnit;
                }  
              }
              tetrinoid = null;
              cleanUpLines();
              sendPlayZone(0,0,width-1,height-1);
           }
        }
      }  
      try
      {
        Thread.sleep(250);
      }
      catch (InterruptedException e) {}
    }
  }        
  
  public void stopThread()
  {
    tetrisRunning = false;        
  } 

 private synchronized void sendPlayZone(int x1, int y1, int x2, int y2)
  {                
    if(x1 < 0)
      x1 = 0;
    else if( x1 >= width)
      x1 = width - 1;
    if(y1 < 0)
      y1 = 0;
    else if(y1 >= height)
      y1 = height -1;
        
    if(x2 < 0)
      x2 = 0;
    else if( x2 >= width)
      x2 = width - 1;
    if(y2 < 0)
      y2 = 0;
    else if(y2 >= height)
      y2 = height - 1;  
    int[][] temp = new int[y2-y1+1][x2-x1+1];
    for (int j = y1; j <= y2; j++)
      for (int i = x1; i <= x2; i++)
      {                
        temp[j-y1][i-x1] = playZone[j][i];
        if( tetrinoid != null &&
            j>=ypos && j<ypos+tetrinoid.height &&
            i>=xpos && i<xpos+tetrinoid.width )
        {
          int shapeUnit = tetrinoid.getShapeUnit(j-ypos,i-xpos);
          if(shapeUnit != 0)
            temp[j-y1][i-x1] = shapeUnit;      
        }        
      }  
      display.setPlayZone( temp, x1, y1 );     
  }              
  
  private void cleanUpLines()
  {
    boolean compLine; // Complete Line
    int numLines = 0; // Number of Lines to erase 
    for (int j = height-1; j >=0; j--)
    {
      compLine = true;      
      for (int i=0; i < width && compLine; i++)
        compLine = compLine & ( playZone[j][i] != 0);
      if( compLine )
        numLines++;
      else
      {
        if(numLines>0)
        {
          if(j+numLines<=height)
            playZone[j+numLines] = playZone[j];
          playZone[j] = new int[width];
        }  
      }        
    }    
  }        
      
  private synchronized boolean isCollision(Sprite tetrinoid, int x, int y)
  {      
    int i=0;
    int j=0;
    boolean collision = false;
    if( tetrinoid != null)
    {
      while( !collision && j < tetrinoid.height )
      { 
        if( i >= tetrinoid.width )
        {
          i=0;              
          j++;
        }
        else if( tetrinoid.getShapeUnit(j,i) != 0 &&
                 ( i+x < 0 || i+x >= width ||
                   j+y < 0 || j+y >= height ||
                   playZone[j+y][i+x] != 0 ))
        {
          collision = true;
        }
        else
        {
          i++;
        }  
      }
    }
    return collision;    
  }        

  public void moveLeft()
  {
    if(tetrinoid != null && tetrisRunning && !isCollision(tetrinoid, xpos-1, ypos))  
      sendPlayZone(--xpos,ypos,xpos+tetrinoid.width,ypos+tetrinoid.height-1);
  }
  
  public void moveRight()
  {  
    if(tetrinoid != null && tetrisRunning && !isCollision(tetrinoid,xpos+1,ypos))
      sendPlayZone(xpos++,ypos,xpos+tetrinoid.width-1,ypos+tetrinoid.height-1);
  }

  public boolean moveDown()
  {   
    if(tetrinoid != null && tetrisRunning && !isCollision(tetrinoid,xpos,ypos+1))
    {
      sendPlayZone(xpos,ypos++,xpos-1+tetrinoid.width,ypos+tetrinoid.height-1);
      return true;
    }
    else
      return false;
  }

  public void rotate()
  {     
    if(tetrinoid != null && tetrisRunning)
    { 
      Sprite temp = (Sprite)tetrinoid.clone();
      temp.rotate();              
      if( !isCollision(temp,xpos,ypos) )
      {  
        tetrinoid.rotate();
        sendPlayZone(xpos,ypos,xpos+tetrinoid.width-1,ypos+tetrinoid.height-1); 
      }
    }
  }        
}
