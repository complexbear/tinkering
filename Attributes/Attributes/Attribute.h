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
#include <vector>
#include <boost/variant.hpp>


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


    #define CREATE_ATTR(NAME, TYPE, DEFAULT) \
        struct NAME##Attribute { \
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

// aka GRQR Variant
typedef boost::variant<long, std::string> Variant;


// From http://stackoverflow.com/questions/16594002/for-stdtuple-how-to-get-data-by-type-and-how-to-get-type-by-index

template<int Index, class Search, class First, class... Types>
struct get_internal
{
    typedef typename get_internal<Index + 1, Search, Types...>::type type;
    static constexpr int index = Index;
};

template<int Index, class Search, class... Types>
struct get_internal<Index, Search, Search, Types...>
{
    typedef get_internal type;
    static constexpr int index = Index;
};

template<class T, class... Types>
T& get(std::tuple<Types...>& tuple)
{
    return std::get<get_internal<0,T,Types...>::type::index>(tuple);
}

template<class T, class... Types>
const T& get(std::tuple<Types...> const& tuple)
{
    return std::get<get_internal<0,T,Types...>::type::index>(tuple);
}


// From http://stackoverflow.com/questions/1198260/iterate-over-tuple

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
extract(std::tuple<Tp...>& t, std::vector<Variant>& vars)
{ }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
extract(std::tuple<Tp...>& t, std::vector<Variant>& vars)
{
    vars.push_back( std::get<I>(t).value );
    extract<I + 1, Tp...>(t, vars);
}

#endif /* Attribute_h */
