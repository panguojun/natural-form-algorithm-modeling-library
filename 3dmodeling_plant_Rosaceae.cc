/*
    3dmodeling->plant->Rosaceae
*/
// --------------------------------------------
// 梅花 (For Tree.bonsai)
// --------------------------------------------
bool plum3D(VECLIST e, vec o, vec v0, real maxsz, int len0, int depth, int maxdepth, tree_t& tree)
{
    int treeedges = blend(3, 17, tree.dat.sz0 / 0.45);
	if (renderfrm == 1)
	{
		gindexmap[tree.dat.index % 1024] = 1;

	}
	tree.dat.renderfrm = renderfrm;
	tree.dat.starttri = gsubmesh.tris.size();

	int branches_color0 = tree.dat.lvcor;
	if (depth == 0)
	{
        if(season == 1)
        {
            branches_color0 = RGB(24, 8, 18);
        }
		else if (season == 2)
		{
			branches_color0 = 0xFF081510;
		}
		else if (season == 3)
		{
			branches_color0 = 0xFF002030;
		}
        else
        {
            branches_color0 = 0xFF102007;
        }
	}

	branches_color0 = tree.dat.ya > 0 ? 0xFF0000FF : branches_color0;

	if (tree.dat.health < 0)
		branches_color0 = tree.dat.lvcor = 0xFF000000;
	branches_cnt++;
	color = branches_color0;
	vec p = depth == 0 ? o : tree.dat.p0;
	vec rp = o;
	vec v = v0;
	int index = depth == 0 ? int((p.x + p.z + p.y) * 256) % 1024 : tree.dat.index;

	real dpdeta = blend(0, 172.862, rndmap[index][0]);
	real dpdeta2 = blend(0, 172.862, rndmap[index][1]);
	real dpdeta3 = blend(0, 172.862, rndmap[index][2]);
	if (!blastpicked && tree.dat.health > 0)
	{
		if ((season == 1 || season == 2 || season == 3) && tree.dat.sz0 < maxsz && (tree.dat.power > 0 && rand() % 100 == 0 || tree.dat.drug > 0 && rand() % 2 == 0))
		{
            real dsz = 0;//depth == 0 ? 0.002 : 0.005;
            if(depth == 0)
                dsz = 0.00025;
            else if(depth > 0 && depth <= 2)
                dsz = 0.008;
            else
                dsz = 0.003;
            
            tree.dat.sz0 += dsz;
            if(depth > 0 && tree.dat.sz0 > 1)
            {
                tree.dat.sz0  = 1;
            }
			if (tree.dat.ya > 0)
				tree.dat.ya--;
			if (tree.dat.drug <= 0)
				tree.dat.power--;
		}
		if ((season == 1 || season == 2 && depth != 0) && tree.dat.len < len0 && tree.dat.power > 0)
		{
			if (depth != 0 && rand() % int(tree.dat.len / (real)tree.dat.len0 * 10 + 1) == 0 || depth == 0 && tree.dat.len < 0.5 * len0 || rand() % 18 == 0)
			{
				tree.dat.len++;
				if (tree.dat.ya > 0)
					tree.dat.ya--;
				tree.dat.power--;
			}
		}
	}
	if ((year > 2 && season >= 3) && ((depth > 0 && tree.dat.power <= 0 && rand() % 10 == 0) // 秋天生病
		|| (depth < 3 && tree.children.size() == 0)) && rand() % 10 == 0 // 秃枝易得病
		|| season != 1 && depth == 0 && (tree.children.size() == 0 && rand() % 3 == 0)) // 秃主干易得病
	{
        tree.dat.health--;
	}
    
	tree.dat.len = MIN(tree.dat.maxlen, tree.dat.len);
	int len = MIN(len0, tree.dat.len);
	if (tree.dat.cutindex != -1)
		len = MIN(tree.dat.cutindex, len);
	if(len == 0) len = 1;
    if(tree.dat.len == 0) tree.dat.len = 1;
    
    real sz0 = tree.dat.sz0;

    vec p0, vv0;
    vec hudrp;
	int fruitnode = 0;
	int upfactor = blend(0.1, 0.3, rndmap[index][3]);
	real fstep0 = blend(0.05, 0.20, tree.dat.len / (real)len0);

	sz0 < 0.3 ? gcommonvertex = 1 : gcommonvertex = 0;
	//gcommonvertex = 1;
	gverindfindstart = gsubmesh.vertices.size();
    
    if(!blastpicked && len <= 1 && tree.dat.cutindex != -1)
    {
        return false;
    }
    g_brucheslencnt += 10 + len;
	int childrencnt = 0;
	for (int i = 0; i <= len; i++)
	{
        int starttri = (int)gsubmesh.tris.size();
		if (tree.dat.drugpos != -1 && tree.dat.drugind == -1 && starttri + treeedges * 2 >= tree.dat.drugpos)
		{
			tree.dat.drugind = i;
		}
		if (tree.cuttingpos == -2 || tree.cuttingpos != -1 && starttri + treeedges * 2 >= tree.cuttingpos)
		{
			if (picktype == 1)
				branches_color0 = 0xFF0000FF;
			else if (picktype == 2)
				branches_color0 = 0xFF00FF00;
		}

		if (tree.dat.cutindex == -1 && tree.dat.cutpos != -1 && starttri + treeedges * 2 >= tree.dat.cutpos)
		{
			tree.dat.cutindex = i;
			tree.dat.cutpos = starttri;
            vec rp0 = getedgecenter(e);
            if (depth < 3 || sz0 > 0.1)
                face(e, rp0 + v * (sz0 / 2));
            tree.dat.endtri = starttri;
            
			{ // 删除附带
				{// flowers
					std::map<int, std::vector<flower_t> >::iterator iter = flowermap.find(tree.dat.id);
					if (iter != flowermap.end())
					{
						std::vector<flower_t>& flist = (*iter).second;
						std::vector<flower_t>::iterator it = flist.begin();
						while (it != flist.end())
						{
							if ((*it).i >= tree.dat.cutindex)
							{
								flist.erase(it, flist.end());
								break;
							}
							++it;
						}
					}
				}
				{// Leaves
					std::map<int, std::vector<leaf_t> >::iterator iter = leavemap.find(tree.dat.id);
					if (iter != leavemap.end())
					{
						std::vector<leaf_t>& flist = (*iter).second;
						std::vector<leaf_t>::iterator it = flist.begin();
						while (it != flist.end())
						{
							if ((*it).i >= tree.dat.cutindex)
							{
								flist.erase(it, flist.end());
								break;
							}
							++it;
						}
					}
				}
				{// Fruit
					std::map<int, std::vector<fruit_t> >::iterator iter = fruitmap.find(tree.dat.id);
					if (iter != fruitmap.end())
					{
						std::vector<fruit_t>& flist = (*iter).second;
						std::vector<fruit_t>::iterator it = flist.begin();
						while (it != flist.end())
						{
							if ((*it).i >= tree.dat.cutindex)
							{
								flist.erase(it, flist.end());
								break;
							}
							++it;
						}
					}
				}
                {// mushrooms
                    for (int ii = units.size() - 1; ii  >= 0; ii--)
                    {
                        if(units[ii].id == tree.dat.id)// && i <= units[ii].growpos)
                        {
                            units.erase(units.begin() + ii);
                        }
                    }
                }
			}
            
            tree.cutdis = (rp0 - o).len();
            return true;
		}
		if (!blastpicked)
			if ((tree.dat.cutindex != -1 || tree.dat.cutpos != -1) &&
                tree.children.size() == 0 &&
                (tree.dat.cutindex != -1 && tree.dat.cutindex < 4 ||
                tree.dat.cutindex != -1 && (tree.dat.cutindex < len0 / 4))
                )
				return false;

		bool bgrow = true;
		if (!blastpicked && season == 4)
		{// 冬季枯枝
			if (tree.dat.sz0 < 0.025 && rand() % 500 == 0)
			{
				return false;
			}
		}

		real ai = real(i) / (tree.dat.len);
		real ai0 = real(i) / (len0);
		vec np;
        
        if(emstate[1] == 1)
        {// song
            real perlin1 = perlin(p.x * 4 + depth * 118.8, dpdeta2 + p.y * 2.5, p.z * 2 + dpdeta);
            real perlin2 = perlin(p.x * 2.2 + dpdeta, p.y * 3.1 + dpdeta3, p.z * 2.1 + depth * 118.8);
            vec dp = vec::UX * (perlin1)+vec::UY * blend(-0.025, 0.06, rndmap[1 + i][index + 2]) + vec::UZ * ((perlin2));
            if (depth > 1)
            {
                v = blend(v, dp, blend(0.1, 0.35, ai0));
                v = blend(v, vec::UY * blend(-0.1, 1, ai0), 0.25);//blend(upfactor, 0.4, ai0, 2));
            }
            else if (depth == 1)
            {
                v = blend(v, dp, blend(0.25, 0.6, ai0));
                v = blend(v, vec::UY * blend(-0.15, 0.75, ai0, 2), 0.2);
            }
            else
            {
                v = blend(v, dp, blend(0.2, 0.75, ai0));
            }
        }
        else if(emstate[0] == 2)
        {// ziteng
            real perlin1 = perlin(p.x * 4 + depth * 118.8, dpdeta2 + p.y * 2.5, p.z * 2 + dpdeta);
            real perlin2 = perlin(p.x * 2.2 + dpdeta, p.y * 3.1 + dpdeta3, p.z * 2.1 + depth * 118.8);
            vec dp = vec::UX * (perlin1)+vec::UY * blend(-0.025, 0.06, rndmap[1 + i][index + 2]) + vec::UZ * ((perlin2));
            if (depth > 1)
            {
                v = blend(v, dp, blend(0.1, 0.35, ai0));
                v = blend(v, vec::UY * blend(0, -0.65, ai0, 1), 0.5);
            }
            else if (depth == 1)
            {
                v = blend(v, dp, blend(0.25, 0.6, ai0));
                v = blend(v, vec::UY * blend(0, -0.4, ai0, 1), 0.5);
            }
            else
            {
                v = blend(v, dp, blend(0.2, 0.75, ai0));
            }
        }
        else if(emstate[0] == 3)
        {// yingchun
            real perlin1 = perlin(p.x * 4 + depth * 118.8, dpdeta2 + p.y * 2.5, p.z * 2 + dpdeta);
            real perlin2 = perlin(p.x * 2.2 + dpdeta, p.y * 3.1 + dpdeta3, p.z * 2.1 + depth * 118.8);
            vec dp = vec::UX * (perlin1)+vec::UY * blend(-0.025, 0.06, rndmap[1 + i][index + 2]) + vec::UZ * ((perlin2));
            if (depth > 1)
            {
                v = blend(v, dp, blend(0.1, 0.35, ai0));
                v = blend(v, vec::UY * blend(0, 0.65, ai0, 1), 0.5);
            }
            else if (depth == 1)
            {
                v = blend(v, dp, blend(0.25, 0.6, ai0));
                v = blend(v, vec::UY * blend(-0.01, 0.1, ai0, 1), 0.5);
            }
            else
            {
                v = blend(v, dp, blend(0.2, 0.75, ai0));
            }
        }
        else
        {
            real perlin1, perlin2;
            if (depth == 0)
            {
                perlin1 = perlin(p0.x * 5.12 + depth * 118.8, dpdeta2 + p0.y * 13.5, p0.z * 2 + dpdeta);
                perlin2 = perlin(p0.x * 2.2 + dpdeta, p0.y * 17.7 + dpdeta3, p0.z * 7.1 + depth * 118.8);
            }
            else
            {
                perlin1 = perlin(p.x * 4 + depth * 118.8, dpdeta2 + p.y * 3.5, p.z * 2 + dpdeta);
                perlin2 = perlin(p.x * 2.2 + dpdeta, p.y * 5.1 + dpdeta3, p.z * 2.1 + depth * 118.8);
            }
            vec dp = vec::UX * (perlin1)+vec::UY * blend(-0.025, 0.06, rndmap[1 + i][index + 2]) + vec::UZ * ((perlin2));

            if (depth > 1)
            {
                v = blend(v, dp, blend(0.2, 0.45, ai0));
                v += tree.dat.a;
                if(v.y < 0.5)
                    v = blend(v, vec::UY, 0.25 * blend(upfactor, 0.4, ai0, 2));
            }
            else if (depth > 0)
            {
                v = blend(v, dp, blend(0.45, 0.65, ai0));
                if(v.y < 0.25)
                    v = blend(v, vec::UY, blend(0, 0.5, ai0, 2));
            }
            else
            {
                if(i > 2)
                    v = blend(v, dp, 0.7);
                
                if(i > len0 * 0.45)
                {
                    real lenxz = sqrt(p.x * p.x + p.z * p.z);
                    if(lenxz > 3)
                    {
                        vec ov(-p.x, 0, -p.z);
                        ov /= lenxz;
                        
                        v = blend(v, ov, blend(0.005, 0.05, (ai0 - 0.45) / 0.55));
                    }
                }
                
                vv0 = blend(vv0, dp, blend(0.55, 0.6, ai0 * ai0));
                p0 = p0 + vv0 * (0.15f + 0.0025 * i);
            }
        }
        
		v.norm();
		
		np = p + v * (0.15f + 0.0025 * i);

		
		real sz = blend(sz0, sz0 / 5, ai, v_cuxi);
		VECLIST e1;
		if (i == 0)
		{
			color = branches_color0;
			VECLIST e0;
			//if (sz0 > 0.25 || depth == 0)
			//	roundedge(e0, o, v0, sz * 0.1, MAX(8, 17 - depth));
			//else
				roundedge(e0, o, v0, sz, treeedges);
			if (!e.empty() && e.size() == e0.size())
			{
				face(e, e0);
			}
			e = e0;
		}
		e1 = e;

		if (i < len)
		{//
			color = branches_color0;
			if (i > 0)
			{
				if (depth < 2)
					rotedgebynormex0(rp, e1, v, sz, depth < 2 ? PI / 3 * ai0 : 0);
				else
					rotedgebynormex(rp, e1, v, sz);
			}
            
			// flowers
            if (season == 4 && days > int(seasonday[season] * 0.3) || season == 1)
			{
                if (tree.dat.health > 0 && !blastpicked && (root.dat.len > len0 * 0.5) &&
                    (depth > 0 && sz < 0.05 && i > len0 * 0.2 && (depth > 1 || i > 5)))
				{
                    if(year == 1 && season == 1 && rand() % 5 == 0)
                    {// first year spring flower pos update
                        if (flowermap.find(tree.dat.id) == flowermap.end())
                        {
                            flowermap.insert(std::map<int, std::vector<flower_t> >::value_type(tree.dat.id, std::vector<flower_t>()));
                        }
                        std::vector<flower_t>& flist = flowermap[tree.dat.id];
                        {
                            for (int ii = 0; ii < flist.size(); ii++)
                            {
                                if (flist[ii].i == i)
                                {
                                    flist[ii].p = rp + (i == len - 1 ? v : flist[ii].v) * 0.05f;
                                }
                            }
                        }
                    }
                
                    if(rand() % 30 == 0)
                    {
                        if((i == len - 1 && rand() % 5 == 0 ||
                            (season == 1 && rand() % 25 == 0 || season == 4 && rand() % 5 == 0)) ||
                            tree.dat.drug > 0 && tree.dat.drugind < i && rand() % 2 == 0)
                        {
                            if (flowermap.find(tree.dat.id) == flowermap.end())
                            {
                                flowermap.insert(std::map<int, std::vector<flower_t> >::value_type(tree.dat.id, std::vector<flower_t>()));
                            }
                            std::vector<flower_t>& flist = flowermap[tree.dat.id];
                            
                            flower_t fr;
                            fr.v = (e1[(i + index) % e1.size()].p - rp).normcopy();
                            if(year == 1 && season == 1)
                                fr.p = rp + (i == len - 1 ? v : fr.v) * 0.05f;
                            else
                                fr.p = rp + vec::rndrad() * 0.15 + v * rrnd(0, 0.045);
                            fr.sz = rrnd(0.35, 0.55) * blend(1, 0.65, ai);
                            //fr.v = (e1[(i + index) % e1.size()].p - e1[(i + index + e1.size() / 2) % e1.size()].p).normcopy();
                            
                            tree.dat.cor = blendcor(tree.dat.cor, 0xff000080, 0.001 * rndmap[i % 1024][8]);
                            fr.cor = season == 1 ? tree.dat.cor : blendcor(tree.dat.cor, 0xFFFFFFFF, 0.9);
                            int type = emstate[0];
                            if(type != 0)
                            {//custom color
                                
                                if(type == 1)
                                    curflowercor =  RGB(0, rrnd(0, 200), 255);
                                else if(type == 2)
                                    curflowercor =  RGB(rrnd(50, 150), rrnd(30, 50), rrnd(155, 255));
                                else if(type == 3)
                                    curflowercor =  RGB(255, rrnd(150, 255), 0);
                                else
                                    curflowercor =  RGB(rrnd(150, 255), rrnd(0, 255), rrnd(0, 255));
                                
                                if(curflowercor != 0)
                                {
                                    fr.cor = curflowercor;
                                }
                            }
                            fr.i = i;
                            fr.type = (year == 1 && season == 1 || i < len * 0.6 && rand() % 3 == 0) ? 1 : 0;
         
                            
							fr.bloom = 0;// blend(10, 0, ai);
                            fr.bloomtime = blend(15, 100, ai);
                            flist.push_back(fr);
                        }
                        if (tree.dat.drug > 0 && rand() % 150 == 0)
                            tree.dat.drug--;
                    }
                }
			}
            
            starttri = (int)gsubmesh.tris.size();
			// new brunches
			if (g_lastbruchescnt < 8000 &&
                (picktype == 3 || emstate[1] != 1 || ai > 0.6) // song
                && (season <= 2 || renderfrm0 > 50) && (picktype == 3 || !blastpicked) && renderfrm > 0 && (picktype == 3 || i > 2) && (i < len || picktype == 3) && depth < maxdepth)
			{//
				if (tree.dat.health > 0 && (picktype == 3 || tree.dat.power > 0 || tree.dat.drug > 0 && tree.dat.drugind < i) && sz > 0.01)
				{
					if (picktype == 3 && tree.cuttingpos >= 0 && starttri + treeedges >= tree.cuttingpos && starttri - treeedges < tree.cuttingpos // 嫁接
						// 初始长得快
						|| depth == 0 && tree.dat.len / (real)len0 < 0.35 &&
                            (rand() % 80 == 0 ||
							tree.dat.len / (real)len0 > 0.125 && tree.children.size() < 2)
						// 分叉
						|| (season == 1 || season == 2) && i == len - 3 && (depth == 0 && rand() % 150 == 0 || depth > 0 && rand() % 250 == 0)
						// 药物
						|| tree.dat.drug > 0 && i > tree.dat.drugind &&
                                (depth == 0 && (rand() % 12000 == 0) ||
                                 depth == 1 && ai > 0.35 && rand() % 500 == 0 ||
                                 season == 1 && ai > 0.35 && (depth > 1 && depth < 3 && rand() % 250 == 0 || depth >= 3 && rand() % 150 == 0) ||
                                 season == 2 && ai > 0.35 && depth > 1 && (i / (real)len0) > 0.5 && sz < 0.025 && rand() % 500 == 0)
                        // 第一年繁茂点
                        || year == 1 && season <= 2 && depth > 0 && sz < 0.025 && rand() % 80000 == 0
						)
					{
						bool bfind = false;
						{
							for (int ii = 0; ii < tree.children.size(); ii++)
							{
								if (i == tree.children[ii]->dat.i)// && (season == 1 || angi == tree.children[ii]->dat.angi))
								{
									bfind = true;
									break;
								}
							}
						}
						if (!bfind)
						{
							tree_t* nt = new tree_t();
							nt->parent = &tree;
							nt->dat.power = 1;
							tree.dat.power -= nt->dat.power;

							nt->dat.p0 = nt->dat.p = rp;
							vec nv = (e1[rand() % e1.size()] - rp);
							nv.norm();
                            //static real s_angf  = 0;
                            //s_angf += rrnd(PI / 2, PI);
                            //if(i % 2 == 0)
							nv.rot(rrnd(0.2, 1.8) * PI, v);
                            //else
                            //    nv.rot(PI + rrnd(-PI / 4, PI / 4), v);

							nt->dat.a = blend(vec::UY, -nv, rrnd(0.5, 1)) * rrnd(0.0, 0.05);
							nt->dat.v = blend(nv, v, blend(0.2, 0.6, rndmap[index][11 + i]));
							if(rand() % 5 == 0)
                            {
                                nt->dat.v = blend(nt->dat.v, vec::UY, rrnd(0.5, 0.8));
                            }
                            
                            nt->dat.maxsz = sz * (year < 5 ? 1 : 0.65) * blend(0.5, 0.85, rndmap[index + i][8 + i]);
							nt->dat.i = i;
							
							nt->dat.index = getglobleind();

							nt->dat.id = g_brunchesidcnt++;

							bool fencha = depth < 2 && i == len - 3;
							if (fencha)
							{
								nt->dat.bfencha = fencha;
                                if(year < 5)
                                    nt->dat.maxsz = sz;
							}
                            
                            real tlen = MIN(len0 / 2, len);
							if (i > len0 * 0.75)
							{
								nt->dat.maxlen = blend(0.75, 2, rndmap[i][index + 59]) * tlen;
								nt->dat.len0 = blend(0.65, 1.5, rndmap[i][index + 59]) * len0;
								if (depth <= 1)
									nt->dat.maxlen = MIN(nt->dat.maxlen, nt->dat.len0 * 1.5);
							}
							else if (depth == 0 && i < len0 * 0.3 && i > len0 * 0.05 && nt->dat.v.y > 0)
							{
								nt->dat.maxlen = blend(0.75, 2, rndmap[i][index + 59]) * tlen;
								nt->dat.len0 = blend(0.65, 1.5, rndmap[i][index + 59]) * len0;
								//nt->dat.maxlen = MIN(nt->dat.maxlen, nt->dat.len0);
							}
                            else if(picktype == 3 && tree.cuttingpos >= 0)
                            {// ya
                                nt->dat.maxlen = blend(0.5, 1.5, rndmap[i][index + 59]) * tlen;
                                nt->dat.len0 = blend(0.45, 0.8, rndmap[i][index + 59]) * len0;
                                nt->dat.maxlen = MIN(nt->dat.maxlen, nt->dat.len0);
                            }
							else if (fencha)
							{
								nt->dat.maxlen = blend(0.35, 0.8, rndmap[i][index + 59]) * tlen;
								nt->dat.len0 = blend(0.35, 0.75, rndmap[i][index + 59]) * len0;
								nt->dat.maxlen = MIN(nt->dat.maxlen, nt->dat.len0);
							}
							else if (season == 2 && depth > 1 && sz < 0.025)
							{// 树叶枝干
								nt->dat.maxlen = blend(0.2, 0.75, rndmap[i][index + 59]) * tlen;
								nt->dat.len0 = nt->dat.maxlen;
							}
							else
							{
								nt->dat.maxlen = (0.1 + ai) * blend(0.25, 0.5, rndmap[i][index + 59]) * tlen;
								nt->dat.len0 = (0.1 + ai) * blend(0.25, 0.5, rndmap[i][index + 59]) * len0;
							}
                            if(year == 1 && (depth == 0 || ai > 0.5))
                            {// 第一年长点
                                nt->dat.len0 *= 1.5;
                                nt->dat.maxlen *= 1.5;
                            }
                            nt->dat.len = MAX(1, nt->dat.len);
                            nt->dat.len0 = MAX(1, nt->dat.len0);
                            nt->dat.maxlen = MAX(1, nt->dat.maxlen);

                            nt->dat.cor = RGB(255, rrnd(50, 180), rrnd(120, 255));
							nt->dat.cor = blendcor(nt->dat.cor, tree.dat.cor, 0.5);

							nt->dat.lvcor = RGB(rrnd(25, 50), rrnd(25, 50), 0);
							nt->dat.lvcor = blendcor(nt->dat.lvcor, tree.dat.lvcor, 0.5);

							nt->dat.health = rrnd(50, 80);

							tree.children.push_back(nt);
							std::sort(tree.children.begin(), tree.children.end(), comp);

							if (tree.dat.drug > 0)
								tree.dat.drug--;

							if (picktype == 3 && tree.cuttingpos >= 0 && starttri + treeedges >= tree.cuttingpos && starttri - treeedges < tree.cuttingpos )
							{ // 嫁接
								tree.cuttingpos = -1;

                                nt->dat.p0 = nt->dat.p = rp;
                                nt->dat.v = (ghitpos - rp).normcopy();
                                real dot1 = nt->dat.v.dot(v);
                                if(dot1 > 0.6)
                                    nt->dat.v = depth == 0 ? blend(ghitdir, v, rrnd(0.1f, 0.2f)).normcopy() : blend(ghitdir, v, rrnd(0.25f, 0.4f)).normcopy();
                                
								nt->dat.ya = 3;
                                
                                // buding success
                                budpoint--;
                                budings ++;
							}
						}
					}
				}
			}
			// brunches
			bool bbrunch = 0;
			if (childrencnt < tree.children.size())
			{
				tree_t* nt = tree.children[childrencnt];
				if (nt->dat.i <= i)
				{
					if (!blastpicked && tree.dat.power > 0)
					{
						tree.children[childrencnt]->dat.power += blend(2, 30, ai, 2);
						tree.dat.power--;
					}

					// 生病传染子枝干
					if (tree.dat.health < 0)
						nt->dat.health = tree.dat.health;

					nt->dat.p = rp;
					if (!blastpicked &&
                        nt->dat.drug > 0 &&
                        (rand() % 1000 == 0))
                    {
                        real tmaxsz = MAX(nt->dat.maxsz, sz * blend(0.75, 0.95, rndmap[i][index]));
                        nt->dat.maxsz = blend(nt->dat.maxsz, tmaxsz, 0.1);
                        nt->dat.drug --;
                    }
					if (!blastpicked &&
                        (season == 1 && rand() % 1500 == 0 ||
                         season == 2 && rand() % 2500 == 0) ||
                        nt->dat.drug > 0 && rand() % 100 == 0 )
					{
						nt->dat.maxlen++;
						nt->dat.maxlen = MIN(nt->dat.maxlen, nt->dat.len0);
                        if(nt->dat.drug > 0)
                            nt->dat.drug --;
					}
					nt->parent = &tree;
					bool ret = false;
                    {
						if (tree.cuttingpos != -1 && (int)gsubmesh.tris.size() >= tree.cuttingpos && nt->cuttingpos == -1)
						{// 繼承
							nt->cuttingpos = -2;
						}
						if ((tree.cuttingpos == -1 || (int)gsubmesh.tris.size() < tree.cuttingpos) && nt->cuttingpos == -2)
						{// 重置
							nt->cuttingpos = -1;
						}

						ret = plum3D(e, nt->dat.p, nt->dat.v, nt->dat.maxsz,
							nt->dat.len0,
							depth + 1, maxdepth,
							*nt);
                        
                        if(nt->dat.cutindex != -1)
                        {
                            if(nt->cutdis > 0 && nt->cutdis < sz * 1.5)
                            {
                                ret = false;
                            }
                        }
						if (ret || blastpicked)
						{
							childrencnt++;
							bbrunch = 1;
							//if (depth > 0 && nt->dat.bfencha)
							//	v = blend(v, v * 2 - nt->dat.v, 0.5);
						}
						else
						{
							deletetree(nt);
							tree.children.erase(tree.children.begin() + childrencnt);
						}
						sz0 < 0.25 ? gcommonvertex = 1 : gcommonvertex = 0;
						gverindfindstart = gsubmesh.vertices.size();
					}
				}
			}
            
            real fstep = i > 1 ? fstep0 : fstep0 / 4 + 0.001;
            if(depth < 2)
            {
                fstep *= blend(1.25, 0.25, ai, 2);
            }
            moveedge(e1, v * fstep);
            rp += v * fstep;
            
            
			color = branches_color0;
			if (!blastpicked && tree.dat.health > 0 && season == 2)
			{// 树叶
                if(depth == 1 && i == 10)
                {// bird target
                    if(gtargets.size() < 10)
                    {
                        gtargets.push_back(rp);
                    }
                }
				if (depth > 0 && sz0 < 0.035 && i > len * 0.75 && rand() % int(blend(1, 10, leavecnt / 5000.0f, 2)) == 0)
				{
					if ((rand() % 2000 == 0 || year == 1 && rand() % 200 == 0) || tree.dat.drug > 0 && tree.dat.drugind < i && rand() % 20 == 0)
					{
						{
							if (leavemap.find(tree.dat.id) == leavemap.end())
							{
								leavemap.insert(std::map<int, std::vector<leaf_t> >::value_type(tree.dat.id, std::vector<leaf_t>()));
							}
							std::vector<leaf_t>& tleavelist = leavemap[tree.dat.id];
                            bool bfind = false;
                            if(year == 1)
                            {
                                for (int ii = 0; ii < tleavelist.size(); ii++)
                                {
                                    if (tleavelist[ii].i == i)
                                    {
                                        bfind = true;
                                        break;
                                    }
                                }
                            }
                            if (!bfind && tleavelist.size() < (len + 1) * 5)
							{
                                vec vv = i >= len - 1 ? v : (e1[(i + index) % e1.size()].p - e1[(i + index + e1.size() / 2) % e1.size()].p).normcopy();
                                vec nn = i >= len - 1 ? (e1[(i + index) % e1.size()].p - e1[(i + index + e1.size() / 2) % e1.size()].p).normcopy() : blend(v, vec::rndrad(), rrnd(0.2, 0.7));
								vec lp = rp + vec::rndrad() * 0.4 + vv * rrnd(0, 0.075);

                                leaf_t lf(lp, rp + vv * sz, vv, nn, blend(0.3, 0.6, rndmap[i][index]), i, blendcor(0xFF003000, tree.dat.lvcor, rrnd(0, 0.5)), days - rrnd(0, (seasonday[season] * 0.1)));
                                lf.type = emstate[1];
                                tleavelist.push_back(lf);
                                if (tree.dat.drug > 0 && rand() % 250 == 0)
                                    tree.dat.drug--;
							}
							
						}
					}
				}
            }
            static int lstyearfruitcnt = 0;
            if ((season == 3 || lstyearfruitcnt > 0) && depth > 0 && sz0 < 0.035)
            {
                static int fruit_type_cnt[3] = {0};
                if(lstyearfruitcnt > 0 && season != 3)
                {
                    memset(fruit_type_cnt, 0, sizeof(fruit_type_cnt));
                    lstyearfruitcnt = 0;
                }
                if (season == 3 && rand() % 10000 == 0)
                {// 果实种类很多
                    static int s_fruityear_tab[3][6] = {
                        {8, 10, 15, 15, 15, 15},
                        {0, 8,  10, 15, 15, 15},
                        {0, 0,  1,  2,  3,  4},
                    };
                    bool bnumctrl = 1;
                    for(int ii = 0; ii < 3; ii ++)
                    {
                        if(fruit_type_cnt[ii] > s_fruityear_tab[ii][(year > 6 ? 6 : year) - 1])
                        {
                            bnumctrl = 0;
                        }
                    }
                    // 果子数量控制
                    if(bnumctrl)
                    {
                        int frutype = 1; // 默认加营养的果实
                        if (year >= 3 && depth >= maxdepth - 3 && rand() % 15 == 1 ||
                            year == 3 && emopoint == 0 && emstate[2] == 0 ||
                            year >= 4 && emopoint < 2
                            )
                        {// 恶魔果实
                            frutype = 3;
                        }
                        else if (year >= 2 && rand() % 3 == 1)
                        {// 出芽果实
                            frutype = 2;
                        }
                        
                        if (fruitmap.find(tree.dat.id) == fruitmap.end())
                        {
                            fruitmap.insert(std::map<int, std::vector<fruit_t> >::value_type(tree.dat.id, std::vector<fruit_t>()));
                        }
                        std::vector<fruit_t>& flist = fruitmap[tree.dat.id];
                        bool bfind = false;
                        for (int ii = 0; ii < flist.size(); ii++)
                        {
                            if (flist[ii].i == i)
                            {
                                bfind = true;
                                break;
                            }
                        }
                        if (!bfind)
                        {
                            fruit_t fr(rp, 0.5, frutype, i);
                            fr.i = i;
                            
                            flist.push_back(fr);
                            
                            fruit_type_cnt[frutype - 1] ++;
                            lstyearfruitcnt ++;
                        }
                    }
                }
            }
        }
    
		if(tree.dat.health < 0)
		{
			if (units.size() < 18)
			{// adding mushrooms
				if (i > 2 && i < len - 2 &&  rand() % 500 == 0)
				{
                    bool btaken = false;
                    for (int ii = 0; ii < units.size(); ii++)
                    {
                        if(units[ii].id == tree.dat.id && units[ii].growpos == i)
                        {
                            btaken = true;
                            break;
                        }
                    }
                    if(!btaken)
                    {
                        int findind = -1;
                        for (int ii = 0; ii < units.size(); ii++)
                        {
                            if (!units[ii].bactive)
                            {
                                findind = ii;
                                break;
                            }
                        }

                        // find a pos upside
                        int eind = 0;
                        for (int ii = 0; ii < units.size(); ii++)
                        {
                            if (e[ii].p.y > rp.y)
                            {
                                eind = ii;
                                break;
                            }
                        }
                        if (findind != -1)
                        {
                            units[findind].o = e[eind].p;
                            units[findind].bactive = true;
                            units[findind].id = tree.dat.id;
                            units[findind].growpos = i;
                        }
                        else
                        {
                            unit u;
                            u.maxlevel = 1;
                            if(year >= 6 && depth == 0)
                            {
                                u.maxlevel = 6;
                            }
                            else
                            {
                                u.maxlevel = year;
                            }
                            u.o = e[eind].p;
                            u.id = tree.dat.id;
                            units.push_back(u);
                        }
                    }
				}
			}
			
			if (picktype == 0)
				color = tree.dat.lvcor = branches_color0 = 0xFF000000;

			if (tree.dat.health < 0 &&!blastpicked)
			{
				if (tree.dat.health < -100 && depth > 0 || tree.dat.health < -20 && depth == 0 ||
                    tree.parent && (getedgecenter(e1) - o).len() < 1.1f * tree.parent->dat.sz0 * blend(1, 1.0f / 4, (real)tree.dat.i / tree.parent->dat.len, 2))
				{// 枝干死亡
					return false;
				}
				if (tree.parent && rand() % 100 == 0)
				{// 衰老向上传递
					if (tree.parent->children.size() > 0)
					{
						tree.parent->children[rand() % tree.parent->children.size()]->dat.health-= 1;
					}
                    if(rand() % 20 == 0)
                        tree.parent->dat.health--;
				}
			}
		}
		if (depth > 0 || i != 0)
		{
            if (depth < 1)
				face8(e, e1, branches_color0, tree.dat.health);
			else
				face(e, e1);
			//gverindfindstart = gsubmesh.vertices.size();
		}
		e = e1;
		p = np;
	}
	if (tree.dat.ya > 0)
		color = 0xFF0000FF;
    
    if (tree.dat.cutpos != -1)
        tree.dat.cutpos = gsubmesh.tris.size();
    
	if (depth < 3 || sz0 > 0.1 || tree.dat.ya > 0)
    {
        vec oo = getedgecenter(e) ;
        if(tree.dat.ya > 0)
        {
            if(depth == 1)
                face(e, oo + v * (tree.parent->dat.sz0 * blend(1, 1 / 4, tree.parent->dat.i / tree.parent->dat.len, 2) + 0.1));
            else
                face(e, oo + v * (sz0 + 0.1));
        }
        else
            face(e, oo + v * (sz0 / 2));
    }

	tree.dat.endtri = gsubmesh.tris.size();
	
	//depth <= 1 ? gcommonvertex = 1 : gcommonvertex = 0;
	return true;
}

