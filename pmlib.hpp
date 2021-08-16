#define SMCATCH_START(wid)	submesh sm;	triangle tri1, tri2; int verIndex = 0;int lowerind[wid + 1];	
#define SMCATCH_ING(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[2] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[0] = verIndex - 1;tri2.vertexIndex[2] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[0] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SMCATCH_ING2(wid, s, i, j, pp) 	{const int step = s;	if ((i % step == 0 && j % step == 0)){if(i > 0 && j > 0){tri1.vertexIndex[0] = lowerind[j - step];tri1.vertexIndex[1] = lowerind[j];tri1.vertexIndex[2] = verIndex - 1;tri2.vertexIndex[0] = verIndex;tri2.vertexIndex[1] = verIndex - 1;	tri2.vertexIndex[2] = lowerind[j];sm.tris.push_back(tri1);sm.tris.push_back(tri2);}	sm.vertices.push_back(pp);lowerind[j - step] = verIndex - 1;if(j == wid) lowerind[j] = verIndex;verIndex ++;}}
#define SWITCHSM(sm)	gsubmesh = &sm
#define RESTORESM		gsubmesh = &gsubmesh0
#define SUBMESH			(*gsubmesh)

// **********************************************************************
// 三角形导出
// **********************************************************************
submesh gsubmesh0;	// 默认模型
submesh gimportsm;	// unity导入的模型
submesh* gsubmesh = &gsubmesh0;	

bool gcommonvertex = false;
bool gsearchcomvertex = false;
int gverindfindstart = 0;
bool gcalcnorm = false;

std::vector<vec>	gtargets;

extern int color;

void resetsm(submesh*sm = gsubmesh)
{
	sm->vertices.clear();
	sm->cors.clear();
	sm->tris.clear();

	// 后边两项在读入模型时才会使用
	sm->normals.clear();
	sm->uvs.clear();

	gtargets.clear();
}
inline void clearedgeind(VECLIST& e)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].ind = -1;
	}
}
inline int getverind(vertex& v)
{
	if (gcommonvertex && v.ind != -1)
	{
		gsubmesh->cors[v.ind] = color;
		return v.ind;
	}

	if (gcommonvertex && gsearchcomvertex)
	{
		// 寻找同样的顶点
		for (int i = gsubmesh->vertices.size()-1; i >= gverindfindstart; i--)
		{
			if (gsubmesh->vertices[i] == v.p)
			{
				gsubmesh->cors[i] = color;
				v.ind = i;
				return i;
			}
		}
	}

	int ind = gsubmesh->vertices.size();
	v.ind = ind;
	gsubmesh->vertices.push_back(v);
	gsubmesh->cors.push_back(color);
	return ind;
}

inline void addtri2sm(vertex& p1, vertex& p2, vertex& p3)
{
	//if (*gsubmesh->vertices.size() >= 60000)
	//{
		//LOGMSG("添加失败 三角形数量超过60000!")
		//savelog(logmessage.str());
	//	return;
	//}

	//vec norm = (p2 - p1).cross(p3 - p1);
	//real llen = norm.sqrlen();
	//if (llen > 1e-18 && llen < 1e8)
	{
		triangle tri;
		tri.vertexIndex[0] = getverind(p1);
		tri.vertexIndex[1] = getverind(p2);
		tri.vertexIndex[2] = getverind(p3);

		gsubmesh->tris.push_back(tri);
	}
}
inline int getverind0(const vertex& v)
{
	gsubmesh->vertices.push_back(v.p);
	gsubmesh->cors.push_back(color);
	return gsubmesh->vertices.size() - 1;
}
inline void addtri2sm0(const vertex& p1, const vertex& p2, const vertex& p3)
{
	//if (gsubmesh->vertices.size() >= 60000)
	//{
	//	//LOGMSG("添加失败 三角形数量超过60000!")
	//	//savelog(logmessage.str());
	//	return;
	//}

	vec norm = (p2 - p1).cross(p3 - p1);
	real llen = norm.sqrlen();
	if (llen > 1e-18 && llen < 1e8)
	{
		triangle tri;
		tri.vertexIndex[0] = getverind0(p1);
		tri.vertexIndex[1] = getverind0(p2);
		tri.vertexIndex[2] = getverind0(p3);

		gsubmesh->tris.push_back(tri);
	}
}
inline void addtri2smi(int t1, int t2, int t3)
{
	if (t1 >= gsubmesh->vertices.size() || t2 >= gsubmesh->vertices.size() || t3 >= gsubmesh->vertices.size())
		return;

	if (gsubmesh->vertices.size() >= 60000)
	{
		//LOGMSG("添加失败 三角形数量超过60000!")
		//savelog(logmessage.str());
		return;
	}

	triangle tri;
	tri.vertexIndex[0] = t1;
	tri.vertexIndex[1] = t2;
	tri.vertexIndex[2] = t3;

	gsubmesh->tris.push_back(tri);
}
inline void exportsm()
{
	//exportdat(*gsubmesh);
}
inline void rendersubmesh()
{
	rendersubmesh(*gsubmesh);
}
// ------------------------------------------------
// READ TEMPLATE
inline std::string readTemplate()
{
	std::stringstream templateString;
	std::ifstream f("output/model.cs");
	if (f)
	{
		std::copy(std::istreambuf_iterator<char>(f),
			std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(templateString));
		f.close();
	}
	return templateString.str();
}

