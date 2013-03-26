void countDigits() {
  if (abs(float(latitude)) <= 100) {
    latFloat = float(latitude);
  }
  else if (abs(float(latitude)) <= 1000 && abs(float(latitude)) > 100) {
    latFloat = float(latitude)/10;
  }
  else if (abs(float(latitude)) <= 10000 && abs(float(latitude)) > 1000) {
    latFloat = float(latitude)/100;
  }
  else if (abs(float(latitude)) <= 100000 && abs(float(latitude)) > 10000) {
    latFloat = float(latitude)/1000;
  }
  else if (abs(float(latitude)) <= 1000000 && abs(float(latitude)) > 100000) {
    latFloat = float(latitude)/10000;
  }
  else if (abs(float(latitude)) <= 10000000 && abs(float(latitude)) > 1000000) {
    latFloat = float(latitude)/100000;
  }
  else if (abs(float(latitude)) <= 100000000 && abs(float(latitude)) > 10000000) {
    latFloat = float(latitude)/1000000;
  }
  else {
    latFloat = float(latitude)/10000000;
  }

  if (abs(float(longitude)) <= 100) {
    longFloat = float(longitude);
  }
  else if (abs(float(longitude)) <= 1000 && abs(float(longitude)) > 100) {
    longFloat = float(longitude)/10;
  }
  else if (abs(float(longitude)) <= 10000 && abs(float(longitude)) > 1000) {
    longFloat = float(longitude)/100;
  }
  else if (abs(float(longitude)) <= 100000 && abs(float(longitude)) > 10000) {
    longFloat = float(longitude)/1000;
  }
  else if (abs(float(longitude)) <= 1000000 && abs(float(longitude)) > 100000) {
    longFloat = float(longitude)/10000;
  }
  else if (abs(float(longitude)) <= 10000000 && abs(float(longitude)) > 1000000) {
    longFloat = float(longitude)/100000;
  }
  else if (abs(float(longitude)) <= 100000000 && abs(float(longitude)) > 10000000) {
    longFloat = float(longitude)/1000000;
  }
  else {
    longFloat = float(longitude)/10000000;
  }
}

