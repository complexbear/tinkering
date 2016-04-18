//
//  Instruments.h
//  Attributes
//
//  Created by Matt on 17/04/2016.
//  Copyright © 2016 Matt. All rights reserved.
//

#ifndef Instruments_h
#define Instruments_h

#include "Attribute.h"
#include <iostream>
#include <vector>


struct Bond
{
    Bond()
    {}
    
    ATTR_BEGIN()
        ExpiryAttribute,
        MSymbolAttribute
    ATTR_END()
};


struct Leg {
    TenorAttribute tenor;
};
CREATE_ATTR(PayLeg, Leg, Leg())
CREATE_ATTR(RecLeg, Leg, Leg())

struct Swap
{
    Swap()
    : payLeg({TenorAttribute("1y")}),
      recLeg({TenorAttribute("5y")})
    {}
    
    ExpiryAttribute expiry;
    MSymbolAttribute msymbol;
    PayLegAttribute payLeg;
    RecLegAttribute recLeg;
};

std::ostream& operator<<(std::ostream& o, const Swap& s)
{
    o << "Swap" << std::endl;
    print(s.expiry);
    print(s.msymbol);
    print(s.payLeg);
    print(s.recLeg);
    return o;
}

std::ostream& operator<<(std::ostream& o, const Leg& l)
{
    o << l.tenor.name << "," << l.tenor.value;
    return o;
}

#endif /* Instruments_h */
