//
//  main.cpp
//  FormulaEngine
//
//  Created by Matt on 10/10/2015.
//  Copyright © 2015 Matt. All rights reserved.
//

#include "tree.h"
#include <boost/lexical_cast.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/utility/enable_if.hpp>

struct StdVisitor
{
    typedef std::string result_type;
    
    StdVisitor() {}
    
    std::string operator()(double d) {
        return boost::lexical_cast<std::string>(d);
    }
    std::string operator()(const std::string& s) {
        return s;
    }
    std::string operator()(const binary_op& b) {
        std::string left = boost::apply_visitor(*this, b.left.expr);
        std::string right = boost::apply_visitor(*this, b.right.expr);
        std::stringstream s;
        s << '(' << left << b.op << right << ')';
        return s.str();
    }
    std::string operator()(const unary_op& u) {
        std::stringstream s;
        if(u.op == '-') {
            s << u.op;
        }
        s << boost::apply_visitor(*this, u.subject.expr);
        return s.str();
    }
    std::string operator()(const tree& t) {
        return boost::apply_visitor(*this, t.expr);
    }
};

struct PassthroughVisitor :
public boost::static_visitor<tree>
{
    typedef tree result_type;
    
    virtual tree operator()(double d) { return d; }
    
    virtual tree operator()(const std::string& s) { return s; }
    virtual tree operator()(const binary_op& b) {
        tree left = boost::apply_visitor(*this, b.left.expr);
        tree right = boost::apply_visitor(*this, b.right.expr);
        return binary_op(b.op, left, right);
    }
    virtual tree operator()(const unary_op& u) {
        tree subject = boost::apply_visitor(*this, u.subject.expr);
        return unary_op(u.op, subject);
    }
    virtual tree operator()(const tree& t) {
        return boost::apply_visitor(*this, t.expr);
    }
};

struct ModifyingVisitor : public PassthroughVisitor
{
    tree operator()(const tree& t) {
        return boost::apply_visitor(*this, t.expr);
    }
    
    tree operator()(const std::string& s) {
        return s + ":Field";
    }

};


int main(int argc, const char * argv[]) {
    
    double x = 10;
    std::string y = "XXX";
    binary_op b('+', x, y);
    unary_op u('-', 2);
    
    tree t( binary_op('*', u, b) );
    
    ModifyingVisitor mv;
    tree t2 = mv(t);
    
    StdVisitor v;
    std::cout << v(t) << std::endl;
    std::cout << v(t2) << std::endl;
    return 0;
}
