//
//  main.cpp
//  Attributes
//
//  Created by Matt on 17/04/2016.
//  Copyright © 2016 Matt. All rights reserved.
//

#include <iostream>
#include "Instruments.h"

using namespace std;



int main(int argc, const char * argv[]) {

    Swap s;
    Bond b;

    cout << s << endl;
    
    auto& a = std::get<0>(b.attributes);
    print(a);

    a.value = 20160414;
    print(a);
    
    auto a2 = std::get<0>(b.attributes);
    print(a2);
    
    // Get by type
    auto aa = std::get<MSymbolAttribute>(b.attributes);
    cout << aa.name << ", " << aa.type << ", " << aa.value << endl;
    
    // Number of attr
    size_t l = std::tuple_size<decltype(b.attributes)>();
    cout << "Num of bond attr: " << l << endl;
    
//    auto aaa = std::get<int>(b.attributes);  compile error
//    std::string xxx = a.value;               compile error
    
    return 0;
}
