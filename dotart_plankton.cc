// ================================================================================================
// Code from the game : 'Plankton' : https://store.steampowered.com/app/542550/Plankton/
// ================================================================================================
// Ãÿ–ß
// ------------------------------------------------------------------------------------------------
// ∑¢…‰¡£◊”
void emitParticles(vec2 p, vec2 v0)
{
	static int sindex = 0;
	static real lstemittime = gtime;
	if (gtime - lstemittime > 0.1)
	{
		particle_t & pcl = particles[(sindex++) % 6];
		pcl.p = p;
		pcl.v = v0;
		pcl.btime = gtime;
		lstemittime = gtime;
	}
}
// ------------------------------------------------------------------------------------------------
// ÀØæıZ◊÷∑˚¡£◊”–ßπ˚
void drawSleepingZ(vec2 p, int len, real sz, real ang)
{
	vec2 p1 = p;
	vec2 p2 = p + vec2(sz, 0.0).rotcpy(ang);
	vec2 p3 = p2 + vec2(-sz, -sz).rotcpy(ang);
	vec2 p4 = p3 + vec2(sz, 0.0).rotcpy(ang);

	for (int i = 0; i < len; i++)
	{
		real ai = real(i) / len;
		vec2 pp = blend(p1, p2, ai);
		pixel(pp);
		pp = blend(p2, p3, ai);
		pixel(pp);
		pp = blend(p3, p4, ai);
		pixel(pp);
	}
}
// ªÊ÷∆ÀØæı¡£◊”–ßπ˚
void drawSleepingZEff()
{
	for (int i = 0; i < 6; i++)
	{
		particle_t & pcl = particles[i];
		if (gtime - pcl.btime > 8.0)
			pcl.btime = 0;
		else
		{
			pcl.p = pcl.p + pcl.v * 0.009;
			drawSleepingZ(pcl.p, pcl.len, pcl.sz, pcl.ang);
		}
	}
}

// ªÊ÷∆–›œ¢¡£◊”–ßπ˚
void drawRestEff()
{
	for (int i = 0; i < 8; i++)
	{
		particle_t & pcl = particles[i];
		if (gtime - pcl.btime > 1.0)
			pcl.btime = 0;
		else
		{
			pcl.p = pcl.p + pcl.v * 0.005;
			pixel(pcl.p);
		}
	}
}
// ------------------------------------------------------------------------------------------------
// Œ¥÷™∏°”Œ∂ØŒÔ
// ------------------------------------------------------------------------------------------------
real unkownplankter_dencity = 1;
void unkownplankter(vec2 p, vec2 v, int ind, bool btail, int type = 0)
{
	pixel_size0 = 1;
	int len0 = 10;
	v.norm();
	real sz0 = blend(0.5, 1, rndmap[ind][9]);

	if (shapclass == 1 && lifestep == 3)
		sz0 *= 1.25;

	if (btail && type == 1)
	{
		point5(p - v * 0.004, 0.001);
	}

	for (int i = 0; i < len0; i++)
	{
		real ai = real(i) / len0;
		p = p - v * (sz0 * 0.0012);

		real sz = blend2(0, sz0 * 0.004, ai, 2);
		vec2 p1 = p + vec2(v.y, -v.x) * sz;
		vec2 p2 = p + vec2(-v.y, v.x) * sz;
		if (rand() % 100 < unkownplankter_dencity * 100)
		{
			pixel(p1);
			pixel(p2);
		}

		if (btail && type == 0)
		{
			if (i == int(len0 * 0.4) || i == int(len0 * 0.6))
			{// ª®Œ∆
				for (int ii = 0; ii < 4; ii++)
				{
					real aii = real(ii) / 4;

					pixel(blend(p1, p2, aii));
				}
			}
		}
	}
	if (btail)
	{// Œ≤∞Õ
		int len = len0 / 2;
		for (int j = 0; j < len; j++)
		{
			real aj = real(j) / len;
			real ang = sin(aj * PI * 4 + gtime * 12 + rndmap2[j][ind] * PI) * 0.25;
			v.rot(ang);
			p = p - v * (sz0 * 0.0015);
			pixel(p);
		}
	}
	pixel_size0 = 2;
}
// ------------------------------------------------------------------------------------------------
// ¥¯±ﬁ√´µƒ¬Ã‘Â
void liuzao1(vec2 o, vec2 dir, int len0, int ind)
{
	pixel_size0 = 1;
	real velval = dir.len();
	dir.norm();
	real sz1 = blend(0.5, 1.2, rndmap[108][ind]);
	{// ±ﬁ√´1
		vec p = o;
		vec2 v = vec2::UY;
		v.rot(PI / 3 * (0.5 + 0.5 * sin(rndmap2[100][ind] * PI + blend(0, gtime * 30, velval / 0.5))));
		int len = len0 / 5 * sz1;
		for (int j = 0; j < len; j++)
		{
			real aj = real(j) / len;
			p = p + v * (0.002);
			if (rand() % 100 < unkownplankter_dencity * 100)
				pixel(p);
		}
	}
	{// ±ﬁ√´2
		vec p = o;
		vec2 v = vec2::UY;
		v.rot(-PI / 3 * (0.5 + 0.5 * sin(rndmap2[101][ind] * PI + blend(0, gtime * 30, velval / 0.5))));
		int len = len0 / 5 * sz1;
		for (int j = 0; j < len; j++)
		{
			real aj = real(j) / len;
			p = p + v * (0.002);
			if (rand() % 100 < unkownplankter_dencity * 100)
				pixel(p);
		}
	}
	{// ªÊ÷∆
		vec2 p = o;
		real ang0 = 0;
		real sz0 = blend(0.0025, 0.005, rndmap[108][ind]);
		real bianxing = blend(0.25, 0.75, rndmap[ind][100]);
		{// ¥”‘≤µΩ∑Ω				
			for (int i = 0; i < len0; i++)
			{
				real ai = real(i) / len0;
				real ang = blend(0, 2 * PI, ai);
				real r0 = sz0 + sz0 / 2 * blend2d(-1, 1, p.x, p.y);
				real r1 = r0;
				real dang = ang - int(ang / (PI / 2)) * PI / 2;
				if (dang < PI / 4)
					r1 = r1 / cos(dang);
				else
					r1 = r1 / sin(dang);
				real r = blend(r0, r1, bianxing, 2);
				vec2 v(cos(ang + ang0), sin(ang + ang0));
				p = o + v * r;
				if (rand() % 100 < unkownplankter_dencity * 100)
				{
					pixel(p);
					// Œ∆¿Ì
					if (rand() % 100 < 10)
						pixel(o + v * rrnd(0, r));
				}
			}
		}
	}
	pixel_size0 = 2;
}
// ------------------------------------------------------------------------------------------------
// ¬ﬂº≠
void unkownplankter()
{
	plankerdeadcnt = 0;
	FOREACH(i, PLANKTER_NUM)
	{
		planker_t& ptr = plankers[i];
		if (!ptr.binit)
			ptr.init();

		if (ptr.state == -1 || ptr.state == -2)
		{
			plankerdeadcnt++;
			continue;
		}
		if (ptr.dangertime != 0)
		{
			if (gtime - ptr.dangertime > 1.0)
			{
				ptr.dangertime = 0;
				state == 2 || state == 3 ? state = 1 : 0;
			}
		}
		if (ptr.state == 0 && 
			gtime - ptr.catchtime > 5.0)
		{
			ptr.state = 1;
		}
		if (ptr.catchtime == 0)
		{
			{// ∂„±‹Œ£œ’
				if (ptr.state == 2 ||
				ptr.state == 3 && ptr.vel.len() < 0.0001)
				{
					ptr.state = 3;

					if (lifestep == 3)
					{
						if (ptr.energe > 0)
						{
							ptr.vel = (ptr.pos - pos).normcopy() * 0.002 + vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.004;
							//ptr.energe--;
						}
					}
					else
					{
						ptr.vel = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.004;
					}
					//PRINT("∂„±‹Œ£œ’")
					//	ptr.vel = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.001;
				}
			}

			{// ‘À∂ØÀŸ∂»øÿ÷∆
				vec2 v0 = ptr.vel;
				vec2 npos = ptr.pos + v0 * GAMESPEED;
				if (npos.x < 0.0)
					v0.x = -v0.x;
				if (npos.y < 0.0)
					v0.y = -v0.y;
				if (npos.x > MAXX)
					v0.x = -v0.x;
				if (npos.y > MAXY * 0.9)
					v0.y = -v0.y;
				ptr.pos = ptr.pos + v0 * GAMESPEED;
				ptr.vel = v0;
			}

			// ◊Ë¡¶
			if (shapclass == 1)
				ptr.vel = ptr.vel * pow(0.985, GAMESPEED);
			else
				ptr.vel = ptr.vel * pow(0.995, GAMESPEED);

			if (ptr.vel.len() < 0.0001)
			{
				if (shapclass == 1)
					ptr.vel = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.004;
				else
					ptr.vel = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.003;

				if (ptr.state == 1)
				{// ¬˝¬˝µƒª÷∏¥ƒ‹¡ø
					ptr.energe++;
					if (ptr.energe > 8)
						ptr.energe = 8;
				}
			}
		}
		// ªÊ÷∆	
		if (ptr.state == 0)
			unkownplankter_dencity = blend(1, 0, (gtime - ptr.catchtime) / 2.0);
		else
			unkownplankter_dencity = 1;

		if (shapclass == 1 && shape == 3 && lifestep == 2)
			liuzao1(ptr.pos, ptr.vel, 32, i);
		else if(shapclass == 1 && shape == 3 && lifestep == 3)
			unkownplankter(ptr.pos, ptr.vel, i, ptr.state > 0, 1);
		else
			unkownplankter(ptr.pos, ptr.vel, i, ptr.state > 0);
	}
}

// ###############################################################################################
// ‘≠…˙∂ØŒÔµƒ ¿ΩÁ
// ###############################################################################################
// ∫£¥¯÷≤ŒÔ
// ------------------------------------------------------------------------------------------------
extern void hydra(vec2 p0, vec2 v, int len0, int showtype);
extern void alga(vec2 o, int len, real sz, real ang0, int index);
void kelp(vec2 p0, vec2 v, int len0, int posi = 0)
{
	vec2 p = p0;
	v.norm();
	real a2d0 = blend2d(-1.0, 1.0, p.x, p.y);

	real ang = rndmap2[posi % 512][0] * PI * 0.5 * a2d0;
	bool hasbushizhe = 0;
	for (int i = 0; i < len0; i++)
	{
		real ai = real(i) / len0;
		real a2d = blend2d(-10.0, 10.0, ai, p.y);

		v.rot(a2d * PI * 0.0025 + blend(0, ang, ai));

		p = p + v * 0.008;

		if (i % int(1 + blend(0, 2.5, ai)) == 0)
			pixel(p);
		
	}
}

void kelp0(int len0, real wid)
{
	static vec2 cp[] = {
		vec2(rrnd(0.5, 1.0) * MAXX, 0.0),
		vec2(rrnd(-0.8, 0.0), 0.25 * MAXY),
		vec2(rrnd(1.0, 1.5), 0.75 * MAXY),
		vec2(-0.01, MAXY - 0.05)
	};
	gcp = cp;
	cp[1].x = 0.75 * MAXX * (cos(PI * gtime * 0.0061 + kelpctrlx1));
	cp[2].x = MAXX + 0.75 * MAXX * (sin(PI * gtime * 0.0052 + kelpctrlx2));

	vec2 p, v;
	int lsti = len0 / 8;
	for (int i = 0; i < len0; i++)
	{
		int tt = i % 10;
		if (tt < 3 && tt != 1)
			continue;
		real ai = real(i) / len0;
		real a2d = blend2d(0.1, 4.0, ai, p.x);
		vec2 np = bezier3(cp, ai);
		{
			v = np - p;
			v.norm();

			pixel(p);

			if (i - lsti > len0 / 5)
			{
				lsti = i;
				
				kelp(p, -v, blend(len0 / 10, len0 / 6, rndmap[i % 512][88]), i);
			}
		}
		p = np;
	}
}

// ------------------------------------------------------------------------------------------------
// π˝∂…∂Øª≠œ∏∞˚◊©‘Ï–Õ
// ------------------------------------------------------------------------------------------------
void aniXibaozhuan(vec2 o, vec2 dir, vec2 dir1, int len0, int ind)
{
	flushpixels();pixel_size0 = 1;
	real at = blend(0, 1, gdtime1 / 8);
	real at1 = blend(0, 1, gdtime1 / 6);
	{// ªÊ÷∆
		vec2 p = o;
		dir.norm();
		real ang0 = atan2(dir.y, dir.x);
		real ang1 = atan2(dir1.y, dir1.x);
		ang0 = blend(ang0, ang1, at);
		real sz0 = blend(0.025, 0.008, at1, 2);
		{// ¥”‘≤µΩ∑Ω				
			for (int i = 0; i < len0; i++)
			{
				real ai = real(i) / len0;
				real ang = blend(0, 2 * PI, ai);
				real r0 = sz0 + sz0 / 2 * blend2d(-1, 1, p.x, p.y);
				real r1 = r0;
				real dang = ang - int(ang / (PI / 2)) * PI / 2;
				if (dang < PI / 4)
					r1 = r1 / cos(dang);
				else
					r1 = r1 / sin(dang);
				real r = blend(r0, r1, at, 2);
				vec2 v(cos(ang + ang0), sin(ang + ang0));
				p = o + v * r;
				if (i > 0 && rand() % drawstep == 0)
				{
					pixelBug(p, i);

					// Œ∆¿Ì
					if (rand() % 100 < 10)
						pixelBug(o + v * rrnd(0, r), i);
				}
			}
		}
		{// Œ≤∞Õ
			vec2 v = dir;
			int len = len0 / 4 * blend(1, 0, at);
			for (int j = 0; j < len; j++)
			{
				real aj = real(j) / len;
				real ang = sin(aj * PI * 8 + gtime * 4 + rndmap2[j][ind] * PI) * 0.2;
				v.rot(ang);
				p = p + v * (0.002);
				if (rand() % drawstep == 0)
					pixelBug(p,j);
			}
		}
	}
	flushpixels(); pixel_size0 = 2;
}
void aniXibaozhuan()
{
	static const int xibaozhuannum = 13;
	static vec2 xibaozhuan_pos[xibaozhuannum];
	static vec2 xibaozhuan_vel[xibaozhuannum];
	static vec2 xibaozhuan_vel1[xibaozhuannum];
	static bool sinit = 0;

	real at = blend(0, 1, gdtime1 / 6);

	if (sinit == 0)
	{// ≥ı ºªØ
		sinit = 1;
		xibaozhuan_pos[0] = pos;
		for (int i = 1; i < xibaozhuannum / 2; i++)
		{
			xibaozhuan_pos[i] = vec2(rrnd(MAXX, 2 * MAXX), rrnd(0, MAXY));
		}
		for (int i = xibaozhuannum / 2; i < xibaozhuannum; i++)
		{
			xibaozhuan_pos[i] = vec2(rrnd(-2 * MAXX, 0), rrnd(0, MAXY));
		}
	}

	{// ÀÆÛ£µƒ÷˜÷·œﬂ
		vec2 p(MAXX / 2, 0.1);
		vec2 v(0.01, 0.8); v.norm();
		real dl = blend(0.008, 0.0015 * 0.08 * 10, at, 2);
		
		int len = 250 / 2;
		for (int i = 0; i < len; i++)
		{
			real ai = real(i) / len;
			{
				v = v + vec2(rndmap2[i][0], rndmap2[0][i]) * (0.04);
				real ftt = FT(ai * 2 * PI + gtime * PI * 0.05);
				v.rot(ftt * PI * 0.008);
				if (v.y < 0.4) v.y = 0.4; // œﬁ÷∆∆‰œÚ…œµƒΩ«∂»
			}
			p = p + v * dl;
			//pixel(p);

			int ii = i / 10;
			if (i % 10 == 0 && ii < xibaozhuannum)
			{
				xibaozhuan_vel[ii] = (xibaozhuan_pos[ii] - p) * 0.1;
				xibaozhuan_vel1[ii] = v;
			}
		}
	}
	{// “∆∂Ø

		for (int i = 0; i < xibaozhuannum; i++)
		{
			xibaozhuan_pos[i] = xibaozhuan_pos[i] - xibaozhuan_vel[i] * 0.1;
			//pixel(xibaozhuan_pos[i]);
			aniXibaozhuan(xibaozhuan_pos[i], xibaozhuan_vel[i], xibaozhuan_vel1[i], 50, i);
		}
	}
}

// ------------------------------------------------------------------------------------------------
// ‘Â¿‡
// ------------------------------------------------------------------------------------------------
int alga_variation = 0;
float alga_size = 1.0f;
void alga1(vec2 o, int len, real sz, real ang0, int index)
{
	len = len * blend2d(0.1, 0.25, o.x, o.y);
	int step = 1;
	if (drawinggrass && drawinggrass->beeating)
	{
		real eatingprogress = (gtime - starteattime) / 2.0;
		step = blend(1, 20, eatingprogress);
	}

	if (drawinggrass && gtime - drawinggrass->grassborntime < 2.0)
	{
		real borningprogress = (gtime - grasses[index % (GRASSNUM)].grassborntime) / 2.0;
		step = blend(50, 1, borningprogress);
	}


	for (int i = 0; i < len; i += step)
	{
		real ai = i / real(len);

		real a2d = blend2d(0.25, 1.0, ai, o.y);

		real ang = blend(0, (5 + alga_variation) * PI, ai) + ang0;

		real r = ang * 0.002 * a2d * sz;
		vec2 p = o + vec2(cos(ang), sin(ang)) * r;
		if (i % 10 < grassdrawstep0)
			pixel(p);
	}
}

