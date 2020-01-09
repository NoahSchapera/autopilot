#include<iostream>
#include<math.h>


using namespace std;


double getBearing(double, double, double, double);
double radians(double);
int main()
{
    double lat1 = 39.099912;
    double long1 = -94.581213;
    double lat2 = 38.627089;
    double long2 = -90.200203;

    double bearing = getBearing(lat1,long1,lat2,long2);

    cout << bearing << endl;

    return 0;

}
double radians(double angle)
{
    double Radian = (angle*3.14159)/180;
    return Radian;

}
double  getBearing(double latNow, double longNow, double destinLat, double destinLong)
{
  double X = cos(radians(destinLat))*sin(radians(destinLong-longNow));
  cout << X << endl;
  double Y = cos(radians(latNow))*sin(radians(destinLat))-sin(radians(latNow))*cos(radians(destinLat))*cos(radians(destinLong-longNow));
  cout << Y << endl;
  double bearing = (180*atan2(X,Y))/3.1415;
  return bearing;
}
