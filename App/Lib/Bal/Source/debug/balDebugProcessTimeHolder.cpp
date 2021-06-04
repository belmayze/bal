/*!
 * @file   balDebugProcessTimeHolder.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// std
#include <functional>
// bal
#include <debug/process/balDebugProcessTimeHolder.h>
#include <engine/balEngine.h>
#include <engine/module/gfx/balGfxMeshContainer.h>
#include <engine/module/gfx/balGfxModule.h>
#include <graphics/archiver/balShaderArchive.h>

namespace bal::debug {

// ----------------------------------------------------------------------------
ProcessTimeHolder::ProcessTimeHolder()
{
}
// ----------------------------------------------------------------------------
ProcessTimeHolder::~ProcessTimeHolder()
{
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::initialize(const InitializeArg& arg)
{
    mTreeMap.initialize(arg.maxNode);
    mTreeMapInfo.initialize(arg.maxNode);
    mThreadInfoList.initialize(arg.maxThread);

    // gfx
    {
        // グラフィックス
        IGraphics* p_graphics = Engine::GetInstance().getGraphicsSystem();
        mod::gfx::Module* p_gfx_module = mod::gfx::Module::GetModule();

        if (p_gfx_module)
        {
            // 頂点レイアウト
            std::unique_ptr<InputLayout> p_input_layout = make_unique<InputLayout>(nullptr);
            {
                std::unique_ptr<IInputLayout::InputLayoutDesc[]> descs = make_unique<IInputLayout::InputLayoutDesc[]>(nullptr, 3);
                descs[0] = { .mName = "POSITION", .mType = IInputLayout::Type::Vec3, .mOffset = mod::gfx::MeshContainer::cOffsetPosition };
                descs[1] = { .mName = "NORMAL",   .mType = IInputLayout::Type::Vec3, .mOffset = mod::gfx::MeshContainer::cOffsetNormal };
                descs[2] = { .mName = "TEXCOORD", .mType = IInputLayout::Type::Vec2, .mOffset = mod::gfx::MeshContainer::cOffsetTexcoord };

                IInputLayout::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mNumInputLayout = 3;
                init_arg.mpInputLayouts = descs.get();
                if (!p_input_layout->initialize(init_arg)) { return; }
            }
            // 定数バッファ
            mpGfxConstantBuffer = make_unique<ConstantBuffer>(nullptr);
            {
                ConstantBuffer::InitializeArg init_arg;
                init_arg.mpGraphics   = p_graphics;
                init_arg.mBufferSize  = sizeof(ConstantBufferData) * arg.maxNode;
                init_arg.mBufferCount = 2;
                mpGfxConstantBuffer->initialize(init_arg);
            }
            // デスクリプターヒープ
            mpGfxDescriptorHeap = make_unique<DescriptorHeap>(nullptr);
            {
                const IConstantBuffer* p_constant_buffers[] = { mpGfxConstantBuffer.get() };
                IDescriptorHeap::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mConstantRangeBase = 0;
                init_arg.mNumConstantBuffer = 1;
                init_arg.mpConstantBuffers  = p_constant_buffers;
                if (!mpGfxDescriptorHeap->initialize(init_arg)) { return; }
            }
            // パイプライン
            mpGfxPipeline = make_unique<Pipeline>(nullptr);
            {
                const IDescriptorHeap* p_heaps[] = { mpGfxDescriptorHeap.get() };
                IPipeline::InitializeArg init_arg;
                init_arg.mpGraphics         = p_graphics;
                init_arg.mNumOutput         = 1;
                init_arg.mOutputFormats[0]  = p_graphics->getSwapChainColorFormat();
                init_arg.mNumDescriptorHeap = 1;
                init_arg.mpDescriptorHeaps  = p_heaps;
                init_arg.mpInputLayout      = p_input_layout.get();

                const ShaderArchive::ShaderContainer& shader_container = p_gfx_module->getShaderArchive().getShaderContainer(
                    p_gfx_module->getShaderArchive().findProgram("ProcessTimer")
                );
                init_arg.mpVertexShaderBuffer    = shader_container.mVertexShader.mBuffer;
                init_arg.mVertexShaderBufferSize = shader_container.mVertexShader.mBufferSize;
                init_arg.mpPixelShaderBuffer     = shader_container.mPixelShader.mBuffer;
                init_arg.mPixelShaderBufferSize  = shader_container.mPixelShader.mBufferSize;
                if (!mpGfxPipeline->initialize(init_arg)) { return; }
            }
        }
    }
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::clear()
{
    std::shared_lock lock(mMutexList);

    // クリアする前にコピー
    mTreeMapInfo.clear();
    for (const ProcessHandle::ThreadInfo& info : mThreadInfoList)
    {
        if (info.mpRootNode->getChild())
        {
            TreeMap<ProcessTimeInfo>::Node* p_node = mTreeMapInfo.emplaceChild(nullptr, info.mpRootNode->getData().mName, info.mpRootNode->getData().mDiff);
            
            TreeMap<ProcessHandle::Data>::Node* p_data_node = info.mpRootNode->getChild();
            p_node = mTreeMapInfo.emplaceChild(p_node, p_data_node->getData().mName, p_data_node->getData().mDiff, p_data_node->getData().mColor);

            std::function<void(TreeMap<ProcessTimeInfo>::Node*, TreeMap<ProcessHandle::Data>::Node*)> func = [this, &func](TreeMap<ProcessTimeInfo>::Node* p_info, TreeMap<ProcessHandle::Data>::Node* p_data)
            {
                if (p_data->getChild())
                {
                    TreeMap<ProcessHandle::Data>::Node* p_child_data = p_data->getChild();
                    TreeMap<ProcessTimeInfo>::Node*     p_child_info = mTreeMapInfo.emplaceChild(p_info, p_child_data->getData().mName, p_child_data->getData().mDiff, p_child_data->getData().mColor);
                    func(p_child_info, p_child_data);
                }

                TreeMap<ProcessHandle::Data>::Node* p_sibling_data = p_data->getSibling();
                TreeMap<ProcessTimeInfo>::Node*     p_sibling_info = p_info;
                while (p_sibling_data)
                {
                    p_sibling_info = mTreeMapInfo.emplaceSibling(p_sibling_info, p_sibling_data->getData().mName, p_sibling_data->getData().mDiff, p_sibling_data->getData().mColor);
                    p_sibling_data = p_sibling_data->getSibling();
                }
            };
            func(p_node, p_data_node);
        }
    }

    // クリア
    for (const ProcessHandle::ThreadInfo& info : mThreadInfoList)
    {
        ProcessHandle::TreeMapNode* p_node = info.mpRootNode->getChild();
        while (p_node != nullptr)
        {
            mTreeMap.remove(p_node);
            p_node = info.mpRootNode->getChild();
        }
    }
}
// ----------------------------------------------------------------------------
ProcessHandle ProcessTimeHolder::addThread(const StringPtr& name)
{
    // ツリー構造に登録
    ProcessHandle handle;

    {
        // すでにツリーがあれば削除する
        std::lock_guard lock1(mMutexList);
        for (auto it = mThreadInfoList.begin(); it != mThreadInfoList.end(); ++it)
        {
            if (it->mThreadID == std::this_thread::get_id())
            {
                mThreadInfoList.erase(it);
                break;
            }
        }

        // 再登録する
        ProcessHandle::ThreadInfo* p_thread_info = &mThreadInfoList.emplaceBack(std::this_thread::get_id());

        std::lock_guard lock2(mMutexTreeMap);
        p_thread_info->mpRootNode = mTreeMap.emplaceChild(mTreeMap.getRootNode(), name, MathColor(1.f, 1.f, 1.f), p_thread_info);
        p_thread_info->mpCurrentNode = p_thread_info->mpRootNode;

        handle.mpNode = p_thread_info->mpRootNode;
    }
    return handle;
}
// ----------------------------------------------------------------------------
ProcessHandle ProcessTimeHolder::addNode(const StringPtr& name, const MathColor& color)
{
    // ツリー構造に登録
    ProcessHandle handle;

    // 現在のスレッド情報取得
    ProcessHandle::ThreadInfo* p_thread_info = nullptr;
    {
        std::shared_lock lock(mMutexList);
        for (ProcessHandle::ThreadInfo& info : mThreadInfoList)
        {
            if (info.mThreadID == std::this_thread::get_id())
            {
                p_thread_info = &info;
                break;
            }
        }
    }
    if (!p_thread_info)
    {
        std::lock_guard lock1(mMutexList);
        // 再検索
        for (ProcessHandle::ThreadInfo& info : mThreadInfoList)
        {
            if (info.mThreadID == std::this_thread::get_id())
            {
                p_thread_info = &info;
                break;
            }
        }
        if (!p_thread_info)
        {
            p_thread_info = &mThreadInfoList.emplaceBack(std::this_thread::get_id());

            std::lock_guard lock2(mMutexTreeMap);
            p_thread_info->mpRootNode = mTreeMap.emplaceChild(mTreeMap.getRootNode(), "Thread", MathColor(1.f, 1.f, 1.f), p_thread_info);
            p_thread_info->mpCurrentNode = p_thread_info->mpRootNode;
        }
    }

    // ロック
    {
        std::lock_guard lock(mMutexTreeMap);

        if (mTreeMap.capacity() > 0)
        {
            handle.mpNode = mTreeMap.emplaceChild(p_thread_info->mpCurrentNode, name, color, p_thread_info);
            p_thread_info->mpCurrentNode = handle.mpNode;
        }
    }
    return handle;
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::end(const ProcessHandle& handle, const TimeSpan& diff)
{
    if (handle.mpNode)
    {
        handle.mpNode->getData().mDiff = diff;
        handle.mpNode->getData().mpThreadInfo->mpCurrentNode = handle.mpNode->getData().mpThreadInfo->mpCurrentNode->getParent();
    }
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::update(const FrameworkCallback::UpdateArg& arg)
{
    // 各バーの情報を入れる
    mpGfxConstantBuffer->swapBuffer();
    {
        // ルート直下のバーだけ
        mNumDraw = 0;
        MathVector2 orign;
        ConstantBufferData* p_cbs = mpGfxConstantBuffer->getBufferPtr<ConstantBufferData>();

        auto p_root_node = mTreeMapInfo.getRootNode();
        while (p_root_node != nullptr)
        {
            // スレッド単位
            orign.x() = 0.f;

            // とりあえずトップレベルのみ描画
            auto p_node = p_root_node->getChild();
            while (p_node != nullptr)
            {
                // 横幅 100% = 16.66 ms として換算
                float scale = static_cast<float>(p_node->getData().mDiff.getMicroseconds()) / 16666.f;

                p_cbs[mNumDraw].mPosition = MathVector2(orign);
                p_cbs[mNumDraw].mSize = MathVector2(scale, 0.02f);
                p_cbs[mNumDraw].mColor = p_node->getData().mColor;
                orign.x() += scale * 2.f;

                p_node = p_node->getSibling();
                ++mNumDraw;
            }

            orign.y() += 0.02f * 2.f;
            p_root_node = p_root_node->getSibling();
        }
    }
}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::draw(const bal::FrameworkCallback::DrawArg& arg)
{
    arg.mpCommandList->bindPipeline(*mpGfxPipeline);
    arg.mpCommandList->setDescriptorHeap(*mpGfxDescriptorHeap);
    arg.mpCommandList->setDescriptorTable(0, *mpGfxDescriptorHeap);
    arg.mpCommandList->drawMesh(*mod::gfx::MeshContainer::GetInstance().getBuffer(mod::gfx::MeshContainer::Type::Quad), mNumDraw);
}
// ----------------------------------------------------------------------------

}
