#pragma once

#ifdef JE_TRACE_IMPLEMENTATION
	#define JE_TRACE_TYPE(name, enabled)	\
		namespace Tracers					\
		{									\
			bool name = enabled;			\
		};

#else
	#define JE_TRACE_TYPE(name, enabled)	\
		namespace Tracers					\
		{									\
			extern bool name;				\
		};

#endif
