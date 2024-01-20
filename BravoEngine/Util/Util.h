#pragma once
#include <Windows.h>
#include <string>
#include <comdef.h>

class BVException
{
public:
	BVException() = default;
	BVException(HRESULT hr, const std::wstring& funcName, const std::wstring& fileName, int lineNumber) : _ErrCode(hr), _FunctionName(funcName), _FileName(fileName), _LineNumber(lineNumber) {}

	std::wstring ToString()const
	{
		_com_error err(_ErrCode);
		std::wstring msg = err.ErrorMessage();

		return _FunctionName + L" failed / FileName : " + _FileName + L" / LineNumber : " + std::to_wstring(_LineNumber) + L" / Error : " + msg;
	}

	HRESULT _ErrCode;
	std::wstring _FunctionName;
	std::wstring _FileName;
	int _LineNumber;
};

#ifndef BV_Throw
#define BV_Throw(x)
{
	HRESULT hr = (x);
	std::wstring wFileName = AnsiToWString(__FILE__);
	if (FAILED(hr))
	{
		throw BVException(hr, L#x, wFileName, __LINE__);
	}
}
#endif