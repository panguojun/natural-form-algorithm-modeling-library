/*
	raytrace->celestialbody
*/
// -----------------------------------------------
// black hole
// -----------------------------------------------

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
