# include "Action.h"
#include <iostream>
using std::string;
#include "Auxiliary.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ostream>
using namespace std;

BaseAction::BaseAction():errorMsg(""),status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status= ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg_){
    status= ActionStatus::ERROR;
    errorMsg= errorMsg_;
    cout<<"Error: "+errorMsg<< endl;
}

const string& BaseAction::getErrorMsg() const{
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation& simulation){
    for(int i=0; i<numOfSteps; i++){
        simulation.step();
    }
    complete();
    simulation.addAction(this);
}

const string SimulateStep::toString() const {
    return "step "+to_string(numOfSteps);
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):
settlementName(settlementName), selectionPolicy(selectionPolicy){}

void AddPlan::act(Simulation &simulation){
    if (simulation.isSettlementExists(settlementName)){
        Settlement& settlement=simulation.getSettlement(settlementName); //pay attention!
        SelectionPolicy* policy= nullptr;
        if (selectionPolicy=="nve"){
            policy=new NaiveSelection();
        }
        else if (selectionPolicy=="bal"){
            policy=new BalancedSelection(0,0,0);
        }
        else if (selectionPolicy=="eco"){
            policy=new EconomySelection();
        }
        else if (selectionPolicy=="env"){
            policy=new SustainabilitySelection();
        }
        else {
            error("Cannot create this Plan");
            simulation.addAction(this);
            return;
        }
        simulation.addPlan(settlement, policy);
        complete();
    }
    else error("Cannot create this Plan");
    simulation.addAction(this);
}

const string AddPlan::toString() const{
    return "plan "+settlementName+" "+ selectionPolicy;
}

AddPlan* AddPlan::clone() const{
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType){}

void AddSettlement::act(Simulation &simulation){
    if(!simulation.isSettlementExists(settlementName)){
        Settlement *settlement= new Settlement(settlementName,settlementType);
        simulation.addSettlement(settlement);
        complete();
    }
    else{
        error("Settlement already exists");
    }
    simulation.addAction(this);
 }

 const string AddSettlement::toString() const{
    return "settlement "+ settlementName +" "+to_string(static_cast<int>(settlementType));
 }

 AddSettlement* AddSettlement::clone() const{
     return new AddSettlement(*this);
 }

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore){}

void AddFacility::act(Simulation &simulation){
    if(!simulation.isFacilityExists(facilityName)){
        FacilityType facility= FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
        simulation.addFacility(facility);
        complete();
    }
    else error("Facility already exists");
    simulation.addAction(this);
}

 const string AddFacility::toString() const{
     return "facility "+ facilityName+" "+ FacilityType::toStringFacilityCategory(facilityCategory)+" " + to_string(price)+" "+to_string(lifeQualityScore)+" "+to_string(economyScore)+" "+to_string(environmentScore);
 }

 AddFacility* AddFacility::clone() const{
     return new AddFacility(*this);
 }

 PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

 void PrintPlanStatus::act(Simulation &simulation){
    if(simulation.isPlanExists(planId)){
        cout<<simulation.getPlan(planId).toString()<< endl;
        complete();
    }
    else error("Plan does not exist");
    simulation.addAction(this);
 }

 PrintPlanStatus* PrintPlanStatus:: clone() const{
    return new PrintPlanStatus(*this);
 }

 const string PrintPlanStatus::toString() const{
    return "planStatus "+ to_string(planId);
 }

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
planId(planId), newPolicy(newPolicy){}

void ChangePlanPolicy::act(Simulation &simulation){
    if(simulation.isPlanExists(planId)){
        Plan& plan = simulation.getPlan(planId);
        string previousPolicy=plan.getSelectionPolicy().nameString();
        if(previousPolicy!=newPolicy){
            complete();
            if(newPolicy=="nve"){
                plan.setSelectionPolicy(new NaiveSelection());
                cout<< "planID: "+to_string(planId)+"\n"+"previousPolicy:"+previousPolicy+"\n"+"newPolicy:"+newPolicy <<endl;

            }
            if(newPolicy=="bal"){
                int lifeQualityScore=0;
                int economyScore=0;
                int environmentScore=0;
                for(Facility* facility: plan.getUnderConstruction()){
                    lifeQualityScore=+facility->getLifeQualityScore();
                    economyScore=+facility->getEconomyScore();
                    environmentScore=+facility->getEnvironmentScore();
                }
                plan.setSelectionPolicy(new BalancedSelection(plan.getlifeQualityScore()+lifeQualityScore,plan.getEconomyScore()+economyScore,plan.getEnvironmentScore()+environmentScore));
                cout<< "planID: "+to_string(planId)+"\n"+"previousPolicy: "+previousPolicy+"\n"+"newPolicy: "+newPolicy <<endl;

            }
            if(newPolicy=="eco"){
                plan.setSelectionPolicy(new EconomySelection());
                cout<< "planID: "+to_string(planId)+"\n"+"previousPolicy: "+previousPolicy+"\n"+"newPolicy: "+newPolicy <<endl;

            }
            if(newPolicy=="env"){
                plan.setSelectionPolicy(new SustainabilitySelection());
                cout<< "planID: "+to_string(planId)+"\n"+"previousPolicy: "+previousPolicy+"\n"+"newPolicy: "+newPolicy <<endl;
            }
        }
        else error("Cannot change selection policy");
    }
    else error("Cannot change selection policy");
    simulation.addAction(this);
}

 ChangePlanPolicy* ChangePlanPolicy:: clone() const{
    return new ChangePlanPolicy(*this);
 }

 const string ChangePlanPolicy::toString() const{
    return "ChangePolicy " + to_string(planId) +" " + newPolicy; 
 }

 PrintActionsLog::PrintActionsLog(){}

 void PrintActionsLog::act(Simulation &simulation){
    for (BaseAction* action:simulation.getActionsLog()){
        string status;
        if (action->getStatus() == ActionStatus::COMPLETED){
            status = " COMPLETED";
        }
        else{
            status = " ERROR";
        }
         cout << action->toString() << status << endl;
    }
    simulation.addAction(this);
    complete();
 }

PrintActionsLog* PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}

 const string PrintActionsLog::toString() const{
    return "log"; 
}

Close::Close() {}

void Close::act (Simulation& simulation) {
        //print all plans
    simulation.close();
    complete();
    simulation.addAction(this);
 }


 Close* Close::clone() const{
    return new Close(*this);
 }
 
const string Close::toString() const{
    return "close";
}

BackupSimulation::BackupSimulation(){}

void BackupSimulation::act (Simulation &simulation){
    if(backup!=nullptr){
        delete backup;
    }
    backup = new Simulation(simulation);
    complete();
    simulation.addAction(this);
}

BackupSimulation* BackupSimulation::clone() const{
    return new BackupSimulation(*this);
}

 const string BackupSimulation::toString() const{
    return "backup";
}

RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation::act(Simulation &simulation){
    if(backup==nullptr){
        error("No backup available");
    }
    else{
        simulation=*backup;
        complete();
    }
    simulation.addAction(this);
}

RestoreSimulation* RestoreSimulation::clone() const{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const{
    return "restore";
}



