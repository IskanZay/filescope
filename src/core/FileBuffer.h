#pragma once

#include <filesystem>
#include <iostream>
#include <span>
#include <vector>
#include <fstream>

class FileBuffer
{
private:
	std::vector<std::byte> data_;

public:
	explicit FileBuffer(const std::filesystem::path& pth);
	~FileBuffer() = default;

	FileBuffer(FileBuffer&& other) noexcept;
	FileBuffer& operator=(FileBuffer&& other) noexcept;

	std::span<const std::byte> bytes() const noexcept;

	uintmax_t size() const noexcept;
};