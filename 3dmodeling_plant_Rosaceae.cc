/*
    3dmodeling->plant->Rosaceae
*/
// --------------------------------------------------------
// 盆栽 - 月季(rose)
// --------------------------------------------------------
int rose_len1 = 12;
int rose_len2 = 8;
real rose_ang = PI / 2;
void rose_flower(vec o, vec v, vec n, real r0, real w ,real ang0, int cor1)
{
	gcommonvertex = 1;
	binvnorm = true;
	r0 *= rrnd(0.1, 0.4);
	real vvf = rrnd(0.1, 0.15);
	vec ta = v.cross(n); ta.norm();
	real h = w * 2;
	vec lp[100]; // 足够多
	int len = 8;
	int len2 = rose_len2 % 100;
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI / 2, ai) + ang0;
		real r = r0;
		vec vr = (n * cos(ang) + ta * sin(ang));
		vec p = o + vr * r;
		vec vv = blend(vr, v, vvf + rrnd(-0.05, 0.05));

		real h1 = blend2(h * 0.5, h * 0.75, ai, 2);		
		for (int ii = 0; ii < len2; ii++)
		{
			real aii = ii / real(len2);
			vec tta = vv.cross(vr).normcopy();
			vr = tta.cross(vv); vr.norm();
			vv.rot(blend(-PI / 3.8, rose_ang, aii, 0.75), tta);

			vec up = p + vv * blend(h1, h1 / 8, aii, 0.5);
			if (i > 0)
			{
				color = blendcor(0xFF000020, cor1, ai);
				plane(lp[ii], p, up, lp[ii + 1]);
			}
			lp[ii] = p;
			if(ii == len2 - 1)
				lp[ii + 1] = up;
			p = up;			
		}
	}
	binvnorm = false;
	gcommonvertex = 0;
}

void re_leave(vec o, vec v, vec n, real len, real w)
{
	real t = blend(0.1, 1, gtimealpha);
	color = blendcor(RGB(10, 50, 0), 0xFF00A000, rrnd(0, 1));
	gcommonvertex = 1;
	vec vside = v.cross(n); vside.norm();
	vec p1 = o - vside * (w / 32);
	vec p2 = o + vside * (w / 32);
	real h = w / 200;
	real tk = h / 5;
	VECLIST e;
	e.push_back(o - vside * (w / 180));
	e.push_back(p1 + n * h);
	e.push_back(o - n * tk);
	e.push_back(p2 + n * h);
	e.push_back(o + vside * (w / 180));
	closeedge(e);

	VECLIST le = e;
	real step = w / 4;
	int cnt = 6;
	for (int i = 0; i < cnt; i++)
	{
		real ai = i / real(cnt);
		static real scales[] = { 1, 4, 2, 1.0, 0.5, 0.1 };
		extrudeedge(e, step * 0.75);
		scaleedge(e, scales[i]);
		rotedge(e, rrnd(-PI / 16, PI / 8), vside);
		{
			int cor0 = color;
			for (int ii = 0; ii < le.size() - 1; ii++)
			{
				plane(le[ii].p, le[ii + 1].p, e[ii + 1].p, e[ii].p);
			}
			color = cor0;
		}
		le = e;
	}
	gcommonvertex = 0;
}
int rose_cor;
void rose(vec o, vec v0, real sz, VECLIST e, int depth)
{
	int cor1 = rose_cor;
	color = 1;
	vec p = o;
	real s = rrnd(3, 4) * USIZE / 2.5 * sz;
	real dpdeta = rrnd(0, 172.862);
	real y0 = rrnd(0, 100);
	int len = depth == 0 ? (18 + parm_flowerrad) : 8;

	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		vec np;
		{
			vec dp = vec::UX.rotcopy(rrnd(-PI, PI), v0);
			np = p + (v0 + dp / 5) * (USIZE * blend(1, 3, ai, 4));
		}
		VECLIST e1;
		if (i == 0 && e.size() == 0)
		{// 底部
			color = 0xFF204020;
			roundedge(e1, p, vec::UY, s, 3);
		}
		else if (i <= len)
		{// 身体
			color = 0xFF204020;
			real dlen = (np - p).len();
			vec v = (np - p).normcopy();
			if(i % 2 == 0)
			{				
				moveedge(e, v * (dlen), e1);
				scaleedge(e1, 0.95);
				rotedge(e1, rrnd(0, PI / 8), getedgenorm(e1));
			}
			else
			{
				vec oo = getedgecenter(e);
				real ss = 2 * (oo - e[0]).len();
				moveedge(e, v * (ss), e1);

				if (i > len * 0.1 && i < len * 0.75 && rrnd() < 0.5 && depth < 2)
				{// leave
					vec pp = getedgecenter(e1);
					real s2 = rrnd(0.5, 1) * USIZE;
					re_leave(pp, blend(v, vec::UX, 0.5).rotcopy(rrnd(-PI, PI), v), vec::UY, s2 * 9, s2 * 5);
					color = 0xFFFFFFFF;
				}
				if(i > len * 0.1 && i < len * 0.9 && rrnd() < 0.1 && depth < 2)
				{// 分支
					VECLIST ee;
					unionedge(e, e1, rand() % 3, 2, ee); closeedge(ee);
					scaleedgeex(ee, 0.8);
					rose(getedgecenter(ee), blend(v, getedgenorm(ee), rrnd(0.4, 0.6)), sz * rrnd(0.4, 0.8), ee, depth + 1);
				}
			}
			if (i == len && (depth == 0 || rrnd() < 0.1))
			{				
				vec pp = getedgecenter(e1);
				vec vv = e1[0] - pp; vv.norm();
				real ss = rrnd(8, 12) * sz;
				int len2 = rose_len1;
				for (int ii = 0; ii < len2; ii++)
				{
                    real tang = ii % 5;
                    tang *= 2 * PI / 5;
					rose_flower(pp, v, (vv + v * rrnd(-0.5, 0.35)).normcopy(), USIZE * ss, USIZE * ss, tang + rrnd(-PI, PI) / 15, cor1);
				}				
			}
			color = 0xFF204020;
		}	

		if (depth > 0 || i != 0)
			face(e, e1);
		e = e1;
		p = np;
	}
	triang(e[0], e[1], e[2]);
}

