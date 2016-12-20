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

struct Instrument
{
    typedef std::tuple<MSymbolAttribute> Attributes;
    Attributes attributes;
};

struct Bond : Instrument
{
    Bond()
    {}
    
//    ATTR_BEGIN()
//        ExpiryAttribute
//    ATTR_END()
    
    typedef typename std::tuple_cat(std::tuple<ExpiryAttribute>, Instrument::Attributes) Attributes;
    Attributes attributes;
};


struct Leg {
    std::string tenor;
};

struct Swap
{
    Swap()
    : payLeg({"1y"}),
      recLeg({"5y"})
    {}
    
    ExpiryAttribute expiry;
    Leg payLeg;
    Leg recLeg;
};

std::ostream& operator<<(std::ostream& o, const Leg& l)
{
    o << l.tenor;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Swap& s)
{
    o << "Swap" << std::endl;
    print(s.expiry);
    o << s.payLeg << ", " << s.recLeg;
    return o;
}



#endif /* Instruments_h */
