#pragma once
#include <span>
#include <optional>
#include <string_view>
#include <cstring>
#include <algorithm>
#include <type_traits>

namespace filescope
{
	class BinaryReader
	{
	private:
		std::span<const std::byte> data_;

	public:
		explicit BinaryReader(std::span<const std::byte> data) : data_(data) {}

		template <class T>
		std::optional<T> read_at(std::size_t offset) const
		{
			if (offset > data_.size()) return std::nullopt;

			if (data_.size() - offset < sizeof(T)) return std::nullopt;

			T value;
			std::memcpy(&value, data_.data() + offset, sizeof(T));
			return value;
		}

		std::optional<std::string_view> read_cstr(std::size_t offset, std::size_t max_len) const
		{
			std::string_view sv;
			if (offset > data_.size()) return std::nullopt;

			const std::size_t avaible = std::min(max_len, data_.size() - offset);
			const char* start = reinterpret_cast<const char*>(data_.data() + offset);

			std::size_t len = 0;
			while (len < avaible && start[len] != '\0') len++;

			return std::string_view{ start, len };
		}

		std::optional<std::span<const std::byte>> slice(std::size_t offset, std::size_t len)
		{
			if (offset > data_.size() || data_.size() - offset < len) return std::nullopt;

			return data_.subspan(offset, len);
		}

		std::size_t size()
		{
			return data_.size();
		}

	};
}

