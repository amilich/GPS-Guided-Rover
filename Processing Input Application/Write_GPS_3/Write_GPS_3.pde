//This program is covered under the Attribution-NonCommercial-ShareAlike 3.0 Unported license. See liscence on http://bobulisk.github.com/GPS-Guided-Rover/.

import processing.opengl.*;
import processing.serial.*; 
import com.modestmaps.*;
import com.modestmaps.core.*;
import com.modestmaps.geo.*;
import com.modestmaps.providers.*;
import guicomponents.*;

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

void setup() {
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
  size(screen.width - 150, screen.height - 100); 
  smooth();
  try {
    String connection = Serial.list()[2]; 
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
  String template = "http://{S}.mqcdn.com/tiles/1.0.0/osm/{Z}/{X}/{Y}.png"; //map data
  String[] subdomains = new String[] { 
    "otile1", "otile2", "otile3", "otile4"
  };
  latitude = lines[0]; //read from file
  longitude = lines[1];
  map = new InteractiveMap(this, new TemplatedMapProvider(template, subdomains));
  countDigits(); //interpret incoming data
  //latFloat = float(latitude)/1000000; 
  //longFloat = float(longitude)/1000000; 
  map.setCenterZoom(new Location(latFloat, longFloat), 11);  

  addMouseWheelListener(new java.awt.event.MouseWheelListener() { 
    public void mouseWheelMoved(java.awt.event.MouseWheelEvent evt) { 
      mouseWheel(evt.getWheelRotation());
    }
  }
  );
  writer = createWriter("coordinates.txt"); //to save coordinaets (default)
  writer.println(latitude); 
  writer.println(longitude);
}

void draw() {
  if (started) {
    if (screenNum == 0) {
      if (refresh)
        background(0); 
      map.draw();
      textFont(buttonFont, 30); 
      Location mouseLocation = map.pointLocation(mouseX, mouseY);
      rect(5, height-5-g.textSize, textWidth("mouse: " + mouseLocation), g.textSize+textDescent());
      fill(0);
      text("mouse: " + mouseLocation, 5, height-5); //know mouse latitude/longitude
      fill(255); 
      rect(0, 0, width, 80); 
      fill(0); 
      textFont(buttonFont, 25); 
      text("Latitude: ", 10, 95); 
      text("Longitude: ", 10, 195);
      countDigits();  
      text("Latitude: " + latFloat, 400, 35); 
      text("Longitude: " + longFloat, 600, 35); 
      helpButton.show("help", width-100, textY, true); //reconfigure serial configurations
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
          portRead += port.read(); //see arduino's serial data
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
    else if (screenNum == 1) { //help screen
      background(220); 
      text("Available Serial Ports: ", textX, 80);
      line(textX, 85, textX + 200, 85);  
      helpButton.show("return", width-100, textY, true);
      try {
        text("Connected", serialButtons[serialConnected].buttonX + 80, serialButtons[serialConnected].buttonY); //show connections under help page
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

    if (keyPressed) { //move
      if (key == CODED) {
        if (keyCode == LEFT) {
          map.tx += 5.0/map.sc;
        }
        else if (keyCode == RIGHT) {
          map.tx -= 5.0/map.sc;
        }
        else if (keyCode == UP) {
          map.ty += 5.0/map.sc;
        }
        else if (keyCode == DOWN) {
          map.ty -= 5.0/map.sc;
        }
      }  
      else if (key == '+' || key == '=') {
        map.sc *= 1.05;
      }
      else if (key == '_' || key == '-' && map.sc > 2) {
        map.sc *= 1.0/1.05;
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

void serialEvent(Serial port) { //receive serial data
  String inString = port.readStringUntil('\n');
  if (inString != null)
    wholeData = inString;
  else 
    wholeData = "";
}


void keyReleased() {
  if (key == 's' || key == 'S') {
    save("modest-maps-app.png");
  }
  else if (key == 'z' || key == 'Z') {
    map.sc = pow(2, map.getZoom());
  }
  else if (key == ' ') {
    map.sc = 2.0;
    map.tx = -128;
    map.ty = -128;
  }
}

void mouseDragged() { //move map
  boolean hand = false;
  for (int i = 0; i < buttons.length; i++) {
    hand = hand || buttons[i].mouseOver();
    if (hand) break;
  }
  if (!hand) {
    map.mouseDragged();
  }
}

void mouseWheel(int delta) { //scale map
  float sc = 1.0;
  if (delta < 0) {
    sc = 1.05;
  }
  else if (delta > 0) {
    sc = 1.0/1.05;
  }
  float mx = mouseX - width/2;
  float my = mouseY - height/2;
  map.tx -= mx/map.sc;
  map.ty -= my/map.sc;
  map.sc *= sc;
  map.tx += mx/map.sc;
  map.ty += my/map.sc;
}

void mouseClicked() {
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

