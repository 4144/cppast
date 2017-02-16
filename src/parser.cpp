// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/parser.hpp>

#include <cstdio>
#include <mutex>

#include <cppast/diagnostic.hpp>

using namespace cppast;

bool stderr_diagnostic_logger::do_log(const char* source, const diagnostic& d) const
{
    std::fprintf(stderr, "[%s] %s %s", source, d.location.to_string().c_str(), d.message.c_str());
    return true;
}
