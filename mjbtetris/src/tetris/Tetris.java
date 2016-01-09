/*
  Author: Marie-Josee Blais
  Date: 02-12-2001

*/

package tetris;


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;

public class Tetris extends JPanel implements ActionListener,
                                              KeyListener
                            
{
static final long serialVersionUID = 0;
JMenuItem file_new     = new JMenuItem("New");
JMenuItem file_options = new JMenuItem("Options");
JMenuItem file_hs      = new JMenuItem("High Scores");
JMenuItem file_quit    = new JMenuItem("Quit");

int width;
int height;
DisplayPanel displayPanel;
TetrisManager manager;
    
//public Image image;
private Options options = new Options();;
        
public Tetris(int width, int height) 
{        
  this.width = width;
  this.height = height;      
  
  setLayout(null);
  setBackground(Color.white);
  setForeground(Color.red);
  setBorder(new EtchedBorder());
  setOpaque(true);
  
  displayPanel = new DisplayPanel(width,height); 
  
  add(displayPanel);
  //System.out.println("Tetris Initialized");
}

private void startGame()
{
  if(manager != null)
    manager.stopThread();      
  manager = new TetrisManager(displayPanel,10,20);
}        

public  Dimension getPreferredSize()
{
  return new Dimension((int)width/2,(int)(height*4/5));       
}
  
private JMenuBar createMenuBar()
{
  JMenuBar mbar = new JMenuBar();
  JMenu menuFile = new JMenu("File");      
  file_new.addActionListener(this);
  file_options.addActionListener(this);
  file_hs.addActionListener(this);
  file_quit.addActionListener(this);
  menuFile.add(file_new);
  menuFile.add(file_options);
  menuFile.add(file_hs);
  menuFile.add(file_quit);
  mbar.add(menuFile);
  return mbar;        
}



public void actionPerformed(ActionEvent e)
{
  String command = e.getActionCommand();
  if(command.equals("New"))
  {
    startGame();
  }
  else if(command.equals("Options"))
  {
    // option
    //  nombre de joueurs hot seat
    //  reseau avec server
    //  dimension de la grille
    //  nombre de bloc
    //  vitesse
    //  niveau de depart
  }
  else if(command.equals("High Scores"))
  {}
  else if(command.equals("Quit"))
  {
    System.exit(0);
  }    
}

public void keyTyped(KeyEvent e)
{
  if( manager != null)
  {
    char key = e.getKeyChar();
    if( key == 'j' || key == 'J' )
      manager.moveLeft();
    else if (key == 'l' || key == 'L' )
      manager.moveRight();
    else if (key == 'k' || key == 'K' )
      manager.rotate();
    else if (key == ' ')
      manager.moveDown();       
  }        
}

public void keyPressed(KeyEvent e)
{}

public void keyReleased(KeyEvent e)
{}

public static void main(String[] args)
{ 
  int screenHeight;
  int screenWidth;      
  JFrame frame = new JFrame("Tetris");
  frame.addWindowListener(new WindowAdapter()
  {
      public void windowClosing(WindowEvent e)
      {
          System.exit(0);
      }
  });
  Dimension maxDim = Toolkit.getDefaultToolkit().getScreenSize();
  screenHeight = (int)maxDim.getHeight();
  screenWidth = (int)maxDim.getWidth();
  Tetris tetris;
  tetris = new Tetris(screenWidth,screenHeight);
  JMenuBar mbar = tetris.createMenuBar();
  frame.setJMenuBar(mbar);
  frame.setContentPane(tetris);
  frame.pack();
  Thread t = Thread.currentThread();
  t.setPriority(Thread.NORM_PRIORITY-2);
  frame.addKeyListener(tetris);
  frame.setVisible(true);
}



class Options
{
  public String network = null;
  public byte hotSeat = 1;
  public Dimension gridSize = new Dimension(10,20);
  public byte blockCount = 4;
  public byte speed = 1;
  public byte startLevel = 1;        
}

}
