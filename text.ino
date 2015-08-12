#include<SoftwareSerial.h>

SoftwareSerial ss(4,3);//(TX,RX)

char buffer[100];
char *parseptr;
char buffidx;
char latdir, longdir;

uint8_t hour, minute, second;//Time value
uint32_t latitude, longitude, SatellitiesNum, Height;// lat, lon value
double latvalue1, latvalue2, lonvalue1, lonvalue2;
double latvalue3, latvalue, relaspeed;
double lonvalue3, lonvalue;
char status;

void setup() {

  Serial.begin( 9600 );
  ss.begin( 9600 );

}
void loop() {

uint32_t tmp;
readline();

  if ( strncmp( buffer, "$GPGGA", 6 ) == 0 )
  {

    //Time caculate and print
    parseptr = 0;
    parseptr = buffer + 7;
    tmp = parsedecimal( parseptr );
    hour = tmp / 10000 + 8;
    minute = ( tmp / 100 ) % 100;
    second = tmp % 100;

    parseptr = strchr( parseptr, ',' ) + 1;
    status = parseptr[0];

    Serial.print( "Time: " );
    Serial.print( hour, DEC );
    Serial.print( ':' );
    Serial.print( minute, DEC );
    Serial.print( ':' );
    Serial.print( second ,DEC );
    
    Serial.print( ", " );

    //latitude
    latitude = parsedecimal( parseptr );
    if ( latitude != 0 )
    {

      latitude *= 100000;
      parseptr = strchr( parseptr, '.' ) + 1;
      latitude += parsedecimal(parseptr);
      
    }

    parseptr = strchr( parseptr, ',' ) + 1;

    if ( parseptr[0] != ',' )
      latdir = parseptr[0];

    Serial.print("Lat: ");
    
    if ( latdir == 'N' )
      Serial.print( "N " );
    else if ( latdir == 'S' )
      Serial.print( "S " );

    latvalue1 = (latitude / 10000000);
    latvalue2 = (latitude / 100000 ) % 100;
    latvalue3 = double(latitude % 100000 ) * 6 / 10000;    
    latvalue = latvalue1 + latvalue2 / 60 + latvalue3 / 3600;

    //Serial.print( latitude / 1000000, DEC );
    //Serial.print( "/" );
    //Serial.print( ( latitude / 10000 ) % 100, DEC );
    //Serial.print( "/" );
    //Serial.print( ( latitude % 10000 ) * 6 / 1000, DEC );
    Serial.print( latvalue, 8 );
    Serial.print( ", " );

    //longitude
    parseptr = strchr( parseptr, ',' ) + 1;
    longitude = parsedecimal( parseptr );
    if ( longitude != 0 )
    {

       longitude *= 100000;
       parseptr = strchr( parseptr, '.' ) + 1;
       longitude += parsedecimal( parseptr );
      
    }

    parseptr = strchr( parseptr, ',' ) + 1;

    if ( parseptr[0] != ',' );
      longdir = parseptr[0];

    Serial.print( "Long: " );
    if ( longdir == 'E' )
      Serial.print( "E " );
    else if ( longdir == 'W' )
      Serial.print( "W " );

    lonvalue1 = longitude / 10000000;
    lonvalue2 = uint32_t( longitude / 100000 ) % 100;
    lonvalue3 = double(longitude % 100000 ) * 6 / 10000;    
    lonvalue = lonvalue1 + lonvalue2 / 60 + lonvalue3 / 3600;

    //Serial.print( longitude / 1000000 );
    //Serial.print( "/" );
    //Serial.print( ( longitude / 10000 ) % 100);
    //Serial.print( "/" );
    //Serial.print(  ( longitude % 10000 ) * 6 / 1000 );
    Serial.print( lonvalue, 8 );
    Serial.print( ", " );

    //Number of Satellities
    parseptr += 4;
    tmp = parsedecimal( parseptr );
    SatellitiesNum = tmp;

    Serial.print( "Number of Satellities: " );
    Serial.print( tmp );
    Serial.print( ", " );

    //Height
    parseptr += 6;
    tmp = parsedecimal( parseptr );
    Height = tmp;

    Serial.print( "Height: " );
    Serial.print( Height );
    Serial.print( ", " );

  }

  if ( strncmp( buffer, "$GPRMC", 6 ) == 0 )
  {

    parseptr = 0;
    parseptr = buffer + 30;
Serial.print( parseptr );
    tmp = parsedecimal( parseptr );
    relaspeed = tmp;
    
    Serial.print( "Relation ground speed: " );
    Serial.print( relaspeed );
    Serial.println();
  }
  

}
  
void readline( void ) //read NMEA Date
{
  
  char c;

  buffidx = 0; //Star at begninning

  if ( ss.available() )
  {

    while(1)
    {

      c = ss.read();
      if ( c == -1 )
        continue;
      if ( c == '\n' )
        continue;
      if ( ( buffidx == 99 ) || ( c == '\r' ) )
        {

             buffer[buffidx] = 0; 
             return;
             
        }

      buffer[buffidx++] = c;
      
    }
    
  }

}

uint32_t parsedecimal( char *str )
{

  uint32_t d = 0;

  while( str[0] != 0 )
  {

    if ( ( str[0] > '9' ) || ( str[0] < '0' ) )
      return d;

    d *= 10;
    d += str[0] - '0';
    str++;   
      
  }

  return d;
  
}

