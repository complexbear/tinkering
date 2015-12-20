//
//  engine.h
//  FormulaEngine
//
//  Created by Matt on 10/10/2015.
//  Copyright © 2015 Matt. All rights reserved.
//

#ifndef _tree_h_
#define _tree_h_

#include <vector>
#include <iostream>
#include <boost/variant.hpp>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

using namespace boost::assign;

struct binary_op;
struct unary_op;
struct tree;

std::ostream& operator<<(std::ostream& o, const tree& t);
std::ostream& operator<<(std::ostream& o, const binary_op& b);
std::ostream& operator<<(std::ostream& o, const unary_op& u);


typedef boost::variant< std::string,
                        double,
                        boost::recursive_wrapper<binary_op>,
                        boost::recursive_wrapper<unary_op>,
                        boost::recursive_wrapper<tree>
> type;

struct tree
{
    tree() : expr(0) {}
    
    template<typename T>
    tree(const T& t) : expr(t) {}
    
    type expr;
};

struct binary_op
{
    binary_op(char op, const tree& left, const tree& right)
    :   op(op), left(left), right(right)
    {}
    
    char op;
    tree left;
    tree right;
};

struct unary_op
{
    unary_op(char op, const tree& subject)
    :   op(op), subject(subject)
    {}
    
    char op;
    tree subject;
};

std::ostream& operator<<(std::ostream& o, const tree& t)
{
    return o << t.expr;
}
std::ostream& operator<<(std::ostream& o, const binary_op& b)
{
    return o << '(' << b.left << b.op << b.right << ')';
}
std::ostream& operator<<(std::ostream& o, const unary_op& u)
{
    return o << u.op << u.subject;
}




#endif /* _tree_h_ */