// ------------------------------------------------------------------------------------------------
// —©ª®‘Ï–Õ1
void alga2(vec2 o, int len, real sz, real ang0, int index)
{
	len = len * blend2d(0.02, 0.05, o.x, o.y);
	int step = 1;
	if (drawinggrass && drawinggrass->beeating)
	{
		real eatingprogress = (gtime - starteattime) / 2.0;
		step = blend(1, 20, eatingprogress);
	}

	if (drawinggrass && gtime - drawinggrass->grassborntime < 2.0)
	{
		real borningprogress = (gtime - grasses[index].grassborntime) / 2.0;
		step = blend(50, 1, borningprogress);
	}	

	pixel_size0 = 1;
	pixel_size1 = 2;
	for (int j = 0; j < 6; j++)
	{
		ang0 += 2 * PI / 6 + alga_variation * PI / 10;
		vec2 p = o;
		vec2 v = vec2::UX;
		v.rot(ang0);
		for (int i = 0; i < len; i += step)
		{
			real ai = i / real(len);
			real a2d = blend2d(1.5, 2.0, p.x, p.y);
			p = p + v * (0.00025 * alga_size * a2d);
			if (i % 10 < grassdrawstep0)
				pixel(p);
		}
		if (j % 10 < grassdrawstep0)
			pixel1(o + v.rotcpy(2 * PI / 12) * (0.025 * sz));
	}
}
// ------------------------------------------------------------------------------------------------
void alga3(vec2 o, int len, real sz, real ang0, int index)
{
	len = len * blend2d(0.01, 0.1, o.x, o.y);
	int step = 1;
	if (drawinggrass && drawinggrass->beeating)
	{
		real eatingprogress = (gtime - starteattime) / 2.0;
		step = blend(1, 20, eatingprogress);
	}

	if (drawinggrass && gtime - drawinggrass->grassborntime < 2.0)
	{
		real borningprogress = (gtime - grasses[index].grassborntime) / 2.0;
		step = blend(50, 1, borningprogress);
	}

	for (int j = 0; j < 3; j++)
	{
		ang0 += 2 * PI / 3;
		vec2 p = o;
		vec2 v = vec2::UX;
		v.rot(ang0);
		for (int i = 0; i < len; i += step)
		{
			real ai = i / real(len);

			real a2d = blend2d(0.5, 2.5, p.x, p.y);

			real ang = blend(0, (5 + alga_variation) * PI * a2d, ai) + ang0;

			p = p + v * (0.0005 * alga_size * a2d);
			v.rot(PI * blend2(0, 0.1, ai));
			if (i % 10 < grassdrawstep0)
				pixel(p);
		}
	}
}

// ------------------------------------------------------------------------------------------------
void alga4(vec2 o, int len, real sz, real ang0, int index)
{
	len = len * blend2d(0.01, 0.1, o.x, o.y);
	int step = 1;
	if (drawinggrass && drawinggrass->beeating)
	{
		real eatingprogress = (gtime - starteattime) / 2.0;
		step = blend(1, 20, eatingprogress);
	}

	if (drawinggrass && gtime - drawinggrass->grassborntime < 2.0)
	{
		real borningprogress = (gtime - grasses[index].grassborntime) / 2.0;
		step = blend(50, 1, borningprogress);
	}

	for (int j = 0; j < 3; j++)
	{
		ang0 += 2 * PI / 3;
		vec2 p = o;
		vec2 v = vec2::UX;
		v.rot(ang0);
		for (int i = 0; i < len; i += step)
		{
			real ai = i / real(len);

			real a2d = blend2d(0.5, 2.5, p.x, p.y);

			real ang = blend(0, (5 + alga_variation) * PI * a2d, ai) + ang0;

			p = p + v * (0.0005* alga_size * a2d);
			v.rot(PI * blend(0.0, 0.1 * a2d, ai));
			if (i % 10 < grassdrawstep0)
				pixel(p);
		}
	}
}

// ------------------------------------------------------------------------------------------------
// —©ª®‘Ï–Õ2
void alga5(vec2 o, int len, real sz, real ang0, int index)
{
	len = len * blend2d(0.01, 0.05, o.x, o.y);
	int step = 1;
	if (drawinggrass && drawinggrass->beeating)
	{
		real eatingprogress = (gtime - starteattime) / 2.0;
		step = blend(1, 20, eatingprogress);
	}

	if (drawinggrass && gtime - drawinggrass->grassborntime < 2.0)
	{
		real borningprogress = (gtime - grasses[index].grassborntime) / 2.0;
		step = blend(50, 1, borningprogress);
	}

	step = step * 4;

	pixel_size0 = 1;
	pixel_size1 = 2;
	for (int j = 0; j < 4; j++)
	{
		ang0 += 2 * PI / 4 + alga_variation * PI / 10;
		vec2 p = o;
		vec2 v = vec2::UX;
		v.rot(ang0);
		for (int i = 0; i < len; i += 1)
		{
			real ai = i / real(len);
			real a2d = blend2d(1.5, 2.0, p.x, p.y);
			p = p + v * (0.00025 * alga_size * a2d);
			if (i % 10 < grassdrawstep0)
				pixel(p);
		}
		if (j % 10 < grassdrawstep0)
			pixel1(o + v.rotcpy(2 * PI / 8) * (0.015 * sz));
	}
}
// ------------------------------------------------------------------------------------------------
// ª·…˙≥§µƒ‘Â¿‡
void growingAlga(vec2 o, int len, real sz, real ang0, int index)
{	
	pixel_size0 = 1;
	pixel_size1 = 1;
	ang0 = rndmap[index][8] * PI;
	vec v0 = vec2::UX.rotcpy(ang0);
	for (int j = 0; j < 3; j++)
	{
		real aj = j / 3.0;
		v0.rot(2 * PI / 3);
		vec p = o;

		int len0 = len / 3;
		for (int i = 0; i < len0; i++)
		{
			real ai = i / real(len0);
			p = p + v0 * 0.002;
			pixel(p);
			if (i == len0 - 1)
			{
				real len1 = len / 2;
				{// ◊Û≤‡Õ‰«˙
					vec v = v0;
					vec p1 = p;
					for (int ii = 0; ii < len1; ii++)
					{
						real aii = ii / real(len1);
						v.rot(aii * PI * 0.12);
						p1 = p1 + v * 0.002;
						pixel(p1);
					}
				}
				{// ”“≤‡Õ‰«˙
					vec v = v0;
					vec p2 = p;
					for (int ii = 0; ii < len1; ii++)
					{
						real aii = ii / real(len1);
						v.rot(-aii * PI * 0.12);
						p2 = p2 + v * 0.002;
						pixel(p2);
					}
				}
				
				if (j < grasses[index].growgrasses.size())
				{
					if (grasses[index].growgrasses[j].grassstates == 0)
					{
						grasses[index].growgrasses[j].grassposmap = p + v0 * 0.02;
						drawinggrass = &grasses[index].growgrasses[j];
						alga(p + v0 * 0.02,
							400,
							0.015 + rndmap[j][11] * 0.25,
							rndmap[j][13] * 2 * PI + 0.4 * PI * sin(1.65 + +rndmap[j][25] * PI + gtime * 1.58),
							-1
							);
					}
				}
			}
		}
	}
}
// -----------------------------------------------------------------
void alga(vec2 o, int len, real sz, real ang0, int index, int grassshape)
{	
	sz *= 1.78; // ∑≈¥Û ≥ŒÔ
	if (grassshape == 1)
		alga1(o, len, sz, ang0, index);
	else if (grassshape == 2)
		alga2(o, len, sz, ang0, index);
	else if (grassshape == 3)
		alga3(o, len, sz, ang0, index);
	else if (grassshape == 4)
		alga4(o, len, sz, ang0, index);
	else if (grassshape >= 5)
		alga5(o, len, sz, ang0, index);
}

