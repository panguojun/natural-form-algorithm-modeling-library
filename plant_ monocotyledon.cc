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
