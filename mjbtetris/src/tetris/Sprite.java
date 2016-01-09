/*
  Author: Marie-Josee Blais
  Date: 02-12-2001

*/

package tetris;

import java.util.*;

public class Sprite implements Cloneable
{
  private int[][] shape;
  protected int width;
  protected int height;
  
  private static final int[][][] tetrinoids = {{{0,0,0,0},
                                               {1,1,1,0},
                                               {0,0,1,0},
                                               {0,0,0,0}},
                                              {{0,0,0,0},
                                               {0,2,2,2},
                                               {0,2,0,0},
                                               {0,0,0,0}},
                                              {{0,0,0,0},
                                               {3,3,3,3},
                                               {0,0,0,0},
                                               {0,0,0,0}},
                                              {{0,0,0,0},
                                               {0,4,4,0},
                                               {0,4,4,0},
                                               {0,0,0,0}},
                                              {{0,0,0,0},
                                               {5,5,5,0},
                                               {0,5,0,0},
                                               {0,0,0,0}},
                                              {{0,0,0,0},
                                               {6,6,0,0},
                                               {0,6,6,0},
                                               {0,0,0,0}},
                                              {{0,0,0,0},
                                               {0,0,7,7},
                                               {0,7,7,0},
                                               {0,0,0,0}}};         
                                                      
  public Sprite()
  { Random random = new Random();
    int index = (random.nextInt()&0x7FFFFFFF)%7;
    //System.out.println("index 2:" + index);
    shape = tetrinoids[index];      
    width = this.shape[0].length;
    height = this.shape.length;
  }

  public Sprite(int index)
  {
    if (index >= 0)
    {
      //System.out.println("index 1:" + index);
      shape = tetrinoids[index];                
      width = this.shape[0].length;
      height = this.shape.length;
    }  
  }

  public Object clone()
  {
    if(this != null)
    { 
      Sprite temp = new Sprite(-1);
      temp.shape = new int[height][width];              
      for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++)
          temp.shape[j][i] = shape[j][i];
      temp.width = width;
      temp.height = height;
      return temp;
    }
    else
      return null;
  }        
  
  public int getWidth()
  {
    return width;       
  }

  public int getHeight()
  {
    return height;       
  }        
  
  public int getUMost()
  { 
    int x=0;
    int y=0;

    while( y < height && shape[y][x] == 0 )        
    {     
      while( x < width && shape[y][x] == 0 )
        x++;
      if(x == width)
      {
        x=0;
        y++;
      }  
    }
    return (y==width ? -1 : y);        
  }

  public int getShapeUnit(int y, int x)
  {
    return shape[y][x];       
  }        
  
  public void rotate()
  {
    int[][] newShape = new int[width][height];     
    for(int x=0;x<width;x++)
      for(int y=0;y<height;y++)
        newShape[y][x] = this.shape[x][height-1-y];      
    this.shape = newShape;
  }

}
