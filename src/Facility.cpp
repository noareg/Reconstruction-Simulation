#include "Facility.h"
using namespace std;


// Constructor
FacilityType::FacilityType (const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score): 
name(name),  category(category), price(price), lifeQuality_score (lifeQuality_score), economy_score (economy_score), environment_score(environment_score){}

// Methods
const string& FacilityType :: getName() const {
    return name;
}
int FacilityType :: getCost() const {
    return price;
}
int FacilityType :: getLifeQualityScore() const {
    return lifeQuality_score;
}
int FacilityType :: getEnvironmentScore() const {
    return environment_score;
}
int FacilityType :: getEconomyScore() const {
    return economy_score;
}
FacilityCategory FacilityType :: getCategory() const {
    return category;
}

// Constructor
Facility::Facility (const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}

// Constructor, input- Facility type object
Facility::Facility (const FacilityType &type, const string &settlementName): 
FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}

//methods
const string& Facility:: getSettlementName() const {
    return settlementName;
}

const int Facility:: getTimeLeft() const{
    return timeLeft;
}

FacilityStatus Facility:: step(){
    if(status==FacilityStatus::UNDER_CONSTRUCTIONS){
        timeLeft--;
    }
    if (timeLeft==0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

void Facility:: setStatus(FacilityStatus _status){
    status= _status;
}

const FacilityStatus& Facility:: getStatus() const{
    return status;
}

const string Facility:: toString() const{
    return "Facility name: " + name + "\n" +"Facility category: " + toStringFacilityCategory(category)+ "\n" +"Facility status: "+ toStringFacilityStatus(status) +"\n"+"Price: " + to_string(price) + "\n" +"Life quality score: "+ to_string(lifeQuality_score)
    +"\n" + "Economy score: "+ to_string(economy_score)+"\n" + "Environment score: "+ to_string(environment_score)+"\n" + "Settlement Name: "+settlementName+"\n" + "Time left: "
    + to_string(timeLeft)+"\n" ;        
}

string FacilityType::toStringFacilityStatus(FacilityStatus facilityStatus) {
    const string facilityStatusNames[] = {
        "Under Construction",       
        "Operational"    
    };
    return facilityStatusNames[static_cast<int>(facilityStatus)];
}

string FacilityType::toStringFacilityCategory(FacilityCategory facilityCategory) {
    const string facilityCategoryNames[] = {
        "Life Quality",       
        "Economy",          
        "Environment"    
    };
    return facilityCategoryNames[static_cast<int>(facilityCategory)];
}