// --------------------------------------------------------
// 山茶花叶子
// --------------------------------------------------------
void leafunit(VECLIST& e, real fw0, crvec v, vec up, real sz, int len, real fang, real fang2, real ang3, real pm1, real pm2, real pm3, real pm4)
{
	gcommonvertex = 1;
	vec forward = v;
	vec right = up.cross(forward).normcopy();
	up = forward.cross(right).normcopy();

	float w = fw0 / 2 * clamp(0.5, 4, 12, 5);
	VECLIST le = e;
	for (int ii = 0; ii <= len; ii++)
	{
		real aii = ii / real(len);
		if (g_leafunit_yemai)
			color = blendcor(RGB(100, 150, 100), gcurcor, 0.5 + 0.5 * sin((len / 2)* PI*aii));
		else
			color = blendcor(RGB(150, 180, 150), gcurcor, aii, 0.5);
		if (ii < len * 0.4)
			SETPARM(10);
		else if (ii > len * 0.6)
			SETPARM(11);
		else
			SETPARM(12);
		{
			real step[] = { 15 * USIZE, 15 * USIZE, 15 * USIZE, 15 * USIZE, 15 * USIZE };

			real fw = fw0 * (1 - aii) + w * blend(1, pm3, aii, pm1) * blend3(0.1, pm4, aii, 0.35, pm2); //blend(pm4, 0.5, aii, pm2);

			forward.rot(fang * (1 - aii), right);
			up.rot(fang2, forward);
			right = up.cross(forward).normcopy();

			real ang = blend(-ang3, ang3, aii);
			vec left1 = right.rotcopy(-ang, up);
			vec right1 = right.rotcopy(ang, up);

			up = forward.cross(right).normcopy();

			vec p = e[2].p;
			e.clear();

			e.push_back(p + forward * step[0] - left1 * (fw)+v * (fw));
			e.push_back(p + forward * step[1] - left1 * (fw / 4) + v * (fw * lerp(0.8, -0.25, aii)) + up * (-fw / 8));
			e.push_back(p + forward * step[2]);
			e.push_back(p + forward * step[3] + right1 * (fw / 4) + v * (fw * lerp(0.8, -0.25, aii)) + up * (-fw / 8));
			e.push_back(p + forward * step[4] + right1 * (fw)+v * (fw));
		}
		{
			int color0 = color;
			for (int i = 0; i < int(le.size() - 1); i++)
			{
				color = color0;
				if (i == le.size() / 2)
					color = RGB(100, 150, 100);
				plane(le[i], le[i + 1], e[i + 1], e[i]);
			}
		}
		le = e;
	}
	gcommonvertex = 0;
}
void leafunit(vec o, vec v, real sz, int len, real fang, real fang2, real ang3, real pm1, real pm2, real pm3, real pm4)
{
	vec forward = v;
	vec up = vec::UY;
	vec right = -up.cross(forward).normcopy();
	up = forward.cross(right).normcopy();

	VECLIST e;
	float w = sz / 10;
	e.push_back(o - right * (w));
	e.push_back(o - right * (w / 4) + up * (w / 4));
	e.push_back(o);
	e.push_back(o + right * (w / 4) + up * (w / 4));
	e.push_back(o + right * w);

	leafunit(e, w, v, up, sz, len, fang, fang2, ang3, pm1, pm2, pm3, pm4);
}

