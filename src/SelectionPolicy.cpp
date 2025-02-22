#include "SelectionPolicy.h"
using namespace std;
#include <limits.h>

// Constructor
NaiveSelection::NaiveSelection():lastSelectedIndex(-1){}

// Select facility method
const FacilityType& NaiveSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    const FacilityType& nextFacility = facilitiesOptions[lastSelectedIndex];
    return nextFacility;
}

const string NaiveSelection::toString() const{
    return "The current Policy selection is Naive, the last facility index is" + lastSelectedIndex;
}

 NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this); //heap- memory
}

const string NaiveSelection::nameString() const {
    return "nve";
}

//constructor
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
LifeQualityScore(LifeQualityScore) ,EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

// Select facility method
const FacilityType& BalancedSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int minDifference=INT_MAX;
    int index=0;
    int size=facilitiesOptions.size();
    for(int i=0; i<size; i++){
        int maxScore= max(facilitiesOptions[i].getEconomyScore() + EconomyScore, facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore );
        maxScore= max(maxScore, facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore);
        int minScore= min(facilitiesOptions[i].getEconomyScore() + EconomyScore, facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore );
        minScore= min(minScore, facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore);
        int difference= maxScore-minScore;
        if(difference < minDifference){
            minDifference=difference;
            index=i;
        }
    }
    const FacilityType& nextFacility = facilitiesOptions[index];
    EconomyScore+=nextFacility.getEconomyScore();
    EnvironmentScore+=nextFacility.getEnvironmentScore();
    LifeQualityScore+=nextFacility.getLifeQualityScore();
    return  facilitiesOptions[index];
}

const string BalancedSelection::toString() const{
    return string("The current Policy selection is Balanced, the current scores are:\n") + "life quality: " + to_string(LifeQualityScore)+ "\n" +"Economy: "+ to_string(EconomyScore)+ "\n"+"Environment "+ to_string(EnvironmentScore) ;
}

 BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this); //heap- memory
}

const string BalancedSelection::nameString() const {
    return "bal";
}

// Constructor
EconomySelection::EconomySelection(): lastSelectedIndex(-1){}

// selectFacility Method
const FacilityType& EconomySelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    int size = facilitiesOptions.size();
    int i = (lastSelectedIndex + 1) % size; // Start from the next facility
    for (int count = 0; count < size; ++count) { // Ensure no infinite loop
        if (facilitiesOptions[i].getCategory()==FacilityCategory::ECONOMY) { // Check if score is valid
            lastSelectedIndex = i; // Update the last selected index
            break; 
        }
        i = (i + 1) % size; // step forward
    }
    return facilitiesOptions[i];
}


const string EconomySelection::toString() const {    
    return "The current Policy selection is Economy, the last facility index is" + lastSelectedIndex;    
}

 EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this); //heap- memory
 }

 const string EconomySelection::nameString() const {
    return "eco";
}

// Constructor
SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1){}

// selectFacility Method
const FacilityType& SustainabilitySelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    int size = facilitiesOptions.size();
    int i = (lastSelectedIndex + 1) % size; // Start from the next facility
    for (int count = 0; count < size; ++count) { // Ensure no infinite loop
        if (facilitiesOptions[i].getCategory()==FacilityCategory::ENVIRONMENT) { // Check if score is valid
            lastSelectedIndex = i; // Update the last selected index
            break; 

        }
        i = (i + 1) % size; // step forward
    }
    return facilitiesOptions[i];
}


const string SustainabilitySelection::toString() const {    
    return "The current Policy selection is Sustainability, the last facility index is" + lastSelectedIndex;    
}

 SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this); //heap- memory
 }

  const string SustainabilitySelection::nameString() const {
    return "env";
  }

