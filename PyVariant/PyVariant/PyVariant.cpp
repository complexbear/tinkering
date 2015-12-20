//
//  PyVariant.cpp
//  PyVariant
//
//  Created by Matt on 20/12/2015.
//  Copyright © 2015 Matt. All rights reserved.
//

#include <iostream>
#include "PyVariant.h"

#include <boost/python.hpp>


tree make_variant()
{
    return    binary_op('+',
                        tree( std::string("xxx") ),
                        tree( 1.0 ));
}

// Generic conversion of the variant type to any defined Python class type
struct variant_to_object : boost::static_visitor<PyObject *> {
    static result_type convert(VType const &v) {
        return apply_visitor(variant_to_object(), v);
    }
    
    template<typename T>
    result_type operator()(T const &t) const {
        return boost::python::incref(boost::python::object(t).ptr());
    }
};


PyObject * tree_to_object(tree const& self)
{
    return variant_to_object::convert(self.expr);
}



BOOST_PYTHON_MODULE(PyVariant)
{
    using namespace boost::python;
    
    def("make_variant", &make_variant);
    
    class_<tree>("tree", init<>())
        .add_property("expr", &tree_to_object); // Use a property method so we can force the conversion to happen
    
    class_<binary_op>("binary_op", init<binary_op>())
          .def(init<char, tree const&, tree const&>( args("op", "left", "right") ))
        .def_readwrite("op", &binary_op::op)
        .def_readwrite("left", &binary_op::left)
        .def_readwrite("right", &binary_op::right);

    class_<unary_op>("unary_op", init<unary_op>())
        .def(init<char, tree const&>( args("op", "subject")))
        .def_readwrite("op", &unary_op::op)
        .def_readwrite("subject", &unary_op::subject);
    
    to_python_converter<VType, variant_to_object>();
}



