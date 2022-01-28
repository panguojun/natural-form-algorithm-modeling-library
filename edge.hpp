// *********************************************
// 边元素
// *********************************************
struct edge_t
{
	vec3 n;
	VECLIST vlist;

	edge_t() {}
	edge_t(crvec _v)
	{
		vlist.push_back(_v);
	}
	edge_t(VECLIST& e)
	{
		vlist = e;
	}
	bool operator==(const edge_t& v) const
	{
		if (vlist.size() != v.vlist.size())
			return false;
		for (int i = 0; i < vlist.size(); i++)
		{
			if (vlist[i].p != v.vlist[i].p)
				return false;
		}
		return true;
	}
	edge_t operator + (const edge_t& v) const
	{
		edge_t e;
		{
			if (vlist.empty())
				return v;

			e.vlist = vlist;
			vec3 lastp = vec3::ZERO;
			for (auto it : v.vlist)
			{
				vec3 dp = (it.p - lastp);
				vec3 np = e.vlist.back().p + dp;
				lastp = it.p;

				bool bloop = false;
				for (int i = 0; i < e.vlist.size(); i++)
				{
					if (np == e.vlist[i].p)
					{
						if (i != 0)
						{
							e.vlist.assign(e.vlist.begin(), e.vlist.begin() + i + 1);
							bloop = true;
							break;
						}
					}
				}
				if (!bloop)
					e.vlist.push_back(np);
			}
		}
		return e;
	}
	edge_t operator - (const edge_t& v) const
	{
		return (*this) + (-v);
	}
	edge_t operator - () const
	{
		edge_t e;
		for (int i = 0; i < vlist.size(); i++)
		{
			e.vlist.push_back(-vlist[i].p);
		}
		return e;
	}
	
	edge_t operator * (quaternion q) const
	{
		edge_t e;
		{
			e.vlist = vlist;
			vec3 lstp;
			for (int i = 0; i < e.vlist.size(); i++)
			{
				e.vlist[i].p = q * (e.vlist[i].p);

				vec3 v = e.vlist[i].p - lstp;

				e.vlist[i].p = lstp + v;

				lstp = e.vlist[i].p;
			}
		}
		return e;
	}
	edge_t operator / (real fval) const
	{
		edge_t e;
		{
			int len = int(e.vlist.size() / fval);
			for (int i = 0; i < len; i++)

				e.vlist.push_back(vlist[i]);
		}
		return e;
	}
	inline bool issame(const vector3& v1, const vector3& v2) const
	{
		const real c_EPSINON = 1e-2;
		return (fabs(v1.x - v2.x) <= c_EPSINON && fabs(v1.y - v2.y) <= c_EPSINON && fabs(v1.z - v2.z) <= c_EPSINON);
	}
	inline bool _merge(VECLIST& out, const VECLIST& e1, int pos1, const VECLIST& e2, int pos2) const
	{
		bool ret = false;
		for (; pos1 < e1.size(); pos1++)
		{
			for (int j = pos2; j < e2.size(); j++)
			{
				if (issame(e1[pos1].p, e2[j].p))
				{
					out.push_back(e1[pos1]);
					ret = _merge(out, e2, j + 1, e1, pos1 + 1);
					return true;
				}
			}
			out.push_back(e1[pos1]);
		}
		return ret;
	}
	edge_t operator | (const edge_t& v) const
	{
		edge_t e;
		{
			if (vlist.empty())
				return v;

			if (v.vlist.size() == 1)
			{
				e.vlist = vlist;
				e.vlist.push_back(v.vlist.front());
				return e;
			}

			if (!_merge(e.vlist, vlist, 0, v.vlist, 0))
				e.append(v);
		}
		return e;
	}
	edge_t operator & (const edge_t& v) const
	{
		edge_t e;
		{
			if (vlist.empty())
				return v;

			for (int i = 0; i < v.vlist.size(); i++)
			{
				for (int j = 0; j < vlist.size(); j++)
				{
					if (v.vlist[i].p == vlist[j].p)
						e.vlist.push_back(v.vlist[i]);
				}
			}
		}
		return e;
	}
	void append(const edge_t& v)
	{
		vlist.insert(vlist.end(), v.vlist.begin(),	v.vlist.end());
	}
	inline void invert()
	{
		std::reverse(vlist.begin(), vlist.end());
	}
	inline edge_t invertcpy() const
	{
		edge_t e;
		e.vlist = vlist;
		std::reverse(e.vlist.begin(), e.vlist.end());
		return e;
	}
	void mirror(crvec o, crvec ax)
	{
		for (auto& it : vlist)
		{
			vec3 v = o - it.p;
			it.p = it.p + (v - ax * v.dot(ax)) * 2;
		}
	}
	edge_t mirrorcpy(crvec o, crvec ax)
	{
		edge_t e;
		for (auto& it : vlist)
		{
			vec3 v = o - it.p;
			e.vlist.push_back(it.p + (v - ax * v.dot(ax)) * 2);
		}
		return e;
	}

	bool intersect(const edge_t& v)
	{
		for (int i = 0; i < vlist.size(); i++)
		{
			for (int j = 0; j < v.vlist.size(); j++)
			{
				if (vlist[i].p == v.vlist[j].p)
				{
					return true;
				}
			}
		}
		return false;
	}
}; 

// *********************************************
// 边计算
// *********************************************
namespace edgecalc
{
	const real c_precision = 1e-18;
	inline vertex blend_vertex(const vertex& v1, const vertex& v2, real alpha)
	{
		return vertex(v1.p * (1 - alpha) + v2.p * alpha, v1.n * (1 - alpha) + v2.n * alpha);
	}
	inline real check_front(crvec o, crvec n, crvec p)
	{
		if ((p - o).len() <= c_precision)
			return 0;

		real d = (p - o).dot(n);
		return d;
	}
	inline bool cutedge(const edge_t& cur, crvec o, crvec n, edge_t& front, edge_t& back)
	{
		for (int i = 0; i < cur.vlist.size(); i++)
		{
			real d = check_front(o, n, cur.vlist[i].p);
			if (fabs(d) <= c_precision)
			{
				front.vlist.push_back(cur.vlist[i]);
				back.vlist.push_back(cur.vlist[i]);
			}
			else
			{
				if (d > 0)
				{
					front.vlist.push_back(cur.vlist[i]);
				}
				else
				{
					back.vlist.push_back(cur.vlist[i]);
				}
			}
		}
		return true;
	}
}