// --------------------------------------------------------
// 盆栽 - 月季(rose)
// --------------------------------------------------------
int rose_len1 = 12;
int rose_len2 = 8;
real rose_ang = PI / 2;
void rose_flower(vec o, vec v, vec n, real r0, real w ,real ang0, int cor1)
{
	gcommonvertex = 1;
	binvnorm = true;
	r0 *= rrnd(0.1, 0.4);
	real vvf = rrnd(0.1, 0.15);
	vec ta = v.cross(n); ta.norm();
	real h = w * 2;
	vec lp[100]; // 足够多
	int len = 8;
	int len2 = rose_len2 % 100;
	for (int i = 0; i < len; i++)
	{
		real ai = i / real(len);
		real ang = blend(0, PI / 2, ai) + ang0;
		real r = r0;
		vec vr = (n * cos(ang) + ta * sin(ang));
		vec p = o + vr * r;
		vec vv = blend(vr, v, vvf + rrnd(-0.05, 0.05));

		real h1 = blend2(h * 0.5, h * 0.75, ai, 2);		
		for (int ii = 0; ii < len2; ii++)
		{
			real aii = ii / real(len2);
			vec tta = vv.cross(vr).normcopy();
			vr = tta.cross(vv); vr.norm();
			vv.rot(blend(-PI / 3.8, rose_ang, aii, 0.75), tta);

			vec up = p + vv * blend(h1, h1 / 8, aii, 0.5);
			if (i > 0)
			{
				color = blendcor(0xFF000020, cor1, ai);
				plane(lp[ii], p, up, lp[ii + 1]);
			}
			lp[ii] = p;
			if(ii == len2 - 1)
				lp[ii + 1] = up;
			p = up;			
		}
	}
	binvnorm = false;
	gcommonvertex = 0;
}

