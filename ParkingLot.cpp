#include "ParkingLot.h"
#include "Vehicle.h"
namespace MtmParkingLot {
/*
     * ParkingLot functions
     *
     *
     */
ParkingLot::ParkingLot(unsigned int parkingBlockSizes[]):parking_blocks{NULL,NULL,NULL},
parkingBlockSizes{parkingBlockSizes[MOTORBIKE],parkingBlockSizes[HANDICAPPED],parkingBlockSizes[CAR]},
size(parkingBlockSizes[MOTORBIKE]+parkingBlockSizes[CAR]+parkingBlockSizes[HANDICAPPED]),
                                                         sorted{NULL}{
    parking_blocks[MOTORBIKE]=new UniqueArray<Vehicle,Vehicle::Compare>(parkingBlockSizes[MOTORBIKE]);
    parking_blocks[CAR]=new UniqueArray<Vehicle,Vehicle::Compare>(parkingBlockSizes[CAR]);
    parking_blocks[HANDICAPPED]=new UniqueArray<Vehicle,Vehicle::Compare>(parkingBlockSizes[HANDICAPPED]);
    sorted=new UniqueArray<Vehicle,Vehicle::Compare>(size);
}



ParkingLot::~ParkingLot() {
    for(int i=0;i<3;i++){
        delete parking_blocks[i];
    }
    delete sorted;
}

ParkingResult ParkingLot::enterParking(ParkingLotUtils::VehicleType vehicleType,
                                       ParkingLotUtils::LicensePlate licensePlate,
                                       ParkingLotUtils::Time entranceTime) {
    if(vehicleType==MOTORBIKE){
        return ParkingLot::enterVehicle(vehicleType,licensePlate,MOTORBIKE,entranceTime);
    }
    if(vehicleType==CAR){
        return ParkingLot::enterVehicle(vehicleType,licensePlate,CAR,entranceTime);
    }
    return ParkingLot::enterVehicle(vehicleType,licensePlate,HANDICAPPED,entranceTime);
}


ParkingResult ParkingLot::enterVehicle(ParkingLotUtils::VehicleType vehicleType,
                                       ParkingLotUtils::LicensePlate licensePlate,VehicleType parkingBlock,ParkingLotUtils::Time entranceTime){
    ParkingResult result=checkEnterParking(vehicleType,licensePlate,parkingBlock,entranceTime);
    if(result!=SUCCESS){
        return result;
    }
    //there surely is an empty place,except maybe for handicapped
    int parking_index=parking_blocks[parkingBlock]->firstEmpty();
    if(parking_index==-1){
        return enterVehicle(vehicleType,licensePlate,CAR,entranceTime);
    }
    ParkingSpot parkingSpot(parkingBlock,parking_index);
    Vehicle vehicle(vehicleType,licensePlate,entranceTime,parkingSpot,false);
    parking_blocks[parkingBlock]->insert(vehicle);
    ParkingLotPrinter::printVehicle(std::cout,vehicle.getVehicleType(),
                                    vehicle.getLicensePlate(),vehicle.getEntranceTime());
    ParkingLotPrinter::printEntrySuccess(std::cout,vehicle.getParkingSpot());
    enterSorted(vehicle);
    return SUCCESS;
}

ParkingResult ParkingLot::checkEnterParking(ParkingLotUtils::VehicleType vehicleType,
                                            ParkingLotUtils::LicensePlate licensePlate,VehicleType parkingBlock,ParkingLotUtils::Time entranceTime){
    const Vehicle* parked_vehicle=findVehicle(licensePlate);
    if(parked_vehicle!=NULL){
        ParkingLotPrinter::printVehicle(std::cout,vehicleType,licensePlate,parked_vehicle->getEntranceTime());
        ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,parked_vehicle->getParkingSpot());
        return VEHICLE_ALREADY_PARKED;
    }
    if(parking_blocks[parkingBlock]->getCount()>=parkingBlockSizes[parkingBlock]){
        //if the handicapped block is full we call the regular car block to see if there is a place
        if(vehicleType==HANDICAPPED&&parkingBlock==HANDICAPPED){
            return ParkingLot::checkEnterParking(vehicleType,licensePlate,CAR,entranceTime);
        }
        ParkingLotPrinter::printVehicle(std::cout,vehicleType,licensePlate,entranceTime);
        ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
        return NO_EMPTY_SPOT;
    }
    return SUCCESS;//it means no proplem was found
}



ParkingResult ParkingLot::exitParking(ParkingLotUtils::LicensePlate licensePlate, ParkingLotUtils::Time exitTime) {
    const Vehicle* vehicle=findVehicle(licensePlate);
    if(vehicle==NULL){
        ParkingLotPrinter::printExitFailure(std::cout,licensePlate);
        return VEHICLE_NOT_FOUND;
    }
    else{
        ParkingLotPrinter::printVehicle(std::cout,vehicle->getVehicleType(),vehicle->getLicensePlate(),
                                        vehicle->getEntranceTime());
        ParkingLotPrinter::printExitSuccess(std::cout,vehicle->getParkingSpot(),exitTime,vehicle->calculatePay(exitTime));
        removeSorted(*vehicle);
        parking_blocks[vehicle->getParkingSpot().getParkingBlock()]->remove(*vehicle);
        return SUCCESS;
    }
}



