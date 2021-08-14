/*
	3d modeling->plant->compositae
*/
// --------------------------------------------------------
// 菊花
// --------------------------------------------------------
void cm_flower2(vec o, vec v, vec n, real r0, real w)
{
    binvnorm = true;
    vec vside = v.cross(n); vside.norm();
    vec lp;
    int len = 6;
    for (int i = 0; i < len; i++)
    {
        real ai = i / real(len);
        real r = (r0 + r0 * rrnd(-0.25, 0.25));
        vec p = o + v * r + vside * blend(-w / 2, w / 2, ai);
        color = blendcor(0xFF00FFFF, 0xFF00A0FF, rrnd());
        if (i > 0)
            triang0(o, lp, p);
        lp = p;
    }
    binvnorm = false;
}

void cm_flowerex2(vec o, vec v, vec n, real r0, real w)
{
    binvnorm = true;
    gverindfindstart = gsubmesh.vertices.size();
    vec vside = v.cross(n); vside.norm();
    vec lp, lpp, lpp1, lpp2;
    int len = 2;
    int wid = 8;
    vec tn = n;
    for (int i = 0; i < len; i++)
    {
        real ai = i / real(len);
        real r = (r0 + r0 * blend2(-0.2, 0.2, ai, 2)) / 10;
        tn = -tn;
        vec p = o + v * r + vside * blend(-w / 2, w / 2, ai) + tn * (w / 18);
        vec pp = p;
        real tw =w / 4;
        for(int j = 0; j <= wid; j ++)
        {
            real aj = j / real(wid);
            real ww = tw * blend3(0.01, 1, aj, 0.7, 2);
            pp += v * r + n * blend(-0.1, 0.1, aj);
            
            vec vsd = vside * ww;
            vec pp1 = pp - vsd;
            vec pp2 = pp + vsd;
            if (i > 0 && j > 0)
            {
                
                triang0(lpp1, lpp2, pp2);
                triang0(lpp1, pp2, pp1);
            }
            lpp1 = pp1;
            lpp2 = pp2;
            lpp = pp;
        }
        
        //if (i > 0)
            //triang(o, lp, p);
        
        lp = p;
    }
    binvnorm = false;
    
}
void cm_leave2(vec o, vec v, vec n, real len, real w)
{
    real t = blend(0.1, 1, gtimealpha);
    color = blendcor(RGB(10, 50, 0), 0xFF00A000, rrnd(0, 1));
    gcommonvertex = 1;
    vec vside = v.cross(n); vside.norm();
    vec p1 = o - vside * (w / 32);
    vec p2 = o + vside * (w / 32);
    real h = w / 200;
    VECLIST e;
    e.push_back(p1 + n * h);
    e.push_back(p1);
    e.push_back(p2);
    e.push_back(p2 + n * h);
    
    e.push_back(o);
   // closeedge(e);
    
    VECLIST le = e;
    real step = w / 4;
    int cnt = 4;
    for (int i = 0; i < cnt; i++)
    {
        real ai = i / real(cnt);
        static real scales[] = { 1, 5, 0.9, 0.4 };
        extrudeedge(e, step * 0.75);
        scaleedge(e, scales[i]);
        rotedge(e, rrnd(-PI / 8, PI / 8), vside);
        {
            int cor0 = color;
            //face(le, e);
            for (int ii = 0; ii < le.size() - 1; ii++)
            {
                //color = ii > 2 ? 0xFFA0FFA0 : cor0;
                plane(le[ii], le[ii + 1], e[ii + 1], e[ii]);
            }
            color = cor0;
        }
        
        if (i > 1 && t > 0.8)
        {
            real step2 = step / 4;
            static real scales2[] = { 1, 1.5, 1.2, 1.1, 0.1 };
            int cnt2 = 5 * t;
            if (i == cnt - 1)
            {
                VECLIST ee = e;
                for (int ii = 0; ii < cnt2; ii++)
                {
                    extrudeedgeex(ee, step2 * 1.5, scales2[ii] * 1.2);
                }
            }
            {
                {// left
                    VECLIST ee;
                    ee.push_back(le[0]); ee.push_back(le[1]);
                    ee.push_back(e[1]); ee.push_back(e[0]);
                    closeedge(ee);
                    extrudeedgeex(ee, step2 / 2, 0.75);
                    for (int ii = 0; ii < cnt2; ii++)
                    {
                        extrudeedgeex(ee, step2, scales2[ii]);
                        
                    }
                }
                {// right
                    VECLIST ee;
                    ee.push_back(le[2]); ee.push_back(le[3]);
                    ee.push_back(e[3]); ee.push_back(e[2]);
                    closeedge(ee);
                    extrudeedgeex(ee, step2 / 2, 0.75);
                    for (int ii = 0; ii < cnt2; ii++)
                    {
                        extrudeedgeex(ee, step2, scales2[ii]);
                    }
                }
            }
        }
        le = e;
    }
   // gcommonvertex = 0;
}