void re_leave(vec o, vec v, vec n, real len, real w)
{
	real t = blend(0.1, 1, gtimealpha);
	color = blendcor(RGB(10, 50, 0), 0xFF00A000, rrnd(0, 1));
	gcommonvertex = 1;
	vec vside = v.cross(n); vside.norm();
	vec p1 = o - vside * (w / 32);
	vec p2 = o + vside * (w / 32);
	real h = w / 200;
	real tk = h / 5;
	VECLIST e;
	e.push_back(o - vside * (w / 180));
	e.push_back(p1 + n * h);
	e.push_back(o - n * tk);
	e.push_back(p2 + n * h);
	e.push_back(o + vside * (w / 180));
	closeedge(e);

	VECLIST le = e;
	real step = w / 4;
	int cnt = 6;
	for (int i = 0; i < cnt; i++)
	{
		real ai = i / real(cnt);
		static real scales[] = { 1, 4, 2, 1.0, 0.5, 0.1 };
		extrudeedge(e, step * 0.75);
		scaleedge(e, scales[i]);
		rotedge(e, rrnd(-PI / 16, PI / 8), vside);
		{
			int cor0 = color;
			for (int ii = 0; ii < le.size() - 1; ii++)
			{
				plane(le[ii].p, le[ii + 1].p, e[ii + 1].p, e[ii].p);
			}
			color = cor0;
		}
		le = e;
	}
	gcommonvertex = 0;
}
int rose_cor;
void rose(vec o, vec v0, real sz, VECLIST e, int depth)
{
	int cor1 = rose_cor;
	color = 1;
	vec p = o;
	real s = rrnd(3, 4) * USIZE / 2.5 * sz;
	real dpdeta = rrnd(0, 172.862);
	real y0 = rrnd(0, 100);
	int len = depth == 0 ? (18 + parm_flowerrad) : 8;

	for (int i = 0; i <= len; i++)
	{
		real ai = real(i) / (len);
		vec np;
		{
			vec dp = vec::UX.rotcopy(rrnd(-PI, PI), v0);
			np = p + (v0 + dp / 5) * (USIZE * blend(1, 3, ai, 4));
		}
		VECLIST e1;
		if (i == 0 && e.size() == 0)
		{// 底部
			color = 0xFF204020;
			roundedge(e1, p, vec::UY, s, 3);
		}
		else if (i <= len)
		{// 身体
			color = 0xFF204020;
			real dlen = (np - p).len();
			vec v = (np - p).normcopy();
			if(i % 2 == 0)
			{				
				moveedge(e, v * (dlen), e1);
				scaleedge(e1, 0.95);
				rotedge(e1, rrnd(0, PI / 8), getedgenorm(e1));
			}
			else
			{
				vec oo = getedgecenter(e);
				real ss = 2 * (oo - e[0]).len();
				moveedge(e, v * (ss), e1);

				if (i > len * 0.1 && i < len * 0.75 && rrnd() < 0.5 && depth < 2)
				{// leave
					vec pp = getedgecenter(e1);
					real s2 = rrnd(0.5, 1) * USIZE;
					re_leave(pp, blend(v, vec::UX, 0.5).rotcopy(rrnd(-PI, PI), v), vec::UY, s2 * 9, s2 * 5);
					color = 0xFFFFFFFF;
				}
				if(i > len * 0.1 && i < len * 0.9 && rrnd() < 0.1 && depth < 2)
				{// 分支
					VECLIST ee;
					unionedge(e, e1, rand() % 3, 2, ee); closeedge(ee);
					scaleedgeex(ee, 0.8);
					rose(getedgecenter(ee), blend(v, getedgenorm(ee), rrnd(0.4, 0.6)), sz * rrnd(0.4, 0.8), ee, depth + 1);
				}
			}
			if (i == len && (depth == 0 || rrnd() < 0.1))
			{				
				vec pp = getedgecenter(e1);
				vec vv = e1[0] - pp; vv.norm();
				real ss = rrnd(8, 12) * sz;
				int len2 = rose_len1;
				for (int ii = 0; ii < len2; ii++)
				{
                    real tang = ii % 5;
                    tang *= 2 * PI / 5;
					rose_flower(pp, v, (vv + v * rrnd(-0.5, 0.35)).normcopy(), USIZE * ss, USIZE * ss, tang + rrnd(-PI, PI) / 15, cor1);
				}				
			}
			color = 0xFF204020;
		}	

		if (depth > 0 || i != 0)
			face(e, e1);
		e = e1;
		p = np;
	}
	triang(e[0], e[1], e[2]);
}

