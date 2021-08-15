/*
    etart->plant->rosaceae
*/
// --------------------------------------------------------
// 梅花
// --------------------------------------------------------
int meihuabancor = 0xFF00FFFF;
bool huarui = 0;
real curveang = 1.0;
real gwid = 0.007;
void meihuaban(vec p, vec v, vec3 up, real sz0 = 1, real rot0 = 1)
{	
	static vec3 lgt(-1, 1, -1); lgt.norm();
	v.norm();
	up.norm();
	vec3 lowerpp1[200];
	vec3 lowerpp2[200];
	vec3 lstpp1;
	vec3 lstpp2;
	real ang0 = rrnd(0.012, 0.022) * curveang;
	vec vv;
	int len = huarui ? rrnd(50, 70) * sz0 : 100 * sz0;
	real galpha = blend(1.2, 0.025, p.z / 2.5, 4);
	real ruifactor = rrnd(1.5, 2);
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		p = p + v * 0.0003;
		if (i <= 2)
		{
			vv = up.cross(v); vv.norm();
		}
		else
		{
			up = v.cross(vv); up.norm();
			vv = up.cross(v); vv.norm();
		}
		//real ang = ang0 * blend(-1, 1, ai) * blend(0.1, 4, ai);
		//if (huarui)
			//ang = ang0 * ruifactor;
		//v.rot(-ang * rot0, vv);

		vec3 vv1 = vv;// blend(up, vv, 0.5 + ai); vv1.norm();
		vec3 vv2 = -vv;// blend(up, -vv, 0.5 + ai); vv2.norm();

		int wid = 50;
		real rwid = (2 * sz0 * gwid) * blend3(0, 1, ai, 0.6, 3);
		real rot1 = blend(0.001, 0.01, ai, 4);
		for (int ii = 0; ii < wid; ii++)
		{
			real aii = ii / real(wid);
			{
				{
					vv1.rot(blend(0.01, 0.0025, ai), v);
					vec3 pp1 = p + vv1 * blend(0, rwid, aii);

					vec3 normal = (lstpp1 - pp1).cross(lowerpp1[ii] - pp1); normal.norm();
					real ndot = abs(normal.dot(lgt)) * 1.25;

					int cor1 = blendcor(blendcor(0xFFFFFFFF, meihuabancor, ndot), 0xFFFFFFFF, ndot, 8);
					if (rrnd(0, 100) < galpha * 60)
						point(pp1, rrnd(1, 4), cor1);
					lstpp1 = lowerpp1[ii] = pp1;
				}
				{
					vv2.rot(-blend(0.01, 0.0025, ai), v);
					vec3 pp2 = p + vv2 * blend(0, rwid, aii);

					vec3 normal = (lstpp2 - pp2).cross(lowerpp2[ii] - pp2); normal.norm();
					real ndot = abs(normal.dot(lgt)) * 1.25;

					int cor1 = blendcor(blendcor(0xFFFFFFFF, meihuabancor, ndot), 0xFFFFFFFF, ndot, 8);
					if (rrnd(0, 100) < galpha * 60)
						point(pp2, rrnd(1, 4), cor1);
					lstpp2 = lowerpp2[ii] = pp2;
				}
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------
void meihuarui(vec p, vec v, vec up, int len)
{
	v.norm();
	vec vx, vy;
	v2vxvy(v, vx, vy);

	real dis = len / FIMAGESCALE / 3;
	vec cp[] = { 
		p,
		p + blend(v, (vx * rrnd(-1, 1), vy * rrnd(-1, 1)).normcopy(), rrnd(0, 0.2)) * (dis * 1),
		p + blend(v, (vx * rrnd(-1, 1), vy * rrnd(-1, 1)).normcopy(), rrnd(0, 0.2)) * (dis * 2),
		p + blend(blend(v, up, rrnd(0.0, 0.05)), (vx * rrnd(-1, 1), vy * rrnd(-1, 1)).normcopy(), rrnd(0, 0.1)) * (dis * 3),
	};	
		
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		p = bezier3(cp, ai);		
		point(p, 2, blendcor(1, 0xFFFFFFFF, ai, 1));
	}
	point(p + v * rrnd(0.003, 0.004), 4, blendcor(0xFF00CCFF, 0xFF00aaaa, rrnd(0, 1)));
}
// ------------------------------------------------------------------------------------------------
void meihua(vec o, vec up)
{
	up.norm();
	vec vv, vx;
	v2vxvy(up, vx, vv);

	// 托底
	for (int i = 0; i < 5; i++)
		point(o + vx * rrnd(-0.001, 0.001) + vv * rrnd(-0.001, 0.001), rrnd(4, 16), blendcor(1, 0xFF0000a0, rrnd(0, 1)));

	real sz = rrnd(0.5, 1);
	real rot0 = rrnd(0.8, 1);
	//if(rand() % 100 < 40)
	//	rot0 = rrnd(5, 8);
	meihuabancor = RGB(rrnd(150, 255), rrnd(150, 150), rrnd(150, 200));

	for (int i = 0; i < 10; i++)
	{
		vv.rot(2 * PI / 10, up);
		meihuarui(o, blend(up, vv, rrnd(0.4, 0.6)), up, sz * 50 * 0.5);
	}

	for (int i = 0; i < 5; i++)
	{
		vv.rot(2 * PI / 5, up);
		meihuaban(o + vv * (sz * 0.005), blend(up, vv, rrnd(0.7, 0.8)), up, sz * rrnd(0.35, 0.4), rot0 * rrnd(0.75, 1.2));
	}	
}
// ------------------------------------------------------------------------------------------------
void meiguduo(vec o, vec up)
{
	meihuabancor = RGB(rrnd(150, 255), rrnd(150, 150), rrnd(150, 200));
	static vec3 lgt(-1, 1, -1); lgt.norm();
	vec3 lstpp, lowerpp[250];
	up.norm();
	vec dx, dy;
	v2vxvy(up, dx, dy);

	real sz = rrnd(0.05, 0.1);
	int len = 50;
	vec p = o;
	int cor = blendcor(0xFF000020, 0xFF808080, rrnd(0, 1));
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		p = p + up * (sz * 0.0002);
		real r = sz * 0.015 * CircleBlend2(0, 1, ai);
		int wid = 6 * r * IMAGESCALE;
		for (int ii = 0; ii <= wid; ii++)
		{
			real aii = ii / real(wid);
			real ang = blend(0, 2 * PI, aii);
			vec dp = dx * cos(ang) + dy * sin(ang);
			
			vec pp = p + dp * r;
			real dr = 0;// r * 0.1 + r * perlin(pp.x * 20, pp.y * 20, pp.z * 20);
			pp = p + dp * (r + dr);
			if (i > 0 && ii > 0)
			{
				//vec3 normal = (lstpp - pp).cross(lowerpp[ii % 250] - pp); normal.norm();
				//real ndot = (normal.dot(lgt));
				//int cor1 = blendcor(1, meihuabancor, ndot, 4);
				//pixel(pp, blendcor(cor1, 0xFFFFFFFF, ai, 4));
				pixel(pp, cor);
			}
			lstpp = lowerpp[ii % 250] = pp;
		}
	}

}
// ------------------------------------------------------------------------------------------------
void meizhi(vec p, vec v, vec3* lstlowerpp = 0, int len0 = 1000, real r00 = 0.04, int depth = 0)
{
	static vec3 lgt(-1, 1, -1); lgt.norm();
	vec3 lstpp, lowerpp[1000];
	if (lstlowerpp)
	{
		for (int i = 0; i < 1000; i++)
		{
			lowerpp[i] = lstlowerpp[i];
		}
	}
	vec3 tv = v, vv;
	vec3 uz = vec3::UY;
	int len = len0;
	int step = 0;
	int step0 = 0;
	float vdeta = 0.0002 * blend(0.7, 4, depth / 5, 0.5);	
	float dr = 0, vdr = 0, adr = 0;
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		{// 走势
			if (depth == 0)
			{
				v = v + vec(rrnd(-.5, .5), blend(0, 0.1, ai, 4), rrnd(-.5, .5)) * 0.5;
				v.norm();
			}
			else if (depth < 2)
			{
				v = v + vec(rrnd(-.5, .5), blend(0, 0.1, ai, 4), rrnd(-.5, .5)) * 0.1;
				v.norm();
			}
			else if (i % (1 + len / 50) == 0)
			{
				//v = v + vec(rrnd(-1, 1), blend(0, 4, ai, 4), rrnd(-1, 1)) * 0.1;
				v = v + vec(rrnd(-.1, .1), blend(0, 4, ai, 4), rrnd(-.1, .1)) * 0.1;
				v.norm();
			}
		}

		v.norm();
		tv = blend(tv, v, 0.025);
		p = p + tv * vdeta;

		vv = v.cross(uz); vv.norm();
		uz = vv.cross(v); uz.norm();

		real r0 = depth < 2 ? blend(r00, r00 / 1.5, ai, 4) : blend(r00, r00 / 2, ai, 2);
		//if (depth > 2)
		//{
		//	adr = rrnd(-1, 1) * 0.00001;
		//	vdr += adr;
		//	if (dr < 0.0005)
		//		dr += vdr;
		//	if(r0 + dr > 0.00025)
		//		r0 += dr;			
		//}
		if(depth < 7)
		{// 分支			
			if (r0 > 0.001 &&
				(depth <= 2 && (i == len - 1))
				)
			{// 主枝干
				vec nv = blend(v, vv, rrnd(0.5, 0.9));
				if (rrnd(0, 100) < 70)	meizhi(p, nv, lowerpp, len0 * rrnd(0.3, 0.9), r0 * rrnd(0.75, 1.0), depth + 1);
				nv = blend(v, vv, rrnd(0.5, 0.9));
				if (rrnd(0, 100) < 70)	meizhi(p, nv.rotcopy(PI * 2 / 3, v), lowerpp, len0 * rrnd(0.3, 0.9), r0 * rrnd(0.75, 1.0), depth + 1);
				nv = blend(v, vv, rrnd(0.5, 0.9));
				if (rrnd(0, 100) < 70)	meizhi(p, nv.rotcopy(PI * 2 * 2 / 3, v), lowerpp, len0 * rrnd(0.3, 0.9), r0 * rrnd(0.75, 1.0), depth + 1);
			}			
			else if (depth > 1 && (step ++) > step0)
			{
				if (step0 != 0)
				{// 小枝
					if (r0 > 0.0005 &&
						(depth >= 2 && depth < 6 && i > len / 4 && rrnd(0, 100) < 15)
						)
					{
						vec nv = blend(v, vv, rrnd(0.4, 0.6));// 分叉角度
						nv.rot(rrnd(0, PI * 2), v); nv.norm();
						meizhi(p, nv, lowerpp, len0 * blend(0.25, 0.9, rrnd(0, 1), 1), r0 * rrnd(0.4, 0.6), depth + 1);
						tv = blend(v, v - nv, rrnd(0.25, 0.4));// 作用角度
					}
					else if (r0 < 0.0025)
					{	
						vec nv = blend(v, vv, rrnd(0.7, 0.9));// 分叉角度
						nv.rot(rrnd(0, PI * 2), v);
						meizhi(p, blend(v, vv, rrnd(0.6, 1)).rotcopy(rrnd(0, PI * 2), v), lowerpp, blend(8, 50, rrnd(0, 1), 4), r0 * rrnd(0.4, 0.6), 8);
						tv = blend(v, nv, rrnd(0.025, 0.1));// 作用角度
					}
				}
				if(depth > 3)
					step0 = (10 + len / blend(50, 60, rrnd(0, 1)));		
				else
					step0 = (10 + len / blend(10, 20, rrnd(0, 1)));
				step = 0;
			}
		}
		else if (i == len - 1 && len > 20 && rrnd(0, 100) < 25)
		{
			vec vx, vy;
			v2vxvy(v, vx, vy);
			meihua(p, blend(v, -vec::UZ * 0.5 + blend(vx * rrnd(-1, 1), vy * rrnd(-1, 1), rrnd(0, 1)), rrnd(0.6, 1)));
		}
		else if(i == len - 1 && rrnd(0, 100) < 80)
		{
			vec vx, vy;
			v2vxvy(v, vx, vy);
			meiguduo(p, blend(v, blend(vx * rrnd(-1, 1), vy * rrnd(-1, 1), rrnd(0, 1)), rrnd(0, 0.5)));
		}
		
		{// 主干
			int wid = IMAGESCALE * r0 * 6;
			for (int j = 0; j < wid; j++)
			{
				real aj = j / real(wid);
				real ang = aj * 2 * PI;
				vec3 tvv = vv.rotcopy(ang, v);
				tvv.norm();

				real r = r0;// * rrnd(0.9, 1.1);
				vec3 pp = p + tvv * r;
				pp = p + tvv * (r + (r * 1) * RidgedMFex(pp.x * 150, pp.y * 25, pp.z * 150, 4, (r * 0.05)));
				static int cor1 = 1;
				if (pp.y > 0 && i > 0 && j > 0)
				{
					vec3 normal = (lstpp - pp).cross(lowerpp[j % 1000] - pp); normal.norm();
					real ndot = abs(normal.dot(lgt));
					//cor1 = blendcor(1, blendcor(0xFF000010, 0xFF40C0A0, ndot), ndot, 8);
					cor1 = blendcor(1, blendcor(0xFF000010, 0xFF80B0A0, ndot), ndot, 8);
					pixel(pp, cor1);					
				}
				if (depth > 0 && depth < 4 || i > 0)
				{// 自动连接 避免接缝
					int tlen = (lowerpp[j % 1000] - pp).len() * IMAGESCALE;
					if (tlen < 500 && tlen > 2)
					{
						for (int ii = 0; ii < tlen; ii++)
						{
							vec tp = blend(lowerpp[j % 1000], pp, real(ii) / tlen);
							if (tp.y > 0)
								pixel(tp, cor1);
						}
					}
				}
				lstpp = lowerpp[j % 1000] = pp;
			}
		}
	}
}
