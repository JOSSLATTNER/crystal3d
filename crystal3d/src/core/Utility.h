#pragma once
#include <string>
#include <sstream>
#include <memory>

#define FORWARD_DECL(t) class t
#define BIT_HAS_FLAG(b,f) (b & f) == f

namespace Util
{
	template<typename ... Args>
	std::string sprintf_safe(const std::string& format, Args...args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1);
	}

	template<typename T>
	void string_parse(const std::string& a_Str, T& a_Val)
	{
		std::istringstream buffer(a_Str);
		buffer >> a_Val;
	}

	template<typename T>
	T string_parse(const std::string& a_Str)
	{
		T t;
		std::istringstream buffer(a_Str);
		buffer >> t;
		return t;
	}

	template<typename...Args>
	bool sscanf_safe(const std::string& buffer, const std::string& format, Args&...args)
	{
		return sscanf_s(buffer.c_str(), format.c_str(), &args...) == sizeof...(Args);
	}

	inline std::istream& getline_safe(std::istream& is, std::string& t)
	{
		t.clear();
		
		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for (;;)
		{
			int c = sb->sbumpc();
			switch (c)
			{
				case '\n':
				{
					return is;
				}
				case '\r':
				{
					if (sb->sgetc() == '\n')
						sb->sbumpc();
					return is;
				}
				case EOF:
				{
					// Also handle the case when the last line has no line ending
					if (t.empty())
						is.setstate(std::ios::eofbit);
					return is;
				}
				default:
				{
					t += static_cast<char>(c);
				}
			}
		}
	}
}