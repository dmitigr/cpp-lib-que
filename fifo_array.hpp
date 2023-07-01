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

#ifndef DMITIGR_QUE_FIFO_ARRAY_HPP
#define DMITIGR_QUE_FIFO_ARRAY_HPP

#include <algorithm>
#include <array>
#include <utility>

namespace dmitigr::que {

/**
 * @brief A container adapter that gives the functionality of a FIFO structure.
 *
 * @details This class can be used as the underlying container of `std::queue`.
 */
template<class T, std::size_t N>
class Fifo_array final {
public:
  using Underlying_type = std::array<T, N>;
  using value_type = typename Underlying_type::value_type;
  using reference = typename Underlying_type::reference;
  using const_reference = typename Underlying_type::const_reference;
  using size_type = typename Underlying_type::size_type;

  constexpr explicit Fifo_array()
  {}

  constexpr const value_type* data() const noexcept
  {
    return data_.data() + pop_offset_;
  }

  constexpr value_type* data() noexcept
  {
    return const_cast<value_type*>(static_cast<const Fifo_array*>(this)->data());
  }

  constexpr const value_type& back() const noexcept
  {
    return *(data_.begin() + push_offset_ - 1);
  }

  constexpr value_type& back() noexcept
  {
    return const_cast<value_type&>(static_cast<const Fifo_array*>(this)->back());
  }

  constexpr const value_type& front() const noexcept
  {
    return *(data_.begin() + pop_offset_);
  }

  constexpr value_type& front() noexcept
  {
    return const_cast<value_type&>(static_cast<const Fifo_array*>(this)->front());
  }

  constexpr void push_back(const value_type value) noexcept
  {
    data_[push_offset_++] = value;
  }

  constexpr void emplace_back(const value_type value) noexcept
  {
    push_back(value);
  }

  constexpr void pop_front() noexcept
  {
    pop_offset_ = std::min(pop_offset_ + 1, data_.size());
  }

  constexpr void unpop_front() noexcept
  {
    if (pop_offset_) --pop_offset_;
  }

  constexpr void unpop_all() noexcept
  {
    pop_offset_ = 0;
  }

  constexpr size_type size() const noexcept
  {
    return push_offset_ - pop_offset_;
  }

  constexpr bool empty() const noexcept
  {
    return !size();
  }

  constexpr void clear() noexcept
  {
    pop_offset_ = push_offset_ = 0;
  }

  constexpr void swap(Fifo_array& other) noexcept
  {
    using std::swap;
    swap(data_, other.data_);
    swap(pop_offset_, other.pop_offset_);
    swap(push_offset_, other.push_offset_);
  }

private:
  Underlying_type data_;
  size_type pop_offset_{};
  size_type push_offset_{};
};

/// Swaps `lhs` and `rhs`.
template<class T, std::size_t N>
void swap(Fifo_array<T, N>& lhs, Fifo_array<T, N>& rhs) noexcept
{
  lhs.swap(rhs);
}

} // namespace dmitigr::que

#endif  // DMITIGR_QUE_FIFO_ARRAY_HPP
