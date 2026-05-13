#include <stdio.h>

// The "Interface" definition
typedef struct {
    void (*start)(void);
    void (*stop)(void);
} VehicleInterface;

// Implementation A: A Car
void carStart() { printf("Car starting...\n"); }
void carStop() { printf("Car stopping.\n"); }

// Implementation B: A Boat
void boatStart() { printf("Boat starting...\n"); }
void boatStop() { printf("Boat stopping.\n"); }

int main() {
    // Usage
    VehicleInterface myCar = { carStart, carStop };
    VehicleInterface myBoat = { boatStart, boatStop };
    VehicleInterface myHybrid = {boatStart, carStop};

    myCar.start();
    myCar.stop();
    myBoat.start();
    myBoat.stop();
    myHybrid.start();
    myHybrid.stop();

    return 0;
}
