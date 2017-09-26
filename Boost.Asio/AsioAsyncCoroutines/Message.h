#pragma once

#include <vector>
#include <memory>

struct Message
{
	Message() :
		_size(0)
	{
	}

	Message(size_t size) :
		_data(new char[size]),
		_size(size)
	{
	}

	Message(const Message& other) :
		_data(new char[other.size()]),
		_size(other.size())
	{
		memcpy(_data.get(), other._data.get(), other.size());
	}

	Message(Message&& other) :
		Message()
	{
		swap(other);
	}

	Message& operator=(Message other)
	{
		swap(other);
		return *this;
	}

	void swap(Message& other)
	{
		std::swap(_size, other._size);
		std::swap(_data, other._data);
	}

	bool operator==(const Message& other)
	{
		if (_size != other._size)
			return false;

		auto myData = _data.get();
		auto otherData = other._data.get();

		return memcmp (myData, otherData, size()) == 0;
	}

	char* data() { return _data.get(); }

	const char* data() const { return _data.get(); }

	size_t size() const { return _size; }

private:

	std::unique_ptr<char> _data;
	size_t _size;
};
