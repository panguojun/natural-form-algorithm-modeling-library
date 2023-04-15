void shape_alpha()
{
    topo E;
    vec2 V[8];
    cst2 C[8];
    grid1 S[8] = {1,8,18};
    E.walk([V, C, S](topo* to, int s){
        S[s].walk([V, C, to](real t){
            crvec2 p1 = V[to.start];
            crvec2 p2 = V[to.end];
            vec2 p = blend(p1, p2, t) * C[to.c];
            pixel(p);
        }
    })
}
// 插值函数
float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

// Cosine 插值函数
float cosine(float a, float b, float t) {
    float ft = t * M_PI;
    float f = (1 - cos(ft)) * 0.5;
    return lerp(a, b, f);
}

// 生成随机梯度向量
void gen_grad(std::vector<float>& gradient, int size) {
    for (int i = 0; i < size; i++) {
        float x = 2 * static_cast<float>(rand()) / RAND_MAX - 1;
        float y = 2 * static_cast<float>(rand()) / RAND_MAX - 1;
        gradient.push_back(x);
        gradient.push_back(y);
    }
}

// 计算两个向量的点积
float dotProduct(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y1 * y2;
}

// 计算梯度向量的索引
int grad_index(int x, int y, int width) {
    return y * width + x;
}

float perlin_noise(crvec2 p, float freq, float amp, std::vector<vec2>& grad, int w, int h) {
    float x0 = floor(p.x / freq) * freq;
    float x1 = x0 + freq;
    float y0 = floor(p.y / freq) * freq;
    float y1 = y0 + freq;
    float sx = (p.x - x0) / freq;
    float sy = (p.y - y0) / freq;
    int ix0 = int(x0) % w;
    int ix1 = int(x1) % w;
    int iy0 = int(y0) % h;
    int iy1 = int(y1) % h;
    float n0 = dot(grad[grad_index(ix0, iy0, w)], vec2(p.x - x0, p.y - y0));
    float n1 = dot(grad[grad_index(ix1, iy0, w)], vec2(p.x - x1, p.y - y0));
    float ix0Lerp = cosine(n0, n1, sx);
    n0 = dot(grad[grad_index(ix0, iy1, w)], vec2(p.x - x0, p.y - y1));
    n1 = dot(grad[grad_index(ix1, iy1, w)], vec2(p.x - x1, p.y - y1));
    float ix1Lerp = cosine(n0, n1, sx);
    return amp * cosine(ix0Lerp, ix1Lerp, sy);
}
// ----------------------------------
float rndmap(int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}
float bezier(float x, float c[4])
{
    float t = x;
    float u = 1.0 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    float res = uuu * c[0]; // 计算曲线上的点
    res += 3.0 * uu * t * c[1];
    res += 3.0 * u * tt * c[2];
    res += ttt * c[3];
    return res;
}
float* rnd_arr(int size, float minVal, float maxVal) 
{
    static float* arr = new float[size];
    for (int i = 0; i < size; i++) {
        arr[i] = rrnd(minVal, maxVal);
    }
    return arr;
}
void haidai(vec2 p, int len)
{
    real* s_t1 = rnd_arr(4); // 随机数数组

    for (int i = 0; i < len; i++) // 循环绘制每个点
    {
        real ai = real(i) / len; // 计算当前点在曲线上的位置
        p = p + vec2(bezier(ai * PI * 2 + gtime * 1.1, s_t1) * 0.002, 0.004); // 计算当前点的位置
        if (i % 8 < 6) // 每隔6个点绘制一个像素
        {
            pixel(p);
        }
        if (i % 20 == 0) // 每隔20个点绘制一个海带叶片
        {
            vec2 pp = p; // 复制当前点位置
            vec2 vv(rndmap2(0,i), 0.1 + rndmap(0,i)); vv.norm(); // 计算随机向量
            for (int ii = 0; ii < 12; ii++) // 循环绘制叶片的12个点
            {
                pp = pp + vv * 0.002; // 计算叶片的下一个点位置
                pixel(pp); // 绘制叶片的一个点
                vv = vv.rot(PI / 6); // 旋转随机向量
            }
        }
    }
}

void haidai0(vec2 p, int len)
{
    static float s_t1[4] = { rrnd(0, 1), rrnd(0, 1), rrnd(0, 1), rrnd(0, 1) }; // 随机数数组

    for (int i = 0; i < len; i++) // 循环绘制每个点
    {
        real ai = real(i) / len; // 计算当前点在曲线上的位置

        p = p + vec2(FT(ai * PI * 2 + gtime * 1.1, s_t1) * 0.002, 0.004); // 计算当前点的位置
        if (i % 8 < 6) // 每隔6个点绘制一个像素
        {
            pixelCamera(p);
        }
        if (i % 20 == 0) // 每隔20个点绘制一个海带叶片
        {
            vec2 pp = p; // 复制当前点位置
            vec2 vv(rndmap2[0][i], 0.1 + rndmap[0][i]); vv.norm(); // 计算随机向量
            for (int ii = 0; ii < 12; ii++) // 循环绘制叶片的12个点
            {
                pp = pp + vv * 0.002; // 计算叶片的下一个点位置
                pixelCamera(pp); // 绘制叶片的一个点
                vv = vv.rot(PI / 6); // 旋转随机向量
            }
        }
    }
}
