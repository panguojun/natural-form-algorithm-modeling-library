/*
	3d modeling->plant->Mollusca(软体动物)
*/
// ------------------------------------------------
// Gastropoda
// ------------------------------------------------
void shell()
{
	comv(true);
	rgb(200, 200, 150);

	edge e;
	loopi(64) {
		real ang = __ai * PI;
		real r = 12.0f;
		vec p = vec(cos(ang), 0, sin(ang)) * r;
		p.x *= blend(1., 8., __ai, 18);

		e = e | p;
	}

	begintke(e);
	trunk(0, 158,
		trunk_cb(d){
			rot(blend(10, 15, d / 58.), vec3::UX);
			mov(vec3::UX * blend(0.25, 0.15, d / 158.));
			scl(0.97);
			ext((d % 2 == 1 ? 1 : 0.25) * blend(1, 0.25, d / 158.) * 0.05);
			face(d % 2);
		}
	);
	endtk;
}

