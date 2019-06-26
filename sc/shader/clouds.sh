surface
clouds(float Kd=.8,Ka=.2) {

  float sum;
  float i,freq;
  color refl;

  sum=0;
  freq=4.0;

  for (i=0; i<6; i=i+1) {
    sum=sum+1/freq*abs(.5-noise(freq*P));
    freq=2*freq;
  }

  refl=Cs*sum;

  Ci = refl * (Ka * ambient() + Kd * diffuse(faceforward(normalize(N),I)));
/*  Oi = 1.0;  Always make surface opaque */
  Oi = Os;
}
