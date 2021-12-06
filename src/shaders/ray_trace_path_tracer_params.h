//#define DEBUG_RayTraceVisualizePrintClickedInfo
//#define DEBUG_RayTraceVisualizePrintChar4BufferSize 1000

#ifndef RT_VISUALIZE_CB_PARAMS
#define RT_VISUALIZE_CB_PARAMS

enum class RaytraceVisualizeModeEnum : uint32_t
{
    ModeInstanceId,
    ModeBaseInstanceId,
    ModeGeometryId,
    ModeTriangleId,
    ModeBaryCentricCoords,
    ModePosition,
    ModeGeometryNormal,
    ModeShadingNormal,
    ModeTextureCoords,
    ModeDepth,
    ModeDiffuseReflectance,
    ModeSpecularReflectance,
    ModeRoughness
};

struct RaytraceVisualizeCbParams
{
    float4x4 m_camera_inv_view;
    float4x4 m_camera_inv_proj;
    RaytraceVisualizeModeEnum m_mode;
    uint32_t _padding[3];
};

#ifdef __cplusplus
static_assert(sizeof(RaytraceVisualizeCbParams) % (sizeof(uint32_t) * 4) == 0);
#endif

#ifdef DEBUG_RayTraceVisualizePrintClickedInfo
struct RaytraceVisualizeDebugPrintCbParams
{
    uint3 m_selected_thread_id;
    uint m_flag;
};
#endif

#endif