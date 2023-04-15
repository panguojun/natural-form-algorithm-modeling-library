// ------------------------------------------------------------------------------------------------
// xibao
// ------------------------------------------------------------------------------------------------
void xibao(vec o, vec dir, vec dir1, int len0, int ind, real sz0 = 0.2)
{
    real at = 0.6;
    {
        vec p = o;
        dir.norm();
        real ang0 = atan2(dir.y, dir.x);
        real ang1 = atan2(dir1.y, dir1.x);
        ang0 = blend(ang0, ang1, at);
        {
            for (int i = 0; i < len0; i++)
            {
                real ai = real(i) / len0;
                real ang = blend(0, 2 * PI, ai);
                real r0 = sz0;
                real r1 = r0;
                real dang = ang - int(ang / (PI / 2)) * PI / 2;
                if (dang < PI / 4)
                    r1 = r1 / cos(dang);
                else
                    r1 = r1 / sin(dang);
                real r = blend(r0, r1, at, 2);
                vec v(cos(ang + ang0), sin(ang + ang0));
                p = o + v * r;
                if (i > 0)
                {
                    PSET(p, 0xffffffff, 0.02);
                    if (rand() % 25 == 0)
                        PSET(o + v * rrnd(0, r), 0xffa0a0a0, 0.025);
                }
            }
        }
        {// TAIL
            vec v = dir;
            int len = len0 / 3;
            for (int j = 0; j < len; j++)
            {
                real aj = real(j) / len;
                real ang = sin(aj * PI * 8 + gtime * 4 + rndmap[j][ind] * PI) * 0.2;
                v.rot(ang);
                p = p + v * (0.03);
                PSET(p, 0xffffffff, 0.02 * (1 - aj));
            }
        }
    }
}
// ----------------------------------------------------------------
// wugong
// ----------------------------------------------------------------
namespace m_1_3
{
    const int capid = 13;
    int capind = 0;
    int state = 3;
    real animation1 = 0;
    real animation2 = 0;
    vec cappos, headpos;
    vec gtarget, pos = vec(10, 5, 0);
    int becut = 1000;
    void bottledragon(vec& pos, vec& dir)
    {
        if(becut <= 4)
        {
            trigger ++;
            return;
        }
        const real USZ = 0.025;
        static float s_t[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
        real T1 = 0.25, T2 = 0.1, T3 = 0.25;
        if (state == 1)
        {
            if (gtime - animation1 < T1)
                pos.y = blend(0.455, 0.525, (gtime - animation1) / T1);
            else if (gtime - animation1 - T1 < T2)
                pos.y = 0.525;
            else if (gtime - animation1 - T1 - T2 < T3)
                pos.y = blend(0.525, 0.455, (gtime - animation1 - T1 - T2) / T3);
        }
        else if (state == 2)
        {
            dir.rot(PI * 0.005 * FT(gtime * PI, s_t));
            pos += dir * 0.01;
        }
        else if (state == 3)
        {
            vec vv = (gtarget - pos);
            if (vv.len() < USZ * 4)
                state = 2;
            dir = blend(dir, vv.normcopy(), 0.1);
            dir.rot(PI * 0.003 * FT(gtime * 4 * PI, s_t));
            pos += dir * 0.001;
        }
        real r = 10 * USZ;
        {// head
            drawcircle(pos, r, 0xFFFFFFFF);
            PSET(pos + dir.rotcpy(PI / 4) * (r / 2), 0xFFFFFFFF, 0.1);
            PSET(pos + dir.rotcpy(-PI / 4) * (r / 2), 0xFFFFFFFF, 0.1);
            headpos = pos;
        }
        {// mouth
            {
                real mouth = 0;
                if (state == 2)
                    mouth = PI / 50 * sin(gtime * 10 * PI);
                else if (state == 1)
                {
                    if (gtime - animation1 < T1)
                        mouth = blend(0, PI / 50, (gtime - animation1) / T1);
                    else if (gtime - animation1 - T1 < T2)
                        mouth = blend(PI / 50, -PI / 50, (gtime - animation1 - T1) / T2);
                }
                {
                    vec vv = dir.rotcpy(PI / 4);
                    vec pp = pos + vv * r;
                    int len = 10;
                    for (int i = 0; i < len; i++)
                    {
                        vv.rot(-PI / 40 + mouth);
                        pp += vv * USZ;
                        PSET(pp, 0xFFFFFFFF);
                    }
                    PSET(pp, 0xffff00ff, 0.025);
                    attackpt1.push_back(pp);
                }
                {
                    vec vv = dir.rotcpy(-PI / 4);
                    vec pp = pos + vv * r;
                    int len = 10;
                    for (int i = 0; i < len; i++)
                    {
                        vv.rot(PI / 40 - mouth);
                        pp += vv * USZ;
                        PSET(pp, 0xFFFFFFFF);
                    }
                    PSET(pp, 0xffff00ff, 0.025);
                    attackpt1.push_back(pp);
                }
            }
        }
        if (state == 2 || state == 3)
        {
            vec p = pos - dir * r;
            vec v = -dir.normcopy();
            const int len = 100;
            static vec slstpos[len] = { vec(-100, 0) };
            if (slstpos[0].x <= -100){
                for (int i = 0; i < len; i++)
                    slstpos[i] = p;
            }
            static vec lstp = p;
            real step = USZ;
            for (int i = 0; i < len; i++)
            {
                if(i > becut)
                    break;
                real ai = i / real(len);
                vec vv = lstp - slstpos[i];
                if (i > 0)
                {
                    if (state == 3)
                        p = blend(slstpos[i], lstp, 0.75);
                    else
                        p = blend(slstpos[i], lstp, 0.5);
                }
                slstpos[i] = p;
                lstp = p;
                PSET(p, blendcor(0xFFFFFFFF, 1, ai), blend2(0.01, 0.1, ai/0.5, 2));
                // behit
                if(!attackpt0.empty())
                {
                    if((attackpt0[rand() % attackpt0.size()] - p).sqrlenxy() < 0.05)
                    {
                        PSET(p, 0xFF0000FF, 0.05);
                        becut = i;
                       // if(rand() % 10 == 0)
                            food::create(p, vv * 0.1);
                        onfood(p);
                    }
                }
                if (i > 5 && i < 40 && i % 15 == 0)
                {
                    //PSET(p, 0xFFFFFFFF, blend2(0.01, 0.2, (i - 5) / 35.0f, 2));
                    
                    int len1 = 5 * blend2(0.1, 1, ai / 0.5, 2);
                    vec pp1 = p;
                    vec pp2 = p;
                    vv.norm();
                    vec vv1(-vv.y, vv.x);
                    vec vv2(vv.y, -vv.x);
                    for (int ii = 0; ii < len1; ii++)
                    {
                        pp1 += vv1 * (USZ);
                        PSET(pp1, 0xFFFFFFFF, 0.05);
                        pp2 += vv2 * (USZ);
                        PSET(pp2, 0xFFFFFFFF, 0.05);
                    }
                    
                }
            }
        }
    }
}
// ----------------------------------------------------------------
// daphnia
// ----------------------------------------------------------------
namespace daphnia
{
    real daphnia_ang = 0;
    vec daphnia_vel = vec::UX;
    vec daphnia_pos;
    int behitpos = 10000;
    int hp = 20;
    void daphnia(int ind)
    {
        if(hp <= 0)
        {
            trigger ++;
            return;
        }
        daphnia_ang += rrnd(-1, 1) * 0.001f;
        daphnia_vel.rot(daphnia_ang);
        vec npos = daphnia_pos + daphnia_vel * 0.01;
        if(checkpos(npos))
        {
            daphnia_pos = npos;
        }
        
        vec p0 = daphnia_pos;
        int len0 = 80;
        real sz0 = 5;
        {// ªÊ÷∆
            vec v = -daphnia_vel;
            v.norm();
            vec p = p0;
            int len = 1 * len0;
            for (int i = 0; i < len; i++)
            {
                real ai = real(i) / len0;
                real ang = blend(-PI / 1.1, PI / 1.2, ai);
                v.rot(ang * 0.1, vec::UZ);
                
                p = p + v * (0.005 * sz0);
                PSET(p, 0xFFFFFFFF, 0.025);
                
                if (i == int(len0 * 0.925))
                {// eye
                    PSET(p + vec2(v.y, -v.x) * (0.007 * sz0), 0xFFFFFFFF, 0.1);
                }
                if (ai > 0.2 && ai < 0.75)
                {// body
                    vec vv(v.y, -v.x);
                    real sz = blend2(0, 0.07, (ai - 0.2) / 0.55, 2) * sz0 * (0.9 + 0.1 * sin(gtime * 13.5));
                    vec pp = p + vv * sz;
                    if(i < behitpos)
                    {
                        // behit
                        if(!attackpt0.empty())
                        {
                            if((attackpt0[rand() % attackpt0.size()] - p).sqrlenxy() < 0.025)
                            {
                                PSET(p, 0xFF0000FF, 0.05);
                                behitpos = i;
                                
                                food::create(p + vv * rrnd(0.25 * sz, sz), vv * 0.5f);
                                
                                onfood(p);
                            }
                        }
                        
                        PSET(pp, 0xFFFFFFFF, 0.01);
                        
                        if (rand() % 100 < 10)
                            PSET(p + vv * rrnd(0.25 * sz, sz), 0xA0A0A0A0, 0.025);
                    }
                    else
                    {
                        if(hp > 0)
                        {
                            if(rand() % 10000 == 0)
                            {
                                food::create(p + vv * rrnd(0.25 * sz, sz), vv * 0.5f);
                                
                            }
                            if(rand() % 3000 == 0)
                                hp --;
                        }
                    }
                    
                    
                    if (hp > 10 && (i == int(len0 * 0.52) || i == int(len0 * 0.55)))
                    {// legs
                        vv.rot(sin(gtime * 28.1 + blend(-1, 1, rndmap[i][18]) * PI * 2) * PI * 0.15);
                        for (int ii = 0; ii < 8; ii++)
                        {
                            pp = pp + vv * (0.0035 * sz0);
                            PSET(pp, 0xFFFFFFFF, 0.02);
                        }
                        vv.rot(-PI / 4);
                        for (int ii = 0; ii < 4; ii++)
                        {
                            pp = pp + vv * (0.0035 * sz0);
                            PSET(pp, 0xFFFFFFFF, 0.02);
                        }
                    }
                }
            }
        }
    }
}
// ----------------------------------------------------------------
// lian1
// ----------------------------------------------------------------
namespace lian1
{
    int hp = 10;
    void form()
    {
        if(hp <= 0)
        {
            trigger ++;
            return;
        }
        
        real aa = 0.5 + 0.5 * cos(gtime * 0.2);
        real ang0 = PI / 100;
        vec p(0, -10);
        vec v0(0, 1);
        int cnt = 0;
        const int len = 150;
        static int s_state[len + 1] = {0};
        for(int i = 0; i <= len; i ++)
        {
            real ai = i / real(len);
            real ang = ang0 * blend2d(0.45, 0.55, p.x + aa, p.y);
            v0.rot(ang);
            p += v0 * 0.1;
            
            vec v = vec(-v0.y, v0.x);
            if(rand() % 3 == 0)
            {
                PSET(p - v * rrnd(0.15, blend2(1, 5, ai, 4)), 0xff808080, 0.02);
            }
            // behit
            if(!attackpt0.empty() && s_state[i] < 10 && i % 7 == 0)
            {
                if((attackpt0[rand() % attackpt0.size()] - p).sqrlenxy() < 0.05)
                {
                    PSET(p, 0xFF0000FF, 0.05);
                    s_state[i] ++;
                    
                    if(rand() % 10 == 0)
                        food::create(p, v * 0.1);
                    
                    onfood(p);
                }
            }
            if(s_state[i] < 10 && i % 7 == 0)
            {
                xibao(p - v * 0.25, v, v, 50 - s_state[i] * 5, i, 0.25);
                cnt ++;
            }
            else
                PSET(p, 0xffffffff, 0.04);
        }
        if(cnt <= 0)
        {
            hp = 0;
        }
    }
}
// ----------------------------------------------------------------
// forms
// ----------------------------------------------------------------
namespace forms
{
    void shape1()
    {
        if(framecnt % 10 == 0)
        {
            real r = rrnd(0, 1);
            real ang = rrnd(0, PI * 2);
            vec np = m_1_3::gtarget + vec(r * cos(ang), r * sin(ang));
            if(checkpos(np))
            {
                m_1_3::gtarget = np;
            }
        }
        m_1_3::pos = blend(m_1_3::pos, m_1_3::gtarget, 0.015);
        vec dir = (m_1_3::gtarget - m_1_3::pos).normcopy();
        m_1_3::bottledragon(m_1_3::pos, dir);
    }
    
