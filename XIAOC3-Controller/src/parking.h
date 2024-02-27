#ifndef _PARKING_H_
#define _PARKING_H_

class Parking {
   private:
   public:
    static const int Available = 0;
    static const int Occupied = 1;
    static const int Reserved = 2;

    // status | 0: available, 1: occupied , 2: reserved
    int status[4] = {0, 0, 0, 0};

    bool isApproaching = false;
    int approachingParkingId = 0;
};

#endif  // _PARKING_H_