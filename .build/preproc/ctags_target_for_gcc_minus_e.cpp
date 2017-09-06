# 1 "D:\\VsCode\\Direction\\magDirection.ino"
# 1 "D:\\VsCode\\Direction\\magDirection.ino"
# 2 "D:\\VsCode\\Direction\\magDirection.ino" 2
# 3 "D:\\VsCode\\Direction\\magDirection.ino" 2
# 4 "D:\\VsCode\\Direction\\magDirection.ino" 2
# 5 "D:\\VsCode\\Direction\\magDirection.ino" 2
# 6 "D:\\VsCode\\Direction\\magDirection.ino" 2
# 7 "D:\\VsCode\\Direction\\magDirection.ino" 2


DevI2C *ext_i2c;
LIS2MDLSensor *magnetometer;
char wifiBuff[128];
char buff[128];
int axes[3];
static bool isConnected;
float declination_offset_radians = 0;

void InitWiFi()
{
  Screen.print("WiFi \r\n \r\nConnecting...\r\n             \r\n");

  if(WiFi.begin() == WL_CONNECTED)
  {
    IPAddress ip = WiFi.localIP();
    snprintf(wifiBuff, 128, "WiFi Connected\r\n%s\r\n%s\r\n \r\n",WiFi.SSID(),ip.get_address());
    Screen.print(wifiBuff);
    isConnected = true;
  }
  else
  {
    snprintf(wifiBuff, 128, "No WiFi\r\nEnter AP Mode\r\nto config\r\n                 \r\n");
    Screen.print(wifiBuff);
  }
}

float Getheading(double x, double y)
{
  float head = atan2(y, x); // Slope Y, Slope X
  return head;
}

void SetDeclination(int declination_degs , char declination_dir )
{
    // Convert declination to decimal degrees
    switch(declination_dir)
    {
      // North and East are positive   
      case 'E':
        declination_offset_radians = declination_degs * (
# 48 "D:\\VsCode\\Direction\\magDirection.ino" 3
                                                         3.14159265358979323846 
# 48 "D:\\VsCode\\Direction\\magDirection.ino"
                                                              / 180);
        break;

      // South and West are negative    
      case 'W':
        declination_offset_radians = 0 - ( declination_degs * (
# 53 "D:\\VsCode\\Direction\\magDirection.ino" 3
                                                                3.14159265358979323846 
# 53 "D:\\VsCode\\Direction\\magDirection.ino"
                                                                     / 180) ) ;
        break;
    }
}

void showMagneticSensor()
{
  // read magnetometer
  magnetometer->getMAxes(axes);

  // calculate heading 
  float heading = Getheading(axes[0],axes[1]);
  heading += declination_offset_radians;

  // Correct for when signs are reversed.
  if(heading < 0)
     heading += 2*
# 69 "D:\\VsCode\\Direction\\magDirection.ino" 3
                 3.14159265358979323846
# 69 "D:\\VsCode\\Direction\\magDirection.ino"
                     ;

   // Check for wrap due to addition of declination.
   if(heading > 2*
# 72 "D:\\VsCode\\Direction\\magDirection.ino" 3
                 3.14159265358979323846
# 72 "D:\\VsCode\\Direction\\magDirection.ino"
                     )
     heading -= 2*
# 73 "D:\\VsCode\\Direction\\magDirection.ino" 3
                 3.14159265358979323846
# 73 "D:\\VsCode\\Direction\\magDirection.ino"
                     ;

  heading = heading * 180/
# 75 "D:\\VsCode\\Direction\\magDirection.ino" 3
                         3.14159265358979323846
# 75 "D:\\VsCode\\Direction\\magDirection.ino"
                             ;
  int deg = (int)heading;

  // display direction
  snprintf(buff, 128, "Magnetometer \r\n x:%d   y:%d    z:%d \r\n heading : %d  ", axes[0], axes[1], axes[2], deg);
  Screen.print(buff);

}

void setup()
{
  Screen.init();
  Screen.print(1,"Start init",false);

  // initialize magnetometer
  ext_i2c = new DevI2C(D14, D15);
  magnetometer = new LIS2MDLSensor(*ext_i2c);
  magnetometer->init(
# 92 "D:\\VsCode\\Direction\\magDirection.ino" 3 4
                    __null
# 92 "D:\\VsCode\\Direction\\magDirection.ino"
                        );

  // set magnetic deviation
  SetDeclination(2,'W');
  delay(1000);
  Screen.print(1,"End init",false);

  // init wifi connection
  InitWiFi();
  Screen.clean();

}

void loop()
{
  showMagneticSensor();
  delay(500);
}