// --------------------------------------------------------
// 山茶花叶子
// --------------------------------------------------------
void leafunit(VECLIST& e, real fw0, crvec v, vec up, real sz, int len, real fang, real fang2, real ang3, real pm1, real pm2, real pm3, real pm4)
{
	gcommonvertex = 1;
	vec forward = v;
	vec right = up.cross(forward).normcopy();
	up = forward.cross(right).normcopy();

	float w = fw0 / 2 * clamp(0.5, 4, 12, 5);
	VECLIST le = e;
	for (int ii = 0; ii <= len; ii++)
	{
		real aii = ii / real(len);
		if (g_leafunit_yemai)
			color = blendcor(RGB(100, 150, 100), gcurcor, 0.5 + 0.5 * sin((len / 2)* PI*aii));
		else
			color = blendcor(RGB(150, 180, 150), gcurcor, aii, 0.5);
		if (ii < len * 0.4)
			SETPARM(10);
		else if (ii > len * 0.6)
			SETPARM(11);
		else
			SETPARM(12);
		{
			real step[] = { 15 * USIZE, 15 * USIZE, 15 * USIZE, 15 * USIZE, 15 * USIZE };

			real fw = fw0 * (1 - aii) + w * blend(1, pm3, aii, pm1) * blend3(0.1, pm4, aii, 0.35, pm2); //blend(pm4, 0.5, aii, pm2);

			forward.rot(fang * (1 - aii), right);
			up.rot(fang2, forward);
			right = up.cross(forward).normcopy();

			real ang = blend(-ang3, ang3, aii);
			vec left1 = right.rotcopy(-ang, up);
			vec right1 = right.rotcopy(ang, up);

			up = forward.cross(right).normcopy();

			vec p = e[2].p;
			e.clear();

			e.push_back(p + forward * step[0] - left1 * (fw)+v * (fw));
			e.push_back(p + forward * step[1] - left1 * (fw / 4) + v * (fw * lerp(0.8, -0.25, aii)) + up * (-fw / 8));
			e.push_back(p + forward * step[2]);
			e.push_back(p + forward * step[3] + right1 * (fw / 4) + v * (fw * lerp(0.8, -0.25, aii)) + up * (-fw / 8));
			e.push_back(p + forward * step[4] + right1 * (fw)+v * (fw));
		}
		{
			int color0 = color;
			for (int i = 0; i < int(le.size() - 1); i++)
			{
				color = color0;
				if (i == le.size() / 2)
					color = RGB(100, 150, 100);
				plane(le[i], le[i + 1], e[i + 1], e[i]);
			}
		}
		le = e;
	}
	gcommonvertex = 0;
}
void leafunit(vec o, vec v, real sz, int len, real fang, real fang2, real ang3, real pm1, real pm2, real pm3, real pm4)
{
	vec forward = v;
	vec up = vec::UY;
	vec right = -up.cross(forward).normcopy();
	up = forward.cross(right).normcopy();

	VECLIST e;
	float w = sz / 10;
	e.push_back(o - right * (w));
	e.push_back(o - right * (w / 4) + up * (w / 4));
	e.push_back(o);
	e.push_back(o + right * (w / 4) + up * (w / 4));
	e.push_back(o + right * w);

	leafunit(e, w, v, up, sz, len, fang, fang2, ang3, pm1, pm2, pm3, pm4);
}

