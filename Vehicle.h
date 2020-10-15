
#ifndef UNTITLED1_VEHICLE_H
#define UNTITLED1_VEHICLE_H
#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
using namespace ParkingLotUtils;


/*
 * Vehicle class
 *
 *
 */
class Vehicle {
    VehicleType vehicle_type;
    LicensePlate license_plate;
    Time entrance_time;
    ParkingSpot parking_spot;
    bool fine;
public:
    Vehicle(VehicleType vehicleType, LicensePlate licensePlate, Time entrance_time, ParkingSpot parkingSpot,
            bool Fine);
    /*
       * calculatePay:calculates the fees the vehicle has to pay when exiting the parking block
       *
       * &param:exitTime representing the exit time of the vhicle
       *
       * &return:the fees the vehicle has to pay
       */
    unsigned int calculatePay(Time exitTime) const;
/*
     * Pay:calculates the payment of the vehicle based on the paramaters given
     *
     * &param:
     * hours:the number of hours the vehicle was in the parking block
     * pay_per_hour:the amount the vehicle type has to pay per hour of staying in the parking block
     */
    unsigned int Pay(int hours, int pay_per_hour) const;


    /*
        * functions to get to the private fields of the class
        */
    VehicleType getVehicleType() const { return vehicle_type; };
    LicensePlate getLicensePlate() const { return license_plate; };
    Time getEntranceTime() const { return entrance_time; };
    ParkingSpot getParkingSpot() const { return parking_spot; };
    bool getFine() const { return fine; };

    class Compare {
    public:
        bool operator()(const Vehicle &vehicle1, const Vehicle &vehicle2) const {
            return vehicle1.license_plate.compare(vehicle2.license_plate) == 0;//**
        }
    };
};


#endif //UNTITLED1_VEHICLE_H
