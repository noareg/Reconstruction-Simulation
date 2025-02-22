#include "Settlement.h"

    // Constructor
    Settlement::Settlement(const string &name, SettlementType type): name(name), type(type){}

    // Methods
    const string &Settlement::getName() const{
        return name;
    }

    SettlementType Settlement::getType() const{
        return type;
    }

    // Convert the settlement to a string
    const string Settlement::toString() const{
        return "Settlement name: " + name +"\n"+ "Settlement type: " + toStringSettlementType(type);
    }

    const size_t Settlement:: constructionLimit() const{
        if (type == SettlementType::VILLAGE ){
            return 1;
        }
         if (type == SettlementType::CITY ){
            return 2;
        }
        else{
            return 3;
        }
    }

    string Settlement::toStringSettlementType(SettlementType settlementType) {
    const string settlementNames[] = {
        "Village",       
        "City",          
        "Metropolis"    
    };

    return settlementNames[static_cast<int>(settlementType)];
}

  