    void shuizao()
    {
        vec p = vec(-8, -8);
        static float s_t1[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };
        vec v = vec::UY;
        int len = 250;
        for (int i = 0; i < len; i++)
        {
            real ai = real(i) / len;
            v = blend(v, vec::UX, 0.1 * ai);
            
            p = p + vec(-v.y, v.x, 0) * FT(ai * PI * 2 + gtime * 0.2, s_t1) * 0.02 + v * 0.05;
            
            if(i % 20 == 0 || i == len - 1)
            {
                PSET(p + vec(-v.y, v.x, 0) * 0.01,  0xffffffff, 0.018);
                PSET(p + vec(-v.y, v.x, 0) * 0.025, 0xffffffff, 0.05);
                PSET(p + vec(-v.y, v.x, 0) * 0.08,  0xffffffff, 0.1);
            }
            
            PSET(p, 0xFF808080, blend(0.025, 0.01, ai, 4));
        }
    }
    
    void qiuzao()
    {
        int len = 250;
        for(int i = 0; i < len; i ++)
        {
            vec p = vec(blend(-1, 1, rndmap[i][1]), blend(-1, 1, rndmap[i][2]), blend(-1, 1, rndmap[i][3]));
            p.norm();
            p.rot(gtime * 0.5f, vec::UY);
            PSET(p * 0.5, 0xffffffff, 0.015);
        }
    }
}

namespace shuixi {
    int lifestep = 1;
    int pixel_size0;
    real leveluptime = 0;
    int MAXX = SCREENWID;
    int MAXY = SCREENHGT;
    real movespeed = 1;
    int eatnum = 0;
    real eattime = 0;
    int drawstep = 1;
    real powerspeedtime = 0;
    vec2 pos, vel;
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // shuixi
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    inline float FT3(float angle, float t[] = 0, float dt = 0)
    {
        if (t == 0)    {static float s_t0[] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) };t = s_t0;    }
        
