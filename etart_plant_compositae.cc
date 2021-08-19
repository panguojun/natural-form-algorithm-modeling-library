
// ------------------------------------------------------------------------------------------------
int juhuabancor = 0xFF00FFFF;
int juhuabancor0 = 0xFF208080;
bool huarui = 0;
void juhuaban(vec p, vec v, vec3 up, real sz0 = 1, real rot0 = 1)
{	
	vec3 lgt(-1, 1, 1); lgt.norm();
	v.norm();
	up.norm();
	vec3 lowerpp1[200];
	vec3 lowerpp2[200];
	vec3 lstpp1;
	vec3 lstpp2;
	real ang0 = rrnd(0.012, 0.022);
	vec vv;
	int len = huarui ? rrnd(70, 150) * sz0 : 500 * sz0;
	real galpha = blend(1.2, 0.025, p.z / 2.5, 4);
	for(int i = 0; i < len; i ++)
	{
		
		real ai = i / real(len);
		p = p + v * 0.0005;
		if (i <= 2)
		{
			vv = up.cross(v); vv.norm();
		}
		else
		{
			up = v.cross(vv); up.norm();
			vv = up.cross(v); vv.norm();
		}
		real ang = ang0 * blend(-1, 1, ai) * blend(0.1, 4, ai);
		if(huarui)
			ang = ang0 * 2;
		v.rot(-ang * rot0, vv);

		vec3 vv1 = blend(up, vv, ai); vv1.norm();
		vec3 vv2 = blend(up, -vv, ai); vv2.norm();

		int wid = 200;
		real rwid = 0.01 * blend(0, 1, ai) * blend(1, 0, ai, 8);
		for(int ii = 0; ii < wid; ii ++)
		{
			real aii = ii / real(wid);			
			{
				{
					vv1.rot(0.01, v);
					vec3 pp1 = p + vv1 * (aii * rwid);

					vec3 normal = (lstpp1 - pp1).cross(lowerpp1[ii] - pp1); normal.norm();
					real ndot = abs(normal.dot(lgt)) * galpha;

					int cor1 = blendcor(blendcor(juhuabancor0, juhuabancor, ndot), 0xFFFFFFFF, ndot, 8);
					if(rrnd(0, 100) < galpha * 50)
						pixel(pp1, cor1);
					lstpp1 = lowerpp1[ii] = pp1;
				}
				{
					vv2.rot(-0.01, v);
					vec3 pp2 = p + vv2 * (aii * rwid);

					vec3 normal = (lstpp2 - pp2).cross(lowerpp2[ii] - pp2); normal.norm();
					real ndot = abs(normal.dot(lgt)) * galpha;

					int cor1 = blendcor(blendcor(juhuabancor0, juhuabancor, ndot), 0xFFFFFFFF, ndot, 8);
					if(rrnd(0, 100) < galpha * 50)
						pixel(pp2, cor1);
					lstpp2 = lowerpp2[ii] = pp2;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------
void juhua(vec o)
{
	vec up(0.1, 1, -0.5);up.norm();
	vec vv(0.5, 0.5, 0.5);vv.norm();
	real sz = rrnd(0.5, 1);
	real rot0 = rrnd(0.8, 1);
	//if(rand() % 100 < 40)
	//	rot0 = rrnd(5, 8);
	juhuabancor = blendcor(RGB(255, rrnd(100, 255), rrnd(0, 255)), 0xFFFFFFFF, rrnd(0, 1));
	juhuabancor0 = blendcor(RGB(255, rrnd(100, 255), rrnd(0, 255)), 0xFF808080, rrnd(0, 1));

	for(int i = 0; i < 28; i ++)
	{
		vv.rot(2 * PI / 28, up);
		juhuaban(o, vv, up, sz * rrnd(0.5, 1.2), rot0 * rrnd(0.75, 1.2));
	}
}

// ------------------------------------------------------------------------------------------------
void jufuleave(vec p, vec v, vec3 up, real sz0 = 1)
{
	float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	
	vec3 lgt(-1, 1, -1); lgt.norm();
	v.norm();
	up.norm();
	vec3 lowerpp1[400];
	vec3 lowerpp2[400];
	vec3 lstpp1;
	vec3 lstpp2;
	real ang0 = rrnd(0.001, 0.005);
	vec vv;
	int len = 800;
	real vg = rrnd(0.8, 1.2);
	int cor00 = blendcor(0xFF008040, 0xFF00A000, rrnd(0, 1));
	cor00 = blendcor(1, cor00, rrnd(0.6, 1));
	real rot00 = rrnd(0.0025, 0.01);
	real rot11 = rrnd(0.004, 0.006);
	for(int i = 0; i < len; i ++)
	{
		real galpha = blend(1, 0.0, p.z / 2.5, 4);
		real ai = i / real(len);
		//v = v + vec(0,-0.003 * vg,0);v.norm();
		v.rot(blend(0.0, rot11, ai), vv);
		p = p + v * (0.00022 * sz0);
		if (i <= 2)
		{
			vv = up.cross(v); vv.norm();
		}
		else
		{
			up = v.cross(vv); up.norm();
			vv = up.cross(v); vv.norm();
		}

		real ftui = FTU(PI * 6 * ai, s_t0);
		vec3 vv1 = blend(up, blend(vv, v, blend(0.4, 0.6, ai), 2), 0.75 + 0.1 * ftui); vv1.norm();
		vec3 vv2 = blend(up, blend(-vv, v, blend(0.4, 0.6, ai), 2), 0.75 + 0.1 * ftui); vv2.norm();

		int wid = 200;
		real rwid = 0.05 * blend3(0, 1, ai, 0.4, 2) * blend(0.25, 1, ai, 2) + blend2(0, 0.05, ai, 2) * ftui;
		rwid *= sz0;
		for(int ii = 0; ii < wid; ii ++)
		{
			real aii = ii / real(wid);		
			
			{
				int cor0 = blendcor(1, cor00, 1 * ftui, 0.1);
				{
					vv1.rot(-blend(0, rot00, aii), v);
					vv1 = blend(vv1, v, 0.0005);
					vec3 pp1 = p + vv1 * (aii * rwid);

					vec3 normal = (lstpp1 - pp1).cross(lowerpp1[ii] - pp1); normal.norm();
					real ndot = abs(normal.dot(lgt)) * galpha;
					
					int cor1 = blendcor(blendcor(1, cor0, ndot), 0xFFFFFFFF, ndot, 8);
					if(rrnd(0, 100) < blend(galpha, 0, aii, 4) * 100)
						pixel(pp1, cor1, galpha);

					lstpp1 = lowerpp1[ii] = pp1;
				}
				{
					vv2.rot(blend(0, rot00, aii), v);
					vv2 = blend(vv2, v, 0.0005);
					vec3 pp2 = p + vv2 * (aii * rwid);

					vec3 normal = (lstpp2 - pp2).cross(lowerpp2[ii] - pp2); normal.norm();
					real ndot = abs(normal.dot(lgt)) * galpha;
					
					int cor1 = blendcor(blendcor(1, cor0, ndot), 0xFFFFFFFF, ndot, 8);
					if(rrnd(0, 100) < blend(galpha, 0, aii, 4) * 100)
						pixel(pp2, cor1, galpha);
	
					lstpp2 = lowerpp2[ii] = pp2;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------
void juzhi(vec p, vec v, int len0 = 1000, real r00 = 0.005, int depth = 0)
{
	float s_t1[]  = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	vec3 lgt(-1, 1, -1); lgt.norm();
	vec3 vv;
	vec3 uz = vec3::UY;
	int len = len0;
	int cor00 = blendcor(0xFF204020, 0xFF608060, rrnd(0, 1));
	
	for(int i = 0; i < len; i ++)
	{
		real galpha = blend(1, .1, p.z / 2.5);
		
		real ai = i / real(len);
		if(depth == 0)
		{
		//	v = v + vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)) * 0.05; v.norm();
			real ft2d = FTU(4 * PI * ai, s_t1);
			v.rot(-blend(-PI, PI, ft2d) * 0.005, vec::UZ);
			v = v + vec(rrnd(-.1, .1), blend(0.1, 0.75, ai, 4), rrnd(-.1, .1)) * 0.05;
		//	v = v + vec3(rrnd(-1, 1), rrnd(-1, 1.01), rrnd(-1, 1)) * 0.04;
		}
		else
		{
			real ft2d = FTU(4 * PI * ai, s_t1);
			v.rot(-blend(-PI, PI, ft2d) * 0.005, vec::UZ);
			v = v + vec(rrnd(-1, 1), blend(0.1, 1, ai, 4), rrnd(-1, 1)) * 0.01;
			//v = v + vec3(rrnd(-1, 1), rrnd(-1, 1.01), rrnd(-1, 1)) * 0.12;
		}

		v.norm();
		p = p + v * 0.0005;

		vv = v.cross(uz); vv.norm();
		uz = vv.cross(v); uz.norm();

		if(depth < 2 && i % 40 == 0 && rand() % 500 < 50)
		{
			real ssz0 = blend(1.3, 1, ai) * rrnd(0.9, 1.4);
			if(depth > 0)
				ssz0 *= 0.7;
			vec nv = blend(v, vv, rrnd(0.3, 0.7));		
			
			real angm = rrnd(0, PI * 2);

			jufuleave(p, nv.rotcopy(angm, v), vec3::UY, ssz0 * rrnd(0.8, 1.1));
		}
		real r0 = blend(r00, r00 / 2, ai, 4);
		if(depth < 2 && i == len - 1)
		{
			vec nv = blend(v, vv, rrnd(0.6, 0.85));
			if(rrnd(0, 100) < 60)	juzhi(p, nv, 							len0 * rrnd(0.4, 0.8), r0 * rrnd(0.75, 1.0), depth + 1);
			if(rrnd(0, 100) < 60)	juzhi(p, nv.rotcopy(PI * 2 / 3, v), 	len0 * rrnd(0.4, 0.8), r0 * rrnd(0.75, 1.0), depth + 1);
			if(rrnd(0, 100) < 60)	juzhi(p, nv.rotcopy(PI * 2 * 2 / 3, v), len0 * rrnd(0.4, 0.8), r0 * rrnd(0.75, 1.0), depth + 1);
		}
		else if(i == len - 1 && depth > 1)
		{
			huarui = p.y < 0.55 && rand() % 5 != 1;
			juhua(p);
		}

		static vec3 lowerpp[2000];
		static vec3 lstpp;
		int wid = 100;
		for(int j = 0; j < wid; j ++)
		{
			real aj = j / real(wid);
			real ang = aj * 2 * PI;
			vec3 tvv = vv.rotcopy(ang, v);
			tvv.norm();

			real r = r0 * rrnd(0.9, 1.1);
			vec3 pp = p + tvv * r;

			if (pp.y > 0 && i > 0 && j > 0 && rrnd(0, 100) < 100)
			{
				real ft2d = FT2D(12 * PI * ai, 18 * PI * aj);

				vec3 normal = (lstpp - pp).cross(lowerpp[j % 2000] - pp); normal.norm();
				real ndot = abs(normal.dot(lgt));
				int cor1 = blendcor(blendcor(1, 0xFF40C0A0, ndot * ft2d), 0xFFFFFFFF, ndot, 8);
				
				if(rrnd(0, 100) < galpha * 100)
					pixel(pp, blendcor(1, cor1, galpha * ai, 0.25));
			}
			lstpp = lowerpp[j % 1000] = pp;
		}
	}
}