// -----------------------------------------------------------------
// ªÊ÷∆‘Â¿‡
void alga(vec2 o, int len, real sz, real ang0, int index)
{	
	if (index != -1)
	{// ∆’Õ®‘Â¿‡
		drawinggrass == 0 ? drawinggrass = &grasses[index] : 0;
		if (drawinggrass->grassstates == -1)
			return;
		if (drawinggrass->shengzhang == 2)
			return growingAlga(o, len / 18, sz, ang0, index);
	}
	else
	{ // …˙≥§≥ˆ¿¥µƒ‘Â¿‡
		index = 0;
	}

	int grassshape = drawinggrass->grassshape + 1;
	alga(o, len, sz, ang0, index, grassshape);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ◊˜ºÎ
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline void jian(vector2 p, real r, int len0)
{
	pixel_size0 = 1;
	int len = len0;
	for (int i = 0; i < len; i++)
	{
		real ang = blend(0, 2 * PI, i / real(len));
		vec2 dp(r * cos(ang), r * sin(ang));
		vec2 pp = p + dp;
		real a2d = blend2d(0.1, 1, pp.x, pp.y) * 1;
		dp = dp + dp * a2d;
		pixel(p + dp);
		pixel(p + dp * rrnd(0, 1));
	}
	pixel_size0 = 1;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ¬—
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline void luan(vector2 p, int len0, real r0)
{
	pixel_size1 = 1;
	real r = r0;
	real dtime = gtime - borntime - 3;
	if (dtime > 0)
	{
		r = r0 * (0.75 + 0.25 * sin(PI * dtime * 2));
	}

	int len = len0;
	for (int i = 0; i < len; i++)
	{
		real ang = blend(0, 2 * PI, i / real(len));
		vec2 dp(r * cos(ang), r * sin(ang));
		vec2 pp = p + dp;
		real a2d = blend2d(0.1, 1, pp.x, pp.y) * 1;
		dp = dp + dp * a2d;
		pixel(p + dp);
		if (i % 8 == 0)
			pixel1(p + dp * 1.1);
		pixel1(p + dp * rrnd(0, 1));
	}
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ±‰–Œ≥Ê‘Ï–Õ
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline float FT1(float angle, float t[] = 0, float dt = 0)
{
	if (t == 0)
	{
		static float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		t = s_t0;
	}

	float yy = 0;
	yy += 1 * sin(1 * angle + (t[0] + dt) * PI);
	yy += 0.25	* sin(0.5 * angle + (t[1] + dt) * PI);
	return yy;
}
// -----------------------------------------------------------------
void protozoa1(vec2 o, vec2 ndir, int len0)
{	
	// ∑¿÷π◊™œÚÃ´Õª»ª
	static vec2 dir = ndir;
	dir = blend(dir, ndir, 0.1);

	pixel_size1 = 1;
	bool isdrawtail = 0;
	int len = len0;
	real dirang = atan2(dir.y, dir.x);
	real vellen = dir.len();
	real dirlen = vellen / (GAMESPEED);	
	
	static real bordersize0 = 0;
	static real	bordersize1 = 0.08;
	{// ±ﬁ√´≥ﬂ¥Á
		if (state == 7 || state == 6)
		{
			bordersize0 = blend(bordersize0, 0.08, 0.01);
			bordersize1 = blend(bordersize1, 0.08, 0.01);
		}
		else if (state == 10)
		{
			bordersize0 = blend(bordersize0, 0.16, 0.01);
			bordersize1 = blend(bordersize1, 0.16, 0.01);
		}
		else
		{
			bordersize0 = blend(bordersize0, 0, 0.01);
			bordersize1 = blend(bordersize1, 0.08, 0.01);
		}
	}
	static real qunbaisize = 0.52;	
	{// »π∞⁄≥ﬂ¥Á
		if (state == 7 || state == 6)
		{
			qunbaisize = blend(qunbaisize, 0.85, 0.01);

		}
		else if (state == 10)
		{
			qunbaisize = blend(qunbaisize, 0.85, 0.01);
		
		}
		else
		{
			qunbaisize = blend(qunbaisize, 0.52, 0.01);
	
		}
	}

	// ∂Øª≠
	real eatingani = state == 2 ? blend(0.5, 1.5, 0.5 + 0.5 * sin(gtime * PI * 12.5)) : blend(0.75, 1.25, vellen / 0.01 * (0.5 + 0.5 * FT(gtime * PI * 1.25)));
	real growingani = state == 6 ? blend(1, (bugsize + 0.1) / bugsize, 0.5 + 0.5 * sin(gtime * PI * 10.53)) : 1;

	{// ∆´“∆
		static vec2 olddir = dir; if (dir.len() > 0.0001)	olddir = blend(olddir, dir, 0.25);
		o = o - olddir.normcopy() * (bugsize * 0.03);
	}

	real deta;
	static real dlen = dirlen * 5000000;
	{// ◊Ï∞Õ±‰–°µƒ∂Øª≠
		deta = blend(PI / 8, PI / 2, vellen / 0.01) * eatingani;
		if (state == 7 || state == 3 || state == 5 || state == 10)
			dlen = blend(dlen, 0, 0.01);
		else
			dlen = blend(dlen, dirlen * 5000000, 0.01);
		if (dlen > 0.5) dlen = 0.5;
	}

	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);

		real ang = blend(-PI, PI, ai);
		real dang = fabs(dirang - ang); dang > PI ? dang = 2 * PI - dang : 0;
		vec2 dp = vec2(cos(ang), sin(ang)) * (0.05 + dirlen * 15.0 * (0.5 + 0.5 * cos(dang * 2)));
		vec2 p = o + dp;
		real a2d = blend2d(0.5, 1.2, p.x, p.y);
		//if (lifestep > 1)
		if (drawstep == 1)
			point5(o, 0.0025); // –ƒ

		//if (state != 3 && state != 5 && state != 10)
		{// ◊Ï			
			if (dang < deta)
			{
				if (dang / deta < 0.5)
					dp = dp + dp * blend2(
						blend2(0, -dlen / 2, dang / deta),
						dlen, dang / deta, 0.5);
				else
					dp = dp + dp * blend2(
					blend2(0, -dlen / 2, dang / deta),
					dlen, dang / deta, 1.25);
			}
		}

		real sz = bugsize * growingani * MIN(0.1 + a2d, 1.0);
		dp = dp * sz;

		vec2 pp0 = o + dp;

		{// ±ﬂΩÁ–ßπ˚
			if (pp0.x < 0.05)
			{
				dp.x *= blend(0.6, 1, pp0.x / 0.05);
				pp0 = o + dp;
			}
			else if (pp0.x > MAXX - 0.05)
			{
				dp.x *= blend(0.6, 1, (MAXX - pp0.x) / 0.05);
				pp0 = o + dp;
			}
			if (pp0.y < 0.05)
			{
				dp.y *= blend(0.6, 1, pp0.y / 0.05);
				pp0 = o + dp;
			}
			else if (pp0.y > MAXY - 0.05)
			{
				dp.y *= blend(0.6, 1, (MAXY - pp0.y) / 0.05);
				pp0 = o + dp;
			}
		}

		pixelBug(pp0, i);
		real a2d1 = blend2d(0.5, 1.2, pp0.x, pp0.y);

		// ∏˘æ›µ±«∞ƒ‹¡øªÊ÷∆
		if (rrnd(0, 10) * moveenerge < energefood * bugsize)
			pixelBug1(o + dp * rrnd(0, 1), i);
		
		if (lifestep > 1)
		{// »π∞⁄
			if (i % 2 == 0)
			{							
				//deta1 = (1 + (state == 10 ? 0.85 : 0.52) * a2d1 * sz * (0.75 + 0.25 * FT1(ai * PI * 30 + a2d1 * PI * 0.72 + gtime * PI * 2)));
				real deta0 = (0.75 + 0.25 * FT1(ai * PI * 30 + a2d1 * PI * 0.72 + gtime * PI * 2));			

				real deta1 = (qunbaisize * a2d1 * sz * deta0);
				deta1 = (state == 7 || state == 10 || state == 6 ? deta1 : blend(0, deta1, dang / (0.8 * PI), 0.5));

				vec2 tpp = o + dp * (1 + deta1);
				pixelBug1(tpp, i);

				/*static real squnbaiang = 0;
				static const real sdng = 2 * PI / real(len);
				squnbaiang += sdng;
				if (squnbaiang > PI / 30 && deta0 > 0.8)
				{
					squnbaiang = 0;
					pixelBug(o + dp * (1 + deta1 / 2), i);
				}*/
				if (i % 10 == 0)
				{					
					pixelBug(o + dp * (1 + deta1 / 2), i);
				}
			}			

		}
		else
		{
			if (i % 8 == 0)
			{// ÃÂ√´
				real ddv = blend(bordersize0, bordersize1, dang / (0.6 * PI));

				dp.rot(PI * 0.2 * (sin(ai * PI * 10 + a2d1 * PI * 0.72 + gtime * PI * 1)));
				pixelBug1(pp0 + dp * (ddv), i);
				pixelBug1(pp0 + dp * (2 * ddv), i);
			}
		}

		if (lifestep >= 3 && !isdrawtail && fabs(dang - PI) < 0.02)
		{// Œ≤∞Õ
			static float s_t0[] = {	rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
			static float s_t1[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
			static vector<vec> s_tailpts;
			isdrawtail = 1;
			real velfac = blend(1, 0, vel.sqrlen() / 0.01);
			vec tdir = dir;
			tdir.norm();
			tdir.rot(PI * 0.2 * sin(gtime * 1.1) * velfac);

			int len2 = len0 / 4 * bugsize;
			vec2 pp = pp0;	
			
			real a2d0 = blend2d(0.1, 1, pp0.x, pp0.y);

			real lenstep = sz * 0.006;
			int iistep = 1;
			int cnt = 0;
			for (int ii = 0; ii < len2 - 4; ii += 4)
			{
				real aii = ii / real(len2);			
				real ft = FT(gtime * .5 * PI * rndmap2[1][1] + aii * blend(0, PI, aii), s_t0);
				real a2d = blend2d(-1, 1, pp.x, pp.y);
				vec2 vv = blend(tdir, ldir, aii);
				pp = pp - vv.rotcpy(blend(0, ft * 2 + 0.25 * a2d, aii) * velfac) * lenstep;
				//pp = pp - vv * lenstep;
				//pixelBug1(pp, ii);

				if (cnt >= s_tailpts.size())
				{
					pixelBug1(pp, ii);
					s_tailpts.push_back(pp);
				}
				else
				{
					vec& ropp = s_tailpts[cnt];
					ropp = blend(ropp, pp, (1 - aii) + 0.05);
					pixelBug1(ropp, ii);					
				}
				cnt++;
			}
			ldir = blend(ldir, tdir, 0.1);
			ldir.norm();
		}		
	}	

}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ±‚≥Ê‘Ï–Õ
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void protozoa2(vec2 p0, vec2 dir, int len0)
{	
	real deta0 = 1;
	if (gtime - picktime <= 0.25)
	{// µ„ª˜∞⁄∂Ø–ßπ˚
		dir.rot(blend(.1, 0, (gtime - picktime) / 0.25) * sin(15.5 * PI * (gtime - picktime)));
		deta0 = blend(1.5, 1, (gtime - picktime) / 0.25);
	}

	pixel_size1 = 1;
	dir.norm();
	static vec2 olddir = dir; 
	if (dir.len() > 0.0001)
		olddir = blend(olddir, dir, 0.25);
	else
		dir = olddir;

	p0 = p0 - olddir.normcopy() * 0.025 * bugsize;
	
	real eatingani = state == 2 ? blend(0.8, 1.5, 0.5 + 0.5 * sin(gtime * PI * 12.5)) : 1;
	real growingani = state == 6 ? blend(1, (bugsize + 0.4) / bugsize, 0.5 + 0.5 * sin(gtime * PI * 5.53)) : 1;

	if (state != 6)
	{// ¥•–Î
		static float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };	

		vec2 pp1 = p0 + vec2(-dir.y, dir.x) * 0.008;
		vec2 pp2 = p0 - vec2(-dir.y, dir.x) * 0.008;
		real ft = FT(gtime * 0.5, s_t0);
		vec2 vv1 = dir.rotcpy(PI / 5 * eatingani + PI * 0.1 * ft);
		vec2 vv2 = dir.rotcpy(-PI / 5 * eatingani - PI * 0.1 * ft);
		int len = lifestep == 3 ? 0.55 * len0 * bugsize : len0 * bugsize / 6;
		len *= 1.5;

		static real chuxuang1 = 0.18;		
		chuxuang1 = state == 10 ? blend(chuxuang1, -0.1 * ft, 0.01) : 0.18;
		static real chuxuang2 = 0.18;
		chuxuang2 = state == 10 ? blend(chuxuang2, -0.1 *  FT(gtime * 0.5 + PI * 0.35, s_t0), 0.01) : 0.18;

		static real chuxulen = 0.0025;
		chuxulen = state == 10 ? blend(chuxulen, 0.0025 + 0.001 *ft, 0.01) : 0.0025;
		if (lifestep == 3)	chuxulen = 0.0018;

		for (int i = 0; i < len; i += 2)
		{
			real ai = i / real(len);

			pp1 = pp1 + vv1 * chuxulen;
			vv1.rot(PI * blend(0, chuxuang1 * eatingani * growingani, ai) * (0.75 + 0.15 * ft));
			pixelBug1(pp1, i);

			pp2 = pp2 + vv2 * chuxulen;
			vv2.rot(-PI * blend(0, chuxuang2 * eatingani * growingani, ai) * (0.75 + 0.15 * ft));
			pixelBug1(pp2, i);

			if (i >= len - 2 && drawstep == 1)
			{
				point5(pp1, 0.001);
				point5(pp2, 0.001);
			}

		}
	}
	if (drawstep == 1)
		point5(p0, 0.002); // –ƒ

	static vec2 ldir3 = ldir;
	{// …ÌÃÂ≤ø∑÷		
		vec2 p1 = p0;
		vec2 p2 = p0;
		vec2 pp;
		int len = len0 / 2;
		real sz0 = state == 6 ? 7 : 1;
		real a2d0 = lifestep == 1 ? 4 : 6.8;
		real a2d1 = lifestep == 1 ? 2.0 : 1.2;
		for (int i = 1; i < len; i++)
		{
			real ai = i / real(len);
			real ang = blend(0, PI, ai);
			real a2d = blend2d(a2d0, a2d1, ai, p1.y);
			real deta = deta0 * blend(bugsize * 0.04, 0.0, ai, 0.25 * a2d) * growingani; // øÌ∂»
			if (state == 6)// …˙≥§÷–
				deta = growingani * 4 * bugsize * 0.0075 * blend2d(0.5, 1.5, p1.x, p1.y);

			vec2 tdir = blend(dir, ldir3, ai);
			vec2 dzui = vec2(-tdir.y, tdir.x) * blend(blend(0.0028, 0, ai / 0.1, 2) * eatingani, 0, ai); // ◊Ï
			{
				p1 = p1 - tdir * (bugsize * 0.0012 / sz0);
				vec2 vv = tdir.rotcpy(ang);
				pp = p1 + vv * deta;				
				if(state != 6)
					pp = pp + dzui;
				if (lifestep != 1 || i % 2 == 0)
					pixelBug(pp, i);

				if (state == 6 || 
					i > len * 0.2 * blend2d(0.5, 1.0, ai, p1.y))
				{
					// ∏˘æ›µ±«∞ƒ‹¡øªÊ÷∆
					if (rrnd(0, 10) * moveenerge < energefood * bugsize ||
						state == 6)
						pixelBug1(p1 + dzui * rrnd(0, 1) + vv * rrnd(0, deta), i);
					if (lifestep > 1)
						pixelBug1(pp + vv * (0.0075 * bugsize * (0.6 + 0.4 * sin(PI * ai * 20 + gtime * 11.0))), i);
				}
				if (lifestep == 1 && 
					i % 8 == 0 && i > len * 0.2)
				{// ”◊ƒÍ ±∆⁄µƒ√´
					vv.rot(PI * 0.2 * (sin(ai * PI * 10 + a2d1 * PI * 0.72 + gtime * PI * 1)));
					pixelBug1(pp + vv * (0.0025), i);
					pixelBug1(pp + vv * (2 * 0.0025), i);
				}

				if (lifestep == 3 
					&& i % 12 == 0 && i > len * 0.2)
				{// ≥…≥Ê∆¯ø◊
					pixelBug(pp - vv * (0.003), i);
				}
			}
			{
				p2 = p2 - tdir * (bugsize * 0.0012 / sz0);
				vec2 vv = tdir.rotcpy(-ang);
				pp = p2 + vv * deta;
				if (state != 6)
					pp = pp - dzui;
				if (lifestep != 1 || i % 2 == 0)
					pixelBug(pp, i);

				if (state == 6 || 
					i > len * 0.2 * blend2d(0.5, 1.0, ai, p1.y))
				{
					// ∏˘æ›µ±«∞ƒ‹¡øªÊ÷∆
					if (rrnd(0, 10) * moveenerge < energefood * bugsize || 
						state == 6)
						pixelBug1(p1 + dzui * rrnd(0, 1) + vv * rrnd(0, deta), i);
					if (lifestep > 1)
						pixelBug1(pp + vv * (0.0075 * bugsize * (0.6 + 0.4 * sin(PI * ai * 20 + gtime * 12.0))), i);
				}
				if (lifestep == 1 && 
					i % 8 == 0 && i > len * 0.2)
				{// ”◊ƒÍ ±∆⁄µƒ√´
					vv.rot(PI * 0.2 * (sin(ai * PI * 10 + a2d1 * PI * 0.72 + gtime * PI * 1)));
					pixelBug1(pp + vv * (0.0025), i);
					pixelBug1(pp + vv * (2 * 0.0025), i);
				}
				if (lifestep == 3
					&& i % 12 == 0 && i > len * 0.2)
				{// ≥…≥Ê∆¯ø◊
					pixelBug(pp - vv * (0.003), i);
				}
			}
			if (i == len - 1)
			{
				drawpos = (p1 + p2) / 2;
			}

		}
	}
	ldir3 = blend(ldir3, dir, 0.05);
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Œ⁄‘Ù‘Ï–Õ
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline float FT2(float angle, float t[] = 0, float dt = 0)
{
	if (t == 0)
	{
		static float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
		t = s_t0;
	}

	float yy = 0;
	yy += 1 * sin(1 * angle + (t[0] + dt) * PI);
	yy += 0.5	* sin(2 * angle + (t[1] + dt) * PI);
	return yy;
}

void protozoa3(vec2 o, vec2 dir, int len0)
{
	static vec2 ldir3 = ldir;
	pixel_size1 = 1;
	vec2 p1 = o;
	vec2 p2 = o;
	dir = -dir;
	vec2 v1 = vec2(dir.y, -dir.x).normcopy();
	vec2 v2 = -v1;
	real eatingani = state == 2 ? blend(0.8, 1.2, 0.5 + 0.5 * sin(gtime * PI * 2.5)) : 1;

	real a2dO = blend2d(0.5, 2.0, o.x, o.y);	

	vec2 mouthpt1;
	vec2 mouthpt2;
	if (state == 11)
	{// À∫“ß∂Øª≠
		real mouthang = PI * 0.18;
		eatingani = blend(0.1, 0, vel.len() / 0.001);

		mouthpt1 = o - dir.normcopy().rotcpy(mouthang * a2dO * (0.5 + 0.5 * cos(eatingani * gtime * PI * 5))) * 0.015;
		mouthpt2 = o - dir.normcopy().rotcpy(-mouthang * a2dO * (0.5 + 0.5 * cos(eatingani * gtime * PI * 5))) * 0.015;
	}
	else if (state == 2)
	{
		mouthpt1 = o - dir.normcopy().rotcpy(PI / 8 * a2dO * (0.5 + 0.5 * sin(4 * gtime * PI * 5))) * 0.015;
		mouthpt2 = o - dir.normcopy().rotcpy(-PI / 8 * a2dO * (0.5 + 0.5 * sin(4 * gtime * PI * 5))) * 0.015;
	}
	else
	{
		mouthpt1 = o - dir.normcopy().rotcpy(PI / 10 * a2dO * (0.5 + 0.5 * sin(eatingani * gtime * PI * 5))) * 0.015;
		mouthpt2 = o - dir.normcopy().rotcpy(-PI / 10 * a2dO * (0.5 + 0.5 * sin(eatingani * gtime * PI * 3.2134 + 1.35))) * 0.015;
	}

	real growingani = state == 6 ? blend(1, (bugsize + 0.2) / bugsize, 0.5 + 0.5 * sin(gtime * PI * 5.53)) : 1;
	
	//point5(o, 0.002); // –ƒ

	int len = len0 / 2;

	// –›œ¢‘Ï–Õ
	static real rotang = 1;
	rotang = (state == 10) ? blend(rotang, 0.1 * a2dO + 0.025 * (sin(gtime * 0.25)), 0.005) : blend(rotang, 1, 0.01);
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		{// ◊Û±ﬂ
			real a2d = blend2d(0.5, 1.2, p1.x, p1.y);
			p1 = p1 + v1 * (0.001 * bugsize * growingani * a2d);
			v1.rot(blend(0, rotang * PI * 0.05 * a2d, ai, 1));
			if (i < 0.1 * len && state != 3 && state != 5 && state != 6 && state != 10 && state != 7)
			{
				pixelBug(blend(mouthpt1, p1, i / (0.1 * len)), i);
			}
			else
				pixelBug(p1, i);
		}
		{// ”“±ﬂ
			real a2d = blend2d(0.5, 1.2, p1.x, p1.y);
			p2 = p2 + v2 * (0.001 * bugsize * growingani * a2d);
			v2.rot(blend(0, -rotang * PI * 0.05 * a2d, ai, 1));
			if (i < 0.1 * len && state != 3 && state != 5 && state != 6 && state != 10 && state != 7)
			{
				pixelBug(blend(mouthpt2, p2, i / (0.1 * len)), i);
			}
			else
				pixelBug(p2, i);
		}
		
		if (i == int(0.6 * len))
		{
			static float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
			int len1 = len0 / 2;
			for (int ii = 0; ii < len1; ii++)
			{
				real aii = ii / real(len1);
				vec2 p = blend(p1, p2, aii);				

				real a2d = blend2d(0.8, 1.2, aii, p.y);

				vec2 tdir = blend(ldir3, dir, fabs(aii - 0.5) / 0.5);

				tdir = blend(tdir.rotcpy(-PI / 4 * a2d), tdir.rotcpy(PI / 4 * a2d), aii);
				vec2 vv = tdir.normcopy() * blend2(0, 0.1 * growingani * bugsize * a2d, aii, 2);

				p = p + vv * blend2(0, 0.01 * a2d, aii, 2); // Õª≥ˆ“ªµ„

				real rr;
				//real ft = FT(sin(gtime * 4.1 * blend2d(0.8, 1.2, p.x, p.y)) * PI * 0.2, s_t0);
				if (lifestep == 3)
					rr = 0.65 + (0.5 + 0.2 * FT2(aii * 18 * a2d + gtime * PI * 0.3, s_t0));
				else if(lifestep == 2)
					rr = 0.3 + (0.5 + 0.2 * FT2(aii * 10 * a2d + gtime * PI * 0.3, s_t0));
				else
					rr = (0.5 + 0.2 * FT(aii * PI * a2d + gtime * PI * 0.3, s_t0));

				pixelBug1(p + vv * rr, ii);

				if (ii % 8 == 0)
				{
					pixelBug(p + vv * (rr * 0.9), ii);
				}

				// ∏˘æ›µ±«∞ƒ‹¡øªÊ÷∆
				if (rrnd(0, 10) * moveenerge < energefood)
					pixelBug1(p + vv * rrnd(rr * 0.5, rr * 0.7), ii);
			}
		}
	}
	ldir3 = blend(ldir3, dir, 0.1);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ¬›‘Ï–Õ
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void protozoa4(vec2 o, vec2 dir, int len0)
{
	static float s_t[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };

	real idelani = FTU(gtime * PI, s_t);

	//real sz = 0.5; pixel_size0 = 2;
	real sz = 0.25;
	dir = -dir;
	dir.norm();
	vec2 p1, p2;
	{// ¬›ø«
		vec2 p = o;
		vec2 v = dir;
		int len = len0;
		real dt = 1.0 / len;
		vec2 vv;
		int lst = 0;
		real ang1 = PI * 3.9;
		for (int i = 0; i < len; i++)
		{
			real ai = i / real(len);

			real ang = blend(0, ang1, ai, 0.5);
			vv = v.rotcpy(ang);
			p = p + vv * blend(0.0, sz * 0.0025, ai);

			pixelBug(p, i);

			if (i - lst > 10 || i == len - 1)
			{// ±¥ø«Œ∆¿Ì
				lst = i;
				int len2 = blend(0, len / 50, ai);
				vec2 pp = p;
				real step = sz * blend(0.0001, 0.006, ai);
				vec2 tvv = blend(vec2(vv.y, -vv.x), vv, 0.3);
				for (int ii = 0; ii < len2; ii++)
				{
					pp = pp - tvv * (blend(step, step * 6, real(ii) / len2));
					if (i != len - 1)
						pixelBug(pp, ii);

					// —€µ„
					if (ii == len2 - 1 && i == len - 1)
					{
						for (int jj = 0; jj < 20; jj++)
						{
							pixel(pp + dir.rotcpy(jj * PI * 0.1) * 0.005);
							pixel(pp + dir.rotcpy(jj * PI * 0.1) * 0.002);
						}
					}
				}
			}

			if (i == len - 1)
			{
				p1 = p;
			}
			if (i == int(len / 4))
			{
				p2 = p;
			}
		}

	}
	vec2 vdir = dir;
	vec2 vldir = ldir;
	{// ¬›ÃÂ

		int len = len0 / 3;
		int chuxucnt = 0;
		for (int i = 0; i < len; i++)
		{
			real ai = i / real(len);
			vec2 p = blend(p1, p2, ai);
			real a2d = blend2d(.25, 2, p.x, p.y);
			vec2 tvv = vdir.rotcpy(PI * 0.1 * sin(ai * PI * 4.2 + (gtime * 2.35) * PI));
			//pixel(p);
			pixel(p + tvv * (blend2(0, sz * 0.01, ai, 4) * (0.6 + 0.4 * sin(a2d * ai * PI * 2 + (gtime * 1.2) * PI))));
			pixel(p + tvv * (blend2(0, sz * 0.05, ai, 4) * (0.65 + 0.35 * sin(a2d * ai * PI * 6 + (gtime * 4) * PI))));

			if (i == int(len / 4) || i == int(len - len / 4))
			{
				{// ¥•–Î
					static float s_t0[][4] = {
						{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
						{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) }
					};
					static float s_ang0[] = { rrnd(-1, 1), rrnd(-1, 1) };

					int len1 = len0 / 8;
					real a2d0 = blend2d(0.1, 1, p1.x, p1.y);

					vec2 pp = p;

					real lenstep = sz * blend(0.008, 0.01, rndmap[50][i]);
					for (int ii = 0; ii < len1; ii += 2)
					{
						real aii = ii / real(len1);
						vec2 vv = blend(vdir, vldir, aii);// .rotcpy(PI / 3 * rndmap[0][i]);
						real ft = FT(gtime * PI * s_ang0[chuxucnt % 2] + aii * blend(0, PI, aii), s_t0[chuxucnt % 2]);
						real a2d = blend2d(-1, 1, pp.x, pp.y);

						pp = pp + vv.rotcpy(blend(0, ft * 4 * a2d, aii) * blend(1, 0, vel.sqrlen() / 0.01)) * lenstep;
						pixel(pp);
					}
					chuxucnt++;
				}
			}
		}
	}
	ldir = blend(ldir, dir, 0.1);

}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ∫£–«‘Ï–Õ
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void protozoa5(vec2 o, vec2 dir, int len0)
{
	static float s_t[5] = { rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1) };
	real r0 = 0.005;
	real r1 = 0.0125;
	vec2 p, tp;
	for (int i = 0; i < 5; i++)
	{
		real ang1 = PI / 5 * 2 * i;
		vec2 p1 = o + vec2(r0 * cos(ang1), r0 * sin(ang1));
		real ang2 = PI / 5 * 2 * (i + 1);
		vec2 p2 = o + vec2(r0 * cos(ang2), r0 * sin(ang2));

		vec2 pm = blend(p1, p2, 0.5);
		vec2 vv = (pm - o).normcopy();
		vec2 pp = o + vv * r1;

		int len = len0 / 10;
		for (int ii = 0; ii < len; ii++)
		{
			real aii = real(ii) / len;
			real ft = sin(gtime * PI * 0.2 + s_t[i % 5] * 2 * PI);
			real a2d = blend2d(-1, 1, p.x, p.y);
			vec2 tv = vv.rotcpy(blend(0, PI * a2d * 0.1 + 0.1 * PI * ft, aii));
			pp = o + tv * r1;
			p = blend(p1, pp, aii);
			pixel(p);
			tp = blend(o, pp, aii);
			if (ii % 8 <= 3)
				pixel(tp);
			if (rand() % 100 < 30)
				pixel(blend(tp, p, rrnd(0, 1)));

			p = blend(p2, pp, aii);
			pixel(p);
			tp = blend(o, pp, aii);
			if (ii % 8 <= 3)
				pixel(tp);
			if (rand() % 100 < 30)
				pixel(blend(tp, p, rrnd(0, 1)));
		}
	}
}

// -----------------------------------------------------------------------------------------------
// ‘≠…˙∂ØŒÔ∏˜–ŒÃ¨
// -----------------------------------------------------------------------------------------------
void protozoa(vec2 o, vec2 dir, int len)
{
	// ªÊ÷∆
	drawstep = 1;
	if (shape == 1)
	{
		if (aniprogress != 0)
		{
			drawstep = blend(1, 50, aniprogress / 1500.0);			
			protozoa1(o, dir, len);
			drawstep = blend(250, 1, aniprogress / 1000.0);
			lifestep = 1;
			protozoa2(o, dir, len);
			lifestep = 3;
		}
		else
			protozoa1(o, dir, len);		
	}
	else if (shape == 2)
	{		
		if (aniprogress != 0)
		{
			drawstep = blend(1, 50, aniprogress / 1500.0);
			protozoa2(o, dir, len);
			drawstep = blend(250, 1, aniprogress / 1000.0);
			lifestep = 1;
			protozoa3(o, dir, len);
			lifestep = 3;
		}
		else
			protozoa2(o, dir, len);
	}
	else if (shape == 3)
	{
		if (aniprogress != 0)
		{
			drawstep = blend(1, 50, aniprogress / 1500.0);
			protozoa3(o, dir, len);

			drawstep = blend(25, 1, aniprogress / 1000.0);
			aniXibaozhuan(pos, -vel, -vel, 100, 0);
		}
		else
			protozoa3(o, dir, len);		
	}

	if (state == 7)
	{// ÀØ√ﬂ–ßπ˚
		emitParticles(pos, vec2(rrnd(-.015, .015), rrnd(0.05, .1)));
		drawSleepingZEff();
	}	
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ‘≠…˙∂ØŒÔ≥°æ∞ ±‰–Œ≥Ê≥°æ∞
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
extern void unkownplankter();
void protozoaScene()
{		
	//  ≥ŒÔ¬ﬂº≠
	if (lifestep > 0)
	{
		if (shape == 1)
		{// µ⁄“ª–ŒÃ¨

		 //  ≥ŒÔ√ø∏Ùº∏√Î…˙≥§≥ˆ¿¥
			float fddetatime1 = 5;
			if (gtime - grasslstcrttime > fddetatime1)
			{
				grasslstcrttime = gtime;
				{
					if (lifestep == 1)
					{// µ⁄“ª–ŒÃ¨µ⁄“ªΩ◊∂Œ◊‘∂Ø…˙≥§≤ª–Ë“™Œπ ≥
						fddetatime1 = 9;
						int ii = 0;
						for (; ii < GRASSNUM; ii++)
						{
							if (grasses[ii].grassstates == -1 &&
								gtime - grasses[ii].grassdietime > 2)
							{
								grasses[ii].grassposmap = vec2(rrnd(0.1, MAXX * 0.9), rrnd(0.1, MAXY * 0.85));
								grasses[ii].grassshape = rand() % (lifestep);
								grasses[ii].grassstates = 0;
								grasses[ii].grassborntime = gtime;

								playditasound(rand() % NUMDEDASOUND);

								break;
							}
						}
					}
					else if (lifestep == 2 && state != 6)
					{// µ⁄“ª–ŒÃ¨µ⁄∂˛Ω◊∂Œ◊‘∂Ø…˙≥§≤ª–Ë“™Œπ ≥ µ´ «≤ªª·÷ÿ∏¥…˙≥§
						fddetatime1 = 10;
						static bool sbreset = 1;
						if (eatnum >= GRASSNUM) {
							sbreset = 1;
						}
						if (sbreset)
						{
							sbreset = 0;

							for (int ii = 0; ii < GRASSNUM; ii++)
							{
								if (grasses[ii].grassstates == -1)
								{
									grasses[ii].grassposmap = vec2(rrnd(0.1, MAXX * 0.9), rrnd(0.1, MAXY * 0.85));
									grasses[ii].grassshape = rand() % (lifestep);
									grasses[ii].grassstates = 0;
									grasses[ii].grassborntime = gtime;

									playditasound(rand() % NUMDEDASOUND);
								}
							}
						}
					}
					else if (lifestep == 3 && state != 6)
					{// µ⁄“ª–ŒÃ¨µ⁄»˝Ω◊∂Œ◊‘∂Ø…˙≥§≤ª–Ë“™Œπ ≥
						fddetatime1 = 6;

						static bool sbreset = 1;
						if (sbreset)
						{
							sbreset = 0;
							for (int ii = 0; ii < GRASSNUM; ii++) // «Â¿Ì ≥ŒÔ
							{
								grasses[ii].grassstates = -1;
							}
						}

						int ii = 0;
						for (; ii < GRASSNUM; ii++)
						{
							grass_t &gs = grasses[ii];
							if ((gs.grassstates == -1) &&
								(gtime - gs.grassdietime > 20 || gs.grassdietime == 0)
								//&& (grasses[ii].grassdietime < leveluptime || leveluptime == 0)
								)
							{
								gs.grassposmap = vec2(rrnd(0.1, MAXX * 0.9), rrnd(0.1, MAXY * 0.85));
								gs.grassshape = rand() % (lifestep);
								gs.grassstates = 0;
								gs.grassborntime = gtime;

								playditasound(rand() % NUMDEDASOUND);

								break;
							}
						}
					}
				}
			}
		}

		if( (shape == 2 || shape == 3) &&
			gtime - grasslstcrttime > 12)
		{// µ⁄∂˛»˝–ŒÃ¨
			grasslstcrttime = gtime;
			if (shape == 2)
			{
				//	if (lifestep == 1)
				{// µ⁄∂˛–ŒÃ¨µ⁄“ªΩ◊∂Œ–Ë“™Œπ ≥
					static bool sfirsttime = 1;
					if (sfirsttime == 1)
					{
						sfirsttime = 0;
						for (int ii = 0; ii < GRASSNUM; ii++) // «Â¿Ì ≥ŒÔ
						{
							grasses[ii].grassstates = -1;
						}
						grasses[0].grassweishi = 0; // µ⁄“ª¥Œ ”––ß
					}
					else
					{
						int ii = 0;
						for (; ii < GRASSNUM; ii++)
						{
							if (grasses[ii].grassstates == -1 &&
								(grasses[ii].grassdietime == 0 || gtime - grasses[ii].grassdietime > 5) &&
								grasses[ii].grassweishi != 0)
							{
								grasses[ii].grassweishi = 0;
								break;
							}
						}
					}
				}

				if (lifestep == 2 || lifestep == 3)
				{// µ⁄∂˛–ŒÃ¨µ⁄∂˛Ω◊∂Œ”–÷≤ŒÔ≥§≥ˆ ≥ŒÔ

					for (int ii = 0; ii < GRASSNUM; ii++)
					{
						grass_t& grass = grasses[ii];
						if (grass.grassstates == 0 &&
							gtime - grass.grassweishi > 5)
						{
							if (grass.shengzhang == 1)
							{
								grass.shengzhang = 2;
								if (grass.growgrasses.size() < 3)
								{
									grass.growgrasses.resize(3);
									for (int jj = 0; jj < 3; jj++)
									{
										grass_t& gs = grass.growgrasses[jj];

										gs.grassshape = rand() % (lifestep);
										gs.grassstates = 0;
										gs.grassborntime = gtime;
										playditasound(rand() % NUMDEDASOUND);
										gs.parent = &grasses[ii];
									}
								}
							}
							else if (grass.shengzhang == 2 &&
								grass.growgrasses.size() == 3)
							{
								int eatencnt = 0;
								for (int jj = 0; jj < 3; jj++)
								{
									grass_t& gs = grass.growgrasses[jj];
									if (gs.grassstates == -1)
									{// ±ª≥‘µÙ¡À
										eatencnt++;
									}
								}
								if (eatencnt >= 3)
								{
									grass.shengzhang = 0;
									grass.grassstates = -1;
									grass.grassdietime = gtime;
									grass.growgrasses.clear();
								}
							}

							break;
						}
					}
				}
				if (lifestep == 3)
				{// µ⁄∂˛–ŒÃ¨µ⁄»˝Ω◊∂Œ”–÷≤ŒÔ≥§≥ˆ ≥ŒÔ ◊‘∂Øœ˚ ß

					for (int ii = 0; ii < GRASSNUM; ii++)
					{
						grass_t& grass = grasses[ii];
						if (grass.grassstates == 0 && grass.shengzhang == 2 &&
							gtime - grass.grassweishi > 28)
						{
							grass.shengzhang = 0;
							grass.growgrasses.clear();
							grass.grassstates = -1;
						}
						else if (grass.grassstates == 0 && grass.shengzhang == 0 &&
							gtime - grass.grassweishi > 32)
						{
							grass.grassstates = -1;
						}
					}
				}
			}
			else if (shape == 3)
			{// µ⁄»˝–ŒÃ¨	
				if (lifestep == 1)
				{//   ≥ŒÔ‘⁄‘À∂Ø	
					// ◊‘∂Ø…˙≥§
					for (int ii = 0; ii < GRASSNUM; ii++)
					{
						if (grasses[ii].grassstates == -1 &&
							gtime - grasses[ii].grassdietime > 1)
						{
							grasses[ii].grassposmap = vec2(rrnd(0.1, MAXX * 0.9), rrnd(0.1, MAXY * 0.85));
							grasses[ii].grassshape = rand() % (lifestep);
							grasses[ii].grassstates = 0;
							grasses[ii].grassborntime = gtime;
							playditasound(rand() % NUMDEDASOUND);
							break;
						}
					}
				}
				else if (lifestep == 2)
				{// ∏ƒŒ™ ≥»‚
					{// ∞—‘Â¿‡∏…µÙ
						static bool first = 1;
						if (first)
						{
							first = 0;
							for (int ii = 0; ii < GRASSNUM; ii++)
							{
								grasses[ii].grassstates = -1;
							}
						}
					}
					{// ∞—∏°”Œ∂ØŒÔ“˛≤ÿ
						static bool first = 1;
						if (first)
						{
							first = 0;
							FOREACH(i, PLANKTER_NUM)
							{
								planker_t& ptr = plankers[i];
								if (!ptr.binit)
									ptr.init();
								ptr.state = -1;
							}
						}
					}
					FOREACH(i, PLANKTER_NUM)
					{
						planker_t& ptr = plankers[i];
						if (ptr.state == -1)
						{
							ptr.state = 1; // ◊‘∂ØŒπ—¯
							ptr.catchtime = 0;							
							ptr.energe = 8;
							if (rand() % 100 < 50)
							{
								ptr.pos = vec(0, rrnd(0, 0.9 * MAXY));
								ptr.vel = vec2(rrnd(0.001, 1), rrnd(-1, 1)) * 0.004;
							}
							else
							{
								ptr.pos = vec(MAXX, rrnd(0, 0.9 * MAXY));
								ptr.vel = vec2(-rrnd(0.001, 1), rrnd(-1, 1)) * 0.004;
							}
								
							ptr.dangertime = 0;
							
							break;
						}
					}
				}
				else if (lifestep == 3)
				{// ◊º±∏Œπ—¯

					{// ∞—∏°”Œ∂ØŒÔ“˛≤ÿ
						static bool first = 1;
						if (first)
						{
							first = 0;
							FOREACH(i, PLANKTER_NUM)
							{
								planker_t& ptr = plankers[i];
								ptr.state = -1;
							}
						}
					}
					FOREACH(i, PLANKTER_NUM)
					{
						planker_t& ptr = plankers[i];
						if (ptr.state == -1)
						{
							ptr.state = -2;
							break;
						}
					}
				}
			}

		}
		if (shape == 3)
		{
			if (lifestep == 1)
			{
				{// ∞—’˝‘⁄…˙≥§µƒ‘Â¿‡∏…µÙ
					static bool first = 1;
					if (first)
					{
						first = 0;
						for (int ii = 0; ii < GRASSNUM; ii++)
						{
							if (grasses[ii].growgrasses.size() > 0)
							{
								grasses[ii].shengzhang = 0;
								grasses[ii].growgrasses.clear();
								grasses[ii].grassstates = -1;
							}
						}
					}
				}

				// ‘À∂Øµƒ ≥ŒÔ		
				for (int ii = 0; ii < GRASSNUM; ii++)
				{
					if (grasses[ii].grassstates != -1 && !grasses[ii].beeating)
					{
						grass_t& gs = grasses[ii];
						grasses[ii].grassposmap = grasses[ii].grassposmap + grasses[ii].grassvelmap * GAMESPEED;
						if (grasses[ii].grassposmap.x < 0)
						{
							grasses[ii].grassposmap.x += MAXX;
						}
						if (grasses[ii].grassposmap.y < 0)
						{
							grasses[ii].grassposmap.y += 0.9 * MAXY;
						}
						if (grasses[ii].grassposmap.x > MAXX)
						{
							grasses[ii].grassposmap.x = 0.001;
						}

						if (grasses[ii].grassposmap.y > 0.9 * MAXY)
						{
							grasses[ii].grassposmap.y = 0.001;
						}
					}
				}
			}
			else if (lifestep == 2 || lifestep == 3)
			{// ∏°”Œ∂ØŒÔ

				FOREACH(i, PLANKTER_NUM)
				{// ±ªœ≈µΩ¡À Ã”≈‹
					planker_t& ptr = plankers[i];
					if (ptr.state <= 0)
						continue;

					if ((ptr.state == 1 || ptr.state == 3 && ptr.vel.len() < 0.0005) &&
						(ptr.pos - pos).len() < 0.1)
					{
						ptr.state = 2;
						ptr.dangertime = gtime;
						//	PRINT("±ªœ≈µΩ¡À")
					}
				}

				unkownplankter();
			}
		}
	}

	if (lifestep == 0)
	{// ¬—µƒ◊¥Ã¨
		if (gtime - borntime > 5)
		{
			PRINT("≥‘ªı¥”µ∞¿Ô≥ˆ¿¥¡À£°")
				lifestep++;
		}
	}
	else if (lifestep == -1)
	{// À¿Õˆ◊¥Ã¨
		if (gtime - dietime > 5)
		{
			// ÷ÿ…˙
			born();
			lifestep = 0;
		}
		else
		{// À¿Õˆ–ßπ˚
			drawstep = blend(1, 250, (gtime - dietime) / 5);
		}
	}
	else if (state == 9)
	{// …˙≤°¡À

		if (gtime - illtime > 5)
		{
			// …˙≤°øµ∏¥∫Û ≥‘π˝µƒ∂´Œ˜∂º±ªÕ¬π‚¡À
			eatnum = 0;
			PRINT("…˙≤°øµ∏¥¡À ≥‘π˝µƒ∂´Œ˜∂º±ªÕ¬π‚¡À£° ");
			gainpoweraftersleep();
			powervel();

			state = 0; // ª÷∏¥¡À
		}
		else
		{// 

		}
	}
	else if (state == 10)
	{// –›œ¢

		if (gtime - resttime > 5)
		{
			afterrest();
			powervel();

			state = 0; // ª÷∏¥¡À
		}
		else
		{// 

		}
	}
	else if (shape == 0 || eatnum >= getEatNum(level))
	{
		if (lifestep >= 3 || shape == 0)
		{// …˝º∂
			ialocked = 1;
			vec2 sleeppos = vec2(0.5 * MAXX, 0.5 * MAXY);
			vec2 dis = sleeppos - pos;
			if (state != 3 && dis.len() > 0.1)
			{
				vel = blend(vel, dis.normcopy() * 0.001, 0.5);
				vel = vel * blend(1, 0, aniprogress / 1500.0);
				state = 5;
			}
			else
			{// ±‰–Œ∂Øª≠

				if (aniprogress < 1000)
				{
					aniprogress++;
					if (aniprogress == 1)
					{
						playsound(8);
					}

					vel = blend(vel, dis * 0.1 + dis.normcopy().rotcpy(PI / 2) * 0.0001, 0.01);

					int clen = blend(1, 500, aniprogress / 1000.0);
					jian(sleeppos, 0.12, clen);
					state = 3;
				}
				else if (aniprogress < 1500)
				{
					aniprogress++;
					int clen = blend(500, 1, (aniprogress - 1000) / 500.0);
					jian(sleeppos, 0.12, clen);

					state = 4;
				}
				else
				{
					{// ±‰–Œ“‘º∞…˝º∂
						shape++;
						lifestep = 1;
						if (shape == 4)
						{
							shapclass = 2;
							shape = 1;
							lifestep = 0;
						}

						aniprogress = 0;
						PRINT("±‰–Œ " << shape)
						
						borntime = gtime;
						levelup();
						gcurani = 0;
						pos = vec2(0.5 * MAXX, 0.5 * MAXY);
					}
					
				}
			}
		}
		else
		{// …˝º∂±‰¥Û
			ialocked = 1;
			levelupprogress++;
			if (levelupprogress == 1)
			{
				playsound(7);
			}
			if (levelupprogress < 400)
			{
				if (shape == 1)
					vel = blend(vel, vec2::ZERO, 0.1);
				state = 6; // …˝º∂÷–
			}
			else
			{
				lifestep++;
				levelup();
				powervel();
			}
			actiontime = gtime;
		}
	}

	/////////////////////////////////////////////// ∆’Õ®¬ﬂº≠ ///////////////////////////////////////////////
	else if (lifestep > 0)
	{
		actiontime == 0 ? actiontime = gtime : 0;
		moveenerge = lvcst_moveeng[level - 1];

		if (state == 8)
		{ // Ã”≈‹
			vel = vel * (pow(lvcst_zuli[level - 1], GAMESPEED) * 0.9925);// ◊Ë¡¶
			if (vel.len() < 0.001)
			{
				state = 0;
//				PRINT("ª÷∏¥∆’Õ®ÀŸ∂»")
			}
		}
		else if (eatnum < getEatNum(level))
		{// —∞’“ ≥ŒÔ √Ÿ ≥

			if (shape == 3 && lifestep >= 2)
			{// »‚ ≥
				if (state == 0 || state == 11)
				{
					FOREACH(i, PLANKTER_NUM)
					{
						planker_t& ptr = plankers[i];
						if (ptr.state <= 0)
							continue;

						real dis = (ptr.pos - pos).len();
						if (dis < mindis)
						{
							curplanker = &ptr;
							break;
						}
					}
				}
				{// ¥¶¿Ì ≥ŒÔ
					if (curplanker && curplanker->state >= 0)
					{
						vec2 fooddir = curplanker->pos - pos;
						real dis = fooddir.len();
						if (dis < mindis)
						{
							if (dis < 0.015)
							{
								curplanker->vel = vec2(0, 1);
								if (state != 2)
								{
									curplanker->catchtime = gtime;
									starteattime = gtime;
								}

								if (gtime - starteattime > 2)
								{// ≥‘ÕÍ
									eatnum++;
									eatnumlstsleep++;
									state = 0;
									energefood += 3; // ªÒµ√ƒ‹¡ø∂π
									if (eatnumlstsleep >= 7 && energefood > 4 * moveenerge)
									{
										state = 7; // ≥‘±•ÀØæı
										playsound(4);
										eatnumlstsleep = 0;
										energefood = 8 * moveenerge;
										actiontime = gtime;
									}
									else
									{
										powervel();									
									}

									curplanker->state = -1;
									curplanker->catchtime = 0;								
							
								}
								else
								{// ’˝‘⁄≥‘ »‚
									state = 2;
									actiontime = gtime;
									if (curplanker->state > 0)
									{
										curplanker->catchtime = gtime;
										playsound(0);
									}									
									curplanker->state = 0;
									curplanker->pos = blend(curplanker->pos, pos, 0.025); // Œ¸Ω¯◊Ï¿Ô
								}
								mindis = 0.02 + 0.25 * blend(0.4, 0.8, bugsize);
							}
							else
							{
								if (state != 11)
								{// À∫“ß								
									state = 11;
								}
							
								vel = blend(vel, fooddir* 0.015, 0.2);
							}
						}
						else
							goto WUSHIKEGAN;
					}
					else
						goto WUSHIKEGAN;
				}
			}
			else
			{// ≤› ≥	
				if (state != 7 &&
					(curgrass == 0 || curgrass->grassstates == -1) )
					state = 0;
				
				grass_t* findpgs = 0;
				if (state == 0
					// ||state == 8 && energefood < 2 * moveenerge // ±»Ωœ∂ˆ¡À
					)
				{
					for (int i = 0; i < GRASSNUM; i++)
					{
						grass_t* pgs = &grasses[i];

						if (!posValid(pgs->grassposmap))
							continue;
						if (gtime - pgs->grassborntime < 2) // ∏’…˙≥ˆµƒ∫£≤›≤ª≥‘
							continue;
						if (pgs->grassstates == -1)
							continue;
						if (pgs->shengzhang == 2)
						{
							for (int ii = 0; ii < pgs->growgrasses.size(); ii++)
							{
								if (!posValid(pgs->growgrasses[ii].grassposmap))
									continue;
								if (gtime - pgs->growgrasses[ii].grassborntime < 2) // ∏’…˙≥ˆµƒ∫£≤›≤ª≥‘
									continue;
								if (pgs->growgrasses[ii].grassstates == -1)
									continue;

								real dis = (pgs->growgrasses[ii].grassposmap - pos).len();
								if (dis < mindis)
								{
									findpgs = &pgs->growgrasses[ii];								
									curgrass = findpgs; // µ±«∞‘Â¿‡
									break;
								}
							}
						}
						else
						{
							real dis = (pgs->grassposmap - pos).len();
							if (dis < mindis)
							{
								findpgs = pgs;
								curgrass = findpgs; // µ±«∞‘Â¿‡
								break;
							}
						}
					}
				}

				vec2 fooddir = curgrass != 0 ? curgrass->grassposmap - pos : vec(100, 100);
				real dis = fooddir.len();
				if (curgrass && curgrass->grassstates != -1 &&
					dis < mindis)
				{
					if (dis < 0.01)
					{
						if (state != 2)
							starteattime = gtime;
						if (gtime - starteattime > 2)
						{// ≥‘ÕÍ

							eatnum++;
							eatnumlstsleep++;
							PRINT("eatnum = " << eatnum)
							state = 0;
							energefood += grassenerge[curgrass->grassshape % 5]; // ªÒµ√ƒ‹¡ø∂π
							if (eatnumlstsleep >= 6 && energefood > 3 * moveenerge)
							{
								state = 7; // ≥‘±•ÀØæı
								eatnumlstsleep = 0;
								energefood = 3 * moveenerge;
								playsound(4);
								actiontime = gtime;
							}
							else
							{
								powervel();								
							}

							curgrass->beeating = 0;
							curgrass->grassstates = -1; // ±ª≥‘µÙ¡À
							curgrass->grassdietime = gtime; // À¿Õˆ ±º‰			
							curgrass = 0;
			
						}
						else
						{// ’˝‘⁄≥‘
							if (shape == 1)
							{
								if (vel.len() > 0.00025)
									vel = vel * 0.99; // ÀŸ∂»Ωµœ¬¿¥
							}
							if (state != 2)
							{
								state = 2;
								playsound(0);
							}
							
							curgrass->beeating = 1;
							actiontime = gtime;
						}
						mindis = 0.2 * blend(0.4, 0.8, bugsize);
					}
					else
					{
						
						vel = blend(vel, fooddir.normcopy() * 0.0015, 0.2);
						state = 1;			
					}
				}
				else
					goto WUSHIKEGAN;
			}

			if (0)
			{// Œﬁ ¬ø…∏…					
			WUSHIKEGAN:
				if (gamestate == 1)
				{
					if (state == 7)
					{// ÀØæı
						if (gtime - actiontime > 8)
						{
							state = 0;
							actiontime = gtime;
							gainpoweraftersleep();
							powervel();
							
						}
						else
						{
							vel = blend(vel, vec2::ZERO, 0.01);							
						}
					}
					else
					{
						// ÀŸ∂»π˝¬˝ ≥‘ƒ‹¡ø∂πº”ÀŸ
						if (vel.len() < 0.0002 && state != 7)
						{
							// ∂ˆ¡À ƒ‹¡øπ˝µÕ –›œ¢“ªœ¬
							if (energefood < moveenerge * 1)
							{
								rest();
							}
							else if (!powervel())
							{// –›œ¢“ªœ¬ ¿€¡À
								rest();
							}
							actiontime = gtime;
						}
						else
						{
							//if (shape == 1)
							//	vel.rot(FT(gtime * PI + blend2d(-PI, PI, pos.x, pos.y) * 0.1) * 0.01);
							vel = vel * pow(lvcst_zuli[level - 1] - fracpoint * 0.001, GAMESPEED);// ◊Ë¡¶

						}
					}					
				}
			}
		}
	}
	// ---------------------------------------------
	// ‘À∂Øøÿ÷∆
	if (gamestate == 1 &&
		lifestep > 0 &&		// ”◊≥Ê÷Æ∫Û≤≈ƒ‹∂Ø
		state != 9 &&		// …˙≤°¡À≤ªƒ‹∂Ø
		state != 7 &&		// ¥ÚÌÔ	
		state != 10			// –›œ¢
		)
	{// ‘À∂Ø
		//if (state == 0)
		//{// ÀÊª˙¡¶≥°
		//	real a2d = blend2d(0, 1.0, pos.x, pos.y);
		//	vec2 acc = vec2(0, 0.000001).rotcpy(blend(0, 2 * PI, a2d));
		//	vel = vel + acc * GAMESPEED;
		//}
		if (gcurani == 0)
		{
			if (state != 2 && state != 4 && state != 6)
			{// ÀŸ∂»øÿ÷∆
				real vellen = vel.len();
				
				vec2 npos = pos + vel * GAMESPEED;
				if (npos.x < 0.0)
				{
					vel.x = -vel.x;
					if (vellen < 0.001)
						vel.y = rrnd(-0.0025, 0.0025);
				}
				if (npos.y < 0.0)
				{
					vel.y = -vel.y;
					if (vellen < 0.001)
						vel.x = rrnd(-0.0025, 0.0025);
				}
				if (npos.x > MAXX)
				{
					vel.x = -vel.x;
					if (vellen < 0.001)
						vel.y = rrnd(-0.0025, 0.0025);
				}
				if (npos.y > MAXY * 0.9)
				{
					vel.y = -vel.y;
					if (vellen < 0.001)
						vel.x = rrnd(-0.0025, 0.0025);
				}

				pos = pos + vel * GAMESPEED;
			}
		}
	}
	
	// ---------------------------------------------
	// ªÊ÷∆≤ø∑÷	
	if (shape > 0)
	{
		if (gcurani == 0)
		{// ª∑æ≥
			// ∫£¥¯
			//if (shape == 2 || shape == 3)
			//	kelp0(300, 0.001);

			flushpixels();	pixel_size0 = 1;
			for (int i = 0; i < GRASSNUM; i++)
			{
				drawinggrass = &grasses[i];
				alga(grasses[i].grassposmap + vec2(0.0004 * sin(+rndmap[i][20] * PI + gtime * 8.2), 0.0004 * sin(2.75 + rndmap[i][10] * PI + gtime * 3.8)),
					400,
					0.01 + rndmap[i][1] * 0.25,
					rndmap[i][10] * 2 * PI + 0.4 * PI * sin(1.65 + +rndmap[i][15] * PI + gtime * 1.58),
					i
					);
			}

		}
		{// ªÊ÷∆≥Ê◊”

			flushpixels(); pixel_size0 = 2;
			if (lifestep > 0)
				protozoa(pos, vel, 250);
			else
				luan(pos, 100, 0.02);
		}
	}	
}
// ###############################################################################################
// ÀÆÛ£µƒ ¿ΩÁ
// ###############################################################################################
// ÀÆ‘È
// ------------------------------------------------------------------------------------------------
void daphnia(int len0)
{
	{// ªÊ÷∆
		vec2 p0 = daphnia_pos;
		real sz0 = 0.4;
		vec2 v = daphnia_vel;

		v.norm();
		vec2 p = p0;
		int len = daphnia_dencity * len0;
		for (int i = 0; i < len; i++)
		{
			real ai = real(i) / len0;
			real ang = blend(-PI / 2.2, PI / 2.0, ai);
			v.rot(ang * 0.15);

			p = p + v * (0.0025 * sz0);
			pixel(p);

			if (i == int(len0 * 0.05))
			{// —€µ„
				point5(p + vec2(v.y, -v.x) * (0.006 * sz0), 0.0008 * sz0);
			}
			if (ai > 0.3 && ai < 0.75)
			{// ∂«◊”
				vec2 vv(v.y, -v.x);
				real sz = blend2(0, 0.04, (ai - 0.3) / 0.45, 2) * sz0 * (0.9 + 0.1 * sin(gtime * 13.5));
				vec2 pp = p + vv * sz;
				pixel(pp);
				// ƒ‹¡ø
				if (rand() % 100 < 10)
					pixel(p + vv * rrnd(0.25 * sz, sz));

				if (daphnia_dencity == 1 && (i == int(len0 * 0.4) || i == int(len0 * 0.45)))
				{// ◊¶◊”
					vv.rot(sin(gtime * 28.1 + rndmap2[i][18] * PI * 2) * PI * 0.1);
					for (int ii = 0; ii < 8; ii++)
					{
						pp = pp + vv * (0.002 * sz0);
						pixel(pp);
					}
					vv.rot(PI / 4);
					for (int ii = 0; ii < 4; ii++)
					{
						pp = pp + vv * (0.002 * sz0);
						pixel(pp);
					}
				}
			}
		}
	}
}
extern void protozoa1(vec2 o, vec2 dir, int len0);
extern void protozoa2(vec2 o, vec2 dir, int len0);
extern void protozoa3(vec2 o, vec2 dir, int len0);
extern void protozoa4(vec2 o, vec2 dir, int len0);
void daphnia()
{
	vec2& v0 = daphnia_vel;
	{// ¬ﬂº≠ø… ”ªØ	

		{// ‘À∂ØÀŸ∂»øÿ÷∆
			vec2 npos = daphnia_pos + v0 * GAMESPEED;
			if (npos.x < 0.0)
				v0.x = -v0.x;
			if (npos.y < 0.0)
				v0.y = -v0.y;
			if (npos.x > MAXX)
				v0.x = -v0.x;
			if (npos.y > MAXY * 0.9)
				v0.y = -v0.y;
			daphnia_pos = daphnia_pos + v0 * GAMESPEED;
		}

		// ◊Ë¡¶
		v0 = v0 * pow(0.995, GAMESPEED);
		if (v0.len() < 0.0003)
		{
			v0 = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.0035;
		}

		if (daphnia_state == 1)
		{// ±ª”’∂¸Œ¸“˝
			static real lsttime = gtime;
			if (gtime - lsttime > 1)
			{
				lsttime = gtime;
				vec2 vv = youerpos - daphnia_pos;
				if (vv.len() < 0.25 && vv.len() > 0.02)
				{
					vv.norm();
					real v0len = v0.len();
					v0 = blend(v0, vv, rrnd(0, 0.02));
					v0 = v0.normcopy() * v0len;

					//daphnia_pos = daphnia_pos + vv * 0.001;
				}
			}
		}
		static real xiadaoletime = gtime;
		if (daphnia_state == 4)
		{// ±ªœ≈µΩ¡À
			daphnia_state = 5; // Ã”≈‹
			xiadaoletime = gtime;
			v0 = -v0 + vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.04;
			if (v0.len() > 0.01)
			{// œﬁ÷∆“ªœ¬ÀŸ∂»
				v0.norm();
				v0 = v0 * 0.01;
			}
//			PRINT("Ã”≈‹")
		}
		int tstate = daphnia_state;
		if (daphnia_state == 5 && gtime - xiadaoletime > 5)
		{// ±ªœ≈µΩ¡À£¨5√Î÷”∫Ûª÷∏¥
			daphnia_state = 1;
//			PRINT("ª÷∏¥")
		}
	}
	if (daphnia_state != 3)
	{
		bugsize = 0.25;
		if (daphnia_shape == 0)
			daphnia(80);
		else if (daphnia_shape == 1)
			protozoa1(daphnia_pos, v0, daphnia_dencity * 250);
		else if (daphnia_shape == 2)
			protozoa2(daphnia_pos, v0, daphnia_dencity * 250);
		else if (daphnia_shape == 3)
			protozoa3(daphnia_pos, v0, daphnia_dencity * 250);
		else if (daphnia_shape == 4)
			protozoa4(daphnia_pos, v0, daphnia_dencity * 250);
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// shuixi
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
inline float FT3(float angle, float t[] = 0, float dt = 0)
{
	if (t == 0)	{static float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };t = s_t0;	}

	float yy = 0;
	yy += 1 * sin(1 * angle + (t[0] + dt) * PI);
	yy += 0.25	* sin(0.5 * angle + (t[1] + dt) * PI);
	return yy;
}
void hydra(vec2 p0, vec2 v, int len0, 
	int showtype = 1 // ªÊ÷∆¿‡–Õ 1“‘…œŒ™’˝≥£ªÊ÷∆, 0 ∆’Õ®∂Øª≠ -1 ”√”⁄ÀÆÛ£µΩÀÆƒ∏µƒ∂Øª≠
	)
{	
	pixel_size1 = 1;
//////////////////////////////// µ◊◊˘ ////////////////////////////////
	static bool light = 1;
	if (lifestep == 2 || (showtype != -1 && lifestep == 3 && gtime - leveluptime < 4))
	{// œ¬√Ê¬›ø«
		pixel_size0  = 4; //…Ë÷√µ„µƒ¥Û–°
		static float s_t1[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };

		real yy = 0.05;
		if (lifestep == 2 && gtime - leveluptime < 6)
		{// …˝º∂∂Øª≠1
			real at1 = (gtime - leveluptime) / 6;
			yy = blend(-0.2, 0.05, at1);
		}		
		if (lifestep == 3)
		{// …˝º∂∂Øª≠2
			real at1 = (gtime - leveluptime) / 4;
			yy = blend(0.05, -0.7, at1);
		}

		vec2 pp0(0.5 * MAXX, yy);
		vec2 vv(1, 0); vv.norm();
		real ft = 0.5 + 0.5 * sin(gtime * 0.4);
		real ang0 = PI * 1.2 + (ft * PI / 2);

		int len = 200;
		for (int i = 0; i < len; i++)
		{
			real ai = real(i) / len;
			real ang = -blend(4 * PI, 0.1 * PI, ai) + ang0;
			vec2 vr(cos(ang), sin(ang));
			vec2 pp = pp0 + vr * blend(0.0002, 0.2, ai, 2);

			pixel1(pp);

			{// Œ∆¿Ì
				static vec2 slpp = pp;
				if (i % 8 == 0)
				{
					vec vvr = pp - slpp; vvr.norm();
					for (int ii = 0; ii < 8; ii++)
					{
						real aii = ii / 8.0;
						real dlen1 = blend(0.0005, blend(0.05, 0.06, rndmap[ii][ii]), aii);
						dlen1 *= blend(0, 1, ai);
						dlen1 *= blend(0.1, 2, aii);

						pixel1(pp - vec2(vvr.y, -vvr.x) * dlen1);
					}
				}
				if (i % 8 == 4)
				{
					vec vvr = pp - slpp; vvr.norm();
					pixel(pp - vec2(vvr.y, -vvr.x) * blend(0, 0.02, ai));
				}
				slpp = pp;
			}

			if (i == int(len / 1.25))
			{// Œª÷√”Î∑ΩœÚ
				if (lifestep == 2 && gtime - leveluptime < 2)
				{// …˝º∂∂Øª≠1	
					real at2 = (gtime - leveluptime) / 2;
					p0 = blend(p0, pp, at2);
					v = blend(v, vr, at2);
				}
				else
				{
					p0 = pp;
					v = vr;
				}

			}			
		}		
	}
	else if (lifestep == 3)
	{// œ¬√Ê”–ø√¿º≤›		
		static vec2 pp0;
		static bool isinitpp0 = 0;
		if (!isinitpp0)
		{
			pp0 = vec2(-MAXX + 0.4 * MAXX, 0.01);
			isinitpp0 = 1;
		}

		static vec2 panpadian;
		if (gtime - leveluptime < 10)
		{// …˝º∂π˝∂»∂Øª≠ 
			// ¿º≤›µƒ“∆∂Ø
			pp0 = blend(pp0, vec2(0.4 * MAXX, 0.01), 0.01);
		}		
		
		static float lens[4] = { 0.01, 0.2, 0.5, 0.1 };
		for (int j = 0; j < 4; j++)
		{	
			vec2 pp = pp0;
			vec2 vv(1, 0);
			if (j == 2)
			{
				vv.rot(PI * 0.51 + PI * 0.05 * (0.5 + 0.5 * sin(gtime * PI * 0.05)));
			}
			else
				vv.rot(j * PI / 4.8 + PI / 5 + PI * 0.01 * (0.5 + 0.5 * cos(gtime * PI * 0.5 + PI * 0.118)));

			int len = blend(200 / 8, 200, lens[j]);
			int panpaii = blend(0, len * 0.75, (gtime - leveluptime - 2.5) / 2.5);
			for (int ii = 0; ii < len; ii++)
			{
				real aii = real(ii) / len;
				vv = vv + vec2(0, -0.02);
				vv.norm();
				pp = pp + vv * 0.004;
				if (ii == panpaii)
					panpadian = pp;

				real sz = blend2(0, 0.008, aii, 2);
				if (showtype == -1)
				{
					if (ii % 6 < 4 || ii % 6 == 5)
						pixelCamera1(pp);
					pixelCamera1(pp + vec2(-vv.y, vv.x) * sz);
					pixelCamera1(pp - vec2(-vv.y, vv.x) * sz);
				}
				else
				{
					if (ii % 6 < 4 || ii % 6 == 5)
						pixel1(pp);
					pixel1(pp + vec2(-vv.y, vv.x) * sz);
					pixel1(pp - vec2(-vv.y, vv.x) * sz);
				}

				if (j == 2 && ii == int(len * 0.75))
				{
					p0 = pp;
					v = vec2(vv.y, (-vv.x));					
				}
			}
		}	
	}
	else
	{// ∂¥ø⁄µƒ…≥Õ¡
		vec2 vv(1, 0);
		for (int i = 0; i < 58; i++)
		{
			p0 = vec(0.5 * MAXX, 0.1 * MAXY);
			vec tvv = vv.rotcpy(rndmap2[i][10] * PI * 2);
			pixelBug1(p0 + tvv * blend(0.02 + 0.01 * rndmap[i][18], 0.01 * rndmap[i][18], fabs(tvv.y)), i);
		}
	}


//////////////////////////////// ªÊ÷∆ ////////////////////////////////
	if (showtype == -1 && drawstep > 200)
		return;

	if (lifestep <= 0 || lifestep > 3)
		return;

	if (light)
	{
		flushpixels(); pixel_size0 = 2; //…Ë÷√µ„µƒ¥Û–°
	}
	else
	{
		flushpixels(); pixel_size0 = 1; //…Ë÷√µ„µƒ¥Û–°
	}

	static real chuxuzhuangtai[4] = { 0 };		// ≤∂ªÒ ±º‰
	static float s_t[][6] = {
		{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
		{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
		{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
		{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
		{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
		{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) }
	};
	static float s_t1[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	static float s_t2[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };	
	
	real sz = 0.8;
	real ssz = 0.0;
	v.norm();
	vec2 p = p0;
	int len = len0 / 2;
	vec2 p1 = p, p2 = p;
	vec2 eatposv2;

	// ÃÂ–Õ≤Œ ˝
	real bodysizes[] = { 0.005, 0.004, 0.005 };	
	real bodylens[] = { 0.0015, 0.0015, 0.0008 };
	real bodylen = bodylens[lifestep - 1];
	real foodsize = blend2(0.8, 4, eatnum / 10.0); // …ÌÃÂ÷–≤ø¥Û–°∏˙≥‘µΩµƒ ≥ŒÔ¡ø≥…’˝±»

	// ––Œ™∂Øª≠
	real catchpow = lifestep == 3 ? 7.2 : 3;
	static real tmovespeed = movespeed;
	tmovespeed = blend(tmovespeed, movespeed, 0.1, 1 / GAMESPEED);
	real moveain = blend(1, 0, tmovespeed);
	// …‰ª˜Ω«
	real shootang = sin(gtime * PI * 0.2);

	for (int i = 0; i < len; i++)
	{// «˚∏…
		real ai = real(i) / len;
		v = v + vec2(rndmap2[i][0], rndmap2[0][i]) * (0.04 * moveain);		
		{
			real ftt = FT(ai * 2 * PI + gtime * PI * 0.05);
			v.rot(ftt * PI * 0.008 * moveain);
			if (v.y < 0.4) v.y = 0.4; // œﬁ÷∆∆‰œÚ…œµƒΩ«∂»
		}
		v.norm();
		
		if (lifestep == 3 && i == 0)
		{
			v.rot(PI * 0.25 * shootang);
		}
		p = p + v * blend(sz * bodylen, sz * bodylen * catchpow, tmovespeed);
		{
			if (gtime - powerspeedtime < .25)
			{// À∫“ß∂Øª≠
				real ta = (gtime - powerspeedtime) / .25;
				real ssz0 = blend2(0, 3, ta);
				ssz = blend(ssz0 * 0.01, 0, fabs(ai - 1) / 0.25, 2);
			}
			if (eattime > 0 && gtime - eattime < 2)
			{// œ˚ªØ∂Øª≠
				real foodpos = blend(1, 0, (gtime - eattime) / 2);
				ssz = blend(0.01, 0, fabs(ai - foodpos) / 0.25, 2);
				ssz = ssz * (0.5 + 0.5 * sin(gtime * PI * 10));
				if (i == int(len * foodpos))
					eatposv2 = p;
			}
			
			real bodysize = bodysizes[lifestep - 1];
			{// ÷–≤ø¬‘∑ 				
				bodysize = blend2(bodysize * 0.5, bodysize * foodsize, ai, 2);
			}
			real a2d1 = blend2d(0.5, 1.2, p1.x, p1.y);
			real ft1 = FTU(ai * 2 * PI + gtime * PI * 0.1, s_t1);
			p1 = p + vec2(v.y, -v.x) * (bodysize * sz * a2d1 + bodysize / 3 * sz * ft1 + ssz);
			real a2d2 = blend2d(0.5, 1.2, p2.x, p2.y);
			real ft2 = FTU(ai * 2 * PI + gtime * PI * 0.1, s_t2);
			p2 = p - vec2(v.y, -v.x) * (bodysize * sz * a2d2 + 0.01 * sz * ft2 + ssz);
			
			if (i % drawstep == 0)
			{
				pixelBug(p1, i);
				pixelBug(p2, i);
			}
			if (i < len - 8)
			{// …Ì…œµƒÃıŒ∆
				if (i % 10 == 0)
				{
					int len1 = len0 / 30;
					for (int ii = 0; ii < len1; ii++)
					{
						real aii = real(ii) / len1;
						if (ii % drawstep == 0)
							pixelBug1(blend(p1, p2, aii), i);
					}
				}
				if (i % 10 == 4)
				{
					int len1 = len0 / 80;
					for (int ii = 0; ii < len1; ii++)
					{
						real aii = real(ii) / len1;
						if (ii % drawstep == 0)
							pixelBug1(blend(p1, p2, aii), i);
					}
				}
			}
		}

		if (i == len - 4)
		{
			for (int ii = 0; ii <= 6; ii++)
			{
				real aii = real(ii) / 6;
				vec2 pp = blend(p1, p2, aii);
				if (ii % drawstep == 0)
					point5(pp, 0.001);
			}
		}
		if (i == len - 2)
		{// ◊Ï…œµƒ¥•–Î
			for (int ii = 0; ii <= 6; ii++)
			{
				real aii = real(ii) / 6;
				vec2 pp = blend(p1, p2, aii);
				vec2 vv = v.rotcpy(FT(aii * 2 * PI + gtime * PI * 0.1, s_t[5]));
				for (int jj = 0; jj < 8; jj++)
				{
					vv = vv + vec2(rndmap2[jj][ii], rndmap2[jj][ii + 100]) * 0.5;
					vv.norm();
					pp = pp + vv * 0.002;
					if (jj % drawstep == 0)
						pixelBug1(pp, ii);
				}
			}
		}
		if (showtype == -1)
		{// ◊Ï∞Õ∑…◊ﬂµƒ∂Øª≠◊º±∏
			if (i > len * 0.8)
			{
				static bool first = 1;
				if (first)
				{
					first = 1;
					vel = v.normcopy() * 0.0005;
					pos = p + v.normcopy() * 0.005;
					first = 0;
				}
				return;
			}
		}
		if (showtype != -1  // ¥•–Îœ˚ ß
			&& i == len - 1)
		{// ¥•–Î
			flushpixels(); pixel_size0 = 1; //…Ë÷√µ„µƒ¥Û–°
			real chuxulens[] = {0.8, 0.75, 1.2};
			real chuxulen = chuxulens[lifestep - 1];
			{				
				real a2d0 = blend2d(0.1, 1, p.x, p.y);				
				real a2dsz = lifestep == 3 ? 0.5 : 1;
				real tapowerspeedtime = (gtime - powerspeedtime) / .25;
				for (int j = 0; j <= 3; j++)
				{
					real aj = real(j) / 3;
					vec2 pp = blend(p1, p2, aj);
					vec2 vv = v.rotcpy((blend(-0.5 * PI, 0.5 * PI, aj) + rndmap2[0][j] * PI * 0.1) * moveain);
					if (lifestep == 3 && j == 0)
					{
						vv = vec2(1, 1); vv.norm();
					}

					// ¥•–Î≥§∂»
					int len1 = lifestep == 3 ? len0 / 2 * chuxulen : len0 / 2 * (0.7 + rndmap[j][1] * 0.3) * chuxulen;				
					if (lifestep == 3 && j != 0)
						len1 *= 0.5;
					if (len1 % 2 == 0)
						len1 -= 1;
					
					{// ≤∂ ≥ ±¿≠≥§
						len1 += tmovespeed * len1 * 0.8;
					}

					// æÌ«˙Ω«∂»
					static real sdangs[4][128] = {0};
					static real flagtab[4] = { 1, -1, 1, -1 };
					static real catchangflagtab[4] = { 1, 1, -1, -1 };
					real catchang = 5.8 * catchangflagtab[j];

					for (int ii = 0; ii < len1; ii += 2)
					{// ªÊ÷∆
						real aii = ii / real(len1);

						real a2d = blend2d(-a2dsz, a2dsz, pp.x, pp.y);
						real ft = FT3(1.5 * PI * rndmap2[10][j] + gtime * PI * 0.3 + aii * PI * 2, s_t[j]);
						real dft = flagtab[j] * (a2d * 1 + ft * 1.0) * 0.15;

						real dang0 = chuxuzhuangtai[j] == 0 ? dft : catchang;
						if (lifestep == 3)
						{
							if (j != 0)
								dang0 = catchang;
							else if(j == 0)
							{
								dang0 += PI / 4 * FT(gtime * PI * 0.21);
								if (dang0 > .25) dang0 = .25; else if (dang0 < -.25) dang0 = -.25;
							}
						}

						real& ang = sdangs[j][ii % 128];
						if (chuxuzhuangtai[j] == 0)
							ang = blend(ang, dang0, 0.05);
						else
							ang = blend(ang, dang0, 0.008);

						if (lifestep == 3)
							vv.rot(blend(0, blend(catchang, ang, moveain), aii) * a2d * moveain);
						else
							vv.rot(blend(0, ang, aii) * moveain);

						pp = pp + vv * 0.005;

						if (ii % drawstep == 0)
						{
							pixelBug(pp, ii);
							if (ii >= len1 - 2)
							{
								point5(pp, 0.0015);
							}
						}

//////////////////////////////// ≤∂◊Ω¡‘ŒÔ≤∂ ≥¬ﬂº≠ ////////////////////////////////

						if (lifestep == 1)
						{
							for (int jj = 0; jj < NUM_FOOD; jj++)
							{// »Áπ˚¥• ÷≈ˆµΩ ≥ŒÔø≈¡£
								particle_t & pcl = food_particles[(jj) % NUM_FOOD];
								if (pcl.dencity == 1 && pcl.catchtime == 0 && pcl.btime > 0 && (pp - pcl.p).sqrlen() < 0.0006)
								{
									pcl.catchtime = gtime;
									chuxuzhuangtai[j] = gtime;
									eattime = gtime;
									playsound(1);

									// ªÒµ√ƒ‹¡ø
									eatnum++;
									energefood += grassenerge[grasses[jj % 10].grassshape % 5]; // ªÒµ√ƒ‹¡ø∂π
									PRINT("energefood : " << energefood)
								}
							}
						}
						if (lifestep == 2
							&& chuxuzhuangtai[j] == 0)
						{
							for (int jj = 0; jj < PLANKTER_NUM; jj++)
							{
								planker_t& ptr = plankers[jj];
								if (ptr.catchtime == 0 && (pp - ptr.pos).sqrlen() < 0.0004)
								{
									if (ptr.state > 0)
									{// ¥•–Î≈ˆµΩ¡‘ŒÔ ≤ªª·¬Ì…œ◊•µΩ				
										
										{
											ptr.state = 0;
											ptr.catchtime = gtime;
											chuxuzhuangtai[j] = gtime;
											eattime = gtime;
											playsound(1);

											// ªÒµ√ƒ‹¡ø
											eatnum++;
											energefood += grassenerge[grasses[jj].grassshape % 5]; // ªÒµ√ƒ‹¡ø∂π
											PRINT("energefood : " << energefood)
											break;
										}
									}
								}
								
							}
						}
						if (lifestep == 3)
						{// ¥•–Îœ≈µΩÀÆ‘È							
							if (j != 0 &&
								(daphnia_state == 1)
								&& (daphnia_pos - pp).sqrlen() < 0.001)
							{
								daphnia_state = 4; // ±ªœ≈µΩ¡À							
							}

							// ”’∂¸
							if (j == 0 && ii >= len1 - 2 &&
								gtime - leveluptime > 4)
							{								
								youerpos = pp;
								flushpixels(); pixel_size0 = 2; //…Ë÷√µ„µƒ¥Û–°
								unkownplankter(pp, v, 0, 1);
								flushpixels(); pixel_size0 = 1; //…Ë÷√µ„µƒ¥Û–°
							}
						}
					}
				}
			}
		}
	}	

	if (showtype > 0)
	{// ¬ﬂº≠ø… ”ªØ

		if (lifestep == 1) //ÀÆÛ£‘Â¿‡
		{
			if (eattime > 0)
			{
				for (int jj = 0; jj < NUM_FOOD; jj++)
				{//  ≥ŒÔø≈¡£±ª≤∂ªÒ
					particle_t & pcl = food_particles[(jj) % NUM_FOOD];
					if (pcl.catchtime != 0 && gtime - pcl.catchtime < 2)
					{
						pcl.dencity = blend(1, 0, (gtime - pcl.catchtime) / 1.5);
						if ((pcl.p - p).sqrlen() > 0.01 * 0.01 && (pcl.p - eatposv2).sqrlen() > 0.01 * 0.01)
							pcl.p = blend(pcl.p, p, 0.1);
						else
							pcl.p = eatposv2;					
					}					
				}
			}
		}
		else if (lifestep == 2) //ÀÆÛ£∏°”Œ…˙ŒÔ
		{
			if (eattime > 0)
			{
				for (int jj = 0; jj < PLANKTER_NUM; jj++)
				{
					planker_t& ptr = plankers[jj];
					if (ptr.state == 0)
					{
						if (gtime - ptr.catchtime < 2)
						{
							if ((ptr.pos - p).sqrlen() > 0.005 * 0.005 && (ptr.pos - eatposv2).sqrlen() > 0.01 * 0.01)
								ptr.pos = blend(ptr.pos, p, 0.1);
							else
								ptr.pos = eatposv2;
						}
						else
						{
							ptr.catchtime = 0;
							ptr.state = -1; // œ˚ ß
						}
					}
				}
			}
		}
		else if (lifestep == 3)
		{		
			//  π”√œﬂ¿¥≈–∂œ
			vec2 eatpt = p + v * 0.01;
			static vec2 leatpt = eatpt;
			real dis0 = (leatpt - eatpt).len();
			real dis = (daphnia_pos - eatpt).len() + (daphnia_pos - leatpt).len();
			leatpt = eatpt;
			// ±ÿ–Î‘⁄À∫“ß ±≤≈ƒ‹±ª≥‘µÙ
			real ta = (gtime - powerspeedtime) / .25;
			// ±ÿ–Î”√◊Ï÷±Ω”“ßµΩ
			if ((daphnia_state == 1 || daphnia_state == 4 || daphnia_state == 5)
				&& ta < 0.5
				&& (dis - dis0) < 0.04)// && (daphnia_pos - eatpt).cross(v) > 0.0)
			{//  «∑Ò≤∂◊ΩµΩÀÆ‘È

				daphnia_state = 2; // ±ª◊•◊°
				daphnia_pos = p;
				eattime = gtime;	
		
				playsound(1);

				// ªÒµ√ƒ‹¡ø
				eatnum++;
				energefood += rrnd(10, 18); // ªÒµ√ƒ‹¡ø∂π
				

				PRINT("ªÒµ√ƒ‹¡ø energefood : " << energefood)
			}

			// ±ª◊•◊°
			if (daphnia_state == 2 || daphnia_state == 0)
			{
				daphnia_state = 0; // “—æ≠À¿Õˆ
				if (eattime > 0 && gtime - eattime < 2)
				{
					daphnia_dencity = blend(1, 0, (gtime - eattime) / 1.0);
					daphnia_pos = eatposv2;
				//	PRINT("“—æ≠À¿Õˆ")
				}
				else
				{
					daphnia_state = 3; // ±ªœ˚√
					PRINT("±ªœ˚√")

				}
			}
		}

		for (int jj = 0; jj < 4; jj++)
		{
			// ≥¨π˝ ±º‰÷ÿ÷√¥• ÷µƒ◊¥Ã¨
			if (gtime - chuxuzhuangtai[jj] > 2.0)
				chuxuzhuangtai[jj] = 0;
		}

//////////////////////////////// ƒ‹¡ø”Î‘À∂Ø ////////////////////////////////
		pos = (p0 + p) / 2;
		bugsize = (p0 - p).len() / 2;
		
		// ƒ‹¡ø◊‘∂Øª÷∏¥
		static real lstgetpower = gtime;
		if (gtime - lstgetpower > 1)
		{
			energefood += rrnd(1, 5);
			lstgetpower = gtime;
			PRINT("energefood: " << energefood);
		}
		moveenerge = 50; // √ø¥ŒÀ∫“ß–Ë“™µƒƒ‹¡ø
		{// ÀŸ∂»
			static real lstpowtime = gtime;
			if (gtime - lstpowtime > 15)
			{
				lstpowtime = gtime;
				if (lifestep == 3)
				{
					if (daphnia_state != 3 && (daphnia_pos - youerpos).len() < 0.05)
						powerspeed();
				}
				else if (movespeed < 0.001)
				{				
					
					light = 1;
					if (!powerspeed())
					{// ¥ÚÌÔ¡À	
					}
				}
			}
			//if (lifestep == 3)
			//{// π‚œﬂ
			//	if (movespeed < 0.01)
			//		light = 0;
			//	else 
			//		light = 1;
			//}
			//else
			{
				light = energefood > moveenerge;
			}
		}
	}
	// ◊Ë¡¶
	movespeed = movespeed * pow(0.995, GAMESPEED);
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ÀÆÛ£≥°æ∞
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void hydrascene()
{	
	{///////////////////////////////…˝º∂¬ﬂº≠/////////////////////////////////
		if (lifestep == 1)
		{
			if (eatnum >= getEatNum(level) && gtime - eattime > 4.0)
			{// …˝º∂
				lifestep = 2;
				levelup();				
			}
		}
		else if (lifestep == 2)
		{
			lifestep = 2;
			if (eatnum >= getEatNum(level) && gtime - eattime > 4.0)//PLANKTER_NUM)
			{// …˝º∂				
				lifestep = 3;
				levelup();
			}
		}
		else if (lifestep == 3)
		{
			lifestep = 3;
			if (eatnum >= getEatNum(level) && gtime - powerspeedtime > 4.0)
			{// …˝º∂µΩ ÀÆƒ∏Ω◊∂Œ
				shape = 1;
				shapclass = 3;
				lifestep = 0;
				levelup();				
			}
		}
	}

	/////////////////////////////// ≥ŒÔ¬ﬂº≠////////////////////////////////
	if (lifestep == 1)
	{
		static int sindex = 0;
		static real lstemittime = gtime;
		if (gtime - lstemittime > 8)
		{// ∑¢…‰ ≥ŒÔø≈¡£
			particle_t & pcl = food_particles[(sindex++) % NUM_FOOD];
			if (!pcl.catchtime &&
				(sindex == 1 || gtime - pcl.btime > 8 || pcl.p.y < 0))
			{
				pcl.p = vec2(rrnd(0, MAXX), 1);
				pcl.v = vec2(0.01 * rrnd(-1, 1), -0.15 * rrnd(0.4, 1.2));
				pcl.btime = gtime;
				pcl.dencity = 1;
				pcl.type = rand() % 4 + 1;
				pcl.z = rrnd(0, 1);

				playditasound(rand() % NUMDEDASOUND);
			}
			lstemittime = gtime;
		}
		{// ∏¸–¬“‘º∞ªÊ÷∆ ≥ŒÔø≈¡£
			for (int i = 0; i < NUM_FOOD; i++)
			{  
				particle_t & pcl = food_particles[i];
				if (pcl.btime == 0)
					continue;

				real yy = MAXY * 0.2 * blend(0, 1, pcl.z); // Õ∏ ”–ßπ˚
				if (pcl.p.y < yy)
				{
					pcl.p.y = yy; // ¬‰µÿ
					pcl.dencity = blend(0.5, 0, (gtime - pcl.btime) / 20.0);
				}

				if (gtime - pcl.btime > 20.0)
				{
					//pcl.btime = 0;
					pcl.catchtime = 0;
				}
				else
				{
					if (!pcl.catchtime)
					{
						real alpha = blend2d(-1, 1, pcl.p.x, pcl.p.y);
						pcl.p = pcl.p + pcl.v.rotcpy(alpha * PI * 0.1) * 0.01;
					}
					grassdrawstep0 = blend(0, 10, pcl.dencity);					
					alga(pcl.p, 300, 0.05 + rndmap[i][0] * 0.25, (gtime - pcl.btime) * 1.1 * rndmap2[i][10], i, pcl.type);
					grassdrawstep0 = 10;
				}
			}
		}
	}
	else if (lifestep == 2)
	{
		static bool isfirsttime = 1;
		if (isfirsttime)
		{
			isfirsttime = 0;
			FOREACH(i, PLANKTER_NUM)
			{
				planker_t& ptr = plankers[i];				
				ptr.state = -1;
			
			}
		}
		{
			static real slsttime = gtime;
			if (gtime - slsttime > 5)
			{
				slsttime = gtime;
				FOREACH(i, PLANKTER_NUM)
				{
					planker_t& ptr = plankers[i];
					if (ptr.state == -1)
					{
						ptr.state = -2;
						break;
					}
				}
			}
		}
		unkownplankter();
	}
	else if (lifestep == 3)
	{
		// ÀÆ‘È
		daphnia();
	}

	///////////////////////////////ªÊ÷∆≥°æ∞////////////////////////////////

	flushpixels(); pixel_size0 = 2; //…Ë÷√µ„µƒ¥Û–°
	hydra(vec2(MAXX / 2, 0.0), vec2(0.01, 0.8), 250);

}


// ################################################################################################
// ÀÆƒ∏µƒ ¿ΩÁ
// ################################################################################################
// ∫£µ◊÷≤ŒÔ÷Æ‘Â¿‡
// ------------------------------------------------------------------------------------------------
void haidai(vec2 p, int len)
{
	static float s_t1[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	static float s_t2[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	for (int i = 0; i < len; i++)
	{
		real ai = real(i) / len;

		p = p + vec2(FT(ai * PI * 2 + gtime * 1.1, s_t1) * 0.0005, 0.004);
		real sz = (0.5 + 0.5 * FTU(ai * PI * 18, s_t2)) * 0.01;
		sz = blend2(0, sz, ai, 2);
		if (i % 4 < 3)
		{
			pixelCamera(p);
		}
		if (i % 18 < 14 || i % 18 == 16)
		{
			pixelCamera(p + vec2(sz, 0));
			pixelCamera(p - vec2(sz, 0));
		}
	}
}
// ------------------------------------------------------------------------------------------------
// Ω”„‘Â
// ------------------------------------------------------------------------------------------------
void jinyucao(vec2 p, int len)
{
	static float s_t1[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };

	for (int i = 0; i < len; i++)
	{
		real ai = real(i) / len;

		p = p + vec2(FT(ai * PI * 2 + gtime * 1.1, s_t1) * 0.002, 0.004);
		if (i % 4 < 3)
		{
			pixelCamera(p);
		}
		if (i % 12 == 0)
		{
			for (int j = 0; j < 6; j++)
			{
				vec2 pp = p;
				vec2 vv(rndmap2[j][i], 0.1 + rndmap[j][i]); vv.norm();
				for (int ii = 0; ii < 6; ii++)
				{
					pp = pp + vv * 0.002;
					pixelCamera(pp);
				}
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------
//  ≥ŒÔª®
// ------------------------------------------------------------------------------------------------
void hua(vec2 p, int len)
{
	vec2 v(0, 0.0015);
	for (int i = 0; i < len; i++)
	{
		real ai = real(i) / len;
		p = p + v;

		if (i % 4 < 3)
		{
			pixelCamera(p);
		}

		if (i == len - 1)
		{
			vec2 p1 = p;
			vec2 p2 = p;
			vec2 v1 = v;
			vec2 v2 = v;
			for (int ii = 0; ii < len; ii++)
			{
				real aii = real(ii) / len;
				real sz = blend(-0.002, 0.02, aii, .3);
				v1.rot(blend(0, -0.2 * PI, aii, 4));
				v2.rot(blend(0, 0.2 * PI, aii, 4));
				p1 = p1 + v1;
				p2 = p2 + v2;
				{
					pixelCamera(p1 + vec2(sz, 0));
					pixelCamera(p2 - vec2(sz, 0));
				}
				if (ii < len / 2)
				{
					pixelCamera(p1 + vec2(rrnd(0, sz), 0));
					pixelCamera(p2 - vec2(rrnd(0, sz), 0));
				}
			}
		}
	}
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ÀÆƒ∏
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
real jellyfishscale = 1.0f;
void jellyfish(vec2 o, vec2 dir, int len0)
{
	real sz = 0.5 * jellyfishscale;
	dir.norm();
	vec2 p1, p2;
	{// Õ∑≤ø
		vec2 p = o;
		vec2 v = dir;
		int len = len0 / 4;		
		real r = sz * 0.1;
		for (int i = 0; i < len; i++)
		{
			real ai = i / real(len);
			real a2d = blend2d(.5, 1, p.x, p.y);
			//real ang = blend(-0.1 * PI, 1.1 * PI, ai);
			real ang = blend(-0.6 * PI,0.6 * PI, ai);
			vec2 vv = dir.rotcpy(ang);// vec2(cos(ang), 0.8 * sin(ang));
			p = o + vv * (r * a2d);
			if (i > 0)
				pixelBugCamera(p, i);

			// ƒ‹¡ø
			//if (rrnd(0, 10) * moveenerge < energefood)
			if (i > 0)
				pixelBugCamera(o + vv * rrnd(0, r * 0.8 * a2d), i);
			if (i == 0)
				p1 = p;
			if (i == len - 1)			
				p2 = p;						
		}
	}

	dir = -dir;
	vec2 vdir = dir;
	vec2 vldir = ldir;
	{// «˚ÃÂ
		int len = 10;
		int chuxucnt = 0;
		for (int i = 0; i <= len; i++)
		{
			real ai = i / real(len);
			vec2 p = blend(p1, p2, ai);
			p = p - vdir * blend2(0, 0.005, ai, 2);
			real a2d = blend2d(.25, 2, p.x, p.y);
			vec2 tvv0 = vdir.rotcpy(PI * 0.02 * sin(ai * PI * 4.2 + (gtime * 1.35) * PI));
			vec2 ovv = (o - p).normcopy();
			vec2 tvv = blend(tvv0, ovv, 0.4);
			if (drawstep == 1)
			{
				vec2 pp = p;				
				for (int ii = 0; ii < 4; ii++)
				{// ◊Ï√´
					pp = pp + tvv * 0.004;					
					pixelBugCamera(pp, i + ii);
				}
			}			
			if (drawstep == 1 && 
				(i == 4 || i == 6))
			{// Õ»≤ø
				vec2 pp = p;	
		
				for (int ii = 0; ii < 28; ii++)
				{
					real aii = real(ii) / 28;
					vec2 vv = blend(tvv0, -ovv, aii);

					real a2d = blend2d(-1, 1, pp.x, pp.y);
					//vv.rot(a2d * PI * 0.01);
					pp = pp + vv * 0.002;
					real rr = blend2(0, 1, aii, 2) * 0.005;// *(0.5 + 0.5 * FT(aii * PI * 18));
					if (ii % 8 < 6)
					{
						pixelBugCamera(pp + vec2(-vv.y, vv.x) * rr, ii);
						pixelBugCamera(pp + vec2(vv.y, -vv.x) * rr, ii);
					}
				}
			}
			if (drawstep == 1 &&
				(i == 2 || i == 8))
			{
				{// ¥•–Î
					static float s_t0[][4] = {
						{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) },
						{ rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) }
					};
					static float s_ang0[] = { rrnd(-1, 1), rrnd(-1, 1) };

					int len1 = len0 / 8;
					real a2d0 = blend2d(0.1, 1, p1.x, p1.y);

					vec2 pp = p;

					real lenstep = sz * blend(0.008, 0.02, rndmap[50][i]);
					for (int ii = 0; ii < len1; ii += 2)
					{
						real aii = ii / real(len1);
						vec2 vv = blend(vdir, vldir, aii);// .rotcpy(PI / 3 * rndmap[0][i]);
						real ft = FT(gtime * PI * s_ang0[chuxucnt % 2] + aii * blend(0, PI, aii), s_t0[chuxucnt % 2]);
						real a2d = blend2d(-1, 1, pp.x, pp.y);

						pp = pp + vv.rotcpy(blend(0, ft * 4 * a2d, aii) * blend(1, 0, vel.sqrlen() / 0.01)) * lenstep;
						pixelBugCamera(pp, ii);
					}
					chuxucnt++;
				}
			}
		}
	}
	ldir = blend(ldir, dir, 0.1);

}

// ------------------------------------------------------------------------------------------------
// ¥”ÀÆÛ£µΩÀÆƒ∏π˝∂»–ŒÃ¨
// ------------------------------------------------------------------------------------------------
void aniJellyfishForming(vec2 o, vec2 dir, vec2 dir1, int len0, int ind)
{
	static float s_t[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };

	flushpixels(); pixel_size0 = 2;
	real at = blend(1, 0, gdtime1 / 2);
	real at1 = blend(0, 1, gdtime1 / 1.5);
	{// ªÊ÷∆
		vec2 p = o;
		dir.norm();
		real ang0 = atan2(dir.y, dir.x);
		real ang1 = atan2(dir1.y, dir1.x);
		ang0 = blend(ang0, ang1, at);
		real sz0 = blend(0.0065, 0.035, at1, 2);
		{// ¥”‘≤µΩ∑Ω				
			for (int i = 0; i < len0; i++)
			{
				real ai = real(i) / len0;
				real ang = blend(0, 2 * PI, ai);
				real r0 = sz0 + sz0 / 2 * blend2d(-1, 1, p.x, p.y);
				real r1 = r0;
				real dang = ang - int(ang / (PI / 2)) * PI / 2;
				if (dang < PI / 4)
					r1 = r1 / cos(dang);
				else
					r1 = r1 / sin(dang);
				real r = blend(r0, r1, at, 2);
				vec2 v(cos(ang + ang0), sin(ang + ang0));
				p = o + v * r;
				if (i > 0 && rand() % drawstep == 0)
				{
					pixelBug(p, i);

					// Œ∆¿Ì
					if (rand() % 100 < 100 * at1)
						pixelBug(o + v * rrnd(0, r), i);
				}

				if ((ai < 0.15 || ai > 0.85) && i % 4 == 0)
				{// ◊Ï…œµƒ¥•–Î				
					vec2 pp = p;					
					for (int jj = 0; jj < 8; jj++)
					{
						v = v + vec2(rndmap2[jj][i], rndmap2[jj][i + 100]) * 0.5;
						v.norm();
						pp = pp + v * 0.002;
						if (jj % drawstep == 0)
							pixelBug1(pp, i);
					}				
				}
				
			}
		}
		
	}
	flushpixels(); pixel_size0 = 2;
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ÀÆƒ∏≥°æ∞
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void jellyfishScene()
{
	{// ‘À∂Ø
		if (state == 0)
		{// ÀÊª˙¡¶≥°
			real a2d = blend2d(0, 1.0, pos.x, pos.y);
			vec2 acc = vec2(0, 0.000001).rotcpy(blend(0, 2 * PI, a2d));
			vel = vel + acc * GAMESPEED;
		}
		if (state != 2 && state != 4 && state != 6)
		{// ÀŸ∂»øÿ÷∆
			vec2 npos = pos + vel * GAMESPEED;

			if (npos.x < 0)
				vel.x = -vel.x;
			if (npos.y < 0.0)
				vel.y = -vel.y;
			if (npos.x > 3 * MAXX)
				vel.x = -vel.x;
			if (npos.y > MAXY * 0.9)
				vel.y = -vel.y;

			pos = pos + vel * GAMESPEED;
		}
	}
	flushpixels(); pixel_size0 = 2; //…Ë÷√µ„µƒ¥Û–°
	hua(vec2(0.5, 0.0), 50);
	haidai(vec2(0.1, 0.0), 150);
	jinyucao(vec2(0.25, 0.0), 50);

	jellyfish(pos, vel, 250);

	flushpixels(); pixel_size0 = 1; //…Ë÷√µ„µƒ¥Û–°

	jinyucao(vec2(0.28, 0.1), 30);
	haidai(vec2(0.15, 0.12), 40);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// –¬ ÷÷∏“˝
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void drawJianTou(vec p1, vec p2)
{
	flushpixels(); pixel_size0 = 1; //…Ë÷√µ„µƒ¥Û–°
	// ª≠÷∏ æœﬂ
	for (int i = 0; i < 128; i++)
	{
		real ai = i / 128.0;
		vec p = blend(p1, p2, ai);
		pixel(p);
	}
	// º˝Õ∑
	vec v12 = (p2 - p1).normcopy();
	vec p21 = p2 - v12.rotcpy(PI / 5) * 0.02;
	vec p22 = p2 - v12.rotcpy(-PI / 5) * 0.02;
	for (int i = 0; i < 18; i++)
	{
		real ai = i / 18.0;
		pixel(blend(p21, p2, ai));
		pixel(blend(p22, p2, ai));
	}
}

// ================================================================================================
// ÷˜ªÊ÷∆∫Ø ˝
// ================================================================================================
void SpacePlankton::initUI()
{
	/////////////////////////////////////////////////// ΩÁ√Ê≤ø∑÷ /////////////////////////////////////////////////////////////////
	if (mLVLable == 0)
	{// ¥¥Ω®ΩÁ√Êøÿº˛

		int fontsize = 20;
		int hgt = MIN(SIZEY, CCDirector::getInstance()->getWinSizeInPixels().height);
		int detahgt = (fontsize + 8) / MIN(960.0f, CCDirector::getInstance()->getWinSizeInPixels().height) * 960.0f;

		mLVLable = ui::Text::create("Lv.", "Arial", fontsize);
		mLVLable->setPosition(Vec2(fontsize + 8, hgt - detahgt));
		mLVLable->setColor(cocos2d::Color3B::GRAY);
		insertChild(mLVLable, 10);

		mFDLable = ui::Text::create("Fd:", "Arial", fontsize);
		mFDLable->setPosition(Vec2(fontsize + SIZEX / 2.5, hgt - detahgt));
		mFDLable->setColor(cocos2d::Color3B::GRAY);
		insertChild(mFDLable, 10);

		mUPLable = ui::Text::create("Lv-up:", "Arial", fontsize);
		mUPLable->setPosition(Vec2(SIZEX - SIZEX / 8 + 8, hgt - detahgt));
		mUPLable->setColor(cocos2d::Color3B::GRAY);
		insertChild(mUPLable, 10);

		mLVLable->setVisible(0);
		mFDLable->setVisible(0);
		mUPLable->setVisible(0);
	}
}
void SpacePlankton::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	if (!gbinit)
		return;

	// …Ë÷√ΩÁ√Ê
	drawsettingwindow();

	// ’¬Ω⁄—°‘ÒΩÁ√Ê
	drawchapterswindow();

	// draw hotkey window
	drawhotkeywindow();
	
	if (storyid != 1)
	{/////////////////////////////////////////////////// ∆‰À˚’¬Ω⁄ /////////////////////////////////////////////////////////////////

		if (storyid == 2){
			Lab::drawlab(this);
		}

		flushpixels(); flushpixels1();// ∞— £œ¬µƒª≠ÕÍ	
		cocos2d::Node::draw(renderer, transform, flags);

		return;
	}
	/////////////////////////////////////////////////// –Úƒª≤ø∑÷ /////////////////////////////////////////////////////////////////
	if (gamestate == 0)
	{
		drawprologue();
	}
	initUI();

	////////////////////////////////////////////////////”Œœ∑ ±º‰÷·//////////////////////////////////////////////////////////////////
	ialocked = 0;

	if (gamestate >= 1)
	{
		if (borntime == 0 && gtime > 0)
		{
			// ≥Ê◊”≥ˆ…˙
			born();
		}

		// ---------------------------------------------–ŒÃ¨1---------------------------------------------- //
		if (shapclass == 1)
		{
			protozoaScene();
		}
		// ---------------------------------------------–ŒÃ¨2---------------------------------------------- //Ω◊∂Œ2
		else if (shapclass == 2)
		{
			// ÷˜π˝∂…∂Øª≠1
			if (lifestep == 0)
			{// ¬—µƒ◊¥Ã¨
				ialocked = 1;
				gdtime = gtime - borntime;				
				{// ¥”µ•œ∏∞˚µΩ∂‡œ∏∞˚«ª≥¶∂ØŒÔµƒπ˝∂…∂Øª≠	±‰–Œ∂Øª≠			

					{// µ•œ∏∞˚◊¥Ã¨ µΩ œ∏∞˚◊©

						real steptimes[] = { 0, 2, 2 + 8, 2 + 8 + 8 }; //  ±º‰µ„
						if (gdtime < steptimes[1])
						{// »•µÙÕ‚ø«

							drawstep = 1; // π˝∂»‘⁄…œΩ◊∂ŒÕÍ≥…
							gdtime1 = gdtime - steptimes[1];
							aniXibaozhuan(pos, -vel, -vel, 100, 0);
						
						}
						else if (gdtime < steptimes[2])
						{// ∫œ∂‡Œ™“ª
							gdtime1 = gdtime - steptimes[1];
							aniXibaozhuan();
							//PRINT(gdtime)
						}
						else if (gdtime < steptimes[3])
						{// »∫¬‰œ˚ ß ÀÆÛ£≥ˆœ÷
							gdtime1 = gdtime - steptimes[1];
							drawstep = blend(1, 50, (gdtime - steptimes[2]) / 18.0);
							aniXibaozhuan();

							drawstep = blend(10, 1, (gdtime - steptimes[2]) / 4.0);
							movespeed = 0;
							lifestep = 1;
							hydra(vec2(MAXX / 2, 0.1), vec2(0.01, 0.8), 250, 0);
							lifestep = 0;
							//PRINT(gdtime)
						}
						else
						{
							static int sframecnt = 0;
							sframecnt++;
							if (sframecnt == 101)
							{
								// ∂Øª≠ÕÍ≥…
								drawstep = 1;
								lifestep = 1;
							}
							else
							{// «–ªªµΩΩÈ…‹“≥
								if (shuixipage == 0)
								{
									shuixipage = Sprite::create("media/shuixi.jpg");
									float scalex = SIZEX / (float)shuixipage->getContentSize().width;
									shuixipage->setScaleX(scalex);
									float scaley = SIZEY / (float)shuixipage->getContentSize().height;
									shuixipage->setScaleY(scaley);

									shuixipage->setVisible(1);
									insertChild(shuixipage, 108);
								}
								float scalex = SIZEX / (float)shuixipage->getContentSize().width;						
								float scaley = SIZEY / (float)shuixipage->getContentSize().height;
								shuixipage->setScaleX(blend(0, scalex, sframecnt / 100.0));
								shuixipage->setScaleY(blend(0, scaley, sframecnt / 100.0));
								shuixipage->setPosition(SIZEX * 0.5, SIZEY * 0.5);

								// ºÃ–¯ª≠ÀÆÛ£
								movespeed = 0;
								lifestep = 1;
								hydra(vec2(MAXX / 2, 0.1), vec2(0.01, 0.8), 250, 0);
								lifestep = 0;
							}
						}
					}
				}
			}			
			else
			{
				// ΩÈ…‹“≥±ÿ–Îπÿ±’
				if (!shuixipage || !shuixipage->isVisible())
				{
					ialocked = 0;
					hydrascene();
				}
			}
			
		}
		// ---------------------------------------------–ŒÃ¨3---------------------------------------------- //
		else if (shapclass == 3)
		{			
			if (lifestep == 0)
			{// ÀÆÛ£µΩÀÆƒ∏µƒ∂Øª≠ ◊Ï∞Õ∑…◊ﬂ
				//PRINT("borntime " << borntime << " " << gtime << " gdtime1 " << gdtime1)
			//	gdtime = gtime - borntime;
				ialocked = 1; // ≤Ÿ◊˜À¯
				gdtime = gtime - leveluptime;
				static real steptimes[] = { 0, 2.5, 100 }; //  ±º‰µ„
				if (gdtime < steptimes[2])
				{// ÀÆÛ£ µƒ◊Ï≤ø Õ—¬‰
					if (gcamerapos == 0)
						gcamerapos = - MAXX;

					real tcamerapos = gcamerapos;  
					gcamerapos = tcamerapos + MAXX;
					shapclass = 2;
					lifestep = 3;					
					drawstep = blend(1, 250, (gdtime - steptimes[1]) / 5);
					
						hydra(vec2(MAXX / 2, 0.1), vec2(0.01, 0.8), 250, -1);
					lifestep = 0;
					shapclass = 3;

					gdtime1 = gdtime - steptimes[0];

					drawstep = blend(1, 10, (gdtime - steptimes[1]) / 2.5);
					aniJellyfishForming(pos, vel, vel, 100, 0);
					drawstep = blend(50, 1, (gdtime - steptimes[1]) / 1.20);
					if (drawstep < 40)
					{
						jellyfish(pos, vel, 250);						
					}
					gcamerapos = tcamerapos;

					static real ang = 0;
					if (ang < PI)
					{
						ang += 0.002 * PI;
						vel.rot(0.002 * PI);
						
						if (ang >= PI)
						{
							vel = -vel; // ±‰≥…ÀÆƒ∏∫Û∑¥œÚ
						}
					}
					vel = blend(vel, vec(0.001, 0.0001), 0.001);
					
					{// ÀŸ∂»øÿ÷∆
						vec2 npos = pos + vel * GAMESPEED;

						if (npos.x < 0)
							vel.x = -vel.x;
						if (npos.y > 0.9 * MAXY)
							vel.y = -vel.y;
						if (npos.y < 0)
							vel.y = -vel.y;	
						pos = pos + vel * GAMESPEED;
					}
					if (pos.x > 1.1 * MAXX)
					{// ∏’”Œ≥ˆ±ﬂΩÁ						
		
						gmovecam = 1; // “∆∂Ø…„œÒª˙
						if (gcamerapos >= 0)
						{// ∂Øª≠ÕÍ≥…
							drawstep = 1;
							lifestep = 1;
                            
//							PRINT("∂Øª≠ÕÍ≥…")
						}
					}
				}
				else
				{// ∂Øª≠ÕÍ≥…
					drawstep = 1;
					lifestep = 1;
					gmovecam = 1; // “∆∂Ø…„œÒª˙
				}
			}
			else
			{
				gmovecam = 1;
				jellyfishScene();

				{// œ‘ æ∑÷œÌ”Î÷ÿÕÊΩÁ√Ê					
					if (chongwanui == 0)
					{
						chongwanui = Sprite::create("media/chongwan.png");
						chongwanui->setVisible(1);
						insertChild(chongwanui, 108);	

						Vec2 pos;
						pos.x = SIZEX * 0.5;
						pos.y = SIZEY * 0.5;
						chongwanui->setPosition(pos);
						//float scale = 1.0f / 1.5 * devicepixelsize;
						//chongwanui->setScale(scale);
						chongwanui->setScale(0.01);

						//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
						{// steam ≥…æÕ
							if (g_SteamAchievements)
								g_SteamAchievements->SetAchievement("ACH_4");
						}
						//#endif
					}
					if (chongwanui)
					{
						static int sframecnt = 0;						
						if (sframecnt >= 40)
						{
							// ∂Øª≠ÕÍ≥…
							float scale = 1.0f / 1.5 * devicepixelsize;
							chongwanui->setScale(scale);
						}
						else
						{
							sframecnt++;
							float scale = 1.0f / 1.5 * devicepixelsize;
							chongwanui->setScale(blend(0.01f, scale, sframecnt / 40.0));
						}
					}
				}
			}
		}
	}
	////////////////////////////////////////////////////–¬ ÷÷∏“˝//////////////////////////////////////////////////////////////////
	if (gamestate > 0)
	{
		// ∏–”¶ ¬º˛
		if (framecnt_xs == 0)
		{
			if (
				(progress_xs == 1 && level == 1 && shape == 1)	// µ»º∂ΩÈ…‹		
				|| (progress_xs == 2 && level == 1 && shape == 1)	// …˝º∂Ω¯∂»
				|| (progress_xs == 3 && level == 1 && shape == 1)	// µ„À¸
				|| (progress_xs == 4 && level == 4 && shape == 2)	// µ„ª˜ø’∞◊¥¶Œπ ≥
				|| (progress_xs == 5 && level == 4 && shape == 2)	// …˝º∂Ω¯∂»
				)
			{
				framecnt_xs = 1;// ø™ ºº∆ ±	
				PRINT("–¬ ÷÷∏“˝ ø™ ºº∆ ±")
			}
		}
		// —” ±‘›Õ£
		static const int sc_times[][2] = {
			{ 100, 300 },
			{ 100, 300 },
			{ 225, 450 },
			{ 100, 300 },
			{ 100, 300 }
		};
		if (framecnt_xs > 0)
		{
			framecnt_xs++;

			if (progress_xs <= 5)
			{// µ»º∂ΩÈ…‹
				if (framecnt_xs == sc_times[progress_xs - 1][0]) // ¥•∑¢
				{
					xinshouzhiyin_states[progress_xs - 1] = 1;// ø™∆ÙΩÁ√Ê
					zhiyin[progress_xs - 1]->setVisible(1);

					gamestate = 2; // ‘›Õ£”Œœ∑¬ﬂº≠		
					PRINT("¥•∑¢,‘›Õ£”Œœ∑¬ﬂº≠")
				}
				else if (framecnt_xs == sc_times[progress_xs - 1][1]) //	πÿ±’
				{
					xinshouzhiyin_states[progress_xs - 1] = 0;// πÿ±’ΩÁ√Ê
					zhiyin[progress_xs - 1]->setVisible(0);

					framecnt_xs = 0; // πÿ±’º∆ ±
					progress_xs++; //  ◊º±∏œ¬“ª∏ˆ	
					gamestate = 1; // ª÷∏¥”Œœ∑¬ﬂº≠
					PRINT("πÿ±’£¨ª÷∏¥”Œœ∑¬ﬂº≠")
				}
			}
		}

		if (gamestate == 2)
		{// ªÊ÷∆–¬ ÷÷∏“˝
			if (progress_xs == 3)
			{// µ„À¸ –¬ ÷÷∏“˝
				vec2 markpos = vec2(MIN(MAXX, pos.x + 0.1), MAX(0, pos.y - 0.1));
				zhiyin[progress_xs - 1]->setPosition(Vec2(markpos.x * SCALE, markpos.y * SCALE));
				c_xinshouzhiyin_pos[progress_xs - 1] = markpos;
				c_xinshouzhiyin_ptr[progress_xs - 1] = pos;
			}
			for (int i = 0; i < 5; i++)
			{
				if (xinshouzhiyin_states[i])
				{
					drawJianTou(c_xinshouzhiyin_pos[i], c_xinshouzhiyin_ptr[i]);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	flushpixels(); flushpixels1();// ∞— £œ¬µƒª≠ÕÍ	

	cocos2d::Node::draw(renderer, transform, flags);
}

// ================================================================================================
// Touch
// ================================================================================================
int gcp_ctrlindex = 1;
int touchfunc = 0;
real gcamerapos0 = 0;
int lststoryid = 0;
bool SpacePlankton::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	if (!gbinit)
		return 1;

	CCPoint point = pTouch->getLocation();
	point = this->convertToNodeSpace(point);
	if (fullscreen)
		point.y += 32;

	pickpoint = vec2(point.x / SCALE, point.y / SCALE);
	pickestate = 1;	

	if (storyid < 0 && settingwindow[0])
	{/////////////////////////////////////////////////// …Ë÷√ΩÁ√Ê /////////////////////////////////
		
		if (settingwindow[4]->getBoundingBox().containsPoint(point))
		{// »´∆¡ƒ£ Ω«–ªª
			shiftfullscreen();
		}
		else if (settingwindow[3]->getBoundingBox().containsPoint(point))
		{// hot keys			
			settingwindowID = 2;
			lststoryid = storyid;
			storyid = 0;
		}
		else if (settingwindow[1]->getBoundingBox().containsPoint(point))
		{// …˘“Ùø™πÿ
			if (isSoundOn)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
				CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
				isSoundOn = false;
				settingwindow[1]->setTexture("media/setting2_2.png");
			}
			else
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
				isSoundOn = true;
				settingwindow[1]->setTexture("media/setting2.png");
			}
		}
		else if (settingwindow[2]->getBoundingBox().containsPoint(point))
		{// ∏˜∏ˆ’¬Ω⁄
			lststoryid = storyid;
			storyid = 0;
			settingwindowID = 1;
			//resizewindow();
		}
		else if (settingwindow[0]->getBoundingBox().containsPoint(point))
		{// ∑µªÿ
			storyid = abs(storyid);
		}
		return 1;
	}
	if (storyid == 0)
	{///////////////////////////////////////////////////////////////////////////////////
		if (settingwindowID == 1 && chapterswindow[0])
		{
			if (chapterswindow[0]->getBoundingBox().containsPoint(point))
			{// grow
				readdat();
				storyid = 1;
				
				resizewindow();
				if (fullscreen)
				{
					fullscreen = false;
					shiftfullscreen();
				}
				if (gamestate == 0)
					gamestate = 1;
				settingwindowID = 0;
			}
			else if (chapterswindow[1]->getBoundingBox().containsPoint(point))
			{// lab
				storyid = 2;
				resizewindow();
				if (fullscreen)
				{
					fullscreen = false;
					shiftfullscreen();
				}
				settingwindowID = 0;

				if (progress_xs == 6)
				{
					progress_xs = 7;
					{// hot keys			
						settingwindowID = 2;
						lststoryid = storyid;
						storyid = 0;
					}
				}
			}			
		}
		else if (settingwindowID == 2 && hotkeywindow)
		{
			if (hotkeywindow->isVisible() && hotkeywindow->getBoundingBox().containsPoint(point))
			{
				storyid = lststoryid;
				settingwindowID = 0;
			}
		}
		return 1;
	}
	if (storyid != 1)
	{/////////////////////////////////////////////////// ∆‰À˚’¬Ω⁄ /////////////////////////////////

		if (storyid == 2){
			Lab::ontouchlab(point);
		}
		return 1;
	}	
	// –Úƒªµ„ª˜
	if (framecnt > 1801 && framecnt < 2101)
	{
		framecnt = 2101;
		return 1;
	}
	else if (framecnt > 2101 && framecnt < 3001)
	{
		framecnt = 3001;
		return 1;
	}
	else if (framecnt > 3001 && framecnt < 4001)
	{
		framecnt = 4001;
		return 1;
	}
	if (chongwanui != 0)
	{// ÷ÿÕÊ
		if (chongwanui->getBoundingBox().containsPoint(point))
		{
			// ¥”Õ∑‘Ÿ¿¥
			//progress_xs = 1;
			gamestate = 0;
			level = 1;
			shapclass =1;
			shape = 1;
			lifestep = 0;
			bugsize = 0.8;

			storyid = 0; // Ω¯»Î—°‘Ò’¬Ω⁄
			settingwindowID = 1;

			savedat();

			chongwanui->setVisible(0);

			return 1;
		}		
	}
	{// UIµ„ª˜ ¬º˛
		// ÀÆÛ£ΩÈ…‹ΩÁ√Ê
		if (shuixipage && shuixipage->isVisible())
		{
			shuixipage->setVisible(0);
			return 1;
		}
	}
	// ª•∂Ø¬ﬂº≠
	if (ialocked && !gmovecam)
		return 1;

	if (!posValid(pickpoint))
		return 1;

	// –Úƒª≤ø∑÷
	if (gamestate == 0)
		return 1;

	if (shapclass == 1)
	{
		bool istouched = 0;
		
		if (shapclass == 1 && shape == 2)		
		{
			if (fabs((pickpoint - drawpos).len() + (pickpoint - pos).len() - (pos - drawpos).len()) < bugsize * 0.025)
				istouched = 1;
		}
		else if ((pickpoint - pos).len() < bugsize * 0.1)
		{
			istouched = 1;
		}
		if (istouched)
		{
			if (state <= 2 || state == 8 || state == 11) // ≤ø∑÷◊¥Ã¨ª·”–∑¥”¶
			{// ¥Ãº§≥‘ªı
				if (powervel(1))
				{
					if (rand() % 100 < 50 || vel.len() > 0.02)
						playsound(3);
					picktime = gtime;
					state = 8;
				}
			}
			else
			{
				//kelpctrlx1 += rrnd(-PI, PI);
				//kelpctrlx2 += rrnd(-PI, PI);

				if (pickpoint.y < 0.5)
					gcp_ctrlindex = 1;
				else
					gcp_ctrlindex = 2;
			}
		}
		else if (lifestep > 0)
		{
			if (shape == 2)
			{
				// Œπ ≥‘Â¿‡
				int ii = 0;
				for (; ii < GRASSNUM; ii++)
				{
					if (grasses[ii].grassstates == -1 && grasses[ii].grassweishi == 0)
					{
						grasses[ii].grassposmap = pickpoint;
						grasses[ii].grassshape = rand() % (lifestep);
						grasses[ii].grassstates = 0;
						grasses[ii].grassweishi = gtime;
						grasses[ii].grassborntime = gtime;

						if (shape == 2 && lifestep >= 2)
							grasses[ii].shengzhang = rand() % 100 < 25 ? 0 : 1; // ª·…˙≥§µƒ‘Â¿‡
						playsound(2);
						break;
					}
				}
			}
			if (shape == 3)
			{
				if (lifestep == 3)
				{// Œπ ≥∏°”Œ…˙ŒÔ
					FOREACH(i, PLANKTER_NUM)
					{
						planker_t& ptr = plankers[i];
						if (ptr.state == -2)
						{
							ptr.catchtime = 0;
							ptr.state = 1;
							ptr.energe = 8;
							ptr.pos = pickpoint;
							ptr.dangertime = 0;
							ptr.vel = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.001;
							playsound(2);
							break;
						}
					}
				}
			}
		}
		touchfunc = 1;
	}
	else if (shapclass == 2)
	{
		if ((pickpoint - pos).len() < bugsize)
		{// ¥Ãº§ÀÆÛ£
			powerspeed();
		}
		//else if (lifestep == 1 &&
		//	pickpoint.y > 0.5)
		//{// ŒπÀ¸‘Â¿‡
		//	for (int jj = 0; jj < NUM_FOOD; jj++)
		//	{
		//		particle_t & pcl = food_particles[(jj) % NUM_FOOD];
		//		if (pcl.catchtime != 0)
		//		{
		//			pcl.p = vec2(rrnd(0,MAXX), 1);
		//			pcl.v = vec2(0.05 * rrnd(-1, 1), -0.15 * rrnd(0.4, 1.2));

		//			pcl.btime = gtime;
		//			pcl.dencity = 1;
		//			pcl.catchtime = 0;
		//			playsound(2);
		//			break;
		//		}
		//	}
		//}
		else if (lifestep == 2)
		{// ŒπÀ¸∏°”Œ…˙ŒÔ
			FOREACH(i, PLANKTER_NUM)
			{
				planker_t& ptr = plankers[i];
				if ( ptr.state == -2)
				{
					ptr.catchtime = 0;
					ptr.state = 1;
					ptr.pos = pickpoint;
					ptr.vel = vec2(rrnd(-1, 1), rrnd(-1, 1)) * 0.005;
					playsound(2);
					break;
				}
			}
		}
		else if (lifestep == 3)
		{// ŒπÀ¸ÀÆ‘È
			if (daphnia_state == 3)
			{
				daphnia_state = 1;
				daphnia_pos = pickpoint;
				daphnia_dencity = 1;
				daphnia_vel = vec(-rrnd(0, 0.004), rrnd(0.001, 0.0035));
				daphnia_shape = (daphnia_shape + 1) % 6;
				playsound(2);
			}
		}

		touchfunc = 2;
	}	
	else if(shapclass == 3)
	{
		touchfunc = 0;	
		gcamerapos0 = gcamerapos;
	}

	return 1;
}
