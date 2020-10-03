/*!
 * @file   balGfxMeshContainer.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <engine/balEngine.h>
#include <engine/module/gfx/balGfxMeshContainer.h>
#include <graphics/balIGraphics.h>
// bal::d3d12
#include <graphics/d3d12/balMeshBufferD3D12.h>

namespace {

// 矩形
const bal::mod::gfx::MeshContainer::Vertex cQuadVertices[] =
{
    {{-1.f,  1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
    {{-1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
    {{ 1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
    {{ 1.f,  1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
};
const uint16_t cQuadIndices[] = {0, 1, 2, 0, 2, 3};

// キューブ
const bal::mod::gfx::MeshContainer::Vertex cCubeVertices[] =
{
    // +Z
    {{-1.f,  1.f,  1.f}, { 0.f,  0.f,  1.f}, {0.f, 0.f}}, // 0
    {{-1.f, -1.f,  1.f}, { 0.f,  0.f,  1.f}, {0.f, 1.f}},
    {{ 1.f, -1.f,  1.f}, { 0.f,  0.f,  1.f}, {1.f, 1.f}},
    {{ 1.f,  1.f,  1.f}, { 0.f,  0.f,  1.f}, {1.f, 0.f}},
    // +X
    {{ 1.f,  1.f,  1.f}, { 1.f,  0.f,  0.f}, {0.f, 0.f}}, // 4
    {{ 1.f, -1.f,  1.f}, { 1.f,  0.f,  0.f}, {0.f, 1.f}},
    {{ 1.f, -1.f, -1.f}, { 1.f,  0.f,  0.f}, {1.f, 1.f}},
    {{ 1.f,  1.f, -1.f}, { 1.f,  0.f,  0.f}, {1.f, 0.f}},
    // -Z
    {{ 1.f,  1.f, -1.f}, { 0.f,  0.f, -1.f}, {0.f, 0.f}}, // 8
    {{ 1.f, -1.f, -1.f}, { 0.f,  0.f, -1.f}, {0.f, 1.f}},
    {{-1.f, -1.f, -1.f}, { 0.f,  0.f, -1.f}, {1.f, 1.f}},
    {{-1.f,  1.f, -1.f}, { 0.f,  0.f, -1.f}, {1.f, 0.f}},
    // -X
    {{-1.f,  1.f, -1.f}, {-1.f,  0.f,  0.f}, {0.f, 0.f}}, // 12
    {{-1.f, -1.f, -1.f}, {-1.f,  0.f,  0.f}, {0.f, 1.f}},
    {{-1.f, -1.f,  1.f}, {-1.f,  0.f,  0.f}, {1.f, 1.f}},
    {{-1.f,  1.f,  1.f}, {-1.f,  0.f,  0.f}, {1.f, 0.f}},
    // +Y
    {{-1.f,  1.f, -1.f}, { 0.f,  1.f,  0.f}, {0.f, 0.f}}, // 16
    {{-1.f,  1.f,  1.f}, { 0.f,  1.f,  0.f}, {0.f, 1.f}},
    {{ 1.f,  1.f,  1.f}, { 0.f,  1.f,  0.f}, {1.f, 1.f}},
    {{ 1.f,  1.f, -1.f}, { 0.f,  1.f,  0.f}, {1.f, 0.f}},
    // -Y
    {{-1.f, -1.f, -1.f}, { 0.f, -1.f,  0.f}, {0.f, 0.f}}, // 20
    {{-1.f, -1.f,  1.f}, { 0.f, -1.f,  0.f}, {0.f, 1.f}},
    {{ 1.f, -1.f,  1.f}, { 0.f, -1.f,  0.f}, {1.f, 1.f}},
    {{ 1.f, -1.f, -1.f}, { 0.f, -1.f,  0.f}, {1.f, 0.f}}
};
const uint16_t cCubeIndices[] = 
{
    // +Z
    0, 1, 2, 0, 2, 3,
    // +X
    4, 5, 6, 4, 6, 7,
    // -Z
    8, 9, 10, 8, 10, 11,
    // -X
    12, 13, 14, 12, 14, 15,
    // +Y
    16, 17, 18, 16, 18, 19,
    // -Y
    20, 22, 21, 20, 23, 22
};

}

namespace bal::mod::gfx {

// ----------------------------------------------------------------------------

MeshContainer::MeshContainer() {}

// ----------------------------------------------------------------------------

MeshContainer::~MeshContainer() {}

// ----------------------------------------------------------------------------

void MeshContainer::initialize()
{
    // グラフィックスシステム
    IGraphics* p_graphics = Engine::GetInstance().getGraphicsSystem();

    // 計算で作るメッシュ
    // 球体（縦11分割 x 円周30分割）
    std::unique_ptr<Vertex[]>   sphere_vertices = make_unique<Vertex[]>(nullptr, 11 * 30);
    std::unique_ptr<uint16_t[]> sphere_indices  = make_unique<uint16_t[]>(nullptr, 10 * 30 * 6);
    {
        // 球体
        {
            // 0         5             29
            // ----------- ... --------- 0
            // |/|/|/|/|/| ... |/|/|/|/|
            // ----------- ... --------- 1
            // |/|/|/|/|/| ... |/|/|/|/|
            // ....
            // ----------- ... --------- 9
            // |/|/|/|/|/| ... |/|/|/|/|
            // ----------- ... --------- 10

            // 頂点バッファ
            for (uint32_t i_y = 0; i_y <= 10; ++i_y)
            {
                // -90～90 を 10 分割
                // ただし完全に半周する必要があるので11分割相当
                Degree y_degree = Degree((180.f / 10.f) * i_y - 90.f);
                float y = Math::Sin(y_degree);
                float s = Math::Cos(y_degree);

                for (uint32_t i_r = 0; i_r < 30; ++i_r)
                {
                    // 360°を30分割
                    Degree degree = Degree((360.f / 30.f) * i_r);
                    float x = Math::Cos(degree) * s;
                    float z = Math::Sin(degree) * s;

                    // 
                    uint32_t i_vertex = i_y * 30 + i_r;
                    sphere_vertices[i_vertex].mPosition = MathVector3(x, y, z);
                    sphere_vertices[i_vertex].mNormal   = sphere_vertices[i_vertex].mPosition.calcNormalize();
                    sphere_vertices[i_vertex].mTexcoord = MathVector2(static_cast<float>(i_r) / 29.f, static_cast<float>(i_y) / 10.f);
                }
            }
            // インデックスバッファ
            for (uint32_t i_y = 0; i_y < 10; ++i_y)
            {
                for (uint32_t i_r = 0; i_r < 30; ++i_r)
                {
                    uint16_t i0 =  i_y      * 30 +  i_r;;
                    uint16_t i1 = (i_y + 1) * 30 +  i_r;
                    uint16_t i2 = (i_y + 1) * 30 + ((i_r + 1) % 30); // 最後の面は最初とつなげるので余りを求めて一周させる
                    uint16_t i3 =  i_y      * 30 + ((i_r + 1) % 30);

                    uint32_t mesh_index = i_y * 30 + i_r;
                    sphere_indices[mesh_index * 6 + 0] = i0;
                    sphere_indices[mesh_index * 6 + 1] = i1;
                    sphere_indices[mesh_index * 6 + 2] = i2;
                    sphere_indices[mesh_index * 6 + 3] = i0;
                    sphere_indices[mesh_index * 6 + 4] = i2;
                    sphere_indices[mesh_index * 6 + 5] = i3;
                }
            }
        }
    }

    // バッファ確保
    mpBuffers = make_unique<std::unique_ptr<IMeshBuffer>[]>(nullptr, cTypeNum);

    // 矩形を初期化していくリスト
    using InitializeTuple = std::tuple<std::unique_ptr<IMeshBuffer>*, const Vertex*, size_t, const uint16_t*, size_t>;
    std::array initialize_list =
    {
        InitializeTuple(&mpBuffers[static_cast<int>(Type::Quad)],   cQuadVertices, sizeof(cQuadVertices), cQuadIndices, sizeof(cQuadIndices)),
        InitializeTuple(&mpBuffers[static_cast<int>(Type::Cube)],   cCubeVertices, sizeof(cCubeVertices), cCubeIndices, sizeof(cCubeIndices)),
        InitializeTuple(&mpBuffers[static_cast<int>(Type::Sphere)], sphere_vertices.get(), sizeof(Vertex) * 11 * 30, sphere_indices.get(), sizeof(uint16_t) * 10 * 30 * 6)
    };

    // 初期化
    for (InitializeTuple& list : initialize_list)
    {
        *std::get<0>(list) = make_unique<d3d12::MeshBuffer>(nullptr);

        IMeshBuffer::InitializeArg init_arg;
        init_arg.mpGraphics         = p_graphics;
        init_arg.mpVertexBuffer     = std::get<1>(list);
        init_arg.mVertexBufferSize  = std::get<2>(list);
        init_arg.mVertexStride      = sizeof(Vertex);
        init_arg.mpIndexBuffer      = std::get<3>(list);
        init_arg.mIndexBufferSize   = std::get<4>(list);
        init_arg.mIndexBufferFormat = IMeshBuffer::IndexBufferFormat::Uint16;
        init_arg.mIndexCount        = static_cast<uint32_t>(init_arg.mIndexBufferSize / sizeof(uint16_t));
        if (!(*std::get<0>(list))->initialize(init_arg)) { return; }
    }
}

// ----------------------------------------------------------------------------

}
