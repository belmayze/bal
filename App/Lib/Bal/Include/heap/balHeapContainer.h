/*!
 * @file   balHeapContainer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal

// ----------------------------------------------------------------------------
namespace bal::heap {

// --------------------------------------------------------
// Container
// +-----+-----------+-----+
// | Tag | Data .... | Tag |
// +-----+-----------+-----+
// --------------------------------------------------------
class Container
{
public:
    // 前方タグ
    struct ForwardTag
    {
        uint32_t mAllocSize; //!< 確保されたメモリー量 (先頭は開放フラグ)
    };
    // 後方タグ
    struct BackwardTag
    {
        uint32_t mAllSize = 0; //!< 管理領域も含めたメモリー量
    };

public:
    static constexpr size_t cForwardTagSize = sizeof(ForwardTag);
    static constexpr size_t cBackwardTagSize = sizeof(BackwardTag);
    static constexpr size_t cAllTagSize = cForwardTagSize + cBackwardTagSize;

public:
    /*!
     * コンテナとして確保します
     * @param[in] size 確保したサイズ（管理領域除く）
     */
    Container(uint32_t size);

    /*!
     * 開放済みフラグ
     */
    bool isFree() const;

    /*!
     * 使用フラグを立てる
     */
    void setFree(bool free);

    /*!
     * コンテナを分割して、先頭の方を返す
     */
    Container* split(uint32_t size);

    /*!
     * 前後を判断して、結合できる場合は結合します
     */
    void merge();

    /*!
     * データポインターを取得する
     */
    void* getData();

    /*!
     * サイズを取得する
     */
    uint32_t getSize() const;

    /*!
     * 管理領域も含めたサイズを取得する
     */
    uint32_t getAllSize() const;

    /*!
     * 次のコンテナを取得
     */
    Container* getNext();

    /*!
     * 前のコンテナを取得
     */
    Container* getPrev();

    /*!
     * データを特定の値で埋めます
     * @param[in] data 埋めたいデータ
     */
    void setFill(uint8_t data);
};

}
