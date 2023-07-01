// -*- C++ -*-
//
// Copyright 2023 Dmitry Igrishin
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DMITIGR_QUE_FIFO_STRING_HPP
#define DMITIGR_QUE_FIFO_STRING_HPP

#include <algorithm>
#include <string>
#include <utility>

namespace dmitigr::que {

/**
 * @brief A container adapter that gives the functionality of a FIFO structure.
 *
 * @details This class can be used as the underlying container of `std::queue`.
 */
template<class CharT, class Traits = std::char_traits<CharT>,
  class Allocator = std::allocator<CharT>>
class Basic_fifo_string final {
public:
  using Underlying_type = std::basic_string<CharT, Traits, Allocator>;
  using value_type = typename Underlying_type::value_type;
  using reference = typename Underlying_type::reference;
  using const_reference = typename Underlying_type::const_reference;
  using size_type = typename Underlying_type::size_type;

  template<typename ... Types>
  constexpr explicit Basic_fifo_string(Types&& ... args)
    : data_(std::forward<Types>(args)...)
  {}

  constexpr std::basic_string_view<CharT, Traits> view() const noexcept
  {
    return {data(), size()};
  }

  constexpr const value_type* data() const noexcept
  {
    return data_.data() + offset_;
  }

  constexpr value_type* data() noexcept
  {
    return const_cast<value_type*>(
      static_cast<const Basic_fifo_string*>(this)->data());
  }

  constexpr const value_type& back() const noexcept
  {
    return data_.back();
  }

  constexpr value_type& back() noexcept
  {
    return const_cast<value_type&>(
      static_cast<const Basic_fifo_string*>(this)->back());
  }

  constexpr const value_type& front() const noexcept
  {
    return *(data_.begin() + offset_);
  }

  constexpr value_type& front() noexcept
  {
    return const_cast<value_type&>(
      static_cast<const Basic_fifo_string*>(this)->front());
  }

  constexpr void push_back(const value_type value)
  {
    data_.push_back(value);
  }

  constexpr void emplace_back(const value_type value)
  {
    push_back(value);
  }

  constexpr void pop_front() noexcept
  {
    offset_ = std::min(offset_ + 1, data_.size());
  }

  constexpr void unpop_front() noexcept
  {
    if (offset_) --offset_;
  }

  constexpr void unpop_all() noexcept
  {
    offset_ = 0;
  }

  constexpr size_type size() const noexcept
  {
    return data_.size() - offset_;
  }

  constexpr bool empty() const noexcept
  {
    return !size();
  }

  constexpr void clear() noexcept
  {
    data_.clear();
    offset_ = 0;
  }

  constexpr void swap(Basic_fifo_string& other) noexcept
  {
    using std::swap;
    swap(data_, other.data_);
    swap(offset_, other.offset_);
  }

private:
  Underlying_type data_;
  size_type offset_{};
};

/// Swaps `lhs` and `rhs`.
template<class CharT, class Traits = std::char_traits<CharT>,
  class Allocator = std::allocator<CharT>>
inline void swap(Basic_fifo_string<CharT, Traits, Allocator>& lhs,
  Basic_fifo_string<CharT, Traits, Allocator>& rhs) noexcept
{
  lhs.swap(rhs);
}

/// Alias of Basic_fifo_string<char>.
using Fifo_string = Basic_fifo_string<char>;

} // namespace dmitigr::que

#endif  // DMITIGR_QUE_FIFO_STRING_HPP
