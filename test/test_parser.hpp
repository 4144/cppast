// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_TEST_PARSER_HPP_INCLUDED
#define CPPAST_TEST_PARSER_HPP_INCLUDED

#include <fstream>

#include <catch.hpp>

#include <cppast/cpp_entity_kind.hpp>
#include <cppast/libclang_parser.hpp>
#include <cppast/visitor.hpp>

void write_file(const char* name, const char* code)
{
    std::ofstream file(name);
    file << code;
}

std::unique_ptr<cppast::cpp_file> parse(const cppast::cpp_entity_index& idx, const char* name,
                                        const char* code)
{
    using namespace cppast;

    write_file(name, code);

    libclang_compile_config config;
    config.set_flags(cpp_standard::cpp_latest);

    static stderr_diagnostic_logger logger;
    libclang_parser                 p(type_safe::ref(logger));
    return p.parse(idx, name, config);
}

template <typename T, typename Func>
unsigned test_visit(const cppast::cpp_file& file, Func f)
{
    auto count = 0u;
    cppast::visit(file, [&](const cppast::cpp_entity& e, cppast::visitor_info) {
        if (e.kind() == T::kind())
        {
            auto& obj = static_cast<const T&>(e);
            f(obj);
            ++count;
        }

        return true;
    });

    return count;
}

#endif // CPPAST_TEST_PARSER_HPP_INCLUDED
