// Copyright (C) 2017 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef CPPAST_CPP_FILE_HPP_INCLUDED
#define CPPAST_CPP_FILE_HPP_INCLUDED

#include <cppast/cpp_entity_index.hpp>
#include <cppast/cpp_scope.hpp>

namespace cppast
{
    /// A [cppast::cpp_entity]() modelling a file.
    ///
    /// This is the top-level entity of the AST.
    class cpp_file final : public cpp_entity, public cpp_entity_container<cpp_file, cpp_entity>
    {
    public:
        /// Builds a [cppast::cpp_file]().
        class builder
        {
        public:
            /// \effects Sets the file name.
            explicit builder(std::string name) : file_(new cpp_file(std::move(name)))
            {
            }

            /// \effects Adds an entity.
            void add_child(std::unique_ptr<cpp_entity> child) noexcept
            {
                file_->add_child(std::move(child));
            }

            /// \effects Registers the file in the [cppast::cpp_entity_index]().
            /// It will use the file name as identifier.
            /// \returns The finished file.
            std::unique_ptr<cpp_file> finish(const cpp_entity_index& idx) noexcept
            {
                idx.register_entity(cpp_entity_id(file_->name()), type_safe::ref(*file_));
                return std::move(file_);
            }

        private:
            std::unique_ptr<cpp_file> file_;
        };

    private:
        cpp_file(std::string name) : cpp_entity(std::move(name))
        {
        }

        /// \returns [cpp_entity_type::file_t]().
        cpp_entity_type do_get_entity_type() const noexcept override;
    };
} // namespace cppast

#endif // CPPAST_CPP_FILE_HPP_INCLUDED
