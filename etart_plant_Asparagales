// ------------------------------------------------
// 水仙花 Narcissus tazetta subsp. chinensis
// ------------------------------------------------
real factor2 = 1;
void shuixianhuaban(vec3 p, vec3 v, vec3 up0, int cor0)
{
	real sz = .2 * 15 * (0.5);
	
	vec3 lgtdir(1, -1, 1); lgtdir.norm();
	v.norm();
	vec3 g(0, -0.0001, 0);
	vec3 up = up0;
	real deta = rrnd(.5, 1);
	vec3 dup = up * rrnd(-0.5, 0.5);
	for(int i = 0; i < 300; i ++)
	{
		real ai = real(i) / 300;
		v = v + g;
		v.norm();
		
		p = p + v * (0.0001 * sz);
		vec3 norm0 = v.cross(up);norm0.norm();
		up = norm0.cross(v);
		
		real alpha = blend2d(0.8, 1.2, p.x, p.y);		
		real ssz = CircleBlend2(0, 0.01 * sz * alpha, ai, 1) * 0.5;	
		
		for(int ii = 0; ii < 250; ii += 1)
		{
			real aii = real(ii) / 250;	
			
			vec3 p1 = p + (up * blend(0, 1, aii, 2) + norm0) * ssz;
			vec3 p2 = p + (up * blend(0, 1, aii, 2) - norm0) * ssz;	
			
			int cor = cor0;//blendcor(cor0, RGB(108, 17, 17),  ai);
			cor = blendcor(cor, 0xFFFFFFFF, 0.5 + 0.5 * sin(aii * 10));
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
// -------------------------------------------------
void shuixianhua(vec3 p, vec3 v, int len, int wid0)
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
				pixelAA(p + vec3(blend(0, 0.002, beta), 0, 0), 1);//blendcor(0xFF80a0cc, 1, 0.5 + 0.5 * sin(beta * 18 * PI + alp2d * PI), 2));
				pixelAA(p - vec3(blend(0, 0.002, beta), 0, 0), 1);//blendcor(0xFF80a0cc, 1, 0.5 + 0.5 * sin(beta * 18 * PI + alp2d * PI), 2));						
			}			
		}
		
		if(i == len - 1)
		{
			int leaflen = len * 0.5;
			vec3 zz = vec3::UY;
			for(int j = 0; j < 8; j ++)
			{
				real betaj = real(j) / (8);
				int jj = j % 8;
				int ii = j / 8;				
								
				vec3 vv0 = zz.cross(vec3::UZ);vv0.norm();					
				quaternion q(rrnd(-1, 1) * 0.1 * PI + blend(-PI, PI, real(jj) / (8)), vec3::UZ);	
				vv0 = (q * vv0);
				zz = (vv0).cross(vec3::UZ);zz.norm();
				factor2 = rrnd(0.1, .12);
				shuixianhuaban(p, vv0, -vec3::UZ, 0xFF00FFFF);
			}	
		}
		
	}
}
