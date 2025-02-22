#include "Plan.h"
using std::vector;
using namespace std;
#include <iostream>

// constructor

Plan::Plan(const int planId, const Settlement& settlement, SelectionPolicy* selectionPolicy, const vector<FacilityType>& facilityOptions): 
      plan_id(planId), settlement(settlement),selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE),facilities(),underConstruction(),facilityOptions(facilityOptions),          
      life_quality_score(0),
      economy_score(0),
      environment_score(0){}


// Copy constructor
Plan::Plan(const Plan& other):
    plan_id(other.plan_id),
    settlement(other.settlement),
    selectionPolicy(other.selectionPolicy->clone()),
    status(other.status),
    facilities(),  
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.environment_score) {

    for (Facility* facility :other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility :other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));  
    }
} 
//secondary copy constructor
Plan::Plan(const Plan& other, const Settlement& settlement):
plan_id(other.plan_id),
 settlement(settlement),
  selectionPolicy((other.selectionPolicy)->clone()),
status(other.status),
 facilities(),
  underConstruction(),
   facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),
 economy_score(other.economy_score),
  environment_score(other.environment_score) {
    for (Facility* facility: other.facilities){
        facilities.push_back(new Facility(*facility));
    }
    for (Facility* facility: other.underConstruction){
        underConstruction.push_back(new Facility(*facility));
    }
}

//Move copy constructor
Plan::Plan(Plan&& other): 
    plan_id(other.plan_id),
    settlement(other.settlement),
    selectionPolicy(other.selectionPolicy),
    status(other.status),
    facilities(),  
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.environment_score){
    
    for(Facility* facility: other.facilities){
        facilities.push_back(new Facility(*facility));
    }

    for(Facility* facility: other.underConstruction){
        underConstruction.push_back(facility);
    }

    other.selectionPolicy=nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
    }


// Destructor
Plan::~Plan(){    
    delete selectionPolicy;
    
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    for (Facility* facility : facilities) {
        delete facility;
    }
}

// Methods
const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}

const int Plan::getEconomyScore() const{
    return economy_score;
}

const int Plan::getEnvironmentScore() const{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy_){
    delete selectionPolicy;
    selectionPolicy = selectionPolicy_;
}

void Plan::step(){
    if (status == PlanStatus::AVALIABLE ){
        while (underConstruction.size() < settlement.constructionLimit()){
            FacilityType nextFacility= selectionPolicy->selectFacility(facilityOptions);
            Facility* facility=new Facility(nextFacility,settlement.getName());
            addFacility(facility);        
        }
   }
        for(size_t i=0; i<underConstruction.size(); i++ ){
            Facility* facility= underConstruction[i];
            facility->step();
            if (facility->getStatus()==FacilityStatus::OPERATIONAL){
                addFacility(facility);
                underConstruction.erase(underConstruction.begin()+i);
                life_quality_score+=facility->getLifeQualityScore();
                economy_score+=facility->getEconomyScore();
                environment_score+=facility->getEnvironmentScore();
                i--;
            }
        }
        if(underConstruction.size() >= settlement.constructionLimit()){
            status = PlanStatus::BUSY ;
        }
        else{
            status = PlanStatus::AVALIABLE;
        }

}

void Plan::addFacility(Facility* facility){
    if(facility->getStatus()==FacilityStatus::OPERATIONAL){
        facilities.push_back(facility);
    }
    else{
        underConstruction.push_back(facility);
    }
}

const vector<Facility*>& Plan::getFacilities() const{
    return facilities;
}

void Plan::printStatus(){
    cout<<strPlanStatus(status)<<endl;
}

const string Plan::toString() const{
    string strDoneFacilities;
    string underCoFacilities;
    for(Facility* facility: facilities){
        strDoneFacilities+= "FacilityName: "+facility->getName()+"\n"+"FacilityStatus: "+FacilityType::toStringFacilityStatus(facility->getStatus()) + "\n" ;
    }
    for(Facility* facility: underConstruction){
        underCoFacilities+="FacilityName: "+facility->getName()+"\n"+"FacilityStatus: "+FacilityType::toStringFacilityStatus(facility->getStatus() ) +"\n";
    }
    return "PlanID: "+to_string(plan_id)+"\n"+"SettlementName: "+getSettlement().getName()+"\n"+
    "PlanStatus: " + getStatus()+"\n"+ "SelectionPolicy: "+getSelectionPolicy().nameString()+
    "\n"+ "LifeQualityScore: "+to_string(getlifeQualityScore())+"\n"+"EconomyScore: "+to_string(getEconomyScore())+
    "\n"+ "EnvironmentScore: "+to_string(getEnvironmentScore())+"\n"+underCoFacilities+"\n"+strDoneFacilities+"\n";
}

const string Plan::strPlanStatus(PlanStatus status) const{
    if(status==PlanStatus::AVALIABLE){
        return "AVALIBLE";
    }
    else{
        return "BUSY";
    }
}

const int Plan:: getId() const {
    return plan_id;
}

const SelectionPolicy& Plan::getSelectionPolicy() const{
    return *selectionPolicy;
}

const Settlement& Plan::getSettlement() const{
    return settlement;
}

const string Plan::getStatus() const{
    return toStringPlanStatus(status);
}

string Plan::toStringPlanStatus(PlanStatus planStatus) {
    const string PlanStatusNames[] = {
        "AVALIABLE",       
        "BUSY"    
    };
    return PlanStatusNames[static_cast<int>(planStatus)];
}

const vector<Facility*> &Plan::getUnderConstruction() const{
    return underConstruction;
}
