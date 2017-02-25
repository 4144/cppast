// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include "parse_functions.hpp"

#include <cppast/cpp_member_variable.hpp>
#include <cppast/cpp_variable.hpp>
#include <clang-c/Index.h>

#include "libclang_visitor.hpp"

using namespace cppast;

namespace
{
    std::unique_ptr<cpp_expression> parse_default_value(const detail::parse_context& context,
                                                        const CXCursor&              cur)
    {
        std::unique_ptr<cpp_expression> expression;
        detail::visit_children(cur, [&](const CXCursor& child) {
            DEBUG_ASSERT(clang_isExpression(child.kind) && !expression,
                         detail::parse_error_handler{}, cur, "unexpected child cursor of variable");

            expression = detail::parse_expression(context, child);
        });
        return expression;
    }

    cpp_storage_class_specifiers parse_storage_class(const CXCursor& cur)
    {
        switch (clang_Cursor_getStorageClass(cur))
        {
        case CX_SC_Invalid:
            break;

        case CX_SC_None:
            return cpp_storage_class_none;

        case CX_SC_Auto:
        case CX_SC_Register:
            return cpp_storage_class_auto;

        case CX_SC_Extern:
            return cpp_storage_class_extern;
        case CX_SC_Static:
            return cpp_storage_class_static;

        case CX_SC_PrivateExtern:
        case CX_SC_OpenCLWorkGroupLocal:
            // non-exposed storage classes
            return cpp_storage_class_auto;
        }

        DEBUG_UNREACHABLE(detail::parse_error_handler{}, cur, "unexpected storage class");
        return cpp_storage_class_auto;
    }
}

std::unique_ptr<cpp_entity> detail::parse_cpp_variable(const detail::parse_context& context,
                                                       const CXCursor&              cur)
{
    DEBUG_ASSERT(cur.kind == CXCursor_VarDecl, detail::assert_handler{});

    auto name          = get_cursor_name(cur);
    auto type          = parse_type(context, clang_getCursorType(cur));
    auto default_value = parse_default_value(context, cur);
    auto storage_class = parse_storage_class(cur);
    auto is_constexpr  = false;

    // just look for thread local or constexpr
    // can't appear anywhere else, so good enough
    detail::tokenizer tokenizer(context.tu, context.file, cur);
    for (auto& token : tokenizer)
        if (token.value() == "thread_local")
            storage_class =
                cpp_storage_class_specifiers(storage_class | cpp_storage_class_thread_local);
        else if (token.value() == "constexpr")
            is_constexpr = true;

    return cpp_variable::build(*context.idx, get_entity_id(cur), name.c_str(), std::move(type),
                               std::move(default_value), storage_class, is_constexpr);
}

std::unique_ptr<cpp_entity> detail::parse_cpp_member_variable(const detail::parse_context& context,
                                                              const CXCursor&              cur)
{
    DEBUG_ASSERT(cur.kind == CXCursor_FieldDecl, detail::assert_handler{});

    auto name       = get_cursor_name(cur);
    auto type       = parse_type(context, clang_getCursorType(cur));
    auto is_mutable = clang_CXXField_isMutable(cur) != 0u;

    if (clang_Cursor_isBitField(cur))
    {
        auto no_bits = clang_getFieldDeclBitWidth(cur);
        DEBUG_ASSERT(no_bits >= 0, detail::parse_error_handler{}, cur, "invalid number of bits");
        if (name.empty())
            return cpp_bitfield::build(std::move(type), unsigned(no_bits), is_mutable);
        else
            return cpp_bitfield::build(*context.idx, get_entity_id(cur), name.c_str(),
                                       std::move(type), unsigned(no_bits), is_mutable);
    }
    else
    {
        // parse_default_value() doesn't work here
        tokenizer    tokenizer(context.tu, context.file, cur);
        token_stream stream(tokenizer, cur);

        // look for the equal sign, default value starts there
        while (!stream.done() && !skip_if(stream, "="))
            stream.bump();
        auto default_value = parse_raw_expression(context, stream, clang_getCursorType(cur));

        return cpp_member_variable::build(*context.idx, get_entity_id(cur), name.c_str(),
                                          std::move(type), std::move(default_value), is_mutable);
    }
}
