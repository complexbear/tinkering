//
//  main.cpp
//  Attributes
//
//  Created by Matt on 17/04/2016.
//  Copyright © 2016 Matt. All rights reserved.
//

#include <iostream>
#include "Instruments.h"

int main(int argc, const char * argv[]) {

    Swap s;
    Bond b;

    std::cout << s << std::endl;
    
    auto& a = get<ExpiryAttribute>(b.attributes);
    print(a);

    a.value = 20160414;
    print(a);
    
    auto a2 = std::get<0>(b.attributes);
    print(a2);
    
    std::vector<Variant> vars;
    extract(b.attributes, vars);
    
    std::cout << "Extracted" << std::endl;
    for(const auto& v : vars) {
        std::cout << v << std::endl;
    }
    
//    auto aaa = std::get<int>(b.attributes);  compile error
//    std::string xxx = a.value;               compile error
    
    return 0;
}
