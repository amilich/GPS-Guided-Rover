//set new latitude and longitude fromt text boxes

void handleTextFieldEvents(GTextField tfield) {
  if (tfield == latBox) { //change latitude
    switch (latBox.getEventType()) { 
    case GTextField.ENTERED:
      if (latBox.getText() != null)
        latitude = latBox.getText(); //set new latitude
      break;
    }
  }
  else if (tfield == longBox) { //changed longitude
    switch (longBox.getEventType()) {
    case GTextField.ENTERED:
      if (longBox.getText() != null)
        longitude = longBox.getText(); 
      break;
    }
  }
  countDigits(); //again, interpret new data
  writer = createWriter("coordinates.txt"); 
  writer.println(latitude); 
  writer.println(longitude);

  map.setCenterZoom(new Location(latFloat, longFloat), 11);
}

