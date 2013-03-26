import processing.core.*; 
import processing.data.*; 
import processing.opengl.*; 

import processing.opengl.*; 
import processing.serial.*; 
import com.modestmaps.*; 
import com.modestmaps.core.*; 
import com.modestmaps.geo.*; 
import com.modestmaps.providers.*; 
import guicomponents.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class Write_GPS_3 extends PApplet {


 






PFont buttonFont; 
Serial port; 
GTextField latBox, longBox; 
myButton helpButton; 
myButton sendData; 
myButton startButton; 
myButton[] serialButtons = new myButton[Serial.list().length]; 
int screenNum = 0; 
int textY = 30; 
int textX = 80; 
int serialConnected = 2; 
boolean sent = false; 
String latitude = "" ; 
String longitude = ""; 
String portRead = ""; 
String wholeData = ""; 
int readY = 40 + height/2; 
boolean refresh = true; 
boolean started = false; 
float data1, data2; 
float latFloat, longFloat; 
PrintWriter writer;
InteractiveMap map;
ZoomButton out = new ZoomButton(5, 5, 14, 14, false);
ZoomButton in = new ZoomButton(22, 5, 14, 14, true);
PanButton up = new PanButton(14, 25, 14, 14, UP);
PanButton down = new PanButton(14, 57, 14, 14, DOWN);
PanButton left = new PanButton(5, 41, 14, 14, LEFT);
PanButton right = new PanButton(22, 41, 14, 14, RIGHT);

Button[] buttons = {
  in, out, up, down, left, right
};
double tx, ty, sc;

public void setup() {
  String lat = "ENTER LATITUDE";
  String lon = "ENTER LONGITUDE"; 
  latBox = new GTextField(this, lat, 10, 100, 100, 50, true);
  longBox = new GTextField(this, lon, 10, 200, 100, 50, true);
  frameRate(32); 
  String lines[] = loadStrings("coordinates.txt");

  startButton = new myButton(); 
  sendData = new myButton(); 
  //buttonFont = loadFont("ArialMT-14.vlw");
  buttonFont = loadFont("AngsanaUPC-Bold-30.vlw");
  size(displayWidth - 150, displayHeight - 100); 
  smooth();
  try {
    String connection = Serial.list()[0]; 
    port = new Serial(this, connection, 115200);
  }
  catch(Exception e) {
    e.printStackTrace();
  }
  println(Serial.list());
  helpButton = new myButton(100, 40);
  for (int qq = 0; qq < serialButtons.length; qq++) {
    serialButtons[qq] = new myButton();
  }
  String template = "http://{S}.mqcdn.com/tiles/1.0.0/osm/{Z}/{X}/{Y}.png";
  String[] subdomains = new String[] { 
    "otile1", "otile2", "otile3", "otile4"
  };
  latitude = lines[0]; 
  longitude = lines[1];
  map = new InteractiveMap(this, new TemplatedMapProvider(template, subdomains));
  countDigits(); 
  //latFloat = float(latitude)/1000000; 
  //longFloat = float(longitude)/1000000; 
  map.setCenterZoom(new Location(latFloat, longFloat), 11);  

  addMouseWheelListener(new java.awt.event.MouseWheelListener() { 
    public void mouseWheelMoved(java.awt.event.MouseWheelEvent evt) { 
      mouseWheel(evt.getWheelRotation());
    }
  }
  );
  writer = createWriter("coordinates.txt"); 
  writer.println(latitude); 
  writer.println(longitude);
}

public void draw() {
  if (started) {
    if (screenNum == 0) {
      if (refresh)
        background(0); 
      map.draw();
      textFont(buttonFont, 30); 
      Location mouseLocation = map.pointLocation(mouseX, mouseY);
      rect(5, height-5-g.textSize, textWidth("mouse: " + mouseLocation), g.textSize+textDescent());
      fill(0);
      text("mouse: " + mouseLocation, 5, height-5);
      fill(255); 
      rect(0, 0, width, 80); 
      fill(0); 
      textFont(buttonFont, 25); 
      text("Latitude: ", 10, 95); 
      text("Longitude: ", 10, 195);
      countDigits();  
      text("Latitude: " + latFloat, 400, 35); 
      text("Longitude: " + longFloat, 600, 35); 
      helpButton.show("help", width-100, textY, true);
      if (!sent) {
        noFill(); 
        rect(70, 10, 310, 30); 
        sendData.show("Send Coordinates to Arduino", 80, 35, false);
      }
      if (sendData.checkPress() && mousePressed && !sent) {
        cursor(WAIT); 
        try {
          port.write(latitude + ", " + longitude);
        }
        catch(Exception e) {
          e.printStackTrace();
        }
        sent = true;
        refresh = false;
      }
      else if (sendData.checkPress()) {
        cursor(HAND);
      }
      if (portRead.length() < 12) {
        try {
          portRead += port.read();
        }
        catch(Exception e) {
          e.printStackTrace();
        }
      }
      else 
        portRead = ""; 
      fill(0); 
      textFont(buttonFont, 25); 
      text(wholeData, 100, 60); 

      if (readY < height) {
        refresh = false;
      }
      else {
        readY = 50 + height/2;
        refresh = true;
      }
    }
    else if (screenNum == 1) {
      background(220); 
      text("Available Serial Ports: ", textX, 80);
      line(textX, 85, textX + 200, 85);  
      helpButton.show("return", width-100, textY, true);
      try {
        text("Connected", serialButtons[serialConnected].buttonX + 80, serialButtons[serialConnected].buttonY);
      }
      catch(Exception e) {
        e.printStackTrace();
      }
      for (int qq = 0; qq < serialButtons.length; qq++) {
        serialButtons[qq].show(Serial.list()[qq], textX + 80, qq*40 + 150, false);
        if (serialButtons[qq].checkPress() && mousePressed) {
          cursor(WAIT);
          try {
            String connection = Serial.list()[qq]; 
            port = new Serial(this, connection, 9600);
            serialConnected = qq;
          }
          catch(Exception e) {
            e.printStackTrace();
          }
        }
        else if (serialButtons[qq].checkPress()) {
          cursor(HAND);
        }
        else {
          cursor(ARROW);
        }
      }
    }

    if (helpButton.checkPress() && mousePressed) {
      cursor(WAIT);

      if (screenNum == 0 && mousePressed) {
        screenNum = 1; 
        delay(100);
        refresh = true;
      }
      else if (screenNum == 1 && mousePressed) {
        screenNum = 0;
        delay(100);
      }
    }
    else if (helpButton.checkPress()) {
      cursor(HAND);
    }
    else {
      cursor(ARROW);
    }

    boolean hand = false;
    for (int i = 0; i < buttons.length; i++) {
      buttons[i].draw();
      hand = hand || buttons[i].mouseOver();
    }

    if (keyPressed) {
      if (key == CODED) {
        if (keyCode == LEFT) {
          map.tx += 5.0f/map.sc;
        }
        else if (keyCode == RIGHT) {
          map.tx -= 5.0f/map.sc;
        }
        else if (keyCode == UP) {
          map.ty += 5.0f/map.sc;
        }
        else if (keyCode == DOWN) {
          map.ty -= 5.0f/map.sc;
        }
      }  
      else if (key == '+' || key == '=') {
        map.sc *= 1.05f;
      }
      else if (key == '_' || key == '-' && map.sc > 2) {
        map.sc *= 1.0f/1.05f;
      }
    }
  }
  else {
    background(255); 
    textFont(buttonFont, 55); 
    startButton.show("Start", width/2, height/2, false); 
    if (startButton.checkPress() && mousePressed) {
      started = true;
      cursor(WAIT);
    }
    else if (startButton.checkPress()) {
      cursor(HAND);
    }
    else {
      cursor(ARROW);
    }
  }
}

public void serialEvent(Serial port) { 
  String inString = port.readStringUntil('\n');
  if (inString != null)
    wholeData = inString;
  else 
    wholeData = "";
}


public void keyReleased() {
  if (key == 's' || key == 'S') {
    save("modest-maps-app.png");
  }
  else if (key == 'z' || key == 'Z') {
    map.sc = pow(2, map.getZoom());
  }
  else if (key == ' ') {
    map.sc = 2.0f;
    map.tx = -128;
    map.ty = -128;
  }
}

public void mouseDragged() {
  boolean hand = false;
  for (int i = 0; i < buttons.length; i++) {
    hand = hand || buttons[i].mouseOver();
    if (hand) break;
  }
  if (!hand) {
    map.mouseDragged();
  }
}

public void mouseWheel(int delta) {
  float sc = 1.0f;
  if (delta < 0) {
    sc = 1.05f;
  }
  else if (delta > 0) {
    sc = 1.0f/1.05f;
  }
  float mx = mouseX - width/2;
  float my = mouseY - height/2;
  map.tx -= mx/map.sc;
  map.ty -= my/map.sc;
  map.sc *= sc;
  map.tx += mx/map.sc;
  map.ty += my/map.sc;
}

public void mouseClicked() {
  if (in.mouseOver()) {
    map.zoomIn();
  }
  else if (out.mouseOver()) {
    map.zoomOut();
  }
  else if (up.mouseOver()) {
    map.panUp();
  }
  else if (down.mouseOver()) {
    map.panDown();
  }
  else if (left.mouseOver()) {
    map.panLeft();
  }
  else if (right.mouseOver()) {
    map.panRight();
  }
}

class Button {
  float x, y, w, h;

  Button(float x, float y, float w, float h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
  } 
  
  public boolean mouseOver() {
    return (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h);
  }
  
  public void draw() {
    stroke(80);
    fill(mouseOver() ? 255 : 220);
    rect(x,y,w,h); 
  }
  
}

class ZoomButton extends Button {
  boolean in = false;
  
  ZoomButton(float x, float y, float w, float h, boolean in) {
    super(x, y, w, h);
    this.in = in;
  }
  
  public void draw() {
    super.draw();
    stroke(0);
    line(x+3,y+h/2,x+w-3,y+h/2);
    if (in) {
      line(x+w/2,y+3,x+w/2,y+h-3);
    }
  }
  
}

class PanButton extends Button {
  
  int dir = UP;
  
  PanButton(float x, float y, float w, float h, int dir) {
    super(x, y, w, h);
    this.dir = dir;
  }
  
  public void draw() {
    super.draw();
    stroke(0);
    switch(dir) {
      case UP:
        line(x+w/2,y+3,x+w/2,y+h-3);
        line(x-3+w/2,y+6,x+w/2,y+3);
        line(x+3+w/2,y+6,x+w/2,y+3);
        break;
      case DOWN:
        line(x+w/2,y+3,x+w/2,y+h-3);
        line(x-3+w/2,y+h-6,x+w/2,y+h-3);
        line(x+3+w/2,y+h-6,x+w/2,y+h-3);
        break;
      case LEFT:
        line(x+3,y+h/2,x+w-3,y+h/2);
        line(x+3,y+h/2,x+6,y-3+h/2);
        line(x+3,y+h/2,x+6,y+3+h/2);
        break;
      case RIGHT:
        line(x+3,y+h/2,x+w-3,y+h/2);
        line(x+w-3,y+h/2,x+w-6,y-3+h/2);
        line(x+w-3,y+h/2,x+w-6,y+3+h/2);
        break;
    }
  }
}
public void countDigits() {
  if (abs(PApplet.parseFloat(latitude)) <= 100) {
    latFloat = PApplet.parseFloat(latitude);
  }
  else if (abs(PApplet.parseFloat(latitude)) <= 1000 && abs(PApplet.parseFloat(latitude)) > 100) {
    latFloat = PApplet.parseFloat(latitude)/10;
  }
  else if (abs(PApplet.parseFloat(latitude)) <= 10000 && abs(PApplet.parseFloat(latitude)) > 1000) {
    latFloat = PApplet.parseFloat(latitude)/100;
  }
  else if (abs(PApplet.parseFloat(latitude)) <= 100000 && abs(PApplet.parseFloat(latitude)) > 10000) {
    latFloat = PApplet.parseFloat(latitude)/1000;
  }
  else if (abs(PApplet.parseFloat(latitude)) <= 1000000 && abs(PApplet.parseFloat(latitude)) > 100000) {
    latFloat = PApplet.parseFloat(latitude)/10000;
  }
  else if (abs(PApplet.parseFloat(latitude)) <= 10000000 && abs(PApplet.parseFloat(latitude)) > 1000000) {
    latFloat = PApplet.parseFloat(latitude)/100000;
  }
  else if (abs(PApplet.parseFloat(latitude)) <= 100000000 && abs(PApplet.parseFloat(latitude)) > 10000000) {
    latFloat = PApplet.parseFloat(latitude)/1000000;
  }
  else {
    latFloat = PApplet.parseFloat(latitude)/10000000;
  }

  if (abs(PApplet.parseFloat(longitude)) <= 100) {
    longFloat = PApplet.parseFloat(longitude);
  }
  else if (abs(PApplet.parseFloat(longitude)) <= 1000 && abs(PApplet.parseFloat(longitude)) > 100) {
    longFloat = PApplet.parseFloat(longitude)/10;
  }
  else if (abs(PApplet.parseFloat(longitude)) <= 10000 && abs(PApplet.parseFloat(longitude)) > 1000) {
    longFloat = PApplet.parseFloat(longitude)/100;
  }
  else if (abs(PApplet.parseFloat(longitude)) <= 100000 && abs(PApplet.parseFloat(longitude)) > 10000) {
    longFloat = PApplet.parseFloat(longitude)/1000;
  }
  else if (abs(PApplet.parseFloat(longitude)) <= 1000000 && abs(PApplet.parseFloat(longitude)) > 100000) {
    longFloat = PApplet.parseFloat(longitude)/10000;
  }
  else if (abs(PApplet.parseFloat(longitude)) <= 10000000 && abs(PApplet.parseFloat(longitude)) > 1000000) {
    longFloat = PApplet.parseFloat(longitude)/100000;
  }
  else if (abs(PApplet.parseFloat(longitude)) <= 100000000 && abs(PApplet.parseFloat(longitude)) > 10000000) {
    longFloat = PApplet.parseFloat(longitude)/1000000;
  }
  else {
    longFloat = PApplet.parseFloat(longitude)/10000000;
  }
}

public void handleTextFieldEvents(GTextField tfield) {
  if (tfield == latBox) {
    switch (latBox.getEventType()) {
    case GTextField.ENTERED:
      if (latBox.getText() != null)
        latitude = latBox.getText(); 
      break;
    }
  }
  else if (tfield == longBox) {
    switch (longBox.getEventType()) {
    case GTextField.ENTERED:
      if (longBox.getText() != null)
        longitude = longBox.getText(); 
      break;
    }
  }
  countDigits(); 
  writer = createWriter("coordinates.txt"); 
  writer.println(latitude); 
  writer.println(longitude);

  map.setCenterZoom(new Location(latFloat, longFloat), 11);
}

public class myButton {
  int xLength, yLength, zLength; 
  int xCol = 0; 
  int buttonX, buttonY; 
  float rotateXPos = 0; 
  float rotateYPos = 0; 
  boolean pressed = false; 
  boolean goToWhite = true;
  String name;  

  myButton() {
    xLength = 300; 
    yLength = 50;
  }

  myButton(int x, int y) {
    xLength = x; 
    yLength = y;
  }

  public void show(String n, int xPos, int yPos, boolean changeCol) {
    stroke(0); 
    strokeWeight(1);  
    noFill(); 
    if (!checkPress() && changeCol) {
      if (goToWhite) {
        xCol += 10;
        if (xCol > 240) {
          goToWhite = false;
        }
      }
      else if (!goToWhite) {
        xCol -= 10;
        if (xCol <= 20) {
          goToWhite = true;
        }
      }
    }
    name = n; 
    if (n.equals("Send Coordinates to Arduino")) {
      fill(0); 
      textFont(buttonFont, 35);
    }
    else if (n.equals("Start")) {
      fill(0); 
      textFont(buttonFont, 45);
    }
    else {
      fill(xCol); 
      textFont(buttonFont, 28);
    }
    text(n, xPos, yPos);
    buttonX = xPos; 
    buttonY = yPos;
  }

  public boolean checkPress() {
    if (mouseX > buttonX && mouseX <= buttonX + xLength && mouseY >= buttonY - 40 
      && mouseY <= buttonY + yLength) {
      xCol = color(255);  
      return true;
    }
    else {
      return false;
    }
  }
}

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Write_GPS_3" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
