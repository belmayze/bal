/*!
 * @file   balList.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * 双方向リスト
 * 末尾に dummy が入る実装です
 * Prev / Next のポインターは循環しています
 *                                                  dummy
 *     +------+------+     +------+------+     +------+------+
 * +-> | prev | next | <-> | prev | next | <-> | prev | next | <-+
 * |   +------+------+     +------+------+     +------+------+   |
 * +-------------------------------------------------------------+
 */
template <typename T>
class List
{
private:
    //! ノード
    class Node
    {
    public:
        Node() {}
        //! デストラクター
        ~Node() { if (isLinked()) { unlink(); destruct(); } }

        //! バッファー確保
        void alloc() { mpBuffer = bal::make_unique<uint8_t[]>(nullptr, sizeof(T)); }

        //! ダミー扱い
        void setDummy() { mpNext = this; mpPrev = this; }

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

        //! 前に追加します
        void linkPrev(Node* p_node)
        {
            BAL_ASSERT(!p_node->isLinked());

            // 前のノードの双方向リスト
            p_node->mpPrev = mpPrev;
            mpPrev->mpNext = p_node;
            // 自身のノードの双方向
            p_node->mpNext = this;
            mpPrev = p_node;
        }
        //! 次に追加します
        void linkNext(Node* p_node)
        {
            BAL_ASSERT(!p_node->isLinked());

            // 次のノードの双方向リスト
            p_node->mpNext = mpNext;
            mpNext->mpPrev = p_node;
            // 自身のノードの双方向
            p_node->mpPrev = this;
            mpNext = p_node;
        }

        //! リンクを解除します
        void unlink()
        {
            BAL_ASSERT(isLinked());
            mpNext->mpPrev = mpPrev;
            mpPrev->mpNext = mpNext;
            mpPrev = nullptr;
            mpNext = nullptr;
        }

        //! 次のノードを取得
        Node* getNext() { return mpNext; }

        //! 前のノードを取得
        Node* getPrev() { return mpPrev; }

        //! リンク済みかどうか
        bool isLinked() { return (mpNext != nullptr || mpPrev != nullptr); }

        //! データを取得します
        T& getData()
        {
            BAL_ASSERT(mpData);
            return *mpData;
        }

    private:
        std::unique_ptr<uint8_t[]> mpBuffer;
        T*    mpData    = nullptr;
        Node* mpNext    = nullptr;
        Node* mpPrev    = nullptr;
    };

public:
    //! const イテレーター
    class const_iterator
    {
    public:
        const_iterator(Node* p_node) :mpNode(p_node) {}

        const T& operator*() const
        {
            return mpNode->getData();
        }
        const T* operator->() const
        {
            return &mpNode->getData();
        }

        const_iterator& operator++()
        {
            mpNode = mpNode->getNext();
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator it = *this;
            ++*this;
            return it;
        }
        const_iterator& operator--()
        {
            mpNode = mpNode->getPrev();
            return *this;
        }
        const_iterator operator--(int)
        {
            const_iterator it = *this;
            --*this;
            return it;
        }

        bool operator==(const const_iterator& rhs) const
        {
            return (mpNode == rhs.mpNode);
        }
        bool operator!=(const const_iterator& rhs) const
        {
            return (mpNode != rhs.mpNode);
        }

    private:
        Node* mpNode = nullptr;
    };
    //! イテレーター
    class iterator
    {
    public:
        iterator(Node* p_node) : mpNode(p_node) {}

        T& operator*() const
        {
            return mpNode->getData();
        }
        T* operator->() const
        {
            return &mpNode->getData();
        }

        iterator& operator++()
        {
            mpNode = mpNode->getNext();
            return *this;
        }
        iterator operator++(int)
        {
            iterator it = *this;
            ++* this;
            return it;
        }
        iterator& operator--()
        {
            mpNode = mpNode->getPrev();
            return *this;
        }
        iterator operator--(int)
        {
            iterator it = *this;
            --* this;
            return it;
        }

        bool operator==(const iterator& rhs) const
        {
            return (mpNode == rhs.mpNode);
        }
        bool operator!=(const iterator& rhs) const
        {
            return (mpNode != rhs.mpNode);
        }

    private:
        Node* mpNode = nullptr;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
    /*!
     * 初期化
     * @param[in] max_size 最大サイズ
     */
    void initialize(size_t max_size);

    /*!
     * 先頭に追加します
     * @param[in] t オブジェクト
     */
    void push_front(const T& t);
    void push_front(T&& t);

    /*!
     * 末端に追加します
     * @param[in] t オブジェクト
     */
    void push_back(const T& t);
    void push_back(T&& t);

    /*!
     * 先頭から削除します
     */
    void pop_front();

    /*!
     * 末端から削除します
     */
    void pop_back();

    /*!
     * 全要素削除
     */
    void clear();

    /*!
     * 先頭要素への参照を取得します
     */
    T& front();
    const T& front() const;

    /*!
     * 末端要素への参照を取得します
     */
    T& back();
    const T& back() const;

    //! begin
    iterator begin() { return iterator(mDummyNode.getNext()); }
    const_iterator begin() const { return const_iterator(mDummyNode.getNext()); }
    const_iterator cbegin() const { return begin(); }
    //! end
    iterator end() { return iterator(&mDummyNode); }
    const_iterator end() const { return const_iterator(&mDummyNode); }
    const_iterator cend() const { return end(); }
    //! rbegin
    reverse_iterator rbigin() { return reverse_iterator(&mDummyNode); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(&mDummyNode); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    //! rend
    reverse_iterator rend() { return reverse_iterator(mDummyNode.getNext()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(mDummyNode.getNext()); }
    const_reverse_iterator crend() const { return rend(); }

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
    Node                     mDummyNode;
    std::unique_ptr<Node[]>  mpNodeBuffer;
    std::unique_ptr<Node*[]> mpFreeList;
    size_t                   mMaxSize      = 0;
    size_t                   mFreeListSize = 0;
};

}

// inl
#include <container/balList_inl.h>
