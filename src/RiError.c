/* \subsection{RiError.c} \label{RiError.c}
   Funktionen zum ErrorHandling
\begin{verbatim} */
#include <stdio.h>

#include "RiTypes.h"
#include "RiError.h"

RtString RmMsg;
RtFunc   RmErrorHandler;
/* \end{verbatim} \label{RiError.c:RiErrorPrint} \begin{verbatim}
 * RiErrorPrint: Hier werden alle Fehler ausgegeben!
 *
 *      INPUT : code	= Art des Fehler
 *		severity= Wertigkeit
 *		message = Fehlermeldung
 */
void
RiErrorPrint(code,severity,message)
	RtInt code,severity;
	RtString message;
{
  switch(code) {
    case RI_WARNING	: fprintf(stderr,"RI:warning:%s\n",message);break;
    case RI_ERROR	: fprintf(stderr,"RI:error:%s\n",message);break;
    case RI_FATAL	: fprintf(stderr,"RI:fatal error:%s\n",message);
			  fprintf(stderr,"*** Abort\n"); exit(-1); break;
    case RI_INTERNAL	: fprintf(stderr,"RI:internal error:%s\n",message);
			  fprintf(stderr,"*** Abort\n"); exit(-1); break;
    default		: (*RmErrorHandler)(RI_INTERNAL,(RtInt)0,"unexpected error-code");
  }
}
/* \end{verbatim} \label{RiError.c:RiErrorAbort} \begin{verbatim}
 * RiErrorAbort: Hier werden alle Fehler ausgegeben bei denen Abbruch
 *		 erfolgt!
 *
 *      INPUT : code    = Art des Fehler
 *              severity= Wertigkeit
 *              message = Fehlermeldung
 */
void
RiErrorAbort(code,severity,message)
RtInt code,severity;
RtString message;
{
  switch(code) {
    case RI_WARNING	: fprintf(stderr,"RI:warning:%s\n",message); break;
    case RI_ERROR	: fprintf(stderr,"RI:error:%s\n",message);
			  if (severity<1) 
			     fprintf(stderr,"*** Abort\n"); exit(-1); break;
    case RI_FATAL	: fprintf(stderr,"RI:fatal error:%s\n",message);
			  fprintf(stderr,"*** Abort\n"); exit(-1); break;
    case RI_INTERNAL	: fprintf(stderr,"RI:internal error:%s\n",message);
                          fprintf(stderr,"*** Abort\n"); exit(-1); break;    
    default		: (*RmErrorHandler)(RI_INTERNAL,(RtInt)0,"unexpected error-code");
  }
}
/* \end{verbatim} \label{RiError.c:RiErrorIgnore} \begin{verbatim}
*/
void
RiErrorIgnore(code,severity,message)
RtInt code,severity;
RtString message;
{}
/* \end{verbatim} \label{RiError.c:_init_RiError} \begin{verbatim}
*/
void
_init_RiError()
{
  RmMsg = (RtString)RmMalloc(200);
  RmErrorHandler = RiErrorPrint;
}
/* \end{verbatim} \label{RiError.c:_exit_RiError} \begin{verbatim}
*/
void
_exit_RiError()
{
   RmFree(RmMsg);
}
/*\end{verbatim}*/