// -------------------------------------------------
// 法线逻辑
// -------------------------------------------------
inline void uniformnorm(const vec& p, const vec& n)
{
	for (int i = 0; i < gsubmesh->vertices.size(); i++)
	{
		if (gsubmesh->vertices[i] == p)
		{
			gsubmesh->vertices[i].n = n;
		}
	}
}
inline void closeedgenorm(VECLIST& e)
{
	e[e.size() - 1].n = e[0].n;
}
void calcroundnorm(VECLIST& e, crvec o)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].n = (e[i].p - o).normcopy();
	}
}

// ================================================================================================
// 绘制
// 三角形元素
// 渲染光栅化
// ================================================================================================
// 渲染状态
int drawround = 0;
vec light(0.5, 0.5, -0.5);	// 全局光的反向
int color = 0xFF808080;		// 当前色彩
int lummode = 0;			// -1 暗 0 正常 1 亮
bool backfacecull = false;	// 背面剔除
int renderstate = -1;		// 渲染模式 0 3D构型， 1 正常渲染 2 网格渲染, >= 3 2d绘图，-1 not rendering 
int debugmode = 0;
bool binvnorm = false;
bool bclosed = true;

// ------------------------------------------------------------------------------------------------
void triang(vertex& p1, vertex& p2, vertex& p3)
{
	//{// GL渲染器中绘制
	//	/*PRINT("glVertex3f(" << p1.p.x << "," << p1.p.y << "," << p1.p.z << ");");
	//	PRINT("glVertex3f(" << p2.p.x << "," << p2.p.y << "," << p2.p.z << ");");
	//	PRINT("glVertex3f(" << p3.p.x << "," << p3.p.y << "," << p3.p.z << ");");*/
	//	PRINT("glVertex2f(" << p1.p.x << "," << p1.p.y << ");");
	//	PRINT("glVertex2f(" << p2.p.x << "," << p2.p.y << ");");
	//	PRINT("glVertex2f(" << p3.p.x << "," << p3.p.y << ");");
	//}
	if (renderstate == 0)
	{
		if (gcalcnorm)
		{
			vec n = binvnorm ? (p3 - p1).cross(p2 - p1) : (p2 - p1).cross(p3 - p1);
			n.norm();
			if (p1.n == vec::ZERO) p1.n = n;
			if (p2.n == vec::ZERO) p2.n = n;
			if (p3.n == vec::ZERO) p3.n = n;
		}
		if (binvnorm)
			addtri2sm(p1, p2, p3);
		else
			addtri2sm(p1, p3, p2);
		return;
	}

	int cor = 0xFFFFFFFF;
	if (renderstate == 1)
	{
		vec norm = binvnorm ? (p3 - p1).cross(p2 - p1) : (p2 - p1).cross(p3 - p1);
		if (norm.sqrlen() == 0)
		{
			//pixelAA((p1 + p2 + p3) / 3.0, cor);
			//line3d(p1, p2, cor);
			//line3d(p1, p3, cor);
			//line3d(p2, p3, cor);
			return; // 无效三角形
		}
		norm.norm();
		if (backfacecull && norm.z > 0)
			return;

		if (color == 0xFFFFFFFF)
		{
			int cor0 = 0x000000FF & cor;
			if (lummode == 1)
			{
				int lum = blend(cor0, 255, norm.dot(light) * 1.5, 3);
				cor = RGB(lum, lum, lum);
			}
			else if (lummode == -1)
			{
				int lum = blend(cor0, 188, norm.dot(light), 2);
				cor = RGB(lum, lum, lum);
			}
			else
			{
				int lum = blend(cor0, 255, norm.dot(light));
				cor = RGB(lum, lum, lum);
			}
		}
		else
		{
			int lum = blend(0, 255, norm.dot(light) * 1.5, 3);
			cor = RGB(lum, lum, lum);
			cor = blendcor(cor, color, 0.5);
		}

		LOOP0(i, len, getviewdis(p1.p, p2.p))
			vec p12 = blend(p1.p, p2.p, ai);
		LOOP0(j, wid, getviewdis(p12, p3.p))
			vec p = blend(p12, p3.p, aj);
		pixel(p, cor);
		END
			END

	}
	else if (renderstate == 2)
	{
		line3d(p1.p * DRAWSCALE, p2.p * DRAWSCALE, cor);
		line3d(p2.p * DRAWSCALE, p3.p * DRAWSCALE, cor);
		line3d(p3.p * DRAWSCALE, p1.p * DRAWSCALE, cor);
	}
}

