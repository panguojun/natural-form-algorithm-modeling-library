/*
	raytrace->celestialbody
*/
// ***********************************************
// black hole
// ***********************************************

#define ROT(p, a) p=cos(a)*p+sin(a)*vec2(p.y, -p.x)

const float pi = 3.1415927;
const float R = 0.1;
const float ISCO = 3.0;
const float ICO = 1.5;
const vec3 c1 = vec3(0.05, 1.0, 0.0), c2 = vec3(0.05, 1.0, 2.25);

// iq's noise    
float hash( float n ) { return fract(sin(n)*753.5453123); }
float noise( vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}
vec3 HSV2RGB(vec3 c)
{
      vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
      vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
      return c.z * mix(K.xxx, clamp(p - K.xxx,0.0,1.0), c.y);
}
vec3 getCor(float r, vec3 p, vec3 camray)
{
    float d = (r - R * ISCO);
		
 	float w = 0.01/(d*d*d + 0.001);
    
    float v = w * r * dot(cross(p / r, vec3(0.,-1.,0.)), camray);
    
    ROT(p.xz, (1.1 + (d * 0.1)*w) * (iTime + 15.0));
    
    vec3 cor = mix(c1,c2,1.8 * noise(vec3(p.xz*8., r*8. + iTime*3.5))) * mix(0.0,1.0,smoothstep(0., 0.1, d));

    cor.xz *= mix(1.25, 0., d / (R * 14. -R * ISCO));

    // Doppler effect
    cor.x *= 1./(1. - 0.2*v);

    cor.z *= mix(0.,3.0,cor.x * 4.0);

    return HSV2RGB(cor);
}
//--------------------------------------------------------------

float sphere(vec3 p, vec3 o, float r)
{
    float x = smoothstep(0.0, -r, (length(p-o)-r));

    return x;
}
float cone(vec3 p, vec3 o1, vec3 o2, float r1, float r2)
{
    ROT(p.xy, (0.1) * sin(p.y + 15.0 + iTime*5.1));
    vec3 ux = vec3(1.,0.,0.);
    vec3 uy = normalize(o2-o1);
    vec3 uz = normalize(cross(ux,uy));
    ux = (cross(uy,uz));
    vec3 op = p - o1;
    vec3 pp = vec3(dot(op, ux),dot(op, uy),dot(op, uz));
    
    float fh = pp.y / (o2.y-o1.y);
    if(fh > 0. && fh < 1.)
    {
        float r = mix(r1, r2, (pp.y / (o2.y-o1.y)));
        if(length(pp.xz) < r)
            return 1.0;
    }
    return 0.;
}

vec3 transform(vec3 p)
{
    //ROT(p.xz, (0.1) * (iTime + 15.0));
    
    return vec3(p.x+0.71,p.z-0.61,p.y-0.21)*10.0;
}
bool shadow(vec3 p)
{
    vec3 v=vec3(0.,-0.01,0.01);
    for(int i = 0; i < 20; i ++)
    {
        p += v;
        float f1 = cone(p, vec3(0.,0.,0.),vec3(0.,0.5,-0.5),0.25,0.5);
        float f2 = cone(p, vec3(0.,0.5,-0.5),vec3(0.21,2.,0.0),0.25,0.5);
         if(f1+f2 > 0.0)
            return true;
    }
    return false;
}

vec3 shade(vec3 p, float v)
{
    vec3 cor=vec3(0.2,0.7,0.9);
    cor = mix(cor,vec3(0.), -normalize(p).z);
    if(shadow(p))
        return cor * 0.5;
    return cor;
}
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 pp = (-iResolution.xy + 2.0*fragCoord.xy) / iResolution.y;
    float eyer = 1.5;
    float eyea = -(iMouse.x / iResolution.x) * pi * 2.0;
    float eyef = ((iMouse.y / iResolution.y)-0.24) * pi * 2.0;
    
	vec3 cam = vec3(
        eyer * cos(eyea) * sin(eyef),
        eyer * cos(eyef),
        eyer * sin(eyea) * sin(eyef));
    
	vec3 front = normalize(- cam);
	vec3 left = normalize(cross(normalize(vec3(0.0,1,-0.01)), front));
	vec3 up = normalize(cross(front, left));
	vec3 v = normalize(front*1.5 + left*pp.x + up*pp.y);
    
    vec3 p = cam;
    float dt = 0.01;
    vec3 cor = vec3(0.0);
    
    for(int i=0;i<400;i++)
    {
		float r = length(p);
        float rr = length(p.xz);
        
        if(r > R)
        { 
            dt = mix(0.004,0.02,smoothstep(0.,0.05,abs(p.y))); // To be smooth
            
            float f = R/(r*r);
            float n = 1.0 / (1.0 - R/r);
            f = n*f;
            vec3 a = normalize(p) * (-f);
            
            v += a * dt;	
            vec3 np = p + (v / n) * dt;
        
            
            if (np.y * p.y < 0.)
            {
                if(r >= R * ISCO && r <= R * 13.)
                {
                    cor = getCor(r, np,front); 
                    break;
            	}
            }
            else
            {
                vec3 pp = transform(p);
                float head = sphere(pp, vec3(0.,0.,0.),0.25);
                float body = cone(pp, vec3(0.,0.25,0.),vec3(0.,-0.5,0.),0.3,0.1);
                float arm1 = cone(pp, vec3(0.,0.1,0.0),vec3(-1.,-0.8,0.0),0.05,0.1);
                float arm2 = cone(pp, vec3(0.,0.1,0.0),vec3(1.,-0.8,0.0),0.05,0.1);
                float leg1 = cone(pp, vec3(0.,0.75,0.0),vec3(-0.15,1.8,0.0),0.05,0.1);
                float leg2 = cone(pp, vec3(0.,0.75,0.0),vec3(0.15,1.8,0.0),0.05,0.1);       
                float f = (head+body+arm1+arm2+leg1+leg2)/6.;
                if(f > 0.0)
                {
                    cor = shade(pp, f);
                    break;
                }
            }
            p = np;
        }
        else
        {
            break;
        }
    }
    fragColor = vec4(cor,1.0);
}

