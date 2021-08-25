/// Gymnospermae

// ------------------------------------------------------------------------------------------------
// 崖柏 Thuja sutchuenensis Franch
// ------------------------------------------------------------------------------------------------
void baiguo(vec3 o, vec3 v, int sz = 100)
{
	float s_t0X[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	 real alpha = rrnd(0, 0.25);
	 v = vec(0, rrnd(-0.1, 0.1), 1);
	 v.norm();

	 vec3 lgt(-.1, -.5, 1); lgt.norm();
	 COLOR_HSL hsl;
	 hsl.luminance = blend(50, 250, rrnd(0, 1), 1);
	 hsl.luminance = blend(hsl.luminance, 255, -lgt.dot(v), 8); // 高光
	 
	 vec3 vv0 = v.cross(vec3::UX); vv0.norm();
	 real sw = 0.0004 * rrnd(0.6, 1.5);
	 int len = 500;
	 int corind = rand() % 2;
	 for (int i = 0; i < len; i++)
	 {
		 real ai = i / real(len);
		 real ang = blend(0, 2 * PI, ai);

		 vec3 vv = vv0.rotcopy(ang, v);

		 int wid = sz * (0.75 + 0.25 * FT(ang, s_t0X));
		 {
			 vec p = o;
			 for (int ii = 0; ii < wid; ii++)
			 {
				 real aj = ii / real(wid);
				 p = p + blend(vv, v, aj) * sw;

				 real ft2d = FT2D(2 * PI * ai, 2 * PI * aj);
				 real alpha1 = FT2D(2 * PI * aj, 2 * PI * ai);
				 real ft = sin(ai * 10 * alpha1 + aj * 10 * ft2d);

				 hsl.hue = blend(0, 10, alpha1, abs(ft2d) * 4);
				 hsl.saturation = blend(200, 255, ft, 2);

				 int corr = HSLtoRGB(&hsl);
				 vec ddp = vec(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)) * rrnd(0, 0.001);
				// vec ddp = v * rrnd(0, 0.001);
				 //pixel(p + ddp, corr);
				 if (p.y > 0)
				 {
					 point(p + ddp, rrnd(1, 8), blendcor(1, corind == 0 ? RGB(255, 255, 0) : RGB(255, 0, 0), 0.5 + 0.5 * sin(PI * 4 * aj)), alpha);

					 {// daoying
						 vec ttpp = p;
						 vec dttpp = vec(blend(0.0, 0.01 * alpha1, ttpp.y / 0.25) * sin(ttpp.y * 100 * PI), 0, 0);
						 ttpp.y = -ttpp.y;
						 pixel(ttpp + dttpp, blendcor(1, corr, 0.5), 0.5 * alpha);
					 }
				 }
			 }
		 }
	 }
}

