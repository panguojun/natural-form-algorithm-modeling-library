/*
	3d modeling->plant->monocotyledon(单子叶植物)
*/
// ------------------------------------------------
// 睡莲
// ------------------------------------------------
void Nymphaea_Leaf(vec o, real sz, vec up = vec::UY)
{
	vec vx, vy;
	v2vxvy(up, vx, vy);
	real ang0 = rrnd(0, PI);
	VECLIST e1;
	int len = 16;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI, ai);
		real r = blend(0, sz, ai, 0.25);
		real deta = (i ==0 || i == len) ? 0 : rrnd(0, sz / 8);
		e1.push_back(o + vx * (r * cos(ang + ang0)) + up * deta + vy * (r * sin(ang + ang0)));
	}
	closeedge(e1);
	face(e1);

	VECLIST e2;
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI, ai);
		real r = blend(0, sz, ai, 0.25);
		real deta = (i == 0 || i == len) ? 0 : rrnd(0, sz / 8);
		e2.push_back(o + vx * (r * cos(-ang + ang0)) + up * deta + vy * (r * sin(-ang + ang0)));
	}
	vec oo = e2[0];
	invedge(e2);
	closeedge(e2);	
	face(e2, oo);
}

// ------------------------------------------------
int Nymphaea_Flower_color0 = 0xFFB080FF;
int Nymphaea_Flower_color1 = 0xFF8040FF;
int Nymphaea_Flower_n = 4;
int Nymphaea_Flower_aa = 12;
int Nymphaea_Flower_an = 8;
void Nymphaea_Flower(vec o, real s)
{
	vec v0 = vec::UY;
	int n = Nymphaea_Flower_n;
	int aa = Nymphaea_Flower_aa;
	int an = Nymphaea_Flower_an;
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	real dpdeta = rrnd(0, 172.862);
	real dpdeta2 = rrnd(0, 172.862);
	vec p = o;
	int len = n + 8;
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec np;
		{
			vec dp = vec::UX * ((perlin(p.x * 1, p.y * 0.1 + dpdeta2, p.z * 1 + dpdeta))) + vec::UZ * ((perlin(p.x * 1 + dpdeta2, p.y * 0.15 + dpdeta, p.z * 1)));
			np = p + (v0 + dp * 0.58 - vec::UY * 0.1) * (USIZE * 2 * blend(1.8, 5, ai, 4) * 50 * s);
		}
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				roundedge(e1, p, vec::UY, 1.5 * s, aa);
			}
			else if (i < len)
			{// 茎
				color = 0xFF001000;
				real dlen = (np - p).len();
				vec gv = (np - p).normcopy();
				{
					moveedge(e, gv * (dlen), e1);
					scaleedge(e1, 0.995);
				}
				color = 0xFF008000;
			}
			else
			{// 花
				color = blendcor(Nymphaea_Flower_color0, Nymphaea_Flower_color1, rrnd());
				moveedge(e, (np - p) * 0.02, e1);
				//scaleedge(e1, 1.2);
				for (int ii = 0; ii < aa; ii++)
				{
					VECLIST ee;
					unionedge(e, e1, ii, 2, ee);
					closeedge(ee);
					vec v = blend(vec::UY, getedgenorm(ee), rrnd(0.25, 1));
					real cvg = rrnd(0.025, 0.75) * rrnd(0.25, 0.5);
					for (int jj = 0; jj < an; jj++)
					{
						real ajj = jj / real(an);
						v += vec::UY * blend(-0.0, cvg, ajj);
						v.norm();
						VECLIST ee1;
						moveedge(ee, v * (s * 5), ee1);
						float sc = blend(1.8, 0.1, ajj, 1.5);
						scaleedge(ee1, sc);
						face(ee, ee1);
						ee = ee1;
					}
				}
			}
			//if (i != 0)
			//	face(e, e1);
			e = e1;
		}
		p = np;
	}
	face(e, getedgecenter(e));
	color = 0xFFFFFFFF;
}

