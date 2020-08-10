/*!
 * @file   balShaderArchiver.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/archiver/balShaderArchiver.h>

namespace bal::gfx {

// ----------------------------------------------------------------------------

void ShaderArchiver::loadArchiver(File&& file)
{

}

// ----------------------------------------------------------------------------

uint32_t ShaderArchiver::findProgram(const StringPtr& name) const
{
    for (uint32_t i_program = 0; i_program < mNumProgram; ++i_program)
    {
        if (mNameTable[i_program].isEquals(name))
        {
            return i_program;
        }
    }
    return -1;
}

// ----------------------------------------------------------------------------

const ShaderArchiver::ShaderData& ShaderArchiver::getShaderData(uint32_t index) const
{
    return mShaderDataList[index];
}

// ----------------------------------------------------------------------------

}
