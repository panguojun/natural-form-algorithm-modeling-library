/**************************************************
	Algorithm to generate nature forms
		  算法生成自然形态的艺术
		Minimal code
		  最简化代码
**************************************************/
#define PI2			(PI * 2.0f)
#define edge			edge_t
#define loopi(len)		for(int __len = len, i=0; i < __len; i++)
#define __ai			(i / real(__len - 1))
#define loopj(len)		for(int __lenj = len, j=0; j < __lenj; j++)
#define __aj			(j / real(__lenj - 1))

#define begintke(e)		pushc();push(e);
#define begintk			pushc();push();
#define endtk			popc();pop();

#define curstack		estack.back()

namespace nforms
{
	// -------------------------------------
	// settings
	// -------------------------------------
	inline void comv(bool b)
	{
		gcommonvertex = b;
	}
	inline int rgb(const vec3& v)
	{
		return _RGB(int(v.x * 255), int(v.y * 255), int(v.z * 255));
	}
	inline int rgb(int r, int g, int b)
	{
		return _RGB(r, g, b);
	}

	// -------------------------------------
	// POLY APIs
	// -------------------------------------
	// stack
	void push(edge& e)
	{
		if (e.vlist.size() > 0)
		{
			if (!coordstack.empty())
			{
				edge ne = e;
				const coord_t& crd = coordstack.back();
				for (auto& it : ne.vlist)
				{
					it.p = crd.o + crd.ux * (it.p.x * crd.scl) + crd.uy * (it.p.y * crd.scl) + crd.uz * (it.p.z * crd.scl);
				}
				estack.push_back(ne.vlist);
			}
			else
				estack.push_back(e.vlist);
		}
	}
	void push()
	{
		if (estack.empty())
			estack.push_back(VECLIST());
		else
		{
			estack.push_back(estack.back());
		}
	}
	inline void pop(int n = 1)
	{
		if (n < 0)
			n = estack.size() + n + 1;
		for (int i = 0; i < n; i++)
			estack.pop_back();
	}
	inline void cls(edge& e)
	{
		closeedge(e.vlist);
	}
	void pushc()
	{
		if (estack.empty() && coordstack.empty())
		{
			coordstack.push_back(coord_t(vec::UX, vec::UY, vec::UZ));
		}
		if (coordstack.empty())
			coordstack.push_back(coord_t(estack.back()));
		else
			coordstack.push_back(coordstack.back());
	}
	inline void popc(int n = 1)
	{
		if (n < 0)
			n = coordstack.size() + n + 1;
		for (int i = 0; i < n; i++)
			coordstack.pop_back();
	}

	// draw
	inline void face(int type = 0)
	{
		if(type == 1)
			face_odd(estack[estack.size() - 2], estack.back());
		else
			face(estack[estack.size() - 2], estack.back());
	}
	
	// coord
	inline coord_t& coord()
	{
		return coordstack.back();
	}
	inline int coordcur()
	{
		return coordstack.size() - 1;
	}
	void rotcoord(real degree, crvec ax)
	{
		float ang = degree * PI / 180.0f;
		coord_t& cb = coordstack.back();

		cb.ux.rot(ang, ax);
		cb.uz.rot(ang, ax);
		cb.uy = cb.ux.cross(cb.uz).normcopy();
	}

	// ext / mov
	void ext(real d)
	{
		VECLIST& e1 = estack.back();
		vec norm = getedgenorm(e1);
		vec dv = norm * d;
		{
			for (int i = 0; i < e1.size(); i++)
			{
				e1[i].p += dv;
				e1[i].ind = -1;
			}
		}
		coordstack.back().o += dv;
	}
	void mov(crvec v)
	{
		VECLIST& e = estack.back();
		for (int i = 0; i < e.size(); i++)
		{
			e[i] = e[i] + v;
		}
		coordstack.back().o += v;
	}
	// scaling
	inline void scl(real s)
	{
		scaleedge(estack.back(), s, coordstack.back().o);
		coordstack.back().scl *= s;
	}

