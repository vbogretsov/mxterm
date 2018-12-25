#ifndef MX_REND_REND_H
#define MX_REND_REND_H

#define MX_REND_OPENGL  1
#define MX_REND_METAL   2


#ifndef MX_REND
#define MX_REND MX_REND_OPENGL
#endif

#if MX_REND != MX_REND_OPENGL && MX_REND != MX_REND_METAL
#error invalid MX_REND should have one of the following values: 1, 2
#endif

#endif // !MX_REND_REND_H