// ***********************************************
// 土星
// ***********************************************
#define ROT(p, a) p=cos(a)*p+sin(a)*vec2(p.y, -p.x)
const float pi = 3.1415926;

const vec3 
    c1 = vec3(0.04, 0.02, 0.3),
	c2 = vec3(0.12, 0.06,1.25);
const vec3 
    c3 = vec3(0.06, 0.1, 0.5),
	c4 = vec3(0.10, 0.10,0.9);

const float R = 0.2;
const float R1 = R * 1.25;

float time;

//--------------------------------------------------------------
// otaviogood's noise from https://www.shadertoy.com/view/ld2SzK
//--------------------------------------------------------------
// This spiral noise works by successively adding and rotating sin waves while increasing frequency.
// It should work the same on all computers since it's not based on a hash function like some other noises.
// It can be much faster than other noise functions if you're ok with some repetition.
const float nudge = 0.739513;	// size of perpendicular vector
float normalizer = 1.0 / sqrt(1.0 + nudge*nudge);	// pythagorean theorem on that perpendicular to maintain scale
float SpiralNoiseC(vec3 p)
{
    float n = 0.0;	// noise amount
    float iter = 1.0;
    for (int i = 0; i < 8; i++)
    {
        // add sin and cos scaled inverse with the frequency
        n += -abs(sin(p.y*iter) + cos(p.x*iter)) / iter;	// abs for a ridged look
        // rotate by adding perpendicular and scaling down
        p.xy += vec2(p.y, -p.x) * nudge;
        p.xy *= normalizer;
        // rotate on other axis
        p.xz += vec2(p.z, -p.x) * nudge;
        p.xz *= normalizer;
        // increase the frequency
        iter *= 1.733733;
    }
    return n;
}

float SpiralNoise3D(vec3 p)
{
    float n = 0.0;
    float iter = 1.0;
    for (int i = 0; i < 5; i++)
    {
        n += (sin(p.y*iter) + cos(p.x*iter)) / iter;
        p.xz += vec2(p.z, -p.x) * nudge;
        p.xz *= normalizer;
        iter *= 1.33733;
    }
    return n;
}

float NebulaNoise(vec3 p)
{
   float final = p.y + 4.5;
    final -= SpiralNoiseC(p.xyz);   // mid-range noise
    final += SpiralNoiseC(p.zxy*0.5123+100.0)*4.0;   // large scale features
    final -= SpiralNoise3D(p);   // more large scale features, but 3d

    return final;
}