	// rotation
	void rotex(real degree, crvec ax, crvec o)
	{
		float ang = degree * PI / 180.0f;
		coord_t& cb = coordstack.back();

		rotedge(estack.back(), ang, o, ax);

		cb.ux.rot(ang, ax);
		cb.uz.rot(ang, ax);
		cb.uy = cb.ux.cross(cb.uz).normcopy();
	}
	void rot(real degree, crvec ax)
	{
		float ang = degree * PI / 180.0f;
		coord_t& cb = coordstack.back();
		
		rotedge(estack.back(), ang, cb.o, ax);

		cb.ux.rot(ang, ax);
		cb.uz.rot(ang, ax);
		cb.uy = cb.ux.cross(cb.uz).normcopy();
	}
	void yaw(real degree)
	{
		float ang = degree * PI / 180.0f;
		vec enorm = getedgenorm2(estack.back());
		coord_t& cb = coordstack.back();
		rotedge(estack.back(), ang, cb.o, enorm);

		cb.ux.rot(ang, enorm);
		cb.uz.rot(ang, enorm);
		cb.uy = cb.ux.cross(cb.uz).normcopy();
	}
	void pit(real degree)
	{
		float ang = degree * PI / 180.0f;
		VECLIST& e = estack.back();
		vec ux, uy, uz;
		edgeax2(e, ux, uy, uz);
		coord_t& cb = coordstack.back();
		rotedge(e, ang, cb.o, ux);

		cb.uy.rot(ang, ux);
		cb.uz.rot(ang, ux);
		cb.ux = cb.uz.cross(cb.uy).normcopy();
	}
	void roll(real degree)
	{
		float ang = degree * PI / 180.0f;
		VECLIST& e = estack.back();
		vec ux, uy, uz;
		edgeax2(e, ux, uy, uz);
		coord_t& cb = coordstack.back();
		rotedge(e, ang, cb.o, uz);

		cb.uy.rot(ang, uz);
		cb.ux.rot(ang, uz);
		cb.uz = cb.uy.cross(cb.ux).normcopy();
	}

	// smooth
	void smooth()
	{
		VECLIST& e = estack.back();
		VECLIST ee;
		doublevnum(e, ee, isedgeclosed(e) ? e.size() : e.size() - 1);
		vec3 o = getedgecenter(e);
		for (int i = 1; i < e.size(); i++)
		{
			crvec p1 = e[i - 1].p;
			crvec p2 = e[i].p;
			vec3 p = (p1 + p2) / 2.;
			p = o + (p - o).normcopy() * (((p1 - o).len() + (p2 - o).len()) / 2.0);
			ee[(i * 2 - 1) % ee.size()].p = p;
		}
		e = ee;
	}

	// -------------------------------------
	// debug
	// -------------------------------------
	void coorddummy0(real x = 0, real y = 0, real z = 0)
	{
		color = 0xFF0000FF;
		ptr(vec(x, y, z), vec(x + 0.5, y, z), 0.025);
		color = 0xFF00FF00;
		ptr(vec(x, y, z), vec(x, y + 0.25, z), 0.025);
		color = 0xFFFF0000;
		ptr(vec(x, y, z), vec(x, y, z + 0.125), 0.025);
	}
	void coorddummy()
	{
		int oldcolor = color;
		coord_t& cb = coordstack.back();
		color = 0xFF0000FF;
		ptr(cb.o, cb.o + cb.ux, 0.05);
		color = 0xFF00FF00;
		ptr(cb.o, cb.o + cb.uy, 0.05);
		color = 0xFFFF0000;
		ptr(cb.o, cb.o + cb.uz, 0.05);
		color = oldcolor;
	}
	void dump(edge& e)
	{
		PRINT("----DUMP----");
		for (auto& it : e.vlist)
			PRINTVEC3(it.p);
	}

	// -------------------------------------
	// TOPE 3D
	// -------------------------------------
	#define trunk_cb(d) [](int d)->void
	void trunk(int d, int maxd, std::function<void(int)> fun)
	{
		begintk;
		fun(d);

		if (d < maxd)
			trunk(d + 1, maxd, fun);
		endtk;
	}
	void trunk(int d = 0)
	{
		begintk;
			rot(blend(-10, 10, d / 13.), coord().ux);
			scl(blend(1.2, 0.25, d / 13., 2));
			ext(1.);
			face();

			if (d < 13)
				trunk(d + 1);
		endtk;
	}
	void branch(int d = 0)
	{
		real ang;
		begintk;
			ext(5.);
			face();

			pushc();
			ang = rrnd(-8., 8.);
			loopi(5) {
				push();
				pit(ang);
				yaw(rrnd(0., 18.));
				scl(0.95);
				ext(1.);
				face();
			}
			if (d < 3.) {
				branch(d + 1.);
			}
			pop(5);
			popc();

			pushc();
			ang = rrnd(-8., 8.);
			loopi(6) {
				push();
				pit(ang);
				yaw(-rrnd(0., 18.));
				ext(1.);
				scl(0.925);
				face();
			}
			if (d < 5.) {
				branch(d + 1.);
			}
			pop(6);
			popc();

		endtk;
	}

