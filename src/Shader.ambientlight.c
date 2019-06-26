/* \subsection{Shader.ambientlight.c} \label{Shader.ambientlight.c}
 *	RenderMan: Shading-Module ambientlight
\begin{verbatim}*/
#include <stdio.h>
#include <varargs.h>

#include "RiTypes.h"
#include "RiError.h"
#include "Shader.h"
#include "MathVector.h"

#define EQ(a,b) (!strcmp((a),(b)))

typedef struct {
		RtFloat intensity;
		RtColor lightcolor;
} ambientlight$type;

/*
 * LightShaderName: Liefert den Namen des LightShaders.
 */
static char *
LightShaderName()
{
	return "ambientlight" ;
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
	ambientlight$type *s = (ambientlight$type *)shader->data;
	fprintf(stream,"ambientlight: intensity = %g, \
lightcolor = (%g,%g,%g)\n",
s->intensity,s->lightcolor[0],s->lightcolor[1],
s->lightcolor[2]);
}

static int
LightShaderIlluminate(shader,Ps,direction,illuminance)
LightShader *shader;
RtPoint Ps;
RtPoint direction;
RtColor illuminance;
{
  ambientlight$type *s = (ambientlight$type *)shader->data;

  mulV3(s->lightcolor,s->intensity,illuminance); 
  return 0;  /* this is an ambientlightsource */
}

static LightShadeProcs ambientlight$progs = {
	LightShaderName,
	LightShaderInfo,
	LightShaderIlluminate
};

static LightShader *
NEW_LightShader(argp)
        va_list argp;
{
	RtToken token;

	ambientlight$type *s;
	LightShader   *S;

	s=(ambientlight$type *)RmMalloc(sizeof(ambientlight$type));
	S=(LightShader *)RmMalloc(sizeof(LightShader));

	s->intensity = 1.;
	setV3(1.,1.,1.,s->lightcolor);

        token=va_arg(argp,RtToken);

	while (token != RI_NULL) {

	  if      (EQ(token,"intensity")) 
		s->intensity= *(RtFloat *)va_arg(argp,RtPointer); 
	  else if (EQ(token,"lightcolor")) 
		RmMemCpy(va_arg(argp,RtPointer),
			s->lightcolor,sizeof(RtColor));
	  else {
		sprintf(RmMsg,"[LightShader.ambientlight] \
unknown or wrong token RI_%s (ignored)",token);
	        warning(2,RmMsg);
                break;
          }
	  token=va_arg(argp,RtToken);
	}

	S->data=(void *)s;
	S->procs= &ambientlight$progs ;

	return S;
}

void
_init_ambientlight$shader()
{
  RmInstallNewShader("ambientlight",NEW_LightShader);
}
/*\end{verbatim}*/
