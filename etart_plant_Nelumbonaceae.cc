// -------------------------------------------------
// 荷花 Lotus
// -------------------------------------------------
void heban1(vec3 p, vec3 v0, vec3 v, int len, int wid0)
{
	v0.norm();
	v.norm();
	vec3 vv = v0.cross(v);
	vec3 pp;
	real deta = rrnd(.5, 1);
	for(int i = 0; i < len; i ++)
	{	
		real ai = real(i) / len;	
		v = v + v0 * 0.0025;v.norm();
		p = p + v * 0.00025;	
		
		vec3 p1 = p + (vv + v0 * deta)  * blend2(0, 0.0125, ai, 2);
		vec3 p2 = p + (-vv + v0 * deta) * blend2(0, 0.0125, ai, 2);		
		for(int ii = 0; ii < wid0; ii += 1)
		{
			real aii = real(ii) / wid0;								
			pp = blend(p, p1, aii);
			int cor = blendcor(0xFFFFFFFF, 0xFFC000FF, 0.5 + 0.5 * sin(aii * 4 * PI));
			cor = blendcor(0xFFFFFFFF, cor, ai, 1.5);
			cor = blendcor(cor, 0xFFC000FF, aii, 8);
			//int cor1 = blendcor(0xFFFFFFFF, 1, pp.z);
			pixel(pp, cor);
			pp = blend(p, p2, aii);
			//int cor2 = blendcor(0xFFFFFFFF, 1, pp.z);
			pixel(pp, cor);	

		}
	}
}

// -------------------------------------------------
void hehua1(vec3 p, vec3 v, int len, int wid0)
{		
	for(int i = 0; i < len; i ++)
	{
		real alp = real(i) / len;
		real alp2d = blend2d(-1, 1, p.x, p.y);	
		v.rot(alp2d * 0.00025 * PI, vec3::UZ);			
		v.norm();
		p = p + v * 0.00025;
		int wid = blend(0.25, 1, alp2d) * wid0;
		for(int ii = 0; ii < wid; ii ++)
		{
			real beta = real(ii) / wid0;	
			if(p.y > level)
			{
				pixelAA(p + vec3(blend(0, 0.0025, beta), 0, 0), blendcor(0xFFFFFFCC, 1,  beta, 2));
				pixelAA(p - vec3(blend(0, 0.0025, beta), 0, 0), blendcor(0xFFFFFFCC, 1, beta, 2));
						
			}			
		}
		
		if(i == len - 1)
		{
			int leaflen = len * 0.5;
			//v = vec3(0.5, 0.5, 0.25);v.norm();
			vec3 zz = vec3::UZ;
			for(int j = 0; j < 8; j ++)
			{
				real betaj = real(j) / (8);				
				vec3 vv0 = zz.cross(v);vv0.norm();					
				quaternion q(2 * PI * 1.0 / (8), v);				
				zz = (q * vv0).cross(v);zz.norm();				
				heban1(p, v * blend(-1, 1, rrnd(0, 1), 0.25), vv0, 512, 128);
			}
		}
	}
}

real factor1 = 1;

// ------------------------------------------------
void juhuaban(vec3 p, vec3 v, vec3 up0, int cor0)
{
	real sz = rrnd(0.5, 1.2) * 2.5 * (1 - factor1);
	
	vec3 lgtdir(1, -1, 1); lgtdir.norm();
	v.norm();
	vec3 g(0, -0.0001, 0);
	vec3 up = up0;
	real deta = rrnd(.5, 1);
	vec3 dup = up * rrnd(-0.5, 0.5);
	for(int i = 0; i < 500; i ++)
	{
		real ai = real(i) / 500;
		v = v + g;
		v.norm();
		
		p = p + v * (0.0001 * sz);
		vec3 norm0 = v.cross(up);norm0.norm();
		up = norm0.cross(v);
		
		real alpha = blend2d(0.8, 1.2, p.x, p.y);		
		real ssz = blend3(0, 0.01 * sz * alpha, ai, 0.8, 3) * 0.25;
		
		dup = dup + up * (blend(0, 0.5, ai, 2) * alpha) - v * blend(0, 0.25, ai, 3);
		p = p + dup * 0.0000025; 
		
		for(int ii = 0; ii < 250; ii += 1)
		{
			real aii = real(ii) / 250;	
			
			vec3 p1 = p + (up * blend(0, 1, aii, 2) + norm0) * ssz;
			vec3 p2 = p + (up * blend(0, 1, aii, 2) - norm0) * ssz;	
			
			int cor = cor0;//blendcor(cor0, RGB(108, 17, 17),  ai);
			cor = blendcor(cor, 1, 0.5 + 0.5 * sin(aii * 10));
			{
				vec3 pp = blend(p, p1, aii);					
				pixel(pp, cor);		
			}
			{
				vec3 pp = blend(p, p2, aii);				
				alpha = blend2d(0.8, 1.2, pp.x, pp.y);				
				pixel(pp, cor);
			 }					
		}
	}
}