void triang0(const vertex& p1, const vertex& p2, const vertex& p3)
{
	//if (renderstate == 0)
	{
		if (binvnorm)
			addtri2sm0(p1, p2, p3);
		else
			addtri2sm0(p1, p3, p2);
		return;
	}

	int cor = 0xFFFFFFFF;
	if (renderstate == 1)
	{
		vec norm = binvnorm ? (p3 - p1).cross(p2 - p1) : (p2 - p1).cross(p3 - p1);
		if (norm.sqrlen() == 0)
		{
			//pixelAA((p1 + p2 + p3) / 3.0, cor);
			//line3d(p1, p2, cor);
			//line3d(p1, p3, cor);
			//line3d(p2, p3, cor);
			return; // 无效三角形
		}

		if (backfacecull && norm.z > 0)
			return;
		{
			unsigned char lum = blendi(0, 255, norm.dot(light) * 1.5);
			cor = _RGB(lum, lum, lum);
			cor = blendcor(cor, color, 0.5);
		}
		{
			LOOP0(i, len, getviewdis(p1.p, p2.p))
				vec p12 = blend(p1.p, p2.p, ai);
			LOOP0(j, wid, getviewdis(p12, p3.p))
				vec p = blend(p12, p3.p, aj);
			pixel(p, cor);
			END
				END
		}
	}
	else if (renderstate == 2)
	{
		vec norm = (p2 - p1).cross(p3 - p1);
		if (norm.sqrlen() == 0)
		{
			//pixelAA((p1 + p2 + p3) / 3.0, cor);
			//line3d(p1, p2, cor);
			//line3d(p1, p3, cor);
			//line3d(p2, p3, cor);
			return; // 无效三角形
		}

		line3d(p1.p, p2.p, color);
		line3d(p2.p, p3.p, color);
		line3d(p3.p, p1.p, color);
	}
}

