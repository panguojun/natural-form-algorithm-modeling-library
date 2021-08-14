/*
  Raytrace->PhysicalObject
*
// ------------------------------------------------
// atom
// ------------------------------------------------
#define ROT(p, a) p=cos(a)*p+sin(a)*vec2(p.y, -p.x)

float a0 = 5.1;
int  n = 5;
int  l = 2;
int  m = 1;
float A = 0.;
float Y0 = 0.;

float JC(int x)
{
    float v = 1.;
    for (int i = 1; i <= x; i++)
    {
        v *= float(i);
    }
    return v;
}
int powN1(int n)
{
    return n % 2 == 0 ? 1 : -1;
}
float Cmn(int n, int m)
{
    return JC(n) / (JC(m) * JC(n - m));
}
float laguerreL(int n, int m, float x)
{
    float sum = 0.;
    for (int k = 0; k <= n; k++)
    {
        sum += float(powN1(k))* Cmn(n + m, n - k)* pow(x, float(k)) / JC(k);
    }
    return sum;
}
float PML(float m, float l, float x)
{
    float A1 = pow(1. - x * x, m / 2.);
    float sum = 0.;
    int kl = int((l - m) / 2.);
    for (int k = 0; k <= kl; k++)
    {
        float jk = JC(k);
        float jk2 = JC(int(l) - k);
        float jk3 = JC(int(l) - 2 * k - int(m));
        float B = pow(2., l) * jk * jk2 * jk3;

        float E = pow(x, l - 2. * float(k) - m);
        sum += (float(powN1(k)) * JC(2 * int(l) - 2 * k) / B) * E;
    }
    return A1 * sum;
}

float calcR(float r)
{
    float B = pow(2. * r / (float(n) * a0), float(l));
    float C = laguerreL(n - l - 1, 2 * l + 1, 2. * r / (float(n) * a0));
    float E = exp(-(r / (float(n) * a0)));
    return A * B * C * E;
}
float calcY(float cosang)
{
    float pml = PML(float(m), float(l), 
                    abs(cosang)
                   ); 
    float Yml = Y0 * pml;
    return pml * Yml;
}
vec2 calcF(float fai)
{
    return vec2(cos(float(m) * fai), sin(float(m) * fai));
}
bool mapcor(vec3 p, out float fcolor)
{
	float r = length(p);
	vec3 v = p / r;
	vec2 xz = normalize(v.xz);
	float R = calcR(r);

	float Y = calcY(v.y / length(v));
	float fai = atan(-xz.y, xz.x);
    vec2 VF = calcF(fai);
	
	float epx = R * Y * VF.x;
    float epy = R * Y * VF.y;
	
	float nlum =  (epy*epy) * 10000.0f;
	fcolor += nlum * 100.0f;

	bool ret = fcolor >= 1.0;
	if (ret)
		fcolor = 1.0;
    
	return ret;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 pp = (-iResolution.xy + 2.0*fragCoord.xy) / iResolution.y;
    float eyer = 2.0;
    float eyea = -((iMouse.x + 80.5) / iResolution.x) * 3.1415926 * 2.0;
    float eyef = ((iMouse.y / iResolution.y)-0.24) * 3.1415926 * 2.0;
    
	vec3 cam = vec3(
        eyer * cos(eyea) * sin(eyef),
        eyer * cos(eyef),
        eyer * sin(eyea) * sin(eyef));
    
    ROT(cam.xz, (0.25) * (iTime + 15.0));
    
	vec3 front = normalize(- cam);
	vec3 left = normalize(cross(normalize(vec3(0.25,1,-0.01)), front));
	vec3 up = normalize(cross(front, left));
	vec3 v = normalize(front*1.75 + left*pp.x + up*pp.y);
    
    vec3 p = cam;
    
    float dt = 0.03;
    float cor = 0.0;
    A = sqrt(pow(2. / (float(n) * a0), 3.) * (JC(n - l - 1) / (2.0 * float(n) * JC(n + l))));
    Y0 = (1. / sqrt(2. * 3.1415926)) * sqrt(((2. * float(l) + 1.) / 2.0) * (JC(l - m) / JC(l + m)));
    
    for(int i = 0; i < 100; i ++)
    {
        p += v * dt;
        
		if(mapcor(p * 500., cor))
            break;
    }
    vec4 color = vec4(cor,cor,cor,1.0);
	
    fragColor = color;
}
