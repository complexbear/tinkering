//
//  Attribute.h
//  Attributes
//
//  Created by Matt on 17/04/2016.
//  Copyright © 2016 Matt. All rights reserved.
//

#ifndef Attribute_h
#define Attribute_h

#include <string>
#include <tuple>


/*
enum AttributeType {
    Expiry,
    MSymbol,
    Tenor
};


template<AttributeType Name, class Type>
struct BaseAttribute
{
    BaseAttribute(const Type& val) : value(val) {}
    BaseAttribute() : value(defaultValue) {}
    
    Type value;
    static const Type defaultValue;
};


#define CREATE_ATTR(Name, Type, DefaultVal) \
    struct Name##Attribute : public BaseAttribute<Name, Type>{}; \
    template <> \
    const Type BaseAttribute<Name, Type>::defaultValue = DefaultVal; \

*/

struct BaseAttribute {};

    #define CREATE_ATTR(NAME, TYPE, DEFAULT) \
        struct NAME##Attribute : BaseAttribute { \
            NAME##Attribute(const TYPE& v) : value(v) {} \
            NAME##Attribute() : value(defaultValue) {} \
            typedef TYPE Type; \
            TYPE value; \
            static const TYPE defaultValue; \
            static const std::string type; \
            static const std::string name; \
        }; \
        const TYPE NAME##Attribute::defaultValue = DEFAULT; \
        const std::string NAME##Attribute::type = #TYPE; \
        const std::string NAME##Attribute::name = #NAME;

    #define ATTR_BEGIN() \
        std::tuple<

    #define ATTR_END() \
        > attributes;

CREATE_ATTR(Expiry, long, 19800101);
CREATE_ATTR(MSymbol, std::string, "NONE");
CREATE_ATTR(Tenor, std::string, "0y");

template<class A>
void print(const A& a) {
    std::cout << a.name << ", " << a.type << ", " << a.value << std::endl;
}

#endif /* Attribute_h */
