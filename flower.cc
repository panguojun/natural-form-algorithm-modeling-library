/***************************************
     Algorithm to generate nature forms
	  算法生成自然形态的艺术
	     Minimal code
	       最简化代码

***************************************/
#define edge		PMHG::EDGE
#define loopi(len)	for(int __len = len, i=0; i < __len; i++)
#define __ai		((real)i / __len)
#define PI2			(PI * 2.0f)

namespace flower {
	// -------------------------------------
	// POLY APIs
	// -------------------------------------
	void push(edge& e)
	{
		if (e.vlist.size() > 0)
		{
			if (!coordstack.empty())
			{
				const coord_t& crd = coordstack.back();
				for (auto& it : e.vlist)
				{
					it.p = crd.o + crd.ux * (it.p.x * crd.scl) + crd.uy * (it.p.y * crd.scl) + crd.uz * (it.p.z * crd.scl);
				}
			}
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
	void pop(int n = 1)
	{
		for (int i = 0; i < n; i++)
			estack.pop_back();
	}
	void cls(edge& e)
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
	void popc(int n = 1)
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
	void face()
	{
		face(estack[estack.size() - 2], estack.back());
	}
	void scl(real s)
	{	
		scaleedge(estack.back(), s, coordstack.back().o);
		coordstack.back().scl *= s;
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
	void dump(edge& e)
	{
		for (auto& it : e.vlist)
			PRINTVEC3(it.p);
	}

	// -------------------------------------
	// TOPE
	// -------------------------------------
	void branch(int d)
	{
		pushc();
		push();
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
		pop(5.);
		popc();

		pop();
		popc();
	}
	void pipe(int d)
	{
		push();
		pushc();
		pit(20);
		scl(0.95);
		ext(1.);
		face();

		if (d < 3)
			pipe(d + 1);
		popc();
		pop();
	}
	// -------------------------------------
	// 造型代码
	// -------------------------------------
	void tree1()
	{
		edge e;
		loopi(8) {
			real ang = __ai * PI2;
			real r = 1.0f;
			vec p = vec(cos(ang), 0, sin(ang)) * r;
			e = e | p;
		}
		cls(e);

		dump(e);
		push(e);
		pipe(0);
	}
}
void test()
{
	flower::tree1();
}
