#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)												\
		{													\
			HRESULT hr = (x);								\
			if(FAILED(hr))									\
			{												\
				LPCSTR output;								\
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |	\
				FORMAT_MESSAGE_IGNORE_INSERTS |				\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
				NULL,										\
				hr,											\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,							\
				0,											\
				NULL);										\
				MessageBox(NULL, output, "Error", MB_OK);	\
			}												\
		}
#endif
#ifndef BHR
#define BHR(x)												\
		{													\
			HRESULT hr = (x);								\
			if(FAILED(hr))									\
			{												\
				LPCSTR output;								\
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |	\
				FORMAT_MESSAGE_IGNORE_INSERTS |				\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,				\
				NULL,										\
				hr,											\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
				(LPTSTR) &output,							\
				0,											\
				NULL);										\
				MessageBox(NULL, output, "Error", MB_OK);	\
				return false;								\
			}												\
		}
#endif
#else
	#ifndef HR
		#define HR(x) (x)
	#endif
	#ifndef BHR
		#define BHR(x) (x)
	#endif
#endif 


//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = 0; }