// --------------------------------------------------------------------
void shanchaleave(vec o, vec uy = vec::UY, real s = 0.1, int len0 = 20, int depth = 0)
{
	vec p = o;
	VECLIST e;
	color = 0xff101010;
	roundedge(e, o - vec::UY * (s), uy, s, depth == 0 ? 5 : 3, 0);
	bool lstgcommonvertex = gcommonvertex;
	gcommonvertex = 1;
	int cnt = 0;
	int cnt1 = 0;
	real dpdeta = rrnd(0, 172.862);
	real dpdeta2 = rrnd(0, 172.862);
	{// jing
		int len = depth == 0 ? clamp(3, 20, 10, 3) * 3 : len0;
		for (int i = 0; i < len; i++)
		{
			real ai = real(i) / (len);
			real t = blend(0.0, 1, ai);
			vec dp = vec::UX * ((perlin(p.x * 0.75, p.y * 0.05 + dpdeta, p.z * 0.3))) + vec::UZ * ((perlin(p.x * 0.2, p.y * 0.025 + dpdeta2, p.z * 0.5)));
			vec np = p;
			if (depth == 0)
				np += (uy + dp) * (USIZE * blend(50, 20, ai));
			else
				np += (uy + dp * 0.5) * (USIZE * 55);
			{
				color = blendcor(0xFF102000, 0xFFFFFFFF, ai, 2);
				SETPARM(10);
				VECLIST e1;
				{
					vec dv = np - p;
					moveedge(e, dv, e1);
					scaleedge(e1, 0.99);
				}
				face(e, e1);
				e = e1;
			}
			if (depth < 4 && i > 0.15 * len && ((depth == 0 && rand() % 5 == 0) || (depth == 1 && rand() % 10 == 0) || (depth == 2 && rand() % 30 == 0) || (depth == 3 && rand() % 25 == 0)))
			{
				vec pp = getedgecenter(e);
				if (depth == 0)
				{
					vec v = (np - p).normcopy();
					v = blend(v, vec::UX.rotcpy(cnt * PI / 2, vec::UY), blend(0.6, 0.25, (ai - 0.5) / 0.5));
					cnt++;
					//shanchaleave(pp, v, s * rrnd(0.4, 0.6), MIN(len, 10) / 2, depth + 1);
					shanchaleave(pp, v, s * rrnd(0.4, 0.6), MIN(len * rrnd(0.25, 1.5), 10), depth + 1);
				}
				else
				{
					for (int ii = 0; ii < 4; ii++)
					{
						vec v = (np - p).normcopy();
						v = blend(v, vec::UX.rotcpy(cnt * PI / 2, vec::UY), blend(0.6, 0.25, (ai - 0.5) / 0.5));
						cnt++;
						shanchaleave(pp, v, s * rrnd(0.4, 0.6), len * blend(1.0, 0.25, ai), depth + 1);
					}
				}
			}
			else if (i == len - 1)
			{
				vec v = (np - p).normcopy();
				real ang = rrnd(-PI, PI) / 20;
				vec pp = getedgecenter(e);
				for (int ii = 0; ii < 1; ii++)
				{
					v.rot(ang + PI * ii, uy);
					//g_leafunit_yemai = 1;
					leafunit(pp, v, 0.5, clamp(3, 30, 11, 2), rrnd(-0.1, 0.1), rrnd(-0.1, 0.1), rrnd(-0.1, 0.1),
						1.1, 2.02,
						1.15, 8);
					//g_leafunit_yemai = 0;
				}
			}
			p = np;
		}
	}
	gcommonvertex = lstgcommonvertex;
}