// ------------------------------------------------
void flowercore(vec3 p, vec3 v)
{
	v.norm();
	vec3 up = vec3::UZ;
	vec3 vv = v.cross(up);vv.norm();
	for(int i = 0; i < 500; i ++)
	{	
		real alpha1 = i / 500.0;
		p = p + v * 0.00003;	
		
		for(int j = 0; j < 500; j ++)
		{
			real alpha2 = j / 500.0;
			real ang = blend(0, 2 * PI, real(j) / 500);
			vec3 n = vv.rotcopy(ang, v);
			vec3 pp = p + n * blend(0, 0.01, alpha1);
			real alp2d = blend2d(-1, 1, pp.x, pp.y);	
			int cor = blendcor(1, 0xFFCCFFFF,  0.5 + 0.5 * sin(alpha2 * 18 * PI + alpha1 * PI));
			cor = blendcor(1, cor, 0.5);
			pixel(pp, cor);
			
			if(i == 500 - 1)
			{
				for(int ii = 0; ii < 500; ii ++)
				{
					real alpha3 = ii / 500.0;
					vec3 ppp = blend(p, pp, alpha3);
					int cor1 = blendcor(0xFF00FFF0, 0xFFCCFFFF,  alpha3);
					cor = blendcor(1, cor1, 0.5 + 0.5 * 0.5 * (sin(8 * PI * alpha3) + sin(8 * PI * alpha2)));
					pixel(ppp, cor);
				}	
			}
			
			if(rrnd(0, 10000) < 10 && rrnd(0, 100) < 10 && alpha1 < 0.5)
			{//HUA BAN		
				vec3 zz = vec3::UY;				
				{			
					real jj = alpha2;
					real ii = alpha1;				
									
					vec3 vv0 = zz.cross(v);vv0.norm();					
					quaternion q(rrnd(-1, 1) * 0.1 * PI + blend(-PI, PI, rrnd(0, 1)), v);	
					vv0 = (q * vv0);
					zz = (vv0).cross(v);zz.norm();
					factor1 = blend(0, .25, ii + rrnd(0.01, 0.05));
					juhuaban(p, blend(vv0, v, factor1), v, 0xFF00FFFF);
				}	
			}
		}
	}	
}
// -------------------------------------------------
void lianpeng(vec3 p, vec3 v, int len, int wid0)
{
	vec3 cp1[] = {
		v,
		v.rotcopy(rrnd(-PI, PI), vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy()),
		v.rotcopy(rrnd(-PI, PI), vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy()),
		v.rotcopy(rrnd(-PI, PI), vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy())
	};	
	for(int i = 0; i < len; i ++)
	{
		real alp = real(i) / len;
		real alp2d = blend2d(-1, 1, p.x, p.y);	
		v = bezier2(cp1, alp);		
		v.norm();
		p = p + v * 0.00035;
		int wid = blend(0.25, 1, alp2d) * wid0;
		for(int ii = 0; ii < wid; ii ++)
		{
			real beta = real(ii) / wid0;	
			if(p.y > level)
			{
				pixelAA(p + vec3(blend(0, 0.001, beta), 0, 0), blendcor(0xFFFFFFCC, 1,  beta, 2));
				pixelAA(p - vec3(blend(0, 0.001, beta), 0, 0), blendcor(0xFFFFFFCC, 1, beta, 2));						
			}			
		}
		
		if(i == len - 1)
		{
			int leaflen = len * 0.5;
			//v = vec3(0.5, 0.5, 0.25);v.norm();
			vec3 zz = vec3::UZ;			
			{			
				vec3 vv0 = zz.cross(v);vv0.norm();					
				quaternion q(rrnd(-PI, PI), v);	
				zz = (q * vv0).cross(v);zz.norm();				
				flowercore(p, v);
			}
		}
	}
}

void drawHe1(vec3 p, vec3 v, int len, int wid0)
{
	vec3 cp1[] = {
		v,
		v.rotcopy(rrnd(-PI, PI), vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy()),
		v.rotcopy(rrnd(-PI, PI), vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy()),
		v.rotcopy(rrnd(-PI, PI), vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy())
	};	
	for(int i = 0; i < len; i ++)
	{
		real alp = real(i) / len;
		real alp2d = blend2d(-1, 1, p.x, p.y);	
		v = bezier2(cp1, alp);		
		v.norm();
		p = p + v * 0.00035;
		int wid = blend(0.25, 1, alp2d) * wid0;
		for(int ii = 0; ii < wid; ii ++)
		{
			real beta = real(ii) / wid0;	
			if(p.y > level)
			{
				pixelAA(p + vec3(blend(0, 0.001, beta), 0, 0), blendcor(0xFFFFFFCC, 1,  beta, 2));
				pixelAA(p - vec3(blend(0, 0.001, beta), 0, 0), blendcor(0xFFFFFFCC, 1, beta, 2));						
			}			
		}
		
		
		if(i == len - 1)
		{
			v = blend(v, vec3::UZ, rrnd(0, 0.45));
			v.norm();
			
			int leaflen = len * 0.5;
			//vec3 vv0 = vec3::UZ.cross(v);vv0.norm();
			vec3 zz = vec3::UX;
			for(int j = 0; j < len * 2; j ++)
			{
				real betaj = real(j) / (len * 2);				
				vec3 vv0 = zz.cross(v);vv0.norm();					
				quaternion q(PI * 1.0 / (len * 2), v);				
				zz = (q * vv0).cross(v);zz.norm();			
				vec3 vv = q * vv0;
				vec3 p1 = p;
				
				for(int ii = 0; ii < leaflen; ii ++)
				{
					real beta1 = real(ii) / leaflen;			
					
					real alp2d1 = blend2d(0, 1, 0.5 + 0.5 * sin(2 * PI * betaj), beta1);	
					
					vv = vv + v * blend(-0.005, 0.005, 0.5 + 0.5 * sin(beta1 * 12))*blend(-1, 1, alp2d1);
					vv.norm();
					p1 = p1 + vv * 0.00025;						
					
					real tbetaj = betaj * blend(0.5, 1.5, alp2d1);					
					real xxx = blend2d(0, 1, 0.5 + 0.5 * sin(PI * 10 * betaj * alp2d), blend(0.4, 0.6, beta1));
					if(xxx < 0.9)
					{
						int cor = blendcor(0xFF008080, 1, xxx);
						pixelAA(p1, cor);					
					}
				}
			}
		}
	}
} 
