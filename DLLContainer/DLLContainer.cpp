#include "pch.h"
#include "DLLContainer.h"

// EXCEPTION CLASS DEFINITION *************************

cont::EXCEPTION::EXCEPTION(int what_happened)
{
	what = what_happened;
}

const wchar_t* cont::EXCEPTION::eGet()const
{
	switch (what)
	{
	case BAD_PTR:
		return L"Bad pointer passed in double link list container !";

	case BAD_INDEX:
		return L"Bad index passed to double link list container !";

	case BAD_PARAM:
		return L"Bad parameter passed to double link list container !";
	}

	return L"Unknown error occurred in double link list container !";
}

////////////////////////////////////////////////////////

