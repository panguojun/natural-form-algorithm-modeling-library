/*
    etart->animal->Insecta
*/
// ------------------------------------------------------------------------------------------------
// butterfly 蝴蝶
// ------------------------------------------------------------------------------------------------
int hutcor = 0xFF80a0a0;
int hutcor1 = 0xFF00FFFFF;
void butflyWing(vec p, vec v, vec up)
{
	vec3 lgt(-1, 1, -1); lgt.norm();
	vec3 lowerpp[500];
	vec3 lstpp;

	static vec2 dots[] = {vec2(0.0, 0.75), vec2(0.25, 0.75), vec2(0.5, 0.75), vec2(0.75, 0.75)};
	v.norm();
	up.norm();
	int wid = 500;
	for(int i = 0; i < wid; i ++)
	{
		real ai = i / real(wid);
		real ang0 = PI / 8;
		real ang = blend(ang0, ang0 + PI / 2, ai);
		vec vv = v.rotcopy(ang, up);vv.norm();
		vec pp = p;
		int len = blend(300 + 100 * FTU(ai * PI * 4), 0, ai, 1) + rrnd(0, 4);
		for(int ii = 0; ii < len; ii ++)
		{
			real aii = ii / real(len);
			pp = pp + vv * 0.0005;
			vv.rot(0.0025, up);
			vv.rot(0.0001, v);

			bool isIn = 0;
			for(int j = 0; j < 4; j ++)
			{
				if((vec2(ai, aii) - dots[j]).len() < 0.05)
				{
						isIn = 1;
						break;
				}
			}
			if(isIn)
				pixel(pp, 1);
			else
			{
					if(aii < 0.75 && abs(sin(ai * PI * 4)) < rrnd(0.1, 0.2))
						pixel(pp, 1);
					else if(abs(sin(ai * PI * 8)) < rrnd(0.1, 0.2))
						 pixel(pp, 1);
					else if(aii > 0.85)
						pixel(pp, 1);
					else
					{
						vec3 normal = (lstpp - pp).cross(lowerpp[ii] - pp); normal.norm();
						real ndot = abs(normal.dot(lgt));
						pixel(pp, blendcor(hutcor, hutcor1, ndot));
					}
			}
			lstpp = lowerpp[ii] = pp;
		}
	}
}

void butflyTail(vec p, vec v, vec up)
{
	vec3 lgt(-1, 1, -1); lgt.norm();
	vec3 lowerpp[500];
	vec3 lstpp;

	static vec2 dots[] = {vec2(0.25, 0.75), vec2(0.5, 0.75), vec2(0.75, 0.75)};
	v.norm();
	up.norm();
	real ang0 = PI / 8 + PI / 2;
	int wid = 300;
	for(int i = 0; i < wid; i ++)
	{
		real ai = i / real(wid);
		real ang = blend(ang0, ang0 + PI / 4, ai);
		vec vv = v.rotcopy(ang, up);vv.norm();
		vec pp = p;
		int len = blend2(0, 200 + 25 * sin(abs(0.5 - ai) / 0.5 * PI * 5), ai, 2) + rrnd(0, 4);
	//	len *= blend3(0, 1, ai, 0.05, 8);
		for(int ii = 0; ii < len; ii ++)
		{
			real aii = ii / real(len);
			pp = pp + vv * 0.0005;

			bool isIn = 0;
			for(int j = 0; j < 3; j ++)
			{
				if((vec2(ai, aii) - dots[j]).len() < 0.05)
				{
						isIn = 1;
						break;
				}
			}
			if(isIn)
				pixel(pp, 1);
			else
			{
				if(aii < 0.75 && abs(sin(ai * PI * 4)) < 0.05)
					pixel(pp, 1);
				else if(aii > 0.85)
					pixel(pp, 1);
					else
					{
						vec3 normal = (lstpp - pp).cross(lowerpp[ii] - pp); normal.norm();
						real ndot = abs(normal.dot(lgt));
						pixel(pp, blendcor(hutcor, hutcor1, ndot));
					}
			}
			lstpp = lowerpp[ii] = pp;
		}
	}
}
void butfly(vec p, vec v)
{
	hutcor = blendcor(0xFF008080, 0xFF00a0a0, rrnd(0, 1));
	hutcor1 = blendcor(0xFF20a0a0, 0xFF80FFFF, rrnd(0, 1));

	v.norm();
	vec up = v.cross(vec::UY);
	up = up.cross(v);

	butflyWing(p, v, up.rotcopy(-PI / 4, v));
	butflyTail(p, v, up.rotcopy(-PI / 4, v));
	butflyWing(p, v, up.rotcopy(PI / 4, v));
	butflyTail(p, v, up.rotcopy(PI / 4, v));
}
