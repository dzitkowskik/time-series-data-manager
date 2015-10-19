//
// Created by ghash on 19.10.15.
//

#include "file.h"
#include <cstring>

std::string File::GetPath() const { return _path; }

size_t File::GetSize() const
{
    std::ifstream stream(this->GetPath(), std::ios::binary);
    size_t size = (size_t) stream.seekg(0, std::ifstream::end).tellg();
    stream.seekg(0, std::ifstream::beg);
    return size;
}


bool File::Compare(const File &other) const
{
    auto sizeA = this->GetSize();
    auto sizeB = other.GetSize();

    if(sizeA != sizeB) return false;

    const size_t blockSize = 4096;
    size_t remaining = sizeA;
    std::ifstream inputA(this->GetPath(), std::ios::binary);
    std::ifstream inputB(other.GetPath(), std::ios::binary);


    while(remaining)
    {
        char bufferA[blockSize], bufferB[blockSize];
        size_t size = std::min(blockSize, remaining);

        inputA.read(bufferA, size);
        inputB.read(bufferB, size);

        if(0 != memcmp(bufferA, bufferB, size)) return false;

        remaining -= size;
    }

    return true;
}
