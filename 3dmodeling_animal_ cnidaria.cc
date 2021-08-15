/*
    3dmodeling->animal->cnidaria
*/
// --------------------------------------------------------------------
// Jellyfish
// --------------------------------------------------------------------
namespace Atolla
{
    const int NUM_RIBBON = 20;
    scene1::ribbon_t ribbons[2][NUM_RIBBON];
    vec robbin_pos[NUM_RIBBON];
    int eatcnt = 0;
    // -----------------------------------------------------------------
    bool fixedborder(vec& p)
    {
        bool bout = false;
        {// border
            vec2 op = p.xy() - vec2(0, 23.5);
            if(op.len() > 38)
            {
                p = vec2(0, 23.5) + op.normcopy() * 38;
                bout = 1;
            }
            if(p.y > 24)
            {
                p.y = 24;
                bout = 1;
            }
        }
        return bout;
    }
    int fixedborder2(vec& p)
    {
        {// border
            vec2 op = p.xy() - vec2(0, 23.5);
            if(op.len() > 36)
            {
                p = vec2(0, 23.5) + op.normcopy() * 36;
                return 1;
            }
            if(p.y > 21)
            {
                p.y = 21;
                return 2;
            }
        }
        return 0;
    }
    // -----------------------------------------------------------------
    void createribbon(int group, vec o, vec v, int ind)
    {
        static float s_array[16][4];
        static bool isinit = 0;
        if (!isinit) {
            isinit = 1;
            for (int i = 0; i < 16; i++)
                for (int ii = 0; ii < 4; ii++)
                    s_array[i][ii] = rrnd(0, 1);
        }
        
        VECLIST e;
        vec p = o;
        real fwid = 0.02f;
        real fstep = 0.01f;
        vec side = v.cross(vec::UZ); side.norm();
        vec lp1, lp2;
        vec vv = v;
        int len = 50;
        for (int i = 0; i < len; i++)
        {
            real ai = i / real(len);
            ribbons[group][ind % NUM_RIBBON].addpt(p, vec::ZERO);
            
            vv.rot(PI / 58 * FT(ai * PI * 5, s_array[ind], gtime * PI / 8), vec::UZ); vv.norm();
            p += vv * fstep;
            
        }
    }
    // -----------------------------------------------------------------
    vec jfh_pos = vec(10, 5, 0);
    vec jfh_dir = -vec::UY;
    real jfh_rot = 0;
    int gsuimutype = 0;
    void updateribbon(int group, vec p, vec nv0, int ind)
    {
        if (ribbons[group][ind % NUM_RIBBON].rbpts.empty())
        {
            createribbon(group, p, nv0, ind);
        }
        scene1::ribbon_t& ribbon = ribbons[group][ind % NUM_RIBBON];
        
        real detaind = 0.5 + 0.5 * sin((ind + gtime * 10.1) * PI / 8);
        vec nv = nv0;
        vec uz = vec::UZ;
        //uz.rot(ind * PI / NUM_RIBBON, vec::UY);
        //nv.rot(sin(gtime * 5 + ind * PI / 4 * rndmap[18][ind]) * 0.25 * PI, uz);
        
        real fstep = 1.0f;
        real grow = 1;
        int len0 = ribbon.rbpts.size();
        for (int i = 0; i < len0; i++)
        {
            real ai = real(i) / len0;
            {
                if (i > 0)
                {
                    //ribbon.rbpts[i].v = blend(ribbon.rbpts[i].v, ribbon.rbpts[i - 1].v, 0.1);
                    ribbon.rbpts[i].p += ribbon.rbpts[i].v * (1.5);
                }
                else
                {
                    //ribbon.rbpts[i].v = nv * 0.05;
                    ribbon.rbpts[i].p = p;
                }
            }
            if (i > 0)
            {
                ribbon.rbpts[i].p = blend(ribbon.rbpts[i].p, ribbon.rbpts[i - 1].p, 0.5);
            }
            {// border
                fixedborder(ribbon.rbpts[i].p);
            }
            vec pp = ribbon.rbpts[i].p;
            if (i > 0)
            {
                int lum = blend(255, 250, ai);
                {
                    
                    PSET(pp, RGBA2(lum, 0, 0, 250), blend(0.15, 0.01, ai, 1));
                }
                
                ribbon.rbpts[i].pp = pp;
            }
            else
                PSET(ribbon.rbpts[i].p, RGBA2(255, 250, 0, 250), 0.15);
            
        }
    }
    
    // --------------------------------------------------------------
    const int LEN_NUM = 25;

    std::vector<vec> jfptlist;
    std::vector<vec> ribbonlist;
    void jellyfish(int ind)
    {
        vec p = jfh_pos;
        real time = gtime;
        real d1 = RidgedMF(p.x, p.y, p.z + ind, 2, 0.5) + 0.5;
        real at = fabs(sin(time * 0.25));
        real fstep = 0.01 * at;
        int len = jfptlist.size();
        vec up = vec::UY;
        int cor = blendcor(0xFFFFFFFF, 0xFF000000, rndmap[ind][0]);
        for (int i = 0; i < len; i++)
        {
            real ai = i / real(len);
            real deta1 = perlin(p.x, p.y * 5, p.z + ind) - 0.1;
            real deta2 = perlin(p.x + 5, p.y * 5, p.z + ind) - 0.1;
            p = jfptlist[i];p.z -= 1;
            PSET(p, cor, blend(0.05, 0.2, ai));
        }
        
        vec vsx = vec::UZ;
        for(int i = 0; i < ribbonlist.size(); i ++)
        {
           updateribbon(0, ribbonlist[i], blend(-jfh_dir, vsx.rotcpy(i * 2 * PI / ribbonlist.size(), jfh_dir), 0.5), i);
        }
    }
    
    // ================================================================
    // mesh
    // ================================================================
    const int ROBBIN_NUM = 10;
    vec rob_pos[ROBBIN_NUM][3] = {};
    vec rob_vec[ROBBIN_NUM][3] = {};
    bool brobbin = 0;
    vec s_robbinlstv[ROBBIN_NUM][180];
    real s_robbinstep[ROBBIN_NUM] = { 0 };
    
    // ----------------------------------------------------------------
    void jellyfishMesh(int data, int type)
    {
        vec uz = vec::UZ;
        int ret = 0;
        if(ret = fixedborder2(jfh_pos))
        {
            if(ret == 2)
            {
                jfh_dir.y = abs(jfh_dir.y);
                jfh_pos -= jfh_dir * 0.25f;
            }
            if(ret == 1)
            {
                jfh_rot += PI / 16;
                jfh_dir.rot(jfh_rot, uz);
                jfh_pos -= jfh_dir * 0.1f;
            }
        }
        else
        {
            jfh_rot += rrnd(-1, 1) * 0.001;
            if(jfh_rot > 0.01) jfh_rot = 0.01;
            if(jfh_rot < -0.01) jfh_rot = -0.01;
            
            jfh_dir.rot(jfh_rot, uz);
            jfh_pos -= jfh_dir * 0.1f;
        }
        
        jfptlist.clear();
        ribbonlist.clear();
        
        // hat
        gcommonvertex = 1;
        gverindfindstart = gsubmesh.vertices.size();
        binvnorm = 1;
        real sz = 0.1;
        
        real deta0 = 0.2 * sin(gtime * 5.0);
        VECLIST e;
        vec dx, dy;
        {
            dx = uz;
            dy = jfh_dir.cross(dx).normcopy();
        }
        color = 0xFF0000FF;
        roundedge(e, jfh_pos + jfh_dir * 0.425, dx, dy, 1 * sz, NUM_RIBBON, 0);
        real d = 4 * sz;
        
        int len = LEN_NUM;
        
        color = 0x00ffffff;
        int basecor2 = 0x10FF0000;
        if (gamelevel < 7)
        {
            basecor2 = 0x100000FF;
        }
        real growalpha0 = 0.5;
        
        basecor2 = 0x20FFFFFF;
        int fecolor0 = RGBA(250, 0, 180, 0);
        
        for (int i = 0; i <= len; i++)
        {
            real ai = i / real(len);
            real scl = 5 + 3 * (deta0) * (1 - ai);
            color = (0x10FFFFFF);
            for (int ii = 0; ii < e.size(); ii++)
            {
                //if (ii % 2 == 1)
                if(ai < 0.5)
                    e[ii].cor = 0xff000080;
                else
                    e[ii].cor = color;
                if(ai < 0.5 && ii < e.size() - 1)
                    jfptlist.push_back(e[ii].p);
                
                if(i == (len / 2) && ii < e.size() - 1)
                {
                   ribbonlist.push_back(e[ii].p);
                }
            }
            {
                ai < 0.5 ? binvnorm = true : 0;
                extrudeedgeex2(e, -d * (ai <= 0.5 ? -1.5 * ai : 0.5), (ai <= 0.5 ? blend(0.2, 1, ai / 0.5, 0.5) : blend(1, 0.2, (ai - 0.5) / 0.5, 4)) * (scl));
                binvnorm = 0;
            }
            
            gverindfindstart += NUM_RIBBON;
        }
        
        face(e, getedgecenter(e) - jfh_dir * 0.25f);
        binvnorm = 0;
        gcommonvertex = 0;
    }
}