// -------------------------------------------------
// 面
// -------------------------------------------------
void plane(vertex& p1, vertex& p2, vertex& p3, vertex& p4)
{
	triang(p2, p3, p1);
	triang(p4, p1, p3);
}
void plane0(const vertex& p1, const vertex& p2, const vertex& p3, const vertex& p4)
{
	triang0(p2, p3, p1);
	triang0(p4, p1, p3);
}
// -------------------------------------------------
// 边缘->面
// -------------------------------------------------
int drawmask = 0xFFFFFFFF;
inline int checkflag(int i)
{
	return drawmask & (1 << i);
}
inline int unflag(int i)
{
	return drawmask &= ~(1 << i);
}
void face(VECLIST& edge, bool invnorm = false)
{
	bool oldbinvnorm = binvnorm;
	binvnorm = invnorm;

	int s = edge.size();
	if (s < 3)
		return;
	if (s == 5)
	{
		plane(edge[0], edge[1], edge[2], edge[3]);
	}
	else
	{
		vertex p0 = edge[0];
		for (int i = 1; i < s - 1; i++)
		{
			if (checkflag(i))
				triang(p0, edge[i], edge[(i + 1) % s]);
		}
	}
	drawmask = 0xFFFFFFFF;
	binvnorm = oldbinvnorm;
}
void face(VECLIST& edge, vertex o)
{
	int s = edge.size();
	if (s < 3)
		return;
	for (int i = 0; i < s - 1; i++)
	{
		//if (checkflag(i))
		{
			triang(o, edge[i], edge[(i + 1) % s]);
		}
	}
	drawmask = 0xFFFFFFFF;
}
// 极点
void facepole(VECLIST& edge, vertex o)
{
	int cor = RNDBW;
	int s = edge.size();
	if (s < 3)
		return;
	for (int i = 0; i < s - 1; i++)
	{
		vertex& v1 = edge[i];
		vertex& v2 = edge[(i + 1) % s];
		o.uv.x = (v1.uv.x + v2.uv.x) / 2;

		triang(o, v1, v2);
	}
}
void face(VECLIST& edge1, VECLIST& edge2)
{
	int len = _MIN(edge1.size(), edge2.size());
	len = int(len - 1);
	
	for (int i = 0; i < len; i++)
	{
		plane(edge1[i], edge1[i + 1], edge2[i + 1], edge2[i]);
		if (i == 0 && gcommonvertex && bclosed)
		{
			edge1[len].ind = edge1[0].ind;
			edge2[len].ind = edge2[0].ind;
		}
	}

	drawmask = 0xFFFFFFFF;
}
void face0(VECLIST& edge1, VECLIST& edge2)
{
	int len = int(edge1.size() - 1);
	for (int i = 0; i < len; i++)
	{
		plane(edge1[i], edge1[i + 1], edge2[i + 1], edge2[i]);
	}
}
void face12(VECLIST& edge1, VECLIST& edge2)
{
	int len = int(edge1.size() - 1);
	for (int i = 0; i < len; i++)
	{
		if (checkflag(i))
		{
			plane(edge1[i], edge1[i + 1], edge2[i * 2 + 1], edge2[i * 2]);
			triang(edge1[i + 1], edge2[i * 2 + 2], edge2[i * 2 + 1]);
		}
		if (i == 0 && gcommonvertex)
		{
			edge1[len].ind = edge1[0].ind;
			edge2[int(edge2.size() - 1)].ind = edge2[0].ind;
		}
	}
	drawmask = 0xFFFFFFFF;
}
void face_noclose(VECLIST& edge1, VECLIST& edge2)
{
	for (int i = 0; i < edge1.size(); i++)
	{
		plane(edge1[i], edge1[i + 1], edge2[i + 1], edge2[i]);
	}
}
// -------------------------------------------------
// 边逻辑
// -------------------------------------------------
VECLIST* pworkingedge = 0; 	// 当前工作面
int workingedgeindex = 0;	 		// 当前工作面索引

