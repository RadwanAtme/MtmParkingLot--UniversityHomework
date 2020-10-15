#include "Vehicle.h"
/*
 * Vehicle functions
 *
 *
 */
Vehicle::Vehicle(VehicleType vehicleType,LicensePlate licensePlate, Time entrance_time, ParkingSpot parkingSpot,
                 bool Fine):vehicle_type(vehicleType),license_plate(licensePlate),entrance_time(entrance_time),
                            parking_spot(parkingSpot),fine(Fine){}

unsigned int Vehicle::calculatePay(ParkingLotUtils::Time exitTime) const {
    if(vehicle_type==HANDICAPPED){
        if(fine){
            return 250+15;
        }
        return 15;
    }
    Time parking_time=entrance_time.operator-(exitTime);
    int hours=parking_time.toHours();
    if(vehicle_type==CAR){
        return Pay(hours,20);
    }
    //MOTORBIKE
    return Pay(hours,10);

}
unsigned int Vehicle::Pay(int hours, int pay_per_hour)const {
    int fined=0;
    if(hours>6){
        hours=6;
    }
    if(fine){
        fined=250;
    }
    return pay_per_hour+(pay_per_hour/2)*(hours-1)+fined;
}