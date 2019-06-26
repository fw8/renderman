/* \subsection{Shader.pointlight.c} \label {Shader.pointlight.c}
 *	RenderMan: Shading-Module pointlight
\begin{verbatim}*/
#include <stdio.h>
#include <varargs.h>

#include "RiTypes.h"
#include "RiError.h"
#include "AdminGlobals.h"
#include "Shader.h"
#include "MathVector.h"

#define EQ(a,b) (!strcmp((a),(b)))

typedef struct {
		RtFloat intensity;
		RtColor lightcolor;
		RtPoint from;		/* light position */
} pointlight$type;

/*
 * LightShaderName: Liefert den Namen des LightShaders.
 */
static char *
LightShaderName()
{
	return "pointlight" ;
}

/*
 * LightShaderInfo: 
 * Druckt die Instanzen-Variablen des LightShaders aus.
 */
static void
LightShaderInfo(shader,stream)
LightShader *shader;
FILE *stream;
{
	pointlight$type *s = (pointlight$type *)shader->data;
	fprintf(stream,
	"pointlight: intensity = %g, lightcolor = (%g,%g,%g)\n\
             from = (%g,%g,%g)\n",s->intensity,
	  s->lightcolor[0],s->lightcolor[1],s->lightcolor[2],
	  s->from[0],s->from[1],s->from[2]);
}

#define mm(A,B,R) R[0]=A[0]*B[0],R[1]=A[1]*B[1],R[2]=A[2]*B[2] 
static int
LightShaderIlluminate(shader,Ps,L,CI)
LightShader *shader;
RtPoint Ps;
RtPoint L;
RtColor CI;
{
  RtFloat t;
  pointlight$type *s = (pointlight$type *)shader->data;

/* illuminate(from) CI=intensity*lightcolor/L.L; */

  subV3(Ps,s->from,L);
  t=s->intensity/smulV3(L,L);
  mulV3(s->lightcolor,t,CI);
  mulV3(L,-1.,L); /* point from source to Ps */
  return 1;  /* this is not an ambientlightsource */
}

static LightShadeProcs pointlight$progs = {
	LightShaderName,
	LightShaderInfo,
	LightShaderIlluminate
};

static LightShader *
NEW_LightShader(argp)
        va_list argp;
{
	RtToken token;

	pointlight$type *s;
	LightShader   *S;

	s=(pointlight$type *)RmMalloc(sizeof(pointlight$type));
	S=(LightShader *)RmMalloc(sizeof(LightShader));

	s->intensity = 1.;
	setV3(1.,1.,1.,s->lightcolor);

	/* point from = point "camera" (0,0,0) */
 	setV3(0.,0.,0.,s->from);
	RmTransformPoint(s->from,RmGlobals.CtoW,s->from);

        token=va_arg(argp,RtToken);

	while (token != RI_NULL) {

	  if      (EQ(token,"intensity")) 
		s->intensity= *(RtFloat *)va_arg(argp,RtPointer); 
	  else if (EQ(token,"lightcolor")) 
		RmMemCpy(va_arg(argp,RtPointer),
		s->lightcolor,sizeof(RtColor));
	  else if (EQ(token,"from"))
		RmMemCpy(va_arg(argp,RtPointer),
		s->from,sizeof(RtPoint));
	  else {
		sprintf(RmMsg,"[LightShader.pointlight] \
		unknown or wrong token RI_%s (ignored)",token);
	        warning(2,RmMsg);
                break;
          }
	  token=va_arg(argp,RtToken);
	}

	S->data=(void *)s;
	S->procs= &pointlight$progs ;

	return S;
}

void
_init_pointlight$shader()
{
  RmInstallNewShader("pointlight",NEW_LightShader);
}
/*\end{verbatim}*/
