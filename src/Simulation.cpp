#include "Simulation.h"
#include "Action.h"
#include "Auxiliary.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
using namespace std;
using std::nullptr_t;


// Constructor
Simulation::Simulation(const string &configFilePath)
    : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions() {
    
    std::ifstream configFile(configFilePath); // Allows reading files like Config
    if (!configFile.is_open()) {
        throw std::runtime_error("Could not open config file: " + configFilePath);
    }

    string codeLine;
    while (getline(configFile, codeLine)) {
        if (!codeLine.empty() && codeLine[0] != '#') {
            vector<string> arguments = Auxiliary::parseArguments(codeLine);

            if (!arguments.empty()) {

                if (arguments[0] == "settlement") {
                    int categorynum = stoi(arguments[2]);
                    SettlementType type;

                    // Decode settlement's type:
                    if (categorynum == 0) {
                        type = SettlementType::VILLAGE;
                    } else if (categorynum == 1) {
                        type = SettlementType::CITY;
                    } else {
                        type = SettlementType::METROPOLIS;
                    }

                    string name = arguments[1];

                    // Push settlement to 'settlements' vector
                    settlements.push_back(new Settlement(name, type));
                } 
                else if (arguments[0] == "facility") {
                    string name = arguments[1];
                    int categorynum = stoi(arguments[2]);
                    FacilityCategory category;

                    // Choose category
                    if (categorynum == 0) {
                        category = FacilityCategory::LIFE_QUALITY;
                    } else if (categorynum == 1) {
                        category = FacilityCategory::ECONOMY;
                    } else {
                        category = FacilityCategory::ENVIRONMENT;
                    }

                    int price = stoi(arguments[3]);
                    int lifeQualityImpact = stoi(arguments[4]);
                    int economyImpact = stoi(arguments[5]);
                    int environmentImpact = stoi(arguments[6]);

                    facilitiesOptions.push_back(FacilityType(name, category, price, lifeQualityImpact, economyImpact, environmentImpact));
                } 
                else if (arguments[0] == "plan") {
                    Settlement* settlement = &getSettlement(arguments[1]);
                    SelectionPolicy* selectionPolicy = nullptr;
                    if (arguments[2] == "nve") {
                        selectionPolicy = new NaiveSelection();
                    } else if (arguments[2] == "bal") {
                        selectionPolicy = new BalancedSelection(0, 0, 0); 
                    } else if (arguments[2] == "eco") {
                        selectionPolicy = new EconomySelection();
                    } else {
                        selectionPolicy = new SustainabilitySelection();
                    }

                    plans.push_back(Plan(planCounter++, *settlement, selectionPolicy, facilitiesOptions));
                }
            }
        }
    }
    configFile.close();
}