// -------------------------------------------------
// 边法线方向
inline vec getedgenorm(const VECLIST& e)
{
	if (e.size() < 3)
		return vec::ZERO;
	{
		int ind1 = 1;
		for (; ind1 < e.size(); ind1++)
		{
			if (e[ind1].p != e[0].p)
				break;
		}
		int ind2 = ind1 + 1;
		for (; ind2 < e.size(); ind2++)
		{
			if (e[ind2].p != e[ind1].p && e[ind2].p != e[0].p &&
				(e[ind1].p - e[0].p).cross(e[ind2].p - e[0].p).sqrlen() > 1e-8)
				break;
		}
		if (ind2 >= e.size())
			return vec::ZERO;
		return (e[ind1].p - e[0].p).cross(e[ind2].p - e[0].p).normcopy();
	}
}
// 平均点
vec getedgecenter(const VECLIST& e, int n = 0)
{
	if (n == 0)
		n = (e[0].p == e[e.size() - 1].p) ? e.size() - 1 : e.size();
	vec o;
	for (int i = 0; i < n; i++)
		o = o + e[i].p;
	o = o / n;
	return o;
}
// -------------------------------------------------
// 边缘大小
void radedge(VECLIST& e1, real r, VECLIST& e2)
{
	int len = e1.size();
	if (len <= 1)
		return;
	if (e1[0] == e1[len - 1])
		len--;
	vec o;
	for (int i = 0; i < len; i++)
		o = o + e1[i];
	o = o / len; // 平均点

	for (int i = 0; i < e1.size(); i++)
	{
		vertex v = e1[i];
		v.p = ((v.p - o).normcopy() * r + o);
		e2.PUSH(v);
	}
}
void radedge(VECLIST& e1, real r)
{
	int len = e1.size();
	if (len <= 1)
		return;
	if (e1[0] == e1[len - 1])
		len--;
	vec o;
	for (int i = 0; i < len; i++)
		o = o + e1[i];
	o = o / len; // 平均点

	for (int i = 0; i < e1.size(); i++)
	{
		vertex& v = e1[i];
		v.p = ((v.p - o).normcopy() * r + o);
	}
}
void radedge2(VECLIST& e1, real r)
{
	int len = e1.size();
	if (len <= 1)
		return;
	if (e1[0] == e1[len - 1])
		len--;
	vec o;
	for (int i = 0; i < len; i++)
		o = o + e1[i];
	o = o / len; // 平均点

	vec n = getedgenorm(e1);
	vec v0 = (e1[0] - o).normcopy();
	real ang = 2 * PI / len;
	for (int i = 0; i < e1.size(); i++)
	{
		vertex& v = e1[i];
		v.p = (v0 * r + o);
		v0.rot(ang, n);
	}
}
// -------------------------------------------------
// 缩放边缘
void scaleedge(const VECLIST& e1, real scale, VECLIST& e2)
{
	int len = e1.size();
	if (len <= 1)
		return;
	if (e1[0].p == e1[len - 1].p)
		len--;
	vec o;
	for (int i = 0; i < len; i++)
		o = o + e1[i].p;
	o = o / len; // 平均点

	for (int i = 0; i < e1.size(); i++)
	{
		vertex v = e1[i];
		v.p = (v.p - o) * scale + o;
		v.ind = -1;
		e2.PUSH(v);
	}
}
// -------------------------------------------------
void scaleedge(VECLIST& e1, real scale, int oplen = 0)
{
	int len = e1.size();
	if (len <= 1)
		return;
	if (e1[0] == e1[len - 1])
		len--;
	vec o;
	for (int i = 0; i < len; i++)
		o = o + e1[i];
	o = o / len; // 平均点

	if (oplen == 0)
		oplen = e1.size();
	for (int i = 0; i < oplen; i++)
	{
		vertex& v = e1[i];
		v.ind = -1;
		v.p = ((v.p - o) * scale + o);
	}
}
// -------------------------------------------------
void scaleedge(VECLIST& e1, real sx, real sy, real sz, VECLIST& e2)
{
	if (e1.size() == 0)
		return;
	vec o;
	for (int i = 0; i < e1.size(); i++)
		o = o + e1[i].p;
	o = o / e1.size(); // 平均点

	for (int i = 0; i < e1.size(); i++)
	{
		vertex v = e1[i];
		v.p -= o;
		v.p.x *= sx;
		v.p.y *= sy;
		v.p.z *= sz;
		v.p += o;
		e2.PUSH(v);
	}
}
// -------------------------------------------------
void scaleedge(VECLIST& e1, real sx, real sy, real sz)
{
	if (e1.size() == 0)
		return;
	vec o;
	for (int i = 0; i < e1.size() - 1; i++)
		o = o + e1[i];
	o = o / (e1.size() - 1); // 平均点

	for (int i = 0; i < e1.size(); i++)
	{
		vertex& v = e1[i];
		v.p -= o;
		v.p.x *= sx;
		v.p.y *= sy;
		v.p.z *= sz;
		v.p += o;
		v.ind = -1;
	}
}
// -------------------------------------------------
void scaleeedgeex(VECLIST& e1, std::vector<real> scalelist)
{
	if (e1.size() == 0)
		return;
	vec o;
	for (int i = 0; i < e1.size() - 1; i++)
		o = o + e1[i];
	o = o / (e1.size() - 1); // 平均点

	for (int i = 0; i < e1.size(); i++)
	{
		vertex& v = e1[i];
		v.p -= o;
		if (i < scalelist.size())
			v.p *= scalelist[i];

		v.p += o;
	}
}
// -------------------------------------------------
void moveedgeex(VECLIST& e1, std::vector<real> movelist)
{
	if (e1.size() == 0)
		return;
	vec norm = getedgenorm(e1);
	for (int i = 0; i < e1.size(); i++)
	{
		vertex& v = e1[i];
		if (i < movelist.size())
			v.p += norm * movelist[i];
	}
}

// -------------------------------------------------
// 移动边缘
void moveedge(const VECLIST& e1, vec dv, VECLIST& e2)
{
	for (int i = 0; i < e1.size(); i++)
	{
		vertex v = e1[i];
		v.p += dv;
		v.ind = -1;
		e2.PUSH(v);
	}
}
// -------------------------------------------------
void moveedge(VECLIST& e1, vec dv)
{
	for (int i = 0; i < e1.size(); i++)
	{
		e1[i].p += dv;
		e1[i].ind = -1;
	}
}
void moveedgex(VECLIST& e, vec dv)
{
	VECLIST e1;
	for (int i = 0; i < e.size(); i++)
	{
		e1.PUSH(e[i].p + dv);
	}
	face(e, e1);
	e = e1;
}
// -------------------------------------------------
// extrude 边缘
void extrudeedge(const VECLIST& e1, real d, VECLIST& e2)
{
	if (e1.size() < 3)
		return;
	vec norm = (e1[1].p - e1[0].p).cross(e1[2].p - e1[0].p); norm.norm();
	vec dv = norm * d;
	for (int i = 0; i < e1.size(); i++)
	{
		vertex v = e1[i];
		v.p += dv;
		v.ind = -1;
		e2.PUSH(v);
	}
}
// -------------------------------------------------
void extrudeedge(VECLIST& e1, real d)
{
	if (e1.size() < 3)
		return;
	vec norm = getedgenorm(e1);
	vec dv = norm * d;
	for (int i = 0; i < e1.size(); i++)
	{
		e1[i].p += dv;
		e1[i].ind = -1;
	}
}

