/*
	  etart->animal->Cyprinidae
*/
// -------------------------------------------------
// goldenfish
// -------------------------------------------------
void goldenfish_tail2(vec3 pts[], vec3 v0, int len0, int wid)
{
	vec3 v2 = vec3(-1, 1, 0);v2.normalise();	
	vec3 ax = vector3::UZ;
	
	for(int i = 0; i < wid / 2; i ++)
	{
		real pos = i < wid / 2 ? real(i) / wid : 2 - real(i) / wid;
		
		real ang = 12.0 * PI + blend(- 6.0 * PI, 4 * PI, pos);		
			
		int len = len0 * blend(1, 1,  pos);		
		vec3 dv = ax * (blend(0.5, 1, pos, 1)); 
		vec3 v = v0;
		vec3 pp = pts[i];	
		for(int ii = 0; ii < len; ii ++)
		{
			real pos1 = real(ii) / len;
			
			real alpha2d = blend2d(0.8, 1, pos, pos1);
			real sinalpha = sin(pos * PI * alpha2d * 400);
			real deta = blend(0.0, 0.2, sinalpha);		
			
			real dang = blend(ang / len, 0, pos1, 1);
			v.rot(dang * blend2d(0.1, 2, pos, pos1), ax);
			
			pp = pp + (v + (i < wid / 2 ? dv : -dv) * 2).normalisecopy() * 0.001;
	
			int cor = blendcor(RGB(253, 0, 79), RGB(55, 55, 255), pos);
			cor = blendcor(RGB(255, 255, 255), RGB(253, 0, 79), sinalpha, 1);
			
			pixel(pp, i < wid / 2 ? cor : ~cor, blend(1, 0, real(ii) / 2500), pp.z);				
		}
	}
}

