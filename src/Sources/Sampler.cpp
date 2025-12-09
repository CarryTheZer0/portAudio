/*
 *  Sampler.cpp
 *
 *  Created on: 08 Dec 2025
 *      Author: mchlp
 */

#include <cmath>

#include "Sampler.h"

#include "Utility/WavReader.h"

void Sampler::loadSample(const char* path)
{
    int blockSize = 4096;

    FILE* file = fopen(path, "rb");
    RIFFHeader header = readHeader(file);
    ChunkInfo formatInfo = readFormatInfo(file);
    FormatChunk formatChunk = readFormatChunk(file);
    ChunkInfo dataInfo = readDataInfo(file);

    // for converting from an int in the range minValue < i < maxValue to a float in the range -1 <= f <= 1
    float conversionFactor = pow(2, formatChunk.bitsPerSample - 1);
    
    size_t audioSize = dataInfo.chunkSize / sizeof(int16_t);    
    // rounded up to a mulitple of blockSize
    m_buffer.resize(audioSize + blockSize - (audioSize % blockSize), 0.0f);
    
    int16_t data[blockSize];
    for (int blockIndex = 0; blockIndex < (audioSize / blockSize); blockIndex++)
    {
        readDataChunk<int16_t>(data, blockSize, file);
        // size_t test = fread(data, sizeof(uint16_t), blockSize, file);
        for (int sampleIndex = 0; sampleIndex < blockSize; sampleIndex++)
        {
            m_buffer[blockIndex * blockSize + sampleIndex] = data[sampleIndex] / conversionFactor;
        }
    }
    // cut off trailing zeros
    m_buffer.resize(audioSize);

    fclose(file);
}

float Sampler::processSample(float sample)
{
    if ( m_nextSample > m_buffer.size() ) m_nextSample = 0;
    return sample + (m_buffer[m_nextSample++] * 1.0f);
}