void chrysanthemum(vec o, real sz, real fw1, real fw2, int n0, int color0, int len0)
{
    color = 0xFFFFFFFF;
    real t = blend(0.1, 1, gtimealpha);
    vec p = o;
    VECLIST e;
    real s = sz / 120 * t;
    int len = len0;//blend(4, len0, t);
    vec v0 = vec::UY;
    real dpdeta = rrnd(0, 172.862);
    real dpdeta2 = rrnd(0, 172.862);
    for (int i = 0; i <= len; i++)
    {
        real ai = real(i) / (len);
        vec np;
        {
            vec dp = vec::UX * ((perlin(p.x * 10, p.y * 10 + dpdeta2, p.z * 10 + dpdeta))) + vec::UZ * ((perlin(p.x * 10 + dpdeta2, p.y * 10 + dpdeta, p.z * 10)));
            np = p + (v0 + dp - vec::UY * 0.1) * (s * blend(1.8, 10, ai, 4));
        }
        VECLIST e1;
        if (i == 0)
        {// µ◊≤ø
            color = 0xFF00FFAA;
            roundedge(e1, p, vec::UY, s, 8,0);
        }
        else if (i < len)
        {// …ÌÃÂ
            color = 0xFF00A080;
            real dlen = (np - p).len();
            vec gv = (np - p).normcopy();
            {
                moveedge(e, gv * (dlen), e1);
                scaleedge(e1, 0.995);
            }
            //if(i > len * 0.1 && i < len * 0.75 && rrnd() < 0.5)
            //{// leave
            //    cm_leave(p, blend(gv, vec::UX, 0.5).rotcopy(rrnd(-PI, PI), gv), vec::UY, sz * 2 * t, sz * 0.5 * t);
            //    color = 0xFFFFFFFF;
            //}
            color = 0xFF00A080;
        }
        else if(t > 0.5)
        {
            vec up = (np - p).normcopy();
            vec vx, vy;
            v2vxvy(up, vx, vy);
            vec forward = blend(vx, vy, rrnd());
            forward = blend(forward.rotcopy(rrnd(-PI, PI), up), up, rrnd(0.4, 0.6)).normcopy();
            v2vxvy(forward, vx, vy);
            
            //
            real rr = s * 10 * fw1 / 2;
            VECLIST ee = e;
            moveedge(ee, forward * (s * 2));
            scaleedge(ee, 10 * fw1);
            rotedgebynorm(ee, forward);
            face(e, ee);
            
            color = 0xFF004080;
            if (t > 0.8)
            {
                color = 0xFF0080FF;
                extrudeedgeex(ee, s / 2, 0.9);
                scaleedgeex(ee, 0.8);
                extrudeedgeex(ee, -s / 2, 0.9);
                color = 0xFF0080A0;
            }
            face(ee, p);
            
            
            //int red = rrnd(0, 255);
            //color = blendcor(0xFFFFFFFF, RGB(red, rrnd(0, red), rrnd(0, 255)), rrnd());
            //color = color0;
            real bansz = s * 500 * fw2;
            int n =  4 + n0 * blend(0.25, 1, t) - 1;
            for (int i = 0; i < n; i++)
            {// flower
                real ang = rrnd(-0.01, 0.01) * PI + i / real(n) * 2 * PI;
                vec vv = vx * cos(ang) + vy * sin(ang);
                vec fp = p + vv * (rr * t);
                color = blendcor(color0, RGB(rrnd(0, 255), rrnd(0, 255), rrnd(0, 255)), rrnd(0, 0.2));
                cm_flowerex2(fp, blend(vv, forward, rrnd(0, 0.5)), forward, bansz * sz / 5 * t, bansz * sz / 12 * t);
            }
            color = 0xFF004080;
        }
        gcommonvertex = 1;
        if (i != 0)
            face(e, e1);
        gcommonvertex = 0;
        e = e1;
        
        p = np;
    }
}