// -------------------------------------------------
// cut 边缘
void cutedge(const VECLIST& ei, int start, int len, VECLIST& eo)
{
	if (start >= ei.size() || start + len >= ei.size())
		return;
	for (int i = start; i < start + len; i++)
	{
		eo.PUSH(ei[i]);
	}
}
void cutedge(VECLIST& e, int start, int len)
{
	if (start >= e.size() || start + len >= e.size())
		return;
	int ii = 0;
	for (int i = start; i < start + len; i++)
	{
		e[ii++] = e[i];
	}
}
void cutedge(const VECLIST* ei, int pos, VECLIST* e1 = 0, VECLIST* e2 = 0)
{
	if (!ei || pos >= ei->size())
		return;
	if (e1)
	{
		for (int i = 0; i < pos; i++)
		{
			e1->PUSH((*ei)[i]);
		}
	}
	if (e2)
	{
		for (int i = pos; i < ei->size(); i++)
		{
			e2->PUSH((*ei)[i]);
		}
	}
}
// -------------------------------------------------
// subedge 边缘
void subedge(const VECLIST& e, int start, int sublen, VECLIST& se)
{
	int len = e.size();
	for (int i = start; i < start + sublen; i++)
	{
		int ii = i % len;
		se.PUSH(e[ii]);
	}
}
void subedge2(const VECLIST& e, int start, int sublen, VECLIST& se1, VECLIST& se2)
{
	int len;
	if (bclosed)
		len = (e[0].p == e[e.size() - 1].p) ? e.size() - 1 : e.size();
	else
		len = e.size();
	for (int i = start; i < start + len; i++)
	{
		int ii = i % len;
		if (i >= start && i <= start + sublen)
		{
			se1.PUSH(e[ii]);
			if (i == start + sublen)
				se2.PUSH(e[ii]);
		}
		else
			se2.PUSH(e[ii]);
	}
	//if(isclosed)
	se2.PUSH(e[start % len]);
}