// Other methods
void Simulation::start() {

        open();
        while (isRunning) {
        
        string command = "";
        getline(cin, command); // get line reads input


        vector<string> argument = Auxiliary::parseArguments(command);
         
         if(argument[0]=="step"){
            BaseAction* step = new SimulateStep(stoi(argument[1]));
            step->act(*this);
        }
        else if(argument[0] == "plan"){
            string name(argument[1]);
            string policy(argument[2]);
            BaseAction* plan = new AddPlan(name, policy);
            plan->act(*this);
        }
          else if(argument[0]=="settlement"){
            string name(argument[1]);
            BaseAction* set = new AddSettlement(name, static_cast<SettlementType>(stoi(argument[2])));
            set->act(*this);
        }
        else if(argument[0] == "facility"){
            BaseAction* fac = new AddFacility(argument[1], static_cast<FacilityCategory>(stoi(argument[2])), stoi(argument[3]),stoi(argument[4]),stoi(argument[5]), stoi(argument[6]));
            fac->act(*this);
        }
        else if(argument[0] == "planStatus"){
            BaseAction* ps = new PrintPlanStatus(stoi(argument[1]));
            ps->act(*this);
        }
          else if(argument[0] == "changePolicy"){
            BaseAction* cp = new ChangePlanPolicy(stoi(argument[1]),argument[2]);
            cp->act(*this);
        }
        else if(argument[0] == "log"){
            BaseAction* log = new PrintActionsLog();
            log->act(*this);
        }
        else if(argument[0] == "close"){
            BaseAction* c = new Close();
            c->act(*this);
        }
        else if(argument[0] == "backup"){
            BaseAction* backU = new BackupSimulation();
            backU->act(*this);
        }
        else if(argument[0] == "restore"){
            BaseAction* res = new RestoreSimulation();
            res->act(*this);
        }  
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.push_back(Plan(planCounter++, settlement, selectionPolicy, facilitiesOptions));
}

void Simulation::addAction(BaseAction *action) {
   actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
   if (!isSettlementExists(settlement->getName())){
    settlements.push_back(settlement);
    return true;
    }
    return false;
}

bool Simulation::addFacility(FacilityType facility) {
    if (!isFacilityExists(facility.getName())){
        facilitiesOptions.push_back(facility);
        return true;
    }
    return false;
}

bool Simulation::isSettlementExists(const string &settlementName){
     for (Settlement* currSet : settlements) {
        if (currSet->getName() == settlementName) {
            return true;
        }
    }
    return false;
}
Settlement& Simulation:: getSettlement(const string &settlementName){
    for (Settlement* currSet : settlements) {
        if (currSet->getName() == settlementName) {
            return *currSet; 
        }
    }
    throw runtime_error("Settlement with name " + settlementName + "not found");
}


Plan &Simulation:: getPlan(const int planID){
    for(Plan& currP: plans ){
        if (currP.getId() == planID){
            return currP;
        }
    }
    throw runtime_error("Plan with Id " + to_string(planID) + "not found");
}

bool Simulation::isPlanExists(const int planID){
    for(Plan& currP: plans ){
        if (currP.getId() == planID){
            return true;
        }
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName){
     for (FacilityType facility : facilitiesOptions) {
        if (facility.getName() == facilityName) {
            return true;
        }
    }
    return false;
}

void Simulation::step(){
    for(Plan& plan: plans){
        plan.step();
    }
}

void Simulation:: close(){
    cout << "plans status:" << endl;
    if (plans.empty()){
        cout << "  None" << endl;
    }
    else{
        for (const Plan &plan : plans){
            cout << "planID: " + to_string(plan.getId()) +"\n"+
            "settlementName: " + plan.getSettlement().getName() + "\n"+
            "LifeQuality Score: "+ to_string(plan.getlifeQualityScore()) +"\n"+
            "Economy Score: " +to_string(plan.getEconomyScore()) +"\n"+
            "Environment Score: " +to_string(plan.getEnvironmentScore()) <<endl;
        }
    isRunning = false;
    }
}

void Simulation::open(){
 isRunning = true;
    cout << "the simulation has started" << endl;
}


// rule of 5
//destructor
Simulation::~Simulation(){
    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    for (BaseAction* action : actionsLog) { 
        delete action;
    }
}

//copy constructor
Simulation::Simulation(const Simulation& other):isRunning(other.isRunning), 
planCounter(other.planCounter),
actionsLog(), 
plans(), 
settlements(),
facilitiesOptions()  {

    for(BaseAction* acttion: other.actionsLog){
        addAction(acttion->clone());
    }
    for(Settlement* settlement: other.settlements){
        addSettlement( new Settlement(*settlement));
    }

    for(const Plan& plan: other.plans){
        Plan newPlan(plan, getSettlement( plan.getSettlement().getName()) ) ;
        plans.push_back(newPlan);
    } 

    for(const FacilityType& facility: other.facilitiesOptions){
        addFacility(FacilityType(facility));
    }
}

//move constructor
Simulation::Simulation(Simulation&& other): 
    isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(),  
    plans(other.plans), 
    settlements(),
    facilitiesOptions(other.facilitiesOptions) {

    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action); 
    }
    other.actionsLog.clear(); 

    for (Settlement* settlement : other.settlements) {
        settlements.push_back(settlement); 
    }
    other.settlements.clear(); 
}

//copy assignment operator
Simulation& Simulation::operator=(const Simulation& other) {
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        
        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        for (const Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }

        plans.clear();
        for (const Plan& plan : other.plans) {
            Plan newPlan(plan, getSettlement( plan.getSettlement().getName()) ) ;
            plans.push_back(newPlan);
        }

        facilitiesOptions.clear();
        for (const FacilityType& facility : other.facilitiesOptions) {
            facilitiesOptions.push_back(FacilityType(facility));
        }

        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
    }
    return *this;
}

//move assignment operator
Simulation& Simulation :: operator =(Simulation&& other){
    if(this != &other){
    isRunning = other.isRunning;
    planCounter = other.planCounter;


    facilitiesOptions.clear();
    for (const FacilityType& facility : other.facilitiesOptions) {
        facilitiesOptions.push_back(FacilityType(facility));
    }

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement)); 
    }

    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    plans.clear();
    for (const Plan& plan : other.plans) {
        Plan newPlan(plan, getSettlement(plan.getSettlement().getName()));
        plans.push_back(newPlan);
    }

    other.settlements.clear();
    other.actionsLog.clear();

    }
    return *this;
}

const vector<BaseAction*>& Simulation::getActionsLog() const {
    return actionsLog;
}

vector<Plan> &Simulation::getPlans() {
    return plans;
}