// --------------------------------------------------------------------
void shanchaleave(vec o, vec uy = vec::UY, real s = 0.1, int len0 = 20, int depth = 0)
{
	vec p = o;
	VECLIST e;
	color = 0xff101010;
	roundedge(e, o - vec::UY * (s), uy, s, depth == 0 ? 5 : 3, 0);
	bool lstgcommonvertex = gcommonvertex;
	gcommonvertex = 1;
	int cnt = 0;
	int cnt1 = 0;
	real dpdeta = rrnd(0, 172.862);
	real dpdeta2 = rrnd(0, 172.862);
	{// jing
		int len = depth == 0 ? clamp(3, 20, 10, 3) * 3 : len0;
		for (int i = 0; i < len; i++)
		{
			real ai = real(i) / (len);
			real t = blend(0.0, 1, ai);
			vec dp = vec::UX * ((perlin(p.x * 0.75, p.y * 0.05 + dpdeta, p.z * 0.3))) + vec::UZ * ((perlin(p.x * 0.2, p.y * 0.025 + dpdeta2, p.z * 0.5)));
			vec np = p;
			if (depth == 0)
				np += (uy + dp) * (USIZE * blend(50, 20, ai));
			else
				np += (uy + dp * 0.5) * (USIZE * 55);
			{
				color = blendcor(0xFF102000, 0xFFFFFFFF, ai, 2);
				SETPARM(10);
				VECLIST e1;
				{
					vec dv = np - p;
					moveedge(e, dv, e1);
					scaleedge(e1, 0.99);
				}
				face(e, e1);
				e = e1;
			}
			if (depth < 4 && i > 0.15 * len && ((depth == 0 && rand() % 5 == 0) || (depth == 1 && rand() % 10 == 0) || (depth == 2 && rand() % 30 == 0) || (depth == 3 && rand() % 25 == 0)))
			{
				vec pp = getedgecenter(e);
				if (depth == 0)
				{
					vec v = (np - p).normcopy();
					v = blend(v, vec::UX.rotcpy(cnt * PI / 2, vec::UY), blend(0.6, 0.25, (ai - 0.5) / 0.5));
					cnt++;
					//shanchaleave(pp, v, s * rrnd(0.4, 0.6), MIN(len, 10) / 2, depth + 1);
					shanchaleave(pp, v, s * rrnd(0.4, 0.6), MIN(len * rrnd(0.25, 1.5), 10), depth + 1);
				}
				else
				{
					for (int ii = 0; ii < 4; ii++)
					{
						vec v = (np - p).normcopy();
						v = blend(v, vec::UX.rotcpy(cnt * PI / 2, vec::UY), blend(0.6, 0.25, (ai - 0.5) / 0.5));
						cnt++;
						shanchaleave(pp, v, s * rrnd(0.4, 0.6), len * blend(1.0, 0.25, ai), depth + 1);
					}
				}
			}
			else if (i == len - 1)
			{
				vec v = (np - p).normcopy();
				real ang = rrnd(-PI, PI) / 20;
				vec pp = getedgecenter(e);
				for (int ii = 0; ii < 1; ii++)
				{
					v.rot(ang + PI * ii, uy);
					//g_leafunit_yemai = 1;
					leafunit(pp, v, 0.5, clamp(3, 30, 11, 2), rrnd(-0.1, 0.1), rrnd(-0.1, 0.1), rrnd(-0.1, 0.1),
						1.1, 2.02,
						1.15, 8);
					//g_leafunit_yemai = 0;
				}
			}
			p = np;
		}
	}
	gcommonvertex = lstgcommonvertex;
}
// -------------------------------------------------
// 苹果 apple
// -------------------------------------------------
void drawapple(vec3 p, vec3 v)
{	
	vec3 lgtdir(1, -1, 1); lgtdir.norm();
	v.norm();
	for(int i = 0; i < 500; i ++)
	{
		p = p + v * 0.000025;
		
		real r1 = 0.1 * blend2(0, 1, real(i) / 500, 4);
		real r2 = 0.04 * blend2(0, 1, real(i) / 500, 4);
		real rr = blend(r1, r2, real(i) / 500) * 0.25;
		
		vec3 lstipp1[1000];
		vec3 lstiipp1;
		vec3 norm0 = v.cross(vector3::UZ);norm0.norm();
		for(int ii = 0; ii < 1000; ii ++)
		{	
			vec3 norm = norm0;
			real ang1 = blend(0, 2 * PI, real(ii) / 1000);				
			quaternion q(ang1 < PI ? ang1 : ang1 - PI, v);
			real ang2 = blend(PI / 2 + PI / 2, PI / 114, real(i) / 500, 1);
			norm = v.rotcopy(ang2, ang1 < PI ? q * norm : q * (- norm));
			
			vec3 pp = p + norm * rr;
			
			int cor;
			{// color
				norm = -(lstipp1[ii] - pp).cross(lstiipp1 - pp);
				norm.norm();
				real lum = -lgtdir.dot(norm);	
				
				cor = blendcor(0xFF00FFFF, 0xFF0000FF, blend2d(0, 1, 0.5 + 0.5 * sin(pp.x * 8*PI*real(i) / 500), 0.5 + 0.5*sin(pp.y * 8*PI*real(ii) / 1000)));
				cor = blendcor(0xFF00FF00, cor, real(i) / 500, .25);
				cor = blendcor(1, cor, 0.5 + 0.5 * lum, 1);
				
				lstiipp1 = pp;
				lstipp1[ii] = pp;
			}
			if(i > 0 && ii > 0)
			{
				pixel(viewprj(pp), cor, 1, pp.z);
			}			
		}		
	}
}