	// -------------------------------------
	// tree
	// -------------------------------------
	void tree()
	{
		comv(true);
		rgb(200, 200, 200);

		edge e;
		loopi(8) {
			real ang = __ai * PI2;
			real r = 2.0f;
			vec p = vec(cos(ang), 0, sin(ang)) * r;
			p.y *= blend2(1., 8., __ai, 2.);
			e = e | p;
		}

		begintke(e);
		branch();
		endtk
	}

	// -------------------------------------
	// lotus
	// -------------------------------------
	void trunk_lotus1(int len, int d)
	{
		begintk;
		rot(blend(-10, 10, d / real(len)), coord().ux);
		rot(blend(-10, 10, d / real(len)), coord().uy);
		scl(0.95);
		ext(1.);
		face();

		if (d < len)
			trunk_lotus1(len, d + 1);
		pop();
	}
	void trunk_lotus2(int len, real sz, int d)
	{
		begintk;
		rot(blend(-10, 10, d / 13.), coord().ux);
		scl(blend(1.5 * sz, 0.25, d / 13., 2));
		ext(1.);
		face();

		if (d < len)
			trunk_lotus2(len, sz, d + 1);
		endtk;
	}
	void lotus()
	{
		comv(true);

		pushc();
		int cdc = coordcur();
		loopj(4)
		{
			rotcoord(90, vec3::UY);
			coorddummy();
			color = rgb(50, 150, 0);
			{
				edge e;
				loopi(8) {
					real ang = __ai * PI2;
					real r = 1.0f;
					vec p = vec(cos(ang), 0, sin(ang)) * r;
					e = e | p;
				}
				push(e);
				trunk_lotus1(15 + 5 * j, 0);
				pop();
			}
			{
				edge e;
				loopi(8) {
					real ang = __ai * PI;
					real r = 1.0f;
					vec p = vec(cos(ang), 0, sin(ang)) * r;
					e = e | p;
				}

				color = rgb(240, 150, 250);
				loopi(8) {
					begintke(e);
					yaw(45 * i);
					trunk_lotus2(10 + j * 4, blend(0.8, 1.0, __aj), 0);

					endtk;
				}
			}
			popc(-cdc - 2);
		}
	}

	// -------------------------------------
	// shell
	// -------------------------------------
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

		//savesubmesh3DS("C:\\Users\\18858\\Documents\\LAB\\ZEXE\\shell.obj", SUBMESH);
	}
	void pipeshell()
	{
		comv(true);
		rgb(200, 200, 150);

		edge e;
		loopi(32) {
			real ang = __ai * PI2;
			real r = 12.0f;
			vec p = vec(cos(ang), 0, sin(ang)) * r;
			
			e = e | p;
		}

		begintke(e);
		trunk(0, 158,
			trunk_cb(d){
			//rot(blend(10, 15, d / 58.), vec3::UX);
			//mov(vec3::UX* blend(0.25, 0.15, d / 158.));

			rot(blend(-1, 1, noise(coord().o)), vec3::UX);
			scl(0.995);
			ext((d % 2 == 1 ? 1 : 0.25)* blend(1, 0.25, d / 158.) * 1);
			face(d % 2);
		}
		);
		endtk;

		//savesubmesh3DS("C:\\Users\\18858\\Documents\\LAB\\ZEXE\\shell.obj", SUBMESH);
	}
	void shiyan1()
	{
		comv(true);
		rgb(200, 200, 150);

		edge e;
		loopi(64) {
			real ang = __ai * PI;
			real r = 12.0f;
			vec p = vec(cos(ang), 0, 0.1 + sin(ang)) * r;
			p.z *= blend2(2., 1., __ai, 0.125);

			e = e | p;
		}
		e = e | (e.mirrorcpy(vec::ZERO, vec::UX).invertcpy());

		begintke(e);
		trunk(0, 58,
			trunk_cb(d){
			rot(blend(1, 5, d / 58.), vec3::UZ);
			mov(vec3::UX* blend(0.25, 0.15, d / 58.) * 0.1);
			scl(0.97);
			ext((d % 2 == 1 ? 1 : 0.25)* blend(1, 0.25, d / 58.) * 0.05);
			face(d % 2);
		}
		);
		endtk;
	}
	
}

// -------------------------------------
// test
// -------------------------------------
void test()
{
	nforms::shiyan1();
}
