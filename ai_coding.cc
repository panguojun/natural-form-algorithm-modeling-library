以下是一个绘制海带的例子：

void haidai(vec2 p, int len)
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