        float yy = 0;
        yy += 1 * sin(1 * angle + (t[0] + dt) * PI);
        yy += 0.25    * sin(0.5 * angle + (t[1] + dt) * PI);
        return yy;
    }
    
    void hydra(vec2 p0, vec2 v, int len0, int index,
               int showtype = 1
    )
    {
        static bool light = 1;
        
        if (showtype == -1 && framecnt > 200)
            return;
        
        if (lifestep <= 0 || lifestep > 3)
            return;
        
        
        static real chuxuzhuangtai[10][4] = { 0 };
        float s_t[][6] = {
            { rndmap[index][0], rndmap[index][1], rndmap[index][2], rndmap[index][3] },
            { rndmap[index][10], rndmap[index][11], rndmap[index][12], rndmap[index][13] },
            { rndmap[index][20], rndmap[index][21], rndmap[index][22], rndmap[index][23] },
            { rndmap[index][30], rndmap[index][31], rndmap[index][32], rndmap[index][33] },
            { rndmap[index][40], rndmap[index][41], rndmap[index][42], rndmap[index][43] },
            { rndmap[index][50], rndmap[index][51], rndmap[index][52], rndmap[index][53] },
        };
         float s_t1[4] = { rndmap[1][index], rndmap[2][index], rndmap[3][index], rndmap[4][index] };
         float s_t2[4] = { rndmap[11][index], rndmap[12][index], rndmap[13][index], rndmap[14][index] };
        
        real sz = 8;
        real ssz = 0.0;
        v.norm();
        vec2 p = p0;
        int len = len0 / 2;
        vec2 p1 = p, p2 = p;
        vec2 eatposv2;
        
        
        real bodysizes[] = { 0.005, 0.004, 0.005 };
        real bodylens[] = { 0.0015, 0.0015, 0.0008 };
        real bodylen = bodylens[lifestep - 1];
        real foodsize = blend2(0.8, 4, eatnum / 10.0);
        
        //
        real catchpow = lifestep == 3 ? 7.2 : 3;
        static real tmovespeed = movespeed;
        tmovespeed = blend(tmovespeed, movespeed, 0.1);
        real moveain = blend(1, 0, tmovespeed);
        //
        real shootang = sin(gtime * PI * 0.2);
        
        for (int i = 0; i < len; i++)
        {
            real ai = real(i) / len;
            v = v + vec2(rndmap[i][0], rndmap[0][i]) * (0.04 * moveain);
            {
                real ftt = FT(ai * 2 * PI + gtime * PI * 0.05);
                v.rot(ftt * PI * 0.008 * moveain);
                if (v.y < 0.4) v.y = 0.4;
            }
            v.norm();
            if (lifestep == 3 && i == 0)
            {
                v.rot(PI * 0.25 * shootang);
            }
            p = p + v * blend(sz * bodylen, sz * bodylen * catchpow, tmovespeed);
            {
                if (gtime - powerspeedtime < .25)
                {// À∫“ß∂Øª≠
                    real ta = (gtime - powerspeedtime) / .25;
                    real ssz0 = blend2(0, 3, ta);
                    ssz = blend(ssz0 * 0.01, 0, fabs(ai - 1) / 0.25, 2);
                }
                if (eattime > 0 && gtime - eattime < 2)
                {// œ˚ªØ∂Øª≠
                    real foodpos = blend(1, 0, (gtime - eattime) / 2);
                    ssz = blend(0.01, 0, fabs(ai - foodpos) / 0.25, 2);
                    ssz = ssz * (0.5 + 0.5 * sin(gtime * PI * 10));
                    if (i == int(len * foodpos))
                        eatposv2 = p;
                }
                
                real bodysize = bodysizes[lifestep - 1];
                {// ÷–≤ø¬‘∑
                    bodysize = blend2(bodysize * 0.5, bodysize * foodsize, ai, 2);
                }
                real a2d1 = blend2d(0.5, 1.2, p1.x, p1.y);
                real ft1 = FTU(ai * 2 * PI + gtime * PI * 0.1, s_t1);
                p1 = p + vec2(v.y, -v.x) * (bodysize * sz * a2d1 + bodysize / 3 * sz * ft1 + ssz);
                real a2d2 = blend2d(0.5, 1.2, p2.x, p2.y);
                real ft2 = FTU(ai * 2 * PI + gtime * PI * 0.1, s_t2);
                p2 = p - vec2(v.y, -v.x) * (bodysize * sz * a2d2 + 0.01 * sz * ft2 + ssz);
                
                if (i % drawstep == 0)
                {
                    PSET(p1, 0xffffffff, 0.01);
                    PSET(p2, 0xffffffff, 0.01);
                }
                if (i < len - 8)
                {
                    if (i % 10 == 0)
                    {
                        int len1 = len0 / 30;
                        for (int ii = 0; ii < len1; ii++)
                        {
                            real aii = real(ii) / len1;
                            if (ii % drawstep == 0)
                                PSET(blend(p1, p2, aii));
                        }
                    }
                    if (i % 10 == 4)
                    {
                        int len1 = len0 / 80;
                        for (int ii = 0; ii < len1; ii++)
                        {
                            real aii = real(ii) / len1;
                            if (ii % drawstep == 0)
                                PSET(blend(p1, p2, aii));
                        }
                    }
                }
            }
            
            if (i == len - 4)
            {
                for (int ii = 0; ii <= 6; ii++)
                {
                    real aii = real(ii) / 6;
                    vec2 pp = blend(p1, p2, aii);
                    if (ii % drawstep == 0)
                        PSET(pp);
                }
            }
            if (i == len - 2)
            {
                for (int ii = 0; ii <= 6; ii++)
                {
                    real aii = real(ii) / 6;
                    vec2 pp = blend(p1, p2, aii);
                    vec2 vv = v.rotcopy(FT(aii * 2 * PI + gtime * PI * 0.1, s_t[5]));
                    for (int jj = 0; jj < 8; jj++)
                    {
                        vv = vv + vec2(rndmap[jj][index + ii], rndmap[index + jj][ii + 100]) * 0.5;
                        vv.norm();
                        pp = pp + vv * 0.02;
                        if (jj % drawstep == 0)
                            PSET(pp);
                    }
                }
            }
            if (showtype == -1)
            {//
                if (i > len * 0.8)
                {
                    static bool first = 1;
                    if (first)
                    {
                        first = 1;
                        vel = v.normcopy() * 0.005;
                        pos = p + v.normcopy() * 0.05;
                        first = 0;
                    }
                    return;
                }
            }
            if (showtype != -1  // ¥•–Îœ˚ ß
                && i == len - 1)
            {// ¥•–Î
                //flushpixels(); pixel_size0 = 1; //…Ë÷√µ„µƒ¥Û–°
                real chuxulens[] = {0.8, 0.75, 1.2};
                real chuxulen = chuxulens[lifestep - 1];
                {
                    real a2d0 = blend2d(0.1, 1, p.x, p.y);
                    real a2dsz = lifestep == 3 ? 0.5 : 1;
                    real tapowerspeedtime = (gtime - powerspeedtime) / .25;
                    for (int j = 0; j <= 3; j++)
                    {
                        real aj = real(j) / 3;
                        vec2 pp = blend(p1, p2, aj);
                        vec2 vv = v.rotcopy((blend(-0.5 * PI, 0.5 * PI, aj) + rndmap[index + 0][j] * PI * 0.1) * moveain);
                        if (lifestep == 3 && j == 0)
                        {
                            vv = vec2(1, 1); vv.norm();
                        }
                        
                        // ¥•–Î≥§∂»
                        int len1 = lifestep == 3 ? len0 / 2 * chuxulen : len0 / 2 * (0.7 + rndmap[j][index + 1] * 0.3) * chuxulen;
                        if (lifestep == 3 && j != 0)
                            len1 *= 0.5;
                        if (len1 % 2 == 0)
                            len1 -= 1;
                        
                        {
                            len1 += tmovespeed * len1 * 0.8;
                        }
                        
                        
                        static real sdangs[4][128] = {0};
                        static real flagtab[4] = { 1, -1, 1, -1 };
                        static real catchangflagtab[4] = { 1, 1, -1, -1 };
                        real catchang = 5.8 * catchangflagtab[j];
                        
                        for (int ii = 0; ii < len1; ii += 2)
                        {
                            real aii = ii / real(len1);
                            
                            real a2d = blend2d(-a2dsz, a2dsz, pp.x + index, pp.y);
                            real ft = FT3(1.5 * PI * rndmap[index + 10][j] + gtime * PI * 0.3 + aii * PI * 2 + index * PI / 3, s_t[j]);
                            real dft = flagtab[j] * (a2d * 1 + ft * 1.0) * 0.15;
                            
                            real dang0 = chuxuzhuangtai[index][j] == 0 ? dft : catchang;
                            if (lifestep == 3)
                            {
                                if (j != 0)
                                    dang0 = catchang;
                                else if(j == 0)
                                {
                                    dang0 += PI / 4 * FT(gtime * PI * 0.21 + index * PI / 3, s_t[index]);
                                    if (dang0 > .25) dang0 = .25; else if (dang0 < -.25) dang0 = -.25;
                                }
                            }
                            
                            real& ang = sdangs[(index + j) % 4][ii % 128];
                            if (chuxuzhuangtai[index][j] == 0)
                                ang = blend(ang, dang0, 0.05);
                            else
                                ang = blend(ang, dang0, 0.008);
                            
                            if (lifestep == 3)
                                vv.rot(blend(0, blend(catchang, ang, moveain), aii) * a2d * moveain);
                            else
                                vv.rot(blend(0, ang, aii) * moveain);
                            
                            pp = pp + vv * 0.05;
                            
                            {
                                PSET(pp);
                                if (ii >= len1 - 2)
                                {
                                    PSET(pp);
                                }
                            }
                            
                            
                        }
                    }
                }
            }
        }
        
        if (showtype > 0)
        {
            
            if (lifestep == 3)
            {
                //  π”√œﬂ¿¥≈–∂œ
                vec2 eatpt = p + v * 0.01;
                static vec2 leatpt = eatpt;
                real dis0 = (leatpt - eatpt).len();
                real dis = (plypos - eatpt).len() + (plypos - leatpt).len();
                leatpt = eatpt;
                // ±ÿ–Î‘⁄À∫“ß ±≤≈ƒ‹±ª≥‘µÙ
                real ta = (gtime - powerspeedtime) / .25;
                // ±ÿ–Î”√◊Ï÷±Ω”“ßµΩ
                if (ta < 0.5
                    && (dis - dis0) < 0.04)// && (daphnia_pos - eatpt).cross(v) > 0.0)
                {//  «∑Ò≤∂◊ΩµΩÀÆ‘È
                    
                   // daphnia_state = 2; // ±ª◊•◊°
                    plypos = p;
                    eattime = gtime;
                    
                   // playsound(1);
                    
                    
                    eatnum++;
                }
                
               
            }
            
            for (int jj = 0; jj < 4; jj++)
            {
                if (gtime - chuxuzhuangtai[index][jj] > 2.0)
                    chuxuzhuangtai[index][jj] = 0;
            }
            
            //////////////////////////////// ƒ‹¡ø”Î‘À∂Ø ////////////////////////////////
            pos = (p0 + p) / 2;
           
            static real lstgetpower = gtime;
            if (gtime - lstgetpower > 1)
            {
               // energefood += rrnd(1, 5);
                lstgetpower = gtime;
            }
           // moveenerge = 50; // √ø¥ŒÀ∫“ß–Ë“™µƒƒ‹¡ø
            {
                static real lstpowtime = gtime;
                if (gtime - lstpowtime > 15)
                {
                    lstpowtime = gtime;
                   
                }
                //if (lifestep == 3)
                //{// π‚œﬂ
                //    if (movespeed < 0.01)
                //        light = 0;
                //    else
                //        light = 1;
                //}
                //else
                {
                  //  light = energefood > moveenerge;
                }
            }
        }
        // ◊Ë¡¶
        movespeed = movespeed * pow(0.995, 1);
    }
}
