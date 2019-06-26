surface
txtplastic(
	float	Ks = .5,
		Kd = .5,
		Ka = 1,
		roughness = .1;
	color   specularcolor = 1;
	string  mapname="")
{
	point Nf = faceforward(normalize(N),I);
        point V = normalize(-I);

	if(mapname != "")
          Ci= color texture(mapname,s,t);
        else
	  Ci=Cs;
	Oi=Os;
	Ci=Os*(Ci*(Ka*ambient()+Kd*diffuse(Nf))+
            specularcolor*Ks*specular(Nf,V,roughness));
}