float map(vec3 p) 
{
	#ifdef ROTATION
	R(p.xz, iMouse.x*0.008*pi+iTime*0.1);
	#endif
    
	float NebNoise = abs(NebulaNoise(p/0.5)*0.5);
    
	return NebNoise+0.03;
}
//--------------------------------------------------------------
// iq's noise
//--------------------------------------------------------------
float hash( float n ) { return fract(sin(n)*753.5453123); }
float noise( vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f*f*(3.0-2.0*f);
	
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}
//--------------------------------------------------------------
// hsv and rgb
//--------------------------------------------------------------
vec3 RGB2HSV(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 HSV2RGB(vec3 c)
{
      vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
      vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
      return c.z * mix(K.xxx, (p - K.xxx), c.y);
}
bool shadow(vec3 p)
{
    vec3 v=vec3(0.,0.,-0.1);
    for(int i = 0; i < 10; i ++)
    {
        p += v;
        float r = length(p);
        if(r<R)
            return true;
    }
    return false;
}
vec3 getCor(float r, vec3 p)
{
    bool bshadow = shadow(p);
    float d = (r - R1);
    ROT(p.xz, (0.1) * (iTime + 15.0));
    
    vec3 cor0 = mix(c1,c2,0.5 * (0.75 + 2.0 * sin(r*35.0) + 0.5 * sin(r*188.0)+ 0.25 * sin(r*358.0) + 0.125 * sin(r*828.0)));
    
    vec3 cor = mix(cor0, vec3(0.), 0.05 * NebulaNoise(vec3(r,d,0)));
    
    return bshadow ? mix(vec3(0.),HSV2RGB(cor),0.25) : HSV2RGB(cor);
}
vec3 shadow2(vec3 p)
{
    if(p.y>0.)
         return vec3(0.);
    
    vec3 v=vec3(0.,0.002,0.01);
    for(int i = 0; i < 150; i ++)
    {
        p += v;
        
        float r = length(p);
        if(r<R*0.9)
            return vec3(0.);
        
        if(p.y>=0.)
        {
            if(r >= 0.275 && r <= 0.5)
            {
                vec3 cor = getCor(r, p); 
                if(cor.r > 0.7)
                    return cor;
            }
            return vec3(0.);
        }
    }
    return vec3(0.);
}
vec3 getSaturnCor(float r, vec3 p, vec3 camray, float dc)
{
    vec3 shadow = shadow2(p);
    
    float dr = (r - R1);
    ROT(p.xz, (0.1) * (iTime + 15.0));
    
    float nz = (0.65 + 0.5 * sin(p.y*75.0));
    
    float h = dc * 0.08;
    float h0 = 0.25;
    float n = min(4., floor(log2(h0 / h)));
    h=h0;
    for(float i = 1.; i <= n; i +=1.0)
    {
        float w = i*100.;
        nz += (noise(p*w) - 0.5) * h;
        h /= 2.0;
    }
    vec3 cor0 = mix(c3,c4,nz);
    
    vec3 cor = mix(cor0, vec3(0.), dr / (1.-R1));
    
    return shadow.r >= 0.01 ? mix(shadow * 0.25,HSV2RGB(cor),0.6) : HSV2RGB(cor);
}

//--------------------------------------------------------------
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    time = iTime;
    vec2 pp = (-iResolution.xy + 2.0*fragCoord.xy) / iResolution.y;
    float eyer = 2.0;
    float eyea = -((iMouse.x - 120.5) / iResolution.x) * pi * 2.0;
    float eyef = ((iMouse.y / iResolution.y)-0.22) * pi * 2.0;
    
	vec3 cam = vec3(
        eyer * cos(eyea) * sin(eyef),
        eyer * cos(eyef),
        eyer * sin(eyea) * sin(eyef));
    
    //ROT(cam.xz, (0.25) * (iTime + 15.0));
    
	vec3 front = normalize(- cam);
	vec3 left = normalize(cross(normalize(vec3(0.25,1,-0.01)), front));
	vec3 up = normalize(cross(front, left));
	vec3 v = normalize(front*1.75 + left*pp.x + up*pp.y);
    
    vec3 p = cam;
    
    float dt = 0.005;
    vec3 cor = vec3(0.0);
    
    
    for(int i = 0; i < 800; i ++)
    {
		float r = length(vec3(p.x,p.y*1.1,p.z));
        
        if(r > R)
        { 
            dt = mix(0.002,0.005,smoothstep(0.,0.05,abs(p.y * r))); // To be smooth
            vec3 np = p + (v) * dt;
            
            vec3 rp = np;
            ROT(rp.xz, (0.1) * (iTime + 15.0));
     		vec3 dp1=rp-vec3(0.95,0.0,1.11);
            vec3 dp2=rp-vec3(-0.85,0.0,1.11);
            if(length(dp1) < 0.025)
            {
                cor = mix(mix(vec3(0.10, 0.10,0.0),vec3(0.80, 0.80,0.5),1.5 * noise(p*180.))*0.8*(-normalize(dp1).z), cor, cor.r * 2.); 
            	break;
            }
            else if(length(dp2) < 0.025)
            {
                cor = mix(mix(vec3(0.20, 0.20,0.0),vec3(0.80, 0.80,0.85),1.5 * noise(p*900.))*0.8*(-normalize(dp2).z), cor, cor.r * 2.); 
            	break;
            }
            
            else if(r > R * 1.01)
            {
                if (p.y * np.y < 0.)
                {
                    if(r >= 0.25 && r <= 0.5)
                    {
                        cor = mix(cor, getCor(r, p),0.25);
                    }
                }
            }
            else
            {
                float a = smoothstep(0.05*R, 0.0, (r-R)) * (1.0-abs(dot(-front,-normalize(p))));
                cor = cor + vec3(0.85,0.9,0.95)*(0.01*a);
            }
            p = np;
        }
        else
        {
            cor = mix(getSaturnCor(r, p, front,length(p-cam))*0.8*(-normalize(p).z), cor, cor.r * 2.); 
            break;
        }
    }
    fragColor = vec4(cor,1.0);
}
