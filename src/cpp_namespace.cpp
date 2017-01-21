// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/cpp_namespace.hpp>

#include <cppast/cpp_entity_type.hpp>

using namespace cppast;

cpp_entity_type cpp_namespace::do_get_entity_type() const noexcept
{
    return cpp_entity_type::namespace_t;
}

bool detail::cpp_namespace_ref_predicate::operator()(const cpp_entity& e)
{
    return e.type() == cpp_entity_type::namespace_t;
}

cpp_entity_type cpp_namespace_alias::do_get_entity_type() const noexcept
{
    return cpp_entity_type::namespace_alias_t;
}

cpp_entity_type cpp_using_directive::do_get_entity_type() const noexcept
{
    return cpp_entity_type::using_directive_t;
}

cpp_entity_type cpp_using_declaration::do_get_entity_type() const noexcept
{
    return cpp_entity_type::using_declaration_t;
}
