#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"


enum class PlanStatus {
    AVALIABLE,
    BUSY,
};



class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const int getId() const;
        const SelectionPolicy& getSelectionPolicy() const;
        const Settlement& getSettlement() const;
        const string getStatus() const;
        static string toStringPlanStatus(PlanStatus planStatus);
        const vector<Facility*> &getUnderConstruction() const;

        ~Plan();
        Plan(const Plan&);
        Plan(const Plan& other, const Settlement& settlement); //secondary copy constructor
        Plan(Plan&&);  


        Plan& operator=(const Plan&) = delete;   
        Plan& operator=(Plan&&) = delete;  


    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy;
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
        
        const string strPlanStatus(PlanStatus status) const;
};