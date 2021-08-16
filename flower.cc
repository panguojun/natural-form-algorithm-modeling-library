/**************************************************
	Algorithm to generate nature forms
	  算法生成自然形态的艺术
	     Minimal code
	       最简化代码

**************************************************/
#define edge		PMHG::EDGE
#define loopi(len)	for(int __len = len, i=0; i < __len; i++)
#define __ai		(i / real(__len - 1))
#define PI2			(PI * 2.0f)

#define trunke()	pushc();push(e);
#define trunk()		pushc();push();
#define endtk()		popc();pop();

namespace flowers 
{
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
	inline void comv(bool b)
	{
		gcommonvertex = b;
	}
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
		for (int i = 0; i < n; i++)
			coordstack.pop_back();
	}
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
	inline void face()
	{
		face(estack[estack.size() - 2], estack.back());
	}
	inline void scl(real s)
	{	
		scaleedge(estack.back(), s, coordstack.back().o);
		coordstack.back().scl *= s;
	}
	inline coord_t& coord()
	{
		return coordstack.back();
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
		PRINT("----DUMP----")
		for (auto& it : e.vlist)
			PRINTVEC3(it.p);
	}

	// -------------------------------------
	// TOPE 3D
	// -------------------------------------
	void branch(int d = 0)
	{
		trunk();
		ext(5.);
		face();
			pushc();
			real ang = 20.;
			loopi(5){
				push();
					ang = ang * 0.7 - 10.;
					pit(ang);
					scl(0.95);
					ext(1.);
					face();
			}

			if(d < 3.) {
				branch(d + 1.);
			}
			pop(5);
			popc();

			pushc();
			ang = -20.;
			loopi(5){
				push();
					ang = ang * 0.8 + 10.;
					pit(ang);
					ext(1.);
					scl(0.95);
					face();
			}
			if(d < 4.) {
				branch(d + 1.);
			}
			pop(5);
			popc();

		endtk();
	}

	void pipe(int d = 0)
	{
		trunk();
		//coorddummy();
		rot(blend(-10, 10, d / 13.), coord().ux);
		scl(blend(1.2, 0.25, d / 13., 2));
		ext(1.);
		face();

		if (d < 13)
			pipe(d + 1);
		endtk();
	}
	
	// -------------------------------------
	// 造型代码
	// -------------------------------------
	void pipe_lotus_flower1(int d = 0)
	{
		trunk();
		rot(blend(-10, 10, d / 13.), coord().ux);
		rot(blend(-10, 10, d / 13.), coord().uy);
		scl(0.95);
		ext(1.);
		face();

		if (d < 13)
			pipe_lotus_flower1(d + 1);
		pop();
	}
	void pipe_lotus_flower2(int d = 0)
	{
		trunk();
		rot(blend(-10, 10, d / 13.), coord().ux);
		scl(blend(1.2, 0.25, d / 13., 2));
		ext(1.);
		face();

		if (d < 13)
			pipe_lotus_flower2(d + 1);
		endtk();
	}
	void lotus_flower()
	{
		comv(true);

		{
			edge e;
			loopi(8) {
				real ang = __ai * PI2;
				real r = 1.0f;
				vec p = vec(cos(ang), 0, sin(ang)) * r;
				e = e | p;
			}
			trunke(e);
			pipe_lotus_flower1();
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
				trunke(e);
				dump(e);
				yaw(45 * i);
				pipe_lotus_flower2();

				endtk();
			}
		}
	}
}
void test()
{
	flowers::lotus_flower();
}
