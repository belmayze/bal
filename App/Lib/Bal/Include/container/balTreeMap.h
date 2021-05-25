﻿/*!
 * @file   balTreeMap.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {
    
/*!
 * ツリーマップ
 * +-------+        +------+        +----------+
 * | dummy | <----> | Root | <----> | Child[0] |
 * +-------+        +------+        +----------+
 *                                       |
 *                                  +----------+
 *                                  | Child[1] |
 *                                  +----------+
 */
template <typename T>
class TreeMap
{
public:
    //! ノード
    class Node
    {
        friend TreeMap<T>;
    public:
        //! コンストラクター
        Node() {}
        //! デストラクター
        ~Node() { if (isLinked()) { unlink(); destruct(); } }
        //! データを取得します
        T& getData()
        {
            BAL_ASSERT(mpData);
            return *mpData;
        }

    private:
        //! バッファー確保
        void alloc() { mpBuffer = bal::make_unique<uint8_t[]>(nullptr, sizeof(T)); }

        //! ダミー扱い
        void setDummy() { mpParent = this; mpChild = this; mpSibling = this; }

        //! コンストラクターを呼び出す
        template <class... Args>
        void construct(Args&&... args)
        {
            BAL_ASSERT(!isLinked());
            if (mpBuffer)
            {
                mpData = new (mpBuffer.get()) T(std::forward<Args>(args)...);
            }
        }

        //! デストラクターを呼び出す
        void destruct()
        {
            BAL_ASSERT(!isLinked());
            if (mpData)
            {
                mpData->~T();
                mpData = nullptr;
            }
        }

        //! 子の先頭に追加します
        void linkChild(Node* p_node)
        {
            BAL_ASSERT(!p_node->isLinked());

            p_node->mpSibling = mpChild;
            p_node->mpParent  = this;
            mpChild           = p_node;
        }
        //! きょうだいに追加します
        void linkSibling(Node* p_node)
        {
            BAL_ASSERT(!p_node->isLinked());

            p_node->mpParent  = mpParent;
            p_node->mpSibling = mpSibling;
            mpSibling         = p_node;
        }

        //! リンクを解除します
        void unlink()
        {
            BAL_ASSERT(isLinked());
            BAL_ASSERT(mpChild == nullptr);

            if (mpSibling)
            {
                // 兄を検索
                Node* p = mpParent->mpChild;
                if (p == this)
                {
                    // 自分だったら子を付け替え
                    mpParent->mpChild = mpSibling;
                }
                else
                {
                    while (p->mpSibling != this)
                    {
                        p = p->mpSibling;
                    }
                    // きょうだいから抜ける
                    p->mpSibling = mpSibling;
                }
                mpSibling = nullptr;
            }
            else
            {
                // 兄を検索
                Node* p = mpParent->mpChild;
                if (p == this)
                {
                    // 自分だったら子だったら完全に抜ける
                    mpParent->mpChild = nullptr;
                }
                else
                {
                    while (p->mpSibling != this)
                    {
                        p = p->mpSibling;
                    }
                    // きょうだいから抜ける
                    p->mpSibling = nullptr;
                }
            }
            // 親からも抜ける
            mpParent = nullptr;
        }

        //! 親のノードを取得
        Node* getParent() { return mpParent; }

        //! 子のノードを取得
        Node* getChild() { return mpChild; }

        //! きょうだいのノードを取得
        Node* getSibling() { return mpSibling; }

        //! リンク済みかどうか
        bool isLinked() { return (mpParent != nullptr || mpChild != nullptr || mpSibling != nullptr); }

        //! ダミーかどうか
        bool isDummy() { return (mpData == nullptr); }

    private:
        std::unique_ptr<uint8_t[]> mpBuffer;
        T*    mpData    = nullptr;
        Node* mpParent  = nullptr;
        Node* mpChild   = nullptr;
        Node* mpSibling = nullptr;
    };

public:
    //! const イテレーター

public:
    /*!
     * 初期化
     * @param[in] max_size 最大サイズ
     */
    void initialize(size_t max_size);

    /*!
     * 子に追加する
     * @note 親ノードの指定が無ければ先頭に追加します
     * @param[in] p_node 追加先の親ノード
     */
    Node* addChild(Node* p_node, const T& t) { return addChildImpl_(p_node, t); }
    Node* addChild(Node* p_node, T&& t) { return addChildImpl_(p_node, std::move(t)); }

    /*!
     * きょうだいに追加する
     * @param[in] p_node 追加先のきょうだいノード
     */
    Node* addSibling(Node* p_node, const T& t) { return addSiblingImpl_(p_node, t); }
    Node* addSibling(Node* p_node, T&& t) { return addSiblingImpl_(p_node, std::move(t)); }

    /*!
     * コンテナが空かどうかを判定する
     */
    bool empty() const { return (size() == 0); }

    /*!
     * 要素数を取得する
     */
    size_t size() const { return (max_size() - mFreeListSize); }

    /*! 
     * 格納可能な最大の要素数を取得する
     */
    size_t max_size() const { return mMaxSize; }

private:
    Node* addChildImpl_(Node* p_node, const T& t)
    {
        BAL_ASSERT(mFreeListSize > 0);

        Node* p = mpFreeList[--mFreeListSize];
        p->construct(t);

        (p_node ? p_node : &mDummyNode)->linkChild(p);
        return p;
    }
    Node* addChildImpl_(Node* p_node, T&& t)
    {
        BAL_ASSERT(mFreeListSize > 0);

        Node* p = mpFreeList[--mFreeListSize];
        p->construct(std::move(t));

        (p_node ? p_node : &mDummyNode)->linkChild(p);
        return p;
    }

    Node* addSiblingImpl_(Node* p_node, const T& t)
    {
        BAL_ASSERT(mFreeListSize > 0);
        BAL_ASSERT(p_node != nullptr);

        Node* p = mpFreeList[--mFreeListSize];
        p->construct(t);

        p_node->linkSibling(p);
        return p;
    }
    Node* addSiblingImpl_(Node* p_node, T&& t)
    {
        BAL_ASSERT(mFreeListSize > 0);
        BAL_ASSERT(p_node != nullptr);

        Node* p = mpFreeList[--mFreeListSize];
        p->construct(std::move(t));

        p_node->linkSibling(p);
        return p;
    }

private:
    Node                     mDummyNode;
    std::unique_ptr<Node[]>  mpNodeBuffer;
    std::unique_ptr<Node*[]> mpFreeList;
    size_t                   mMaxSize      = 0;
    size_t                   mFreeListSize = 0;
};

}

// inl
#include <container/balTreeMap_inl.h>
