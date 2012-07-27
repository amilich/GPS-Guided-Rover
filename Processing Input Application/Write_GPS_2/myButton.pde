//serial and help buttons
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

  void show(String n, int xPos, int yPos, boolean changeCol) { //name, pos, flashing
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

  boolean checkPress() { //does not check if mouse is down - really a mouse over function
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

