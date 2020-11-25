#pragma once

#include <iostream>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//•¶Žš•ÏŠ·—p

class WCHAR_char
{
public:
	static const char* Change(const wchar_t* w)
	{
		//–¢ŽÀ‘•
	}

	static const wchar_t* Change(const char* c)
	{
		/*
		const wchar_t* w;
		const size_t d = sizeof(c);
		size_t iReturnValue;
		errno_t ret = mbstowcs_s(
			&iReturnValue,
			w,
			d,
			c,
			_TRUNCATE
		);
		*/
		size_t newsize = strlen(c) + 1;
		wchar_t* w = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, w, newsize, c, _TRUNCATE);

		return w;
	}

	static BSTR Change_BSTR_C(const char* c)
	{
		std::string str = c;
		
		int wslen = ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), NULL, 0);
		
		BSTR wsdata = ::SysAllocStringLen(NULL, wslen);
		::MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), wsdata, wslen);

		return wsdata;
	}
};