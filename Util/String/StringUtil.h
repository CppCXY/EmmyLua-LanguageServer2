#pragma once

#include <vector>
#include <string_view>
#include <string>

/*
 * 字符串处理函数
 */
namespace string_util
{
	std::vector<std::string_view> Split(std::string_view source, std::string_view separator);

	std::string_view TrimSpace(std::string_view source);

	std::string Replace(std::string_view source, std::string_view oldString, std::string_view newString);

	bool IsStringEqualIgnoreCase(std::string_view lhs, std::string_view rhs);

	std::string_view GetFileRelativePath(std::string_view workspace, std::string_view filePath);

	bool FileWildcardMatch(std::string_view sourceFile, std::string_view pattern);

	struct CaseInsensitiveLess final
	{
		bool operator()(std::string_view lhs, std::string_view rhs) const;
	};
}
