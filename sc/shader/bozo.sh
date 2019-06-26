surface
bozo(
	float Ka	= .2,
	      Kd	= .4,
	      Ks	= .6,
	      roughness = .1,
	      turbulence= .3)
{
	point Nf = faceforward(normalize(N),I);
	point turb = P;
	float n,scale;
	color c;

	if (turbulence != 0) {
		for (scale=1;.1<scale;scale=scale*.5) {
			turb = turb + scale * point noise(P);
		}
                P = P + turb * turbulence;
	}
        n = noise(P);

/*	printf("n = %5.2f\nP = %2.2p\n%s\n",n,P,"... und noch ein String");*/
/*	printf("n = %5.2f\n",n); */

	if (n<.4)      c = 1;
	else if (n<.6) c = color (0,1,0);
	else if (n<.8) c = color (0,0,1);
	else           c = color (1,0,0);

	Oi = 1; /* total opaque */
	Ci = c * Cs * (Ka*ambient()+Kd*diffuse(Nf)) 
                 + Ks*specular(Nf,normalize(-I),roughness);
}
