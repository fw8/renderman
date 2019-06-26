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
          Cs= color texture(mapname,u,v);
	Oi=1;
	Ci=Os*(Cs*(Ka*ambient()+Kd*diffuse(Nf))+
            specularcolor*Ks*specular(Nf,V,roughness));
}
