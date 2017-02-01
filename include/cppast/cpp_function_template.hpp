// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_CPP_FUNCTION_TEMPLATE_HPP_INCLUDED
#define CPPAST_CPP_FUNCTION_TEMPLATE_HPP_INCLUDED

#include <cppast/cpp_function.hpp>
#include <cppast/cpp_template.hpp>

namespace cppast
{
    /// A [cppast::cpp_entity]() modelling a function template.
    class cpp_function_template final : public cpp_template
    {
    public:
        /// Builder for [cppast::cpp_function_template]().
        class builder : public basic_builder<cpp_function_template, cpp_function_base>
        {
        public:
            using basic_builder::basic_builder;
        };

        /// A reference to the function that is being templated.
        const cpp_function_base& function() const noexcept
        {
            return static_cast<const cpp_function_base&>(*begin());
        }

    private:
        cpp_function_template(std::unique_ptr<cpp_function_base> func)
        : cpp_template(std::unique_ptr<cpp_entity>(func.release()))
        {
        }

        cpp_entity_kind do_get_entity_kind() const noexcept override;

        friend basic_builder<cpp_function_template, cpp_function_base>;
    };
} // namespace cppast

#endif // CPPAST_CPP_FUNCTION_TEMPLATE_HPP_INCLUDED