// -------------------------------------------------
// union 边缘
void unionedge(const VECLIST& e1, const VECLIST& e2, int start, int len, VECLIST& eo) // 注意要逆时针方向, e2前, e1后关系
{
	if (len > e1.size() || len > e2.size())
		return;

	int sz1 = e1.size();
	for (int i = start; i < start + len; i++)
	{
		eo.PUSH(e1[i % sz1]);
	}
	int sz2 = e2.size();
	for (int i = start + len - 1; i >= start; i--)
	{
		eo.PUSH(e2[i % sz2]);
	}
}
// -------------------------------------------------
// 反向
void invedge(VECLIST& e)
{
	VECLIST ne;
	for (int i = e.size() - 1; i >= 0; i--)
	{
		ne.PUSH(e[i]);
	}
	e = ne;
}
void invplaneedge12(VECLIST& e)
{
	if (e.size() < 4)
		return;
	vec tmp = e[0];
	e[0] = e[1];
	e[1] = tmp;
}
void invplaneedge34(VECLIST& e)
{
	if (e.size() < 4)
		return;
	vec tmp = e[2];
	e[2] = e[3];
	e[3] = tmp;
}
void invplaneedge13(VECLIST& e)
{
	if (e.size() < 4)
		return;
	vec tmp = e[0];
	e[0] = e[2];
	e[2] = tmp;
}
void invplaneedge24(VECLIST& e)
{
	if (e.size() < 4)
		return;
	vec tmp = e[1];
	e[1] = e[3];
	e[3] = tmp;
}
void invplaneedge23(VECLIST& e)
{
	if (e.size() < 4)
		return;
	vec tmp = e[1];
	e[1] = e[2];
	e[2] = tmp;
}
void invplaneedge14(VECLIST& e)
{
	if (e.size() < 4)
		return;
	vec tmp = e[0];
	e[0] = e[3];
	e[3] = tmp;
}
// -------------------------------------------------
// 旋转 边缘
void rotedge0(VECLIST& e, real ang, const vec& ax)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i].p.rot(ang, ax);
	}
}
void rotedge(VECLIST& e, real ang, const vec& ax)
{
	vec o = getedgecenter(e);
	for (int i = 0; i < e.size(); i++)
	{
		e[i] = (e[i] - o).rotcopy(ang, ax) + o;
	}
}
void rotedge(VECLIST& e, real ang, crvec o, const vec& ax)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i] = (e[i] - o).rotcopy(ang, ax) + o;
	}
}
void rotedge(const VECLIST& e, real ang, const vec& ax, VECLIST& eo)
{
	vec o = getedgecenter(e);
	for (int i = 0; i < e.size(); i++)
	{
		eo.PUSH((e[i].p - o).rotcopy(ang, ax) + o);
	}
}
void rotedgebycenter(VECLIST& e, real ang, const vec& ax, const vec& ro)
{
	for (int i = 0; i < e.size(); i++)
	{
		e[i] = (e[i] - ro).rotcopy(ang, ax) + ro;
	}
}
void rotedgebycenter(VECLIST& e, real ang, const vec& ax, const vec& ro, VECLIST& oe)
{
	for (int i = 0; i < e.size(); i++)
	{
		oe.PUSH((e[i].p - ro).rotcopy(ang, ax) + ro);
	}
}
void rotedgebynorm(VECLIST& e, const vec& norm)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;

	vec ax = n.cross(norm); ax.norm();
	real ang = acos(norm.dot(n));

	rotedge(e, ang, ax);
}
void rotedgebynorm(const VECLIST& e, const vec& norm, VECLIST& eo)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
	{
		eo = e;
		return;
	}
	vec ax = n.cross(norm); ax.norm();
	real ang = acos(norm.dot(n));
	vec az = n.cross(ax);
	if (az.dot(norm) > 0)
		ang = -ang;

	rotedge(e, ang, ax, eo);
}
void rotedgebynorm(VECLIST& e, const vec& norm, const vec& ax)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;

	real ang = acos(norm.dot(n));
	vec az = n.cross(ax);
	if (az.dot(norm) > 0)
		ang = -ang;

	rotedge(e, ang, ax);
}
void rotedgebynorm(const VECLIST& e, const vec& norm, const vec& ax, VECLIST& eo)
{
	vec n = getedgenorm(e);
	if (n == norm || n == -norm)
		return;
	real ang = acos(norm.dot(n));
	vec az = n.cross(ax);
	if (az.dot(norm) > 0)
		ang = -ang;

	rotedge(e, ang, ax, eo);
}
// -------------------------------------------------
// 双倍点数
void doublevnum(VECLIST& e, int num = 0)
{
	VECLIST te;
	int n = num == 0 ? e.size() - 1 : num;

	for (int i = 0; i < n; i++)
	{
		te.PUSH(e[i]);
		te.PUSH(e[i]);
		te.back().p = (e[i].p + e[(i + 1) % n].p) / 2;
	}
	te.PUSH(e[e.size() - 1]);
	e = te;
}
// -------------------------------------------------
// 双倍点数
void doublevnum(VECLIST& e, VECLIST& oe, int num = 0)
{
	int n = num == 0 ? e.size() - 1 : num;

	for (int i = 0; i < n; i++)
	{
		oe.PUSH(e[i]);
		oe.PUSH(e[i]);
		oe.back().p = (e[i].p + e[(i + 1) % n].p) / 2;
	}
	oe.PUSH(e[e.size() - 1]);
}
bool isedgeclosed(VECLIST& e)
{
	if (e.empty())
		return false;
	return e.front().p == e.back().p;
}
// 闭合
void closeedge(VECLIST& e)
{
	if (e.empty())
		return;

	e.PUSH(e[0]);
}
// 相位
void phaseedge(VECLIST& e, int offset)
{
	if (e.empty())
		return;
	VECLIST ee;
	for (int i = 0; i < e.size(); i++)
	{
		ee.push_back(e[(i + offset) % e.size()]);
	}
	e = ee;
}
// 链接
void linkedge(VECLIST& e, const VECLIST& le, bool inv = false)
{
	if (inv == false)
	{
		for (int i = 0; i < le.size(); i++)
		{
			e.PUSH(le[i]);
		}
	}
	else
	{
		for (int i = le.size() - 1; i >= 0; i--)
		{
			e.PUSH(le[i]);
		}
	}
}
// 添加中点
void insertp(VECLIST& e, crvec p, int pos)
{
	VECLIST te;
	for (int i = 0; i < e.size(); i++)
	{
		te.PUSH(e[i]);
		if (i == pos)
			te.PUSH(p);
	}
	e = te;
}
int insertp(VECLIST& e, crvec p, crvec pre, crvec aft)
{
	int pos = -1;
	VECLIST te;
	int i = 0;
	for (; i < e.size() - 1; i++)
	{
		te.PUSH(e[i]);
		if (e[i] == pre && e[i + 1] == aft)
		{
			pos = i;
			te.PUSH(p);
		}
	}
	te.PUSH(e.back());
	e = te;
	return i;
}
void blendedge(const VECLIST& e1, const VECLIST& e2, real alpha, VECLIST& eo)
{
	if (e1.size() != e2.size())
		return;
	for (int i = 0; i < e1.size(); i++)
	{
		eo.PUSH(blend(e1[i].p, e2[i].p, alpha));
	}
}
// -------------------------------------------------
// 圆边沿
void roundedge(VECLIST& e, crvec o, crvec vx, crvec vy, real r, int sig, real ang0)
{
	/*if (bclosed && gcommonvertex)
	{
		CIRCLE2(i, r, sig, ang0)
			e.PUSH(o + (vx * dpi.x + vy * dpi.y));
		END
	}
	else*/
	{
		CIRCLE3(i, r, sig, ang0)
			e.PUSH(o + (vx * dpi.x + vy * dpi.y));
		END
	}
}
inline void roundedgex(VECLIST& e, crvec o, crvec vx, crvec vy, real r, int sig, real ang, real ang0)
{
	CIRCLE(i, r, sig, ang, ang0)
		e.PUSH(o + (vx * dpi.x + vy * dpi.y));
	END
}
void roundedge(VECLIST& e, crvec o, vec v, real r, int sig, real ang0 = 0)
{
	v.norm();
	vec dx, dy;
	v2vxvy(v, dx, dy);
	roundedge(e, o, dx, dy, r, sig, ang0);
}
// -------------------------------------------------
// match point
int matchpoint(const VECLIST& e, crvec p, real mindis = 0.000001)
{
	for (int i = 0; i < e.size(); i++)
	{
		if ((p - e[i].p).sqrlen() < mindis)
		{
			return i;
		}
	}
	return -1;
}
// -------------------------------------------------
bool matchedge(const VECLIST& re, VECLIST& e)
{
	real dis = (getedgecenter(re) - getedgecenter(e)).len();
	vec v = getedgenorm(re) * dis;
	VECLIST ne;
	for (int i = 0; i < 4; i++)
	{
		int ind = matchpoint(e, re[i].p + v, 0.00001);
		if (-1 == ind)
			return false;
		ne.PUSH(e[ind]);
	}
	e = ne;
	return true;
}
// -------------------------------------------------
// 一些常用扩展
// -------------------------------------------------
void extrudeedgeex(VECLIST& e, real d, real s = 1)
{
	VECLIST e1;
	extrudeedge(e, d, e1);
	if (s != 1)
		scaleedge(e1, s);
	face(e, e1);
	e = e1;
}
void extrudeedgeex1(VECLIST& e, real d, real s = 1)
{
	VECLIST e1;
	extrudeedge(e, d, e1);
	if (s != 1)
		scaleedge(e1, s);
	e = e1;
}
void extrudeedgeex2(VECLIST& e, real d, real r)
{
	VECLIST e1;
	extrudeedge(e, d, e1);
	radedge(e1, r);
	face(e, e1);
	//	face(e1);
	e = e1;
}
void extrudeedgeex3(VECLIST& e, real d, real r)
{
	radedge2(e, r);
	extrudeedge(e, d);
}
void extrudeedgeex4(VECLIST& e, real d, real r)
{
	radedge2(e, r);
	VECLIST e1;
	extrudeedge(e, d, e1);
	face(e, e1);
	e = e1;
}
// -------------------------------------------------
void extrudeedgeex(const VECLIST& e, real d, real s, VECLIST& oe)
{
	extrudeedge(e, d, oe);
	if (s != 1.0)
		scaleedge(oe, s);
}
void scaleedgeex(VECLIST& e, real s)
{
	VECLIST e1;
	scaleedge(e, s, e1);
	face(e, e1);
	e = e1;
}