// ------------------------------------------------------------------------------------------------
 void baiye(vec3 o, vec3 v, int sz = 100)
{
	 if (o.y < 0)
		 return;

    float s_t0X[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	real alpha = rrnd(0, 1);
	 v = vec(0, rrnd(-0.1, 0.1), 1);
	 v.norm();

	 vec3 lgt(-.1, -.5, 1); lgt.norm();
	 COLOR_HSL hsl;
	 hsl.luminance = blend(50, 250, rrnd(0, 1), 1);
	 hsl.luminance = blend(hsl.luminance, 255, -lgt.dot(v), 8); // 高光

	 vec3 vv0 = v.cross(vec3::UX); vv0.norm();
	 int len = 500;
	 for (int i = 0; i < len; i ++)
	 {
		 real ai = i / real(len);
		 real ang = blend(0, 2 * PI, ai);

		 vec3 vv = vv0.rotcopy(ang, v);

		 int wid = sz * (0.5 + 0.5 * sin(ang * 18));
		 {
			 vec p = o;
			 for (int ii = 0; ii < wid; ii++)
			 {
				 real aj = ii / real(wid);
				 p = p + vv * 0.0003;

				 real ft2d = FT2D(2 * PI * ai, 2 * PI * aj);
				 real alpha1 = FT2D(2 * PI * aj, 2 * PI * ai);
				 real ft = sin(ai * 10 * alpha1 + aj * 10 * ft2d);

				 hsl.hue = blend(30, 40, alpha1, abs(ft2d) * 4);
				 hsl.saturation = blend(100, 255, ft, 2);

				 int corr = HSLtoRGB(&hsl);
				 if (p.y > 0)
				 {
					 pixel(p, blendcor(1, RGB(rrnd(0, 150), 255, rrnd(0, 0)), 0.5 + 0.5 * sin(PI * 4 * aj)), alpha);

					 {// daoying
						 vec ttpp = p;
						 vec dttpp = vec(blend(0.0, 0.01 * alpha1, ttpp.y / 0.25) * sin(ttpp.y * 100 * PI), 0, 0);
						 ttpp.y = -ttpp.y;
						 pixel(ttpp + dttpp, blendcor(1, corr, 0.5), 0.5 * alpha);
					 }

					 // 阴影	
					 vec pp = p;
					 if (pp.y < 0.8 && rrnd(0, 1000) < blend(10, 0, pp.y / 0.8, 0.25))
					 {
						 vec2 lxz(lgt.x, lgt.z);
						 real l = lxz.len() / abs(lgt.y) * pp.y;
						 lxz.norm();
						 vec3 oo(pp.x, 0, pp.z);
						 lxz.rot(blend(0, rrnd(0, PI * 0.05), pp.y / 0.8));
						 vec3 shadowpt = oo + vec(lxz.x, 0, lxz.y) * l;
						 //vec2 p2d = viewprj(shadowpt);
						 //real shadowang = rrnd(0, 2 * PI);

						 if (rrnd(0.25, 1) < 0.5)
							 point(shadowpt, blend(4, 16, ai / 0.5, 1) * rrnd(4, 8), 1, blend(0.001, 0.00001, pp.y / 0.8, 0.5 * alpha));

						 //pointf(int(p2d.x * IMAGESCALE), int(p2d.y * IMAGESCALE), rrnd(32, 64), 1, blend(0.01, 0.001, pp.y / 0.5));
						 //shadowpixel(int(p2d.x * IMAGESCALE), int(p2d.y * IMAGESCALE), 1, 0.5, 1);
					 }
				 }

			 }
		 }
	 }
}
// ------------------------------------------------------------------------------------------------
 void yabai(vec3 o, vec3 v, vec3 v0, int len = 5000, real r00 = 0.04, int depth = 0)
{
	real alphaa = rrnd(0.1, 0.8);
	vec3 s_dpts[] = {
		v,
		vec3(rrnd(-1, 1), rrnd(0, .5), rrnd(-1, 1)),
		vec3(rrnd(-1, 1), rrnd(0, .5), rrnd(-1, 1)),
		vec3(rrnd(-1, 1), rrnd(0, .5), rrnd(-1, 1))
	};
	real s_dptsf[] = {
		1,
		0.85,
		0.75,
		0.5
	};

	float s_t0X[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
	float s_t0Y[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };

	int bcor = blendcor(1, 0xFFFFFF, rrnd(0, 1));

	vec3 lgt(-.1, -.5, 1); lgt.norm();	
	
	vec3 p = o;
	vec3 enddir(rrnd(-0.5, 0.5), rrnd(0.1, 0.5), -rrnd(0.8, 1.25)); enddir.norm();

	vec3 v1;
	vec3 g(0, -0.000011, 0);
	vec3 vg;
	vec3 uz = vec3::UZ;
	real ppsz = 16;
	real dppsz = 0;
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);

		v1 = bezier3(s_dpts, ai); v1.norm();
		if (depth > 1)
			v0 = v0 + vec3(rrnd(-1, 1), rrnd(-1, 1.01), rrnd(-1, 1)) * 0.0;
		else
			v0 = v0 + vec3(rrnd(-1, 1), rrnd(-1, 1.01), rrnd(-1, 1)) * 0.12;

		v0.norm();

		v = blend(v, blend(v0, v1, 0.25), 0.1);
		if (depth > 1)
		{
			vg = vg + g;
			//v = v - vg;
		}
		else
		{
			vg = vg + g;
			v = v - vg;			
		}
		v.norm();
		p = p + v * 0.0004;
		
		vec3 vv = v.cross(uz); vv.norm();
		uz = vv.cross(v); uz.norm();

		real r0 = r00 * bezier3(s_dptsf, ai) * blend(1, depth == 0 ? 1 : 1, ai);

		if (i == len - 1 && 
			depth < 10 &&
			//(rand() % 100 < 50 || depth < 2 || depth > 6) &&
			r0 > 0.000)
		{			
			{
				vec vvv = blend(v, vv, blend(0.75, 0.9, depth / 7.0));
				real len1 = depth == 0 ? len * 0.2 : 0.618 * len;
				
				yabai(p, v, vvv.rotcopy(PI * 2 / 3, v).normcopy(), len1, r0, depth + 1);
				yabai(p, v, vvv.rotcopy(2 * PI * 2 / 3, v).normcopy(), len1, r0, depth + 1);
				yabai(p, v, vvv.rotcopy(0, v).normcopy(), len1, r0, depth + 1);
			}
		}

		dppsz -= rrnd(0, 0.001);
		ppsz += dppsz;
			
		int len1 = 2 * r0 * 3.5 * IMAGESCALE;
		
		{
			if (depth > 3 && rand() % 10500 < 15)
			{
				//baiye(p + vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)) * rrnd(0, 0.005), vv.rotcopy(PI / 2, v), blend(1.5, 0.8, ai) * rrnd(50, 100));
			}

			if (depth > 3 && rand() % 10500 < 15)
			{
				//baiguo(p + vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)) * rrnd(0, 0.005), vv.rotcopy(-PI / 2, v), blend(1.5, 0.8, ai) * rrnd(50, 100));
			}
			if (len1 >= 2 && depth <= 4)
			{
				static vec3 lowerpp[2000];
				static vec3 lstpp;
				for (int j = 0; j < len1; j++)
				{
					real aj = j / real(len1);
					real ang = aj * 2 * PI;
					vec3 tvv = vv.rotcopy(ang, v);
					tvv.norm();

					real r = r0;
					//r = abs(r0 * (1 + 0.25 * FT2D(aj * PI * 4, ai * PI * 4, s_t0X, s_t0Y)));
					r = abs(r0 * (1 + 0.25 * sin(aj * PI * 20)));

					vec3 pp = p + tvv * r;

					if (i > 0 && j > 0)
					{
						vec3 normal = (lstpp - pp).cross(lowerpp[j % 2000] - pp); normal.norm();

						COLOR_HSL hsl;
						hsl.luminance = blend(1, 25, -lgt.dot(normal), 1);
						hsl.luminance = blend(hsl.luminance, 100, -lgt.dot(normal), 8); // 高光						

						real ft2d = FT2D(8 * PI * ai, 8 * PI * aj);
						real alpha1 = FT2D(6 * PI * aj, 6 * PI * ai);
						real ft = sin(aj * 15);

						//hsl.luminance = blend(5, 30, abs(lgt.dot(normal)), 4 * alpha1);

						hsl.hue = blend(40, 99, ft);
						hsl.saturation = depth > 3 ? blend(5, 120, ft, 2) : 0;

						int corr = HSLtoRGB(&hsl);

						//if (depth <= 2)
						//if (j % 50 == 0)
						{
							//point(pp, blend(1, rrnd(2, 16), abs(normal.x), 4), corr);
							//if (depth == 0)
							//	point(pp, blend(16, 1, ai, 0.5) * alpha1, 1);
							//else
							//	point(pp, rrnd(1, 4), 1);
							corr = blendcor(RGB(rrnd(0, 255), rrnd(0, 255), rrnd(0, 255)), corr, rrnd(0.8, 1));
							point(pp, rrnd(1, r00 * IMAGESCALE * 0.1 + 4), corr, rrnd(0.75, 1));

							{// daoying
								vec ttpp = pp;
								vec dttpp = vec(blend(0.0, 0.025, ttpp.y / 0.25) * sin(ttpp.y * 50 * PI), 0, 0);
								ttpp.y = -ttpp.y;
								point(ttpp + dttpp, rrnd(1, r00 * IMAGESCALE * 0.1 + 4), blendcor(1, corr, 0.5), 0.5);
							}						

							// 阴影			
							if (pp.y < 0.99 && rrnd(0, 1000) < blend(20, 0, pp.y / 0.5, 0.25))
							{
								vec2 lxz(lgt.x, lgt.z);
								real l = lxz.len() / abs(lgt.y) * pp.y;
								lxz.norm();
								vec3 oo(pp.x, 0, pp.z);
								lxz.rot(blend(0, rrnd(0, PI * 0.05), pp.y / 0.5));
								vec3 shadowpt = oo + vec(lxz.x, 0, lxz.y) * l;
								//vec2 p2d = viewprj(shadowpt);
								//real shadowang = rrnd(0, 2 * PI);

								if (rrnd(0.25, 1) < 0.5)
									point(shadowpt, blend(4, 16, ai / 0.5, 1) * blend(4, 8, rndmap[j % 512][i % 512]), 1, blend(0.001, 0.00001, pp.y / 0.5, 0.5));

								//pointf(int(p2d.x * IMAGESCALE), int(p2d.y * IMAGESCALE), rrnd(32, 64), 1, blend(0.01, 0.001, pp.y / 0.5));
								//shadowpixel(int(p2d.x * IMAGESCALE), int(p2d.y * IMAGESCALE), 1, 0.5, 1);
							}
						}
					}
					lstpp = lowerpp[j % 2000] = pp;
				}
			}
			else
			{
				if (p.y > 0)
				{					
					point(p, 1 + rrnd(1, 4) * blend(1.5, 0.128, ai), blendcor(0xFF004000, 1, rrnd(0, 0.9)), rrnd(0.8, 1.2) * alphaa);
					vec pp = p;
					{// daoying
						vec ttpp = pp;
						vec dttpp = vec(blend(0.0, 0.025, ttpp.y / 0.25) * sin(ttpp.y * 25 * PI), 0, 0);
						ttpp.y = -ttpp.y;
						point(ttpp + dttpp, rrnd(1, r00 * IMAGESCALE * 0.1 + 4), blendcor(1, 1, 0.5), 0.1);
					}
					// 阴影			
					if (pp.y < 0.99 && rrnd(0, 1000) < blend(20, 0, pp.y / 0.5, 0.25))
					{
						vec2 lxz(lgt.x, lgt.z);
						real l = lxz.len() / abs(lgt.y) * pp.y;
						lxz.norm();
						vec3 oo(pp.x, 0, pp.z);
						lxz.rot(blend(0, rrnd(0, PI * 0.05), pp.y / 0.5));
						vec3 shadowpt = oo + vec(lxz.x, 0, lxz.y) * l;
						//vec2 p2d = viewprj(shadowpt);
						//real shadowang = rrnd(0, 2 * PI);

						if (rrnd(0.25, 1) < 0.5)
							point(shadowpt, blend(4, 16, ai / 0.5, 1) * rrnd(4, 8), 1, blend(0.001, 0.00001, pp.y / 0.5, 0.5));

						//pointf(int(p2d.x * IMAGESCALE), int(p2d.y * IMAGESCALE), rrnd(32, 64), 1, blend(0.01, 0.001, pp.y / 0.5));
						//shadowpixel(int(p2d.x * IMAGESCALE), int(p2d.y * IMAGESCALE), 1, 0.5, 1);
					}
					// if (rand() % 1000 < 15)
					// {
						// point(p + vec3(rrnd(-1, 1), rrnd(-1, 1), rrnd(-1, 1)) * rrnd(0, 0.05), rrnd(1, 32), blendcor(1, 1, rrnd(0, 1), 0.25), rrnd(0.1, 0.8));
					// }
				}
			}			
		}		
	}
}