// -------------------------------------------------------------
// 棕榈树
// -------------------------------------------------------------
void triangleaf(vec o, vec v, vec n, real len, real w)
{
	vec vside = v.cross(n); vside.norm();
	triang0(o - vside * (w / 2), o + vside * (w / 2), o + v * len);
}
void PalmLeaf(vec p, vec v, real s, int len0 = 16)
{
	VECLIST e;
	v.norm();
	for (int i = 0; i < len0; i++)
	{
		vec np = p + v * (s * 18);
		v = v - vec::UY * 0.025;

		if (i == 0)
			roundedge(e, p, v, s, 3);
		else
		{// 茎	
			VECLIST e1;
			vec gv = (np - p);
			if (i < len0)
			{
				scaleedge(e, 0.8, e1);
				moveedge(e1, gv);
				face(e, e1);
				e = e1;
			}
		}
		if (i == len0 - 1)
		{// 叶子	
			color = blendcor(0xFF004000, 0xFF00AA00, rrnd());
			for (int ii = 0; ii < 18; ii++)
			{
				real aii = real(ii) / 18;
				vec nm = v.cross(vec::UY); nm.norm();
				vec vv = v.rotcopy(blend(-PI * 0.8, PI * 0.8, aii), nm);
				triangleaf(p, vv, rrnd() < 0.5 ? nm : -nm, s * blend2(18, 64, aii), s * 4.0);
			}
			color = 1;
		}
		p = np;
	}
}
// -------------------------------------------------------------
void PalmTree(vec o, vec v0, real s, int len0 = 24, int depth = 0)
{
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = len0;
	real dpdeta = rrnd(0, 12.862);
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);
		vec dp = vec::UX * ((-0.5 + perlin(p.x * 10, p.y * 10, p.z * 10 + dpdeta)) * 0.25) + vec::UZ * ((-0.5 + perlin(p.x * 10 + dpdeta, p.y * 10, p.z * 10)) * 0.25);
		vec np = p + (v0 + dp * 0.5 - vec::UY * 0.1) * (s * 10);
		{
			if (i == 0)
			{// 底部
				roundedge(e, p, vec::UY, s / E2, 6);
			}
			else
			{// 身体
				real dlen = (np - p).len();
				vec gv = (np - p);
				{// 茎	
					scaleedge(e, 0.995f);
					VECLIST e1;
					moveedge(e, gv, e1);

					VECLIST e2;
					scaleedge(e1, 1.2, e2);
					face(e, e2);
					e = e1;
				}
				if (i > 0.75 * len)
				{// 分支
					int num = rrnd(5, 8);
					for (int ii = 0; ii < num; ii++)
					{
						vec vv = (gv * rrnd(0.5, 1) + vec(rrnd(-1, 1), rrnd(0, 1), rrnd(-1, 1)).normcopy());
						vv.rot(rrnd(-PI, PI), gv.normcopy());
						PalmLeaf(p, vv, s * 0.5, len * rrnd(0.2, 0.5));
					}
				}
			}
		}
		p = np;
	}
}
// --------------------------------------------------------
// 绿萝
// --------------------------------------------------------
void greendill_leave(vec o, vec v, vec n, real len, real w)
{
	real t = blend(0.1, 1, gtimealpha);
	gcommonvertex = 1;
	vec vside = v.cross(n); vside.norm();
	vec p1 = o - vside * (w / 32);
	vec p2 = o + vside * (w / 32);
	real h = w / 100;
	real tk = h / 2.5;
	VECLIST e;
	e.push_back(o - vside * (w / 180));
	e.push_back(p1 + n * h);
	e.push_back(o - n * tk);
	e.push_back(p2 + n * h);
	e.push_back(o + vside * (w / 180));
	closeedge(e);

	VECLIST le = e;
	real step = w / 4;
	int cnt = 8;
	for (int i = 0; i < cnt; i++)
	{
		real ai = i / real(cnt);
		static real scales[] = { 4, 2, 1.1, 0.95, 0.8, 0.5, 0.25, 0.1 };
		extrudeedge(e, step * 0.75);
		scaleedge(e, scales[i]);
		rotedge(e, PI / 32, vside);
		{
			for (int ii = 0; ii < le.size() - 1; ii++)
			{
				plane(le[ii], le[ii + 1], e[ii + 1], e[ii]);
			}
		}
		le = e;
	}
	gcommonvertex = 0;
}
// 绿萝叶
real greendillleave_lvsz = 1;
int greendillleave_len = 20;
real greendillleave_ang = 0;
void greendillleave(vec o, real sz, real fw1, real fw2, int n0, int color0)
{
	color = color0;
	real t = blend(0.1, 1, gtimealpha);
	vec p = o;
	VECLIST e;
	real s = sz / 120 * t;
	int len = blend(2, greendillleave_len * gsize * 5, t);
	vec v0 = vec::UY;
	real dpdeta = rrnd(0, 172.862);
	vec bz_cp[] = {
		vec(rrnd(-.5, .5), rrnd(0.1, 1), rrnd(-.5, .5)),
		vec(rrnd(-.5, .5), rrnd(0.1, 1), rrnd(-.5, .5)),
		vec(rrnd(-.5, .5), rrnd(0.1, 1), rrnd(-.5, .5)),
		vec(rrnd(-.5, .5), rrnd(0.1, 1), rrnd(-.5, .5))
	};
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		vec np;
		{
			vec dp = bezier3(bz_cp, ai) * (s * blend(12, 15, ai, 4));;
			np = p + dp;
		}
		VECLIST e1;
		if (i == 0)
		{// 底部
			color = 0xFF00FFAA;
			roundedge(e1, p, vec::UY, s, 8);
		}
		else if (i < len)
		{// 身体
			color = blendcor(0xFF00FFAA, color0, ai, 4);
			real dlen = (np - p).len();
			vec gv = (np - p).normcopy();
			{
				moveedge(e, gv * (dlen), e1);
				scaleedge(e1, 0.995);
			}
		}
		else if (t > 0.5)
		{
			color = color0;
			vec up = (np - p).normcopy();
			vec vx, vy;	v2vxvy(up, vx, vy);
			vec vv0 = vx.rotcopy(rrnd(-PI, PI), up);
			vec vv = blend(-up, vv0, rrnd(-0.1, 0.1) + greendillleave_ang).normcopy();
			vec vside = vv.cross(vec::UY);
			vec nn = vside.cross(vv).normcopy();
			{			
				real scl = 0.5;
                
                {// eye
                    vec pp = getedgecenter(e);
                    real s2 = rrnd(0.5, 1) * USIZE;
                    eye_leave(pp, vec::UX, vec::UZ, s2 * 25, s2 * 15);
                    color = 0xFFFFFFFF;
                }
				//greendill_leave(getedgecenter(e), vv, nn, sz * 3 * greendillleave_lvsz * scl, sz * greendillleave_lvsz * scl);
			}
		}
		gcommonvertex = 1;
		if (i != 0)
			face(e, e1);
		gcommonvertex = 0;
		e = e1;
		p = np;
	}
}

