void handleTextFieldEvents(GTextField tfield) {
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