ostream& operator<<(ostream &os, const MtmParkingLot::ParkingLot &parkingLot){

    const Vehicle* vehicle;
    ParkingLotPrinter::printParkingLotTitle(std::cout);
    for(int i=0;i<parkingLot.size;i++){
        vehicle=parkingLot.sorted->findByIndex(i);
            if(vehicle!=NULL){
                ParkingLotPrinter::printVehicle(std::cout,vehicle->getVehicleType(),vehicle->getLicensePlate(),
                                                vehicle->getEntranceTime());
                ParkingLotPrinter::printParkingSpot(std::cout,vehicle->getParkingSpot());
            }
    }
    return os;
}
void ParkingLot::inspectParkingLot(ParkingLotUtils::Time inspectionTime) {
    const int allowed_parking_time=24;
    const Vehicle* vehicle_in_array;
    int count=0;
    Time parking_time;
    for(int parking_block=0;parking_block<3;parking_block++){
        for(unsigned int parking_spot=0;parking_spot<parking_blocks[parking_block]->getSize();parking_spot++){
            vehicle_in_array=parking_blocks[parking_block]->findByIndex(parking_spot);//possible memory leak**
            if(vehicle_in_array==NULL||vehicle_in_array->getFine()==true){
                continue;
            }
            parking_time=vehicle_in_array->getEntranceTime().operator-(inspectionTime);
            if(parking_time.toHours()>allowed_parking_time){
                const Vehicle* vehicle=new Vehicle(vehicle_in_array->getVehicleType(),vehicle_in_array->getLicensePlate(),
                                                   vehicle_in_array->getEntranceTime(),vehicle_in_array->getParkingSpot(),true);
                unsigned int index;
                parking_blocks[parking_block]->getIndex(*vehicle,index);
                parking_blocks[parking_block]->remove(*vehicle);
                parking_blocks[parking_block]->insertTo(*vehicle,index);
                count++;
                delete vehicle;
            }

        }
    }
    ParkingLotPrinter::printInspectionResult(std::cout,inspectionTime,count);
}



VehicleType ParkingLot::Search(Vehicle& vehicle)const {
    if(parking_blocks[MOTORBIKE]->operator[](vehicle)!=NULL){
        return MOTORBIKE;
    }
    if(parking_blocks[CAR]->operator[](vehicle)!=NULL){
        return CAR;
    }
    if(parking_blocks[HANDICAPPED]->operator[](vehicle)!=NULL){
        return HANDICAPPED;
    }
    //we dealt with this possiblity(the car is not parked)in the calling function
    return HANDICAPPED;
}


const Vehicle* ParkingLot::findVehicle(ParkingLotUtils::LicensePlate licensePlate) const{
    Time default_time;
    ParkingSpot default_parking_spot;
    Vehicle temp_vehicle(CAR,licensePlate,default_time,default_parking_spot,false);//CAR parkingSpot and exitTime are just defaults we dont use them
    VehicleType parkingBlock=ParkingLot::Search(temp_vehicle);
    const Vehicle* vehicle=parking_blocks[parkingBlock]->operator[](temp_vehicle);
    return vehicle;
}


    ParkingResult ParkingLot::getParkingSpot(ParkingLotUtils::LicensePlate licensePlate,
                                             ParkingLotUtils::ParkingSpot &parkingSpot) const {
        const Vehicle *vehicle = findVehicle(licensePlate);
        if (vehicle == NULL) {
            return VEHICLE_NOT_FOUND;
        } else {
            parkingSpot = vehicle->getParkingSpot();
            return SUCCESS;
        }
    }
    void ParkingLot::enterSorted(const Vehicle vehicle) const {
    int i;
            for(i=0;i<size&&sorted->findByIndex(i)!=NULL;i++){
                if(sorted->findByIndex(i)->getParkingSpot()<vehicle.getParkingSpot()){
                    continue;
                 }
                else{
                break;
                }
            }
                int k;
                for(k=i;k<size&&sorted->findByIndex(k)!=NULL;k++){
                    continue;
                }
                while(k>i){
                    sorted->insertTo(*sorted->findByIndex(k-1),k);
                    k--;
                    sorted->remove(*sorted->findByIndex(k));
                }
        sorted->insertTo(vehicle,i);
                printf("enter:current state\n");
                const Vehicle* vehicle1=NULL;
        for(int i=0;i<size;i++){
            vehicle1=sorted->findByIndex(i);
            if(vehicle1!=NULL){
                ParkingLotPrinter::printVehicle(std::cout,vehicle1->getVehicleType(),vehicle1->getLicensePlate(),
                                                vehicle1->getEntranceTime());
                ParkingLotPrinter::printParkingSpot(std::cout,vehicle1->getParkingSpot());
            }
        }
    }
    void ParkingLot::removeSorted(const Vehicle vehicle) const{
    unsigned int index;
    sorted->getIndex(vehicle,index);
    for(int i=index;i<size&&sorted->findByIndex(i)!=NULL;i++){
        sorted->remove(*sorted->findByIndex(i));
        sorted->insertTo(*sorted->findByIndex(i+1),i);
        printf("exit:current state\n");
        const Vehicle* vehicle1=NULL;
        for(int i=0;i<size;i++){
            vehicle1=sorted->findByIndex(i);
            printf("index is:%d",i);
                ParkingLotPrinter::printVehicle(std::cout,vehicle1->getVehicleType(),vehicle1->getLicensePlate(),
                                                vehicle1->getEntranceTime());
                ParkingLotPrinter::printParkingSpot(std::cout,vehicle1->getParkingSpot());
            }
        }
    }
    }

