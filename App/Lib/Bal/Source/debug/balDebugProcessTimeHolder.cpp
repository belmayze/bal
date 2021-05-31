/*!
 * @file   balDebugProcessTimeHolder.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
// bal
#include <debug/process/balDebugProcessTimeHolder.h>
#include <engine/balEngine.h>
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
                std::unique_ptr<IInputLayout::InputLayoutDesc[]> descs = make_unique<IInputLayout::InputLayoutDesc[]>(nullptr, 2);
                descs[0] = { .mName = "POSITION", .mType = IInputLayout::Type::Vec3, .mOffset = offsetof(Vertex, mPosition) };
                descs[1] = { .mName = "Color",    .mType = IInputLayout::Type::Vec4, .mOffset = offsetof(Vertex, mColor) };

                IInputLayout::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mNumInputLayout = 2;
                init_arg.mpInputLayouts = descs.get();
                if (!p_input_layout->initialize(init_arg)) { return; }
            }
            // パイプライン
            mpGfxPipeline = make_unique<Pipeline>(nullptr);
            {
                IPipeline::InitializeArg init_arg;
                init_arg.mpGraphics = p_graphics;
                init_arg.mNumOutput = 1;
                init_arg.mOutputFormats[0] = p_gfx_module->getDefaultRenderTarget().getTexture()->getFormat();
                init_arg.mpInputLayout = p_input_layout.get();

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
        p_thread_info->mpRootNode = mTreeMap.emplaceChild(mTreeMap.getRootNode(), name, p_thread_info);
        p_thread_info->mpCurrentNode = p_thread_info->mpRootNode;

        handle.mpNode = p_thread_info->mpRootNode;
    }
    return handle;
}
// ----------------------------------------------------------------------------
ProcessHandle ProcessTimeHolder::addNode(const StringPtr& name)
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
            p_thread_info->mpRootNode = mTreeMap.emplaceChild(mTreeMap.getRootNode(), "Thread", p_thread_info);
            p_thread_info->mpCurrentNode = p_thread_info->mpRootNode;
        }
    }

    // ロック
    {
        std::lock_guard lock(mMutexTreeMap);

        if (mTreeMap.capacity() > 0)
        {
            handle.mpNode = mTreeMap.emplaceChild(p_thread_info->mpCurrentNode, name, p_thread_info);
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

}
// ----------------------------------------------------------------------------
void ProcessTimeHolder::draw(const bal::FrameworkCallback::DrawArg& arg)
{
    arg.mpCommandList->bindPipeline(*mpGfxPipeline);
}
// ----------------------------------------------------------------------------

}
