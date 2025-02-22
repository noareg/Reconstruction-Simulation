#include "Simulation.h"
#include <iostream>
#include "Settlement.h"
#include "Facility.h"
#include <vector>
#include "SelectionPolicy.h"
#include <cassert>
#include "Plan.h"
#include "Action.h"
using namespace std;

 Simulation* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    string configurationFile = argv[1];
    Simulation simulation(configurationFile);
    simulation.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}

// int main(int argc, char** argv) {
//     std::cout << "Creating simulation" << std::endl;
    
//     string configurationFile = argv[1];
//     Simulation sim(configurationFile);

//     // יצירת יישובים והוספתם לסימולציה
//     std::cout << "Creating Settlements" << std::endl;
//     Settlement* s1 = new Settlement("New York",SettlementType::METROPOLIS);
//     Settlement* s2 = new Settlement("Los Angeles",SettlementType::CITY);
//     Settlement* s3 = new Settlement("Chicago",SettlementType::VILLAGE);

//     // הוספת היישובים
//     sim.addSettlement(s1);
//     sim.addSettlement(s2);
//     sim.addSettlement(s3);


//     // ניסיון להוסיף יישוב עם שם קיים
//     Settlement* s4 = new Settlement("New York",SettlementType::METROPOLIS);  // שם שכבר קיים
//     if (!sim.addSettlement(s4)) {
//         std::cout << "Settlement with the name 'New York' already exists!" << std::endl;
//     }

//     // יצירת אובייקט של סימולציה חדש דרך העתקה
//     std::cout << "Copying simulation" << std::endl;
//     Simulation simCopy = sim;

//     // יצירת אובייקט של סימולציה חדש דרך העברה
//     std::cout << "Moving simulation" << std::endl;
//     Simulation simMove = std::move(sim);

//     // סיום – Destructor יימחק את היישובים
//     return 0;
// }

// int main(int argc, char** argv){
//     string configurationFile = argv[1];
//     BaseAction* act= new AddSettlement ("Jafa",SettlementType::CITY);
//     Simulation sim= Simulation(configurationFile);
//     Simulation simulationCheck= Simulation(sim.addAction(act));
//     return 0;
// }


/* void testSelectionPolicy() {
    // יצירת מתקנים לבחירה
    FacilityType facility1("Facility 1", FacilityCategory::ECONOMY, 6, 50, 70, 60);
    FacilityType facility2("Facility 2", FacilityCategory::ENVIRONMENT,5, 30, 50, 90);
    FacilityType facility3("Facility 3", FacilityCategory::LIFE_QUALITY,2, 80, 40, 70);
    FacilityType facility4("Facility 4", FacilityCategory::ENVIRONMENT,5, 30, 50, 90);
    vector<FacilityType> facilities = {facility1, facility2, facility3, facility4};

    // בדיקה של NaiveSelection
    NaiveSelection naivePolicy;
    cout << naivePolicy.toString() << endl;
    for (int i = 0; i < 5; ++i) {
        const FacilityType& selected = naivePolicy.selectFacility(facilities);
        cout << "NaiveSelection chose: " << selected.getName() << endl;
    }

    // בדיקה של BalancedSelection
    BalancedSelection balancedPolicy(10, 20, 15);
    cout << balancedPolicy.toString() << endl;
    const FacilityType& balancedSelected = balancedPolicy.selectFacility(facilities);
    cout << "BalancedSelection chose: " << balancedSelected.getName() << endl;
       cout << balancedPolicy.toString() << endl;

    // בדיקה של EconomySelection
    EconomySelection economyPolicy;
    cout << economyPolicy.toString() << endl;
    for (int i = 0; i < 5; ++i) {
        const FacilityType& selected = economyPolicy.selectFacility(facilities);
        cout << "EconomySelection chose: " << selected.getName() << endl;
    }

    // בדיקה של SustainabilitySelection
    SustainabilitySelection sustainabilityPolicy;
    cout << sustainabilityPolicy.toString() << endl;
    for (int i = 0; i < 5; ++i) {
        const FacilityType& selected = sustainabilityPolicy.selectFacility(facilities);
        cout << "SustainabilitySelection chose: " << selected.getName() << endl;
    }
}*/


/*int main() {
    // יצירת Settlement לדוגמה
    Settlement citySettlement("Test City", SettlementType::CITY);
    
    // יצירת אופציות מתקנים (FacilityTypes)
    vector<FacilityType> facilityOptions = {
        FacilityType("Hospital", FacilityCategory::LIFE_QUALITY, 5, 10, 5, 3),
        FacilityType("School", FacilityCategory::ECONOMY, 2, 8, 7, 5),
        FacilityType("GreenPark", FacilityCategory::ENVIRONMENT, 1, 6, 4, 10)
    };

    // יצירת SelectionPolicy - בחרתי ב-NaiveSelection פה, אפשר לשנות לכל אחד מהסוגים האחרים
    SelectionPolicy* selectionPolicy = new NaiveSelection();
    
    // יצירת תוכנית עם ה-SelectionPolicy שנבחר
    Plan plan(1, citySettlement, selectionPolicy, facilityOptions);
    
    // הדפסת המצב ההתחלתי
    std::cout << "Initial plan status:\n" << plan.toString() << std::endl;

    // קריאה לשלב אחד בתוכנית (תהליך בניית המתקנים)
    plan.step();
    plan.step();
    plan.step();
    plan.step();
    plan.step();

    // הדפסת המצב אחרי שלב התוכנית
    std::cout << "Plan status after step:\n" << plan.toString() << std::endl;

    return 0;
} */



 