// ------------------------------------------------
void appleleaf(vec3 p, vec3 v)
{
	real sz = rrnd(0.5, 1.2);
	
	vec3 lgtdir(1, -1, 1); lgtdir.norm();
	v.norm();
	vec3 g(0, -0.0001, 0);
	vec3 up = vec3::UY;
	real deta = rrnd(.5, 1);
	for(int i = 0; i < 500; i ++)
	{
		real ai = real(i) / 500;
		v = v + g;
		v.norm();
		p = p + v * (0.0002 * sz);
		vec3 norm0 = v.cross(up);norm0.norm();
		up = norm0.cross(v);
		real alpha = blend2d(0.8, 1.2, p.x, p.y);
		vec3 p1 = p + (up + norm0) * blend2(0, 0.02 * sz * alpha, ai, 2);
		vec3 p2 = p + (up - norm0) * blend2(0, 0.02 * sz * alpha, ai, 2);
		
		for(int ii = 0; ii < 250; ii += 1)
		{
			real aii = real(ii) / 250;								
			{
				vec3 pp = blend(p, p1, aii);				
				
				vec3 norm = (up + norm0);
				norm.norm();
				//real lum = -lgtdir.dot(norm);			
		
				int cor = blendcor(RGB(116, 198, 98), RGB(17, 48, 17), 0.5 + 0.5 * sin(alpha * 80 * (0.75 * ai - 0.25 * aii)));	
				if(norm.z > 0)
					cor = blendcor(cor, RGB(27, 48, 20), aii);
				else
					cor = blendcor(cor, RGB(177, 248, 140), aii);
				
				//cor = blendcor(1, cor, 0.5 - 0.5 * up.z, 1);					
				
				pixel(pp, cor);		
	
			}
			{
				vec3 pp = blend(p, p2, aii);		
				alpha = blend2d(0.8, 1.2, pp.x, pp.y);
				vec3 norm = (up - norm0);
				norm.norm();
				//real lum = -lgtdir.dot(norm);			

				int cor = blendcor(RGB(36,98, 58), RGB(17, 48, 17), 0.5 + 0.5 * sin(alpha * 80 * (0.75 * ai - 0.25 * aii)));	
				if(norm.z > 0)
					cor = blendcor(cor, RGB(27, 48, 20), aii);	
				else
					cor = blendcor(cor, RGB(177, 248, 140), aii);	

				// cor = blendcor(1, cor, 0.5 - 0.5 * norm.z, 1);

				pixel(pp, cor);
			 }					
		}
	}
}
