/* \subsection{RiError.h} \label{RiError.h}
  HeaderFile zu den ErrorHandlingroutinen
\begin{verbatim} */
#ifndef _RiError_
#define _RiError_

#define RI_WARNING 1
#define RI_ERROR 2
#define RI_FATAL 3
#define RI_INTERNAL 4

extern RtString RmMsg;
extern RtFunc   RmErrorHandler;
extern void RiErrorPrint();
extern void RiErrorAbort();
extern void RiErrorIgnore();

extern void RiErrorHandler();

#define warning(level,msg) (*RmErrorHandler)(RI_WARNING,level,msg)
#define error(level,msg) (*RmErrorHandler)(RI_ERROR,level,msg)
#define fatal(level,msg) (*RmErrorHandler)(RI_FATAL,level,msg)
#define internal(level,msg) (*RmErrorHandler)(RI_INTERNAL,level,msg)

#endif _RiError_
/*\end{verbatim}*/