// -------------------------------------------------
void goldenfish_body4(vec3 p, vec3 v1, int len)
{
	const int wid = 2000;	
	vec3 lgtdir(-0.5, -0.5, 1); lgtdir.normalise();
	
	v1.normalise();
	vec3 v2(v1.x, v1.y - 2.8, v1.z); v2.normalise();
	vec3 v;
	vec3 lstiipp1;
	vec3 lstipp1[wid];
	vec3 z = vector3::UZ;
	for(int i = 0; i < len; i ++)
	{
		real pos = real(i) / len;
		
		v = blendvec3(v1, v2, pos, 1);
		
		p = p + v * 0.0003;				
		
		vec3 norm0 = v.crossproduct(z);norm0.normalise();
		z = norm0.crossproduct(v);z.normalise();
		
		real r0 = 0.1;
		
		real r1 = blend2(0.2, 1.5, pos, 2);
		
		real power = blend3(18, 1.0, pos, 0.4, 3);
		 
		vec3 pp0 = p - norm0 * (1 * r1 * r0);
		 
		for(int ii = 0; ii < wid; ii ++)
		{
			real pos1 = real(ii) / wid;
			
			real ang1 = blend(0, 2 * PI, pos1);				
			real ang2 = blend(PI / 2 + PI / 8, PI / 2, real(i) / len, 1);
			real pos2 = ang1 < PI ? (ang1) / (PI) : (2 * PI - ang1) / (PI);
			
			real r3;
			{// duzi
				real ang = ang1 < PI ? ang1 : 2 * PI - ang1;				
				if(ang < PI / 2)
				{
					real s = sin(ang);
					real c = blend(1, 0, ang / (PI / 2));
					r3 = sqrt(s*s + c*c);
				}
				else
					r3 = 1;
				r3 *= blend3(0.4, 2.0, pos, 0.4, 2);
			}
			real rr = blend(0, r1*r0, 1 - pos2, power);
			
			vec3 norm = norm0;			
			norm = v.rotcopy(ang2, quaternion(ang1 + PI / 2, v) * norm);			
			norm.normalise();
			vec3 pp = pp0 + norm * rr;
			
			real lum;
			vec3 norm1;
			{// lum
				norm1 = -(lstipp1[ii] - pp).crossproduct(lstiipp1 - pp);norm1.normalise();				
				lum = -lgtdir.dotproduct(norm1);				
				lstipp1[ii] = lstiipp1 = pp;
			}
				
			real alpha2d = blend2d(0, 1, real(i) / len, real(ii) / wid);
			real deta2d = blend2d(0, 1, 0.5 + 0.5 * sin(32*PI*real(i) / len), 0.5 + 0.5 * cos(32*PI*real(ii) / wid));
			
			int cor;
			{// color				
				cor = blendcor(RGB(216, 45, 14), RGB(253, 186, 79), deta2d);
				//cor = blendcor(RGB(222, 85, 30), cor,real(i) / len, .25);
				cor = blendcor(0xFF101010, cor, lum, 1);				
						

			}
			//if(i > 0.18 * len && ii > 0)
			if(ii > 0)
			{
				//pixel(pp, cor, 1, pp.z);
				int glasscor = blendcor(0xFFFFFFFF, 0xFFFCFCFC, alpha2d);

				cor = blendcor(cor, glasscor, lum, 8);
				pixel(pp + norm1 * 0.001, cor, 1, pp.z);
			}			
		}	
	}
	
	goldenfish_tail2(lstipp1, v, 6000, wid);

}
// -------------------------------------------------
void goldenfish_head6(vec3 p0, vec3 v0, int len0, int wid)
{	
	real scale = 0.4;
	vec3 lgtdir(1, -1, 1); lgtdir.normalise();
	int cor1 = 0xFF202020, cor2 = RGB(246, 100, 14);
	
	v0.normalise();
	
	vec3 ax = vector3::UY.crossproduct(v0);ax.normalise();
	
	// border
	vec3 v12 = v0.rotcopy(-PI * 0.15, ax);
	vec3 p1 = p0 - v0 * 0.15 * scale - v12 * 0.05 * scale;
	vec3 p2 = p0 + v12 * 0.2 * scale;
	vec3 n12 = (p2 - p1).rotcopy(PI / 2, ax);n12.normalise();

	vec3 pp, lstiipp1, lstiipp2, lstipp1[8000], lstipp2[8000];
	for(int i = 0; i < wid; i += 1)	
	{
		real pos1 = real(i) / wid;
		real k = 4.5 * 0.002 * scale * blend(0, 1, pos1, 1);
		
		for(int ii = 0; ii < len0; ii ++)
		{				
			real pos2 = real(ii) / len0;
			real ang = blend(0, 4.5 * PI, pos2, 1);
			
			//thickness
			vec3 dv = ax * (0.5 * scale * blend2(0.15, 0.1, pos1, 4) * blend(0.1, 1.0, pos2, 4));			
			{// spiral
				real power = blend(0.75, 1.3, pos2, 4);				
				real r = k * pow(ang, power);			
				pp = p0 + v0.rotcopy(ang, ax) * r + dv;
			}
			{// face
				vec3 pt = blendvec3(p2, p1, pos1);
				vec3 dp12 = n12 * blend2(0, 0.025 * scale, pos1, 2);
				pt = pt - dp12 + dv;			
				pp = blendvec3(pp, pt - dp12, pos2, 40);
			}
			{// face curl
				vec3 dp0 = pp - p0;
				real prjv0 = dp0.dotproduct(n12.rotcopy(blend(0.0, 0.2, pos1) * PI));			
				pp = pp + ax * (prjv0 > 0 ? blend(0.0, 2.4, prjv0 / (1.5 * scale), 2) : blend(0.0, 4.4, -prjv0 / (1.0 * scale), 4) );			
			}
			{// color				
				vec3 norm1 = -(lstipp1[ii] - pp).crossproduct(lstiipp1 - pp);norm1.normalise();				
				real lum = -lgtdir.dotproduct(norm1);				
				lstipp1[ii] = lstiipp1 = pp;
				
				real alpha2d = blend2d(0, 1, real(ii) / len0, real(i) / wid);
				real deta2d = blend2d(0, 1, 0.5 + 0.5 * sin(16*PI*real(ii) / len0), 0.5 + 0.5 * cos(8*PI*real(i) / wid));
			
				int cor = blendcor(RGB(216, 45, 14), RGB(253, 186, 79), deta2d);
				cor = blendcor(0xFF101010, cor, lum, 1);	
	
				int glasscor = blendcor(0xFFFFFFFF, 0xFFFCFCFC, alpha2d);
				cor = blendcor(cor, glasscor, lum, 8);
				
				pixel(pp, cor, 1, pp.z);
			}
		}
	}
}
void draw() 
{		
	vec3 v(0.5, 0.05, -0.5);
	goldenfish_body4(vec3(0.5, 0.5, 0.5), v, 2000);	
	goldenfish_head6(vec3(0.36, 0.46, 0.5) + v * 0.35, vec3(0.8, -0.28, -0.35), 8000, 1000);
}