// -------------------------------------------------------------
// 竹子-〉芦苇
// -------------------------------------------------------------
real bamboo_leavesize = 1;
int bamboocor = 0xFF00A000;
void bamboo(vec o, vec v0, real s, int len0 = 10, int depth = 0)
{
	color = bamboocor;
	s *= 0.5;
	DEF_SCALES2(s);
	VECLIST e;
	v0.norm();
	vec p = o;
	int len = len0;
	real dpdeta = rrnd(0, 172.862);
	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		real t = blend(0.0, 1, ai);

		vec np;
		if (depth == 1)
		{
			vec dp = -vec::UY * 0.1;
			v0 = (v0 + dp).normcopy();
			np = p + (v0)* (s * blend(20, 25, ai, 1));
		}
		else
		{
			vec dp = vec::UX * ((-0.5 + perlin(p.x * 10, p.y * 10, p.z * 10 + dpdeta)) * 0.25) + vec::UZ * ((-0.5 + perlin(p.x * 10 + dpdeta, p.y * 10, p.z * 10)) * 0.25);
			np = p + (v0 + dp - vec::UY * 0.1) * (s * blend(18, 100, ai, 4));
		}
		{
			VECLIST e1;
			if (i == 0)
			{// 底部
				color = bamboocor;
				roundedge(e1, p, vec::UY, s, 3);
			}
			else
			{// 身体
				color = bamboocor;
				real dlen = (np - p).len();
				vec gv = (np - p).normcopy();
				if (depth == 0)
				{// 竹节
					VECLIST e2;
					scaleedge(e, 1 / E, e2);
					moveedge(e2, gv * (USIZE * 0.25));
					face(e, e2); e = e2;
					scaleedge(e, E);
					moveedge(e, gv * (USIZE * 0.25));
					face(e2, e);
					moveedge(e, np - getedgecenter(e), e1);
				}
				else
				{
					moveedge(e, gv * (dlen), e1);
				}
				if (i == len)
				{// 顶部					
					if (depth == 0)
					{
						vec top = np + gv * (dlen * 0.5);
						face(e1, top);
						for (int ii = 0; ii < 18; ii++)
						{
							real aii = ii / 18.0;
							vec vv = blend(gv, vec(rrnd(-1, 1), rrnd(0.5, 1), rrnd(-1, 1)).normcopy(), 0.5);
							bamboo(blend(np, top, aii), vv.normcopy(), s * blend(0.5, 0.1, aii), blend(1, 0.5, aii) * 10, 1);
						}
					}
					else
					{
						gtargets.push_back(np);
					}
				}
				if (depth == 0 && i > len * 0.2)
				{// 叶子
					int ln = rrnd(2, 4);
					for (int ii = 0; ii < ln; ii++)
					{
						vec lvpp = p + vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)).normcopy() * (s * 2.5);
						vec lvv = blend(gv, -vec::UX, rrnd(0.65, 1)); lvv.norm();
						lvv.rot(rrnd(-PI / 4, PI / 4), gv);
						vec lvpp3 = lvpp + lvv * (s * rrnd(58, 108) * bamboo_leavesize);
						vec vside = lvv.cross(rrnd() < 0.5 ? -vec::UZ : vec::UZ); vside.norm();
						color = blendcor(0xFF005000, bamboocor, rrnd());
						triang0(lvpp - vside * (s * 3 * bamboo_leavesize), lvpp + vside * (s * 3 * bamboo_leavesize), lvpp3);
						color = bamboocor;
					}
				}
			}
			color = 0xFF00A0A0;
			if (i != 0)
				face(e, e1);
			e = e1;
		}
		p = np;
	}
	color = 0xFFFFFFFF;
}
// -------------------------------------------------------------
// 玉米
// -------------------------------------------------------------
void yumili(vec o, vec v, vec face, real sz)
{
	color = blendcor(0xff00a0ff, 0xff00ffff, rrnd());
	vec side = face.cross(v).normcopy();
	VECLIST e;
	e.resize(4);
	e[0] = o - side * (sz / 2) + v * (sz / 4);
	e[1] = o - side * (sz / 2) - v * (sz / 4);
	e[2] = o + side * (sz / 2) - v * (sz / 4);
	e[3] = o + side * (sz / 2) + v * (sz / 4);
	closeedge(e);
	plane(e[0], e[1], e[2], e[3]);
	extrudeedgeex(e, sz, 0.5);
	color = 0xffaaaaaa;
	//plane(e[0], e[1], e[2], e[3]);
}
void yumileave(vec o, vec v, real sz)
{
	vec n = vec::UZ;
	color = 0xff008000;
	VECLIST e;
	roundedge(e, o, v, sz, 16, 0);
	vec p = o;
	int len = 15;
	for (int i = 0; i < len; i++)
	{
		real ai = i / (real)len;
		VECLIST e1;
		extrudeedge(e, sz, e1);
		p += v * 0.5f;
		vec side = v.cross(n).normcopy();
		v.rot(PI / 38, side);
		n = side.cross(v).normcopy();

		for (int ii = 0; ii < e1.size(); ii++)
		{
			real aii = ii / real(e1.size());
			real ang = blend2(-1, 1, aii) * blend(PI * 2, -PI / 4, ai, 0.25);
			vertex& v = e1[ii];
			v.p = p + (side * cos(ang) + n * sin(ang)) * blend2(0.01, 1 * aii, ai, 2);
		}

		if (i > 0)
			face(e, e1);
		e = e1;
	}
}
void yumi(vec o, vec v, real sz)
{
	color = gcurcor;
	VECLIST e;
	gcommonvertex = 1;
	roundedge(e, o, v, sz, 16, 0);
	int len = 10;
	for (int i = 0; i < len; i++)
	{
		real ai = i / (real)len;
		VECLIST e1;
		color = gcurcor;
		extrudeedge(e, sz / 2, e1);
		real r = i < 4 ? blend(sz * 0.75, sz, i / 4.0f, 0.25) : blend(sz, sz / 20, ((i - 4) / (real)len), 4);
		r *= 0.5;
		radedge(e1, r);
		vec oo = getedgecenter(e1);

		if (rand() % 10 == 0)
		{
			e1.pop_back();
			e.pop_back();
		}
		for (int ii = 0; ii < e1.size(); ii++)
		{
			yumili(e1[ii], v, (o-e1[ii]).normcopy(), sz);
		}
		if (i > 0)
			face(e, e1);
		e = e1;
	}
	gcommonvertex = 0;
	//yumileave(o, v, sz);
}
// --------------------------------------------------------------------
// 兰花
// --------------------------------------------------------------------
void lan(vec o, real sz, int leaf, real fang, real fang2, int len2)
{
	gcommonvertex = 1;
	VECLIST e1, e2;
	real r = sz * 0.01;
	{
		int len = 4;
		for (int i = 0; i < len; i++)
		{
			real ai = i / real(len);
			real ang = PI / 4 * ai;
			vec p = o + vec(r * cos(ang), 0, r * sin(ang));
			e1.push_back(p);
		}
		for (int i = 0; i < len; i++)
		{
			real ai = i / real(len);
			real ang = -PI / 4 * ai;
			vec p = o + vec(r * cos(ang), 0, r * sin(ang));
			e2.push_back(p);
		}
	}
	vec vv = vec::UY;
	vec uz = vec::UZ;
	vec ux = uz.cross(vv).normcopy();
	real rnd1 = rrnd(0.01, 0.05);
	real rnd2 = rrnd(0.02, 0.08);
	real rnd3 = rrnd(0.5,2);
	real d = r * 1;
	int len = 20 + clamp(0, 200, 10, 5);
	for (int i = 0; i <= len; i++)
	{
		real ai = i / real(len);
		{// 颜色
			color = blendcor(RGB(255, 255, 255), gcurcor, ai, 1);
			if (ai > 0.75)
				SETPARM(10);
			else if(ai < 0.25)
				SETPARM(12);
			else
				SETPARM(11);
		}
		VECLIST ee1, ee2;
		{// 口
			vv.rot(-rnd1 * clamp(0, 1, 11, 5), uz);
			vv = blend(vv, -vec::UY, ai * clamp(0, rnd2, 11, 5), rnd3);
			vv.norm();

			ux = uz.cross(vv).normcopy();
			uz = vv.cross(ux).normcopy();

			r *= 1 + PARAM[3];
			radedgeL(e1, o, r, vv, ux, uz, ai, ee1);
			radedgeR(e2, o, r, vv, ux, uz, ai, ee2);
		}
		{// 节间距 
			o += vv * (1 + PARAM[4]) * d;
		}
		if (i > 0 && i < len)
		{
			face(e1, ee1);
			binvnorm = 1;
			face(e2, ee2);
			binvnorm = false;
		}
		gverindfindstart = (int)gsubmesh.vertices.size();
		e1 = ee1;
		e2 = ee2;
	}
	gverindfindstart = 0;
	gcommonvertex = 0;
	binvnorm = false;
}

