// ------------------------------------------------
// 枫叶
// ------------------------------------------------
void fengyeban(vec3 p, vec3 v, real ssz, vec3 uup)
{
	real sz = ssz;
	
	vec3 lgtdir(1, -1, 1); lgtdir.norm();
	v.norm();
	vec3 g(0, -0.0005, 0);
	uup.norm();
	vec3 up = uup;
	real deta = rrnd(.5, 1);
	
	int cor1 = RGB(rrnd(200, 255), rrnd(115, 215), rrnd(0, 5));
	int cor2 = RGB(rrnd(0, 52), rrnd(18, 28), rrnd(7, 17));
	int cor3 = RGB(rrnd(1, 82), rrnd(8, 18), rrnd(2, 4));
	int cor4 = RGB(rrnd(80, 112), rrnd(8, 80), rrnd(0, 2));
		
	for(int i = 0; i < 1500; i ++)
	{
		real ai = real(i) / 1500;
		v = v + g;
		v.norm();
		p = p + v * (0.00015 * sz);
		vec3 norm0 = v.cross(up);norm0.norm();
		//up = norm0.cross(v);up.norm();
		real alpha = blend2d(0.8, 1.2, p.x, p.y);
		
		vec3 p1 = p + (norm0 + up * 0.1) * blend2(0, 0.05 * sz, ai, 1);
		vec3 p2 = p + (-norm0 + up * 0.1) * blend2(0, 0.05 * sz, ai, 1);
		

		for(int ii = 0; ii < 250; ii += 1)
		{
			real aii = real(ii) / 250;								
			{
				vec3 pp = blend(p, p1, aii);				
				
				vec3 norm = (up + norm0);
				norm.norm();
				//real lum = -lgtdir.dot(norm);			
		
				int cor = blendcor(cor1, cor2, 0.5 + 0.5 * sin(alpha * 80 * (0.75 * ai - 0.25 * aii)));	
				if(norm.z > 0)
					cor = blendcor(cor, cor3, aii);
				else
					cor = blendcor(cor, cor4, aii);
				
				//cor = blendcor(1, cor, 0.5 - 0.5 * up.z, 1);					
				
				pixel(pp, cor);		
	
			}
			{
				vec3 pp = blend(p, p2, aii);		
				alpha = blend2d(0.8, 1.2, pp.x, pp.y);
				vec3 norm = (up - norm0);
				norm.norm();
				//real lum = -lgtdir.dot(norm);			

				int cor = blendcor(cor1, cor2, 0.5 + 0.5 * sin(alpha * 80 * (0.75 * ai - 0.25 * aii)));	
				if(norm.z > 0)
					cor = blendcor(cor, cor3, aii);
				else
					cor = blendcor(cor, cor4, aii);	

				// cor = blendcor(1, cor, 0.5 - 0.5 * norm.z, 1);
				// up * (blend2d(-1, 1, pp.x, pp.y) * 0.1)
				pixel(pp, cor);
			 }					
		}
	}
}
// ------------------------------------------------
void fengye(vec3 p, vec3 v, real sz)
{
	v.norm();
	vec3 up = vec3::UY;
	vec3 norm0 = v.cross(up);norm0.norm();
	
	fengyeban(p, v, 1 * sz, norm0);
	
	fengyeban(p, v.rotcopy(rrnd(-1, 1) * PI / 10 + PI / 4, norm0), 0.75 * sz, norm0);
	fengyeban(p, v.rotcopy(rrnd(-1, 1) * PI / 10 - PI / 4, norm0), 0.75 * sz, norm0);
	
	fengyeban(p, v.rotcopy(rrnd(-1, 1) * PI / 10 + PI / 2, norm0), 0.25 * sz, norm0);
	fengyeban(p, v.rotcopy(rrnd(-1, 1) * PI / 10 - PI / 2, norm0), 0.25 * sz, norm0);	
}
