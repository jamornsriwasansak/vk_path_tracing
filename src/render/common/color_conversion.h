#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H
float3
hue(float H)
{
    half R = abs(H * 6 - 3) - 1;
    half G = 2 - abs(H * 6 - 2);
    half B = 2 - abs(H * 6 - 4);
    return saturate(half3(R, G, B));
}

half3
rgb_from_hsv(const half3 hsv)
{
    return half3(((hue(hsv.x) - 1) * hsv.y + 1) * hsv.z);
}

half3
color_from_uint(uint v)
{
    v ^= v >> 17;
    v *= 0xed5ad4bbU;
    v ^= v >> 11;
    v *= 0xac4c1b51U;
    v ^= v >> 15;
    v *= 0x31848babU;
    v ^= v >> 14;
    half h = half(v % 256) / 256.0h;
    half3 p = normalize(rgb_from_hsv(half3(h, 1.0h, 1.0h)));
    return p * 0.7h + 0.3h;
}

#endif
