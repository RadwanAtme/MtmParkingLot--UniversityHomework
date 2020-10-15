#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "UniqueArray.h"
#include "ParkingLotPrinter.h"
#include "Vehicle.h"
namespace MtmParkingLot {
    using namespace ParkingLotUtils;
    using std::ostream;

/*
 * ParkingLot class
 *
 *
 */
    class ParkingLot {
        UniqueArray<Vehicle, Vehicle::Compare> *parking_blocks[3];//parking_block[0]=motor,parking_block[1]=handicapped,parking_block[2]=car
        unsigned int parkingBlockSizes[3];
        int size;
        UniqueArray<Vehicle,Vehicle::Compare> *sorted;
    public:
        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot();
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot &parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream &operator<<(ostream &os, const ParkingLot &parkingLot);
/*
 * enterVehicle:enters the element of type Vehicle to the given parkingBlock
 *
 * &param:vehicleType,license plate,parking block and the entrance time
 *
 * &return:ALREADY_PARKED in case the car is already parked in the parking block
 *          NO_EMPTY_SPOT if there is no empty spots in the parking block
 *           SUCCESS in case of success
 */
        ParkingResult enterVehicle(ParkingLotUtils::VehicleType vehicleType,
                                   ParkingLotUtils::LicensePlate licensePlate, VehicleType parkingBlock,
                                   ParkingLotUtils::Time entranceTime);

        /*
         * Search:returns the parking block the vehicle is in
         *
         * &param:vehicle reffrence of type Vehicle(class we made)
         *
         * &return:parking block of type VehicleType,we dealt with the possibility that the vehicle is not parked at all
         * in the functions calling search
         */
        VehicleType Search(Vehicle &vehicle) const;
/*
 * findVehicle:functions that returns a pointer to a vehicle that is parked given its licenseplate
 *
 * &param:licenseplate
 *
 * &return:pointer to the vehicle in case of success
 *          NULL otherwise
 */
        const Vehicle *findVehicle(LicensePlate licensePlate) const;
/*
 * checkEnterParking:checks if there is an avilable spot for the vehicle or if the vehicle is already parked
 *
 * &return:VEHICLE_ALREADY_PARKED if the vehicle is already parked
 *          NO_EMPTY_SPOT if there is no empty spot in the parking block
 *           SUCCESS if an empty spot was found and the vehice is not already parked
 */
        ParkingResult checkEnterParking(ParkingLotUtils::VehicleType vehicleType,
                                        ParkingLotUtils::LicensePlate licensePlate, VehicleType parkingBlock,
                                        ParkingLotUtils::Time entranceTime);
        void enterSorted(const Vehicle vehicle) const;
        void removeSorted(const Vehicle vehicle) const;


    };
}

#endif //MTMPARKINGLOT_PARKINGLOT_H
