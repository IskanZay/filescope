#include <climits>

#include "core/FileBuffer.h"

FileBuffer::FileBuffer(const std::filesystem::path& path)
{
	if (std::filesystem::file_size(path) > 100 * 1024 * 1024) 
	{
		throw std::runtime_error("File is too large");
	}

	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (!file) throw std::runtime_error("cannot open: " + path.string());

	auto size = file.tellg();
	if (static_cast<std::uintmax_t>(size) > std::numeric_limits<std::size_t>::max()) throw;

	file.seekg(0);

	data_.resize(static_cast<std::size_t>(size));
	file.read(reinterpret_cast<char*>(data_.data()), size);

	if (!file) throw std::runtime_error("read failed: " + path.string());

}

FileBuffer::FileBuffer(FileBuffer&& other) noexcept 
{
	data_ = std::move(other.data_);
}

FileBuffer& FileBuffer::operator=(FileBuffer&& other) noexcept
{
	if (this != &other)
	{
		data_ = std::move(other.data_);
	}
	return *this;
}

std::span<const std::byte> FileBuffer::bytes() const noexcept
{
	return {data_.data(), data_.size()};
}

uintmax_t FileBuffer::size() const noexcept
{
	return data_.size();
}
