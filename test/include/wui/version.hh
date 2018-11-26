#pragma once

#if defined(WUI_GL46) || defined(WUI_GL45)
#define _USEGL45
#elif defined(WUI_GL44) || defined(WUI_GL43) || defined(WUI_GL42) || defined(WUI_GL41) || defined(WUI_GL40) || defined(WUI_GL33)
#define _USEGL33
#else
#define _INVALIDGLVERSION
#endif