// ------------------------------------------------
// 天南星科
// ------------------------------------------------
void Araceae1(vec o, vec v0, real s, int depth = 0)
{
    DEF_SCALES2(s);
    VECLIST e;
    v0.norm();
    vec p = o;
    int len = 16;
    for (int i = 0; i <= len; i++)
    {
        real ai = real(i) / (len);
        real t = blend(0.0, 1, ai);
        vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.5);
        v0 += vec::UY * 0.4; v0.norm();
        vec np = p + (v0 + dp) * (s * 10);
        {
            color = blendcor(0xFF001000, 0xFF608000, ai);
            VECLIST e1;
            if (i == 0)
            {
                roundedge(e1, p, vec::UY, s, 8);
            }
            else
            {
                moveedge(e, (np - p), e1);
                //float sc = blend(1.35, 0.5, ai, 3.5);
                float sc = blend(1.5, 0.5, ai, 2.5);
                sc *= blend(0.5, 1.35, ai, 0.5);
                scaleedge(e1, sc);
                {//
                    vec pp = e[int(rrnd(0, 7))];
                    gtargets.push_back(pp);
                }
            }
            if (i > 0 && depth < 2 && rrnd() < 0.1)
            {
                vec pp = e[int(rrnd(0, 7))];
                Araceae1(pp, (pp - p).normcopy(), s  * 0.5, depth + 1);
            }
            else
                if (i > len / 2 && depth < 2 && rrnd() < 0.1)
                {
                    vec pp = e[int(rrnd(0, 7))];
                    Araceae2(pp, (pp - p).normcopy(), s  * 0.25, 8, 4, 8);
                }
            if (i != 0)
                face(e, e1);
            e = e1;
        }
        p = np;
    }
    face(e, getedgecenter(e) + vec::UY * (s * 4));
    color = 0xFFFFFFFF;
}
void Araceae2(vec o, vec v0, real s,
              int n,
              int aa,
              int an
              )
{
    DEF_SCALES2(s);
    VECLIST e;
    v0.norm();
    vec p = o;
    int len = n;
    for (int i = 0; i <= len; i++)
    {
        real ai = real(i) / (len);
        real t = blend(0.0, 1, ai);
        vec dp = vec::UX * (perlin(p.x * 10, p.y * 10, p.z * 10 + gtimealpha * 4) * 0.75);
        vec np = p + (v0 + dp) * (s * 10);
        v0 = v0 + vec::UY * 0.25f; v0.norm();
        {
            VECLIST e1;
            if (i == 0)
            {//
                roundedge(e1, p, vec::UY, 8 * s, aa);
            }
            else if (i < len)
            {//
                color = 0xFF001000;
                moveedge(e, (np - p), e1);
                float sc = blend(0.9, 1, ai, 3.5);
                scaleedge(e1, sc);
                //rotedgebynorm(e1, v0);
                color = 0xFF008000;
            }
            else
            {//
                color = 0xFF40AAFF;
                moveedge(e, (np - p) * 0.02, e1);
                //scaleedge(e1, 1.2);
                for (int ii = 0; ii < aa; ii++)
                {
                    VECLIST ee;
                    unionedge(e, e1, ii, 2, ee);
                    closeedge(ee);
                    vec v = getedgenorm(ee);
                    real cvg = rrnd(0.025, 0.75);
                    for (int jj = 0; jj < an; jj++)
                    {
                        real ajj = jj / real(an);
                        v -= vec::UY * blend(-0.25, cvg, ajj);
                        v.norm();
                        VECLIST ee1;
                        moveedge(ee, v * (s * 5), ee1);
                        float sc = blend(1.5, 0.1, ajj, 1.5);
                        scaleedge(ee1, sc);
                        face(ee, ee1);
                        ee = ee1;
                    }
                }
            }
            if (i != 0)
                face(e, e1);
            e = e1;
        }
        p = np;
    }
    //face(e, getedgecenter(e) + vec::UY * (s * 4));
    color = 0xFFFFFFFF;
}
void Araceae3(vec o, vec v0, real s, int n, real sz2)
{
    gcommonvertex = 1;
    for(int j = 0; j < n; j ++)
    {
        real aj = j / real(n);
        color = blendcor(0xFF204000, gcurcor, rrnd());
        VECLIST e;
        roundedge(e, o, vec::UY, s, 8);
        o += vec::UY * (s * E2);
        for (int i = 0; i < e.size() - 1; i++)
        {
            VECLIST e1;
            e1.push_back(o);
            e1.push_back(e[i]);
            e1.push_back(e[i + 1]);
            closeedge(e1);
            
            vec v = getedgenorm(e1);
            v.y *= 0.4; v.norm();
            
            real scl2 = blend(8, 1, aj);
            for (int ii = 0; ii <= 8; ii++)
            {
                real aii = ii / real(8);
                v += vec::UY * blend(0, 0.1, aii);
                VECLIST e2;
                moveedge(e1, v * (s * scl2), e2);
                float sc = blend(1.8 * sz2, 0.5, aii, 1);
                scaleedge(e2, sc);
                face(e1, e2);
                e1 = e2;
                face(e1, getedgecenter(e1) + vec::UY * (s * 4));
            }
        }
        color = 0xFFFFFFFF;
        o += vec::UY * (s * 50);
    }
    gcommonvertex = 0;
}
void Araceae4(vec o, vec v0, real s)
{
    color = blendcor(0xFF204000, 0xFF605000, rrnd());
    VECLIST e;
    roundedge(e, o, vec::UY, s, 8);
    o += vec::UY * (s * E2);
    for (int i = 0; i < e.size() - 1; i++)
    {
        VECLIST e1;
        e1.push_back(o);
        e1.push_back(e[i]);
        e1.push_back(e[i + 1]);
        closeedge(e1);
        
        vec v = getedgenorm(e1);
        v.y *= 0.25; v.norm();
        for (int ii = 0; ii <= 8; ii++)
        {
            real aii = ii / real(8);
            v += vec::UY * blend(0.25, 0.35, aii);
            VECLIST e2;
            moveedge(e1, v * (s * 8), e2);
            float sc = blend(1.8, 0.5, aii, 1);
            scaleedge(e2, sc);
            face(e1, e2);
            e1 = e2;
            face(e1, getedgecenter(e1) + vec::UY * (s * 4));
        }
    }
    color = 0xFFFFFFFF;
}
