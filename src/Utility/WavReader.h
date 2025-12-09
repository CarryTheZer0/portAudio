/*
 *  WavReader.h
 *
 *  Created on: 07 Dec 2025
 *      Author: mchlp
 */

#ifndef WAV_READER_H_
#define WAV_READER_H_

#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <assert.h>

struct RIFFHeader{
    char chunkId[4];  // "RIFF"
    uint32_t chunkSize;
    char format[4];  // "WAVE"
};

struct ChunkInfo{
    char chunkId[4];  // "fmt " or "data"
    uint32_t chunkSize;
};

struct FormatChunk{
    uint16_t audioFormat;
    uint16_t channelCount;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t stepSize;
    uint16_t bitsPerSample;
};

static RIFFHeader readHeader(FILE* file)
{
    RIFFHeader header;

    fread(&header.chunkId, 1, 4, file);
    fread(&header.chunkSize, 1, 4, file);
    fread(&header.format, 1, 4, file);

    constexpr char expectedId[4] = {'R', 'I', 'F', 'F'};
    assert(memcmp(header.chunkId, expectedId, 4) == 0);
    
    return header;
}

static ChunkInfo readChunkInfo(FILE* file)
{
    ChunkInfo chunkInfo;
    fread(&chunkInfo.chunkId, 1, 4, file);
    fread(&chunkInfo.chunkSize, 1, 4, file);
    return chunkInfo;
}

static ChunkInfo readFormatInfo(FILE* file)
{
    constexpr char expectedId[4] = {'f', 'm', 't', ' '};
    constexpr char endOfFile[1] = {'\0'};
    
    ChunkInfo chunkInfo = readChunkInfo(file);
    while( memcmp(chunkInfo.chunkId, expectedId, 4) != 0 )
    {
        assert( memcmp(chunkInfo.chunkId, endOfFile, 1) != 0 );

        fseek(file, chunkInfo.chunkSize, SEEK_CUR);
        chunkInfo = readChunkInfo(file);
    }
    return chunkInfo;
}

static FormatChunk readFormatChunk(FILE* file)
{
    FormatChunk formatChunk;

    fread(&formatChunk.audioFormat, 1, 2, file);
    fread(&formatChunk.channelCount, 1, 2, file);
    fread(&formatChunk.sampleRate, 1, 4, file);
    fread(&formatChunk.byteRate, 1, 4, file);
    fread(&formatChunk.stepSize, 1, 2, file);
    fread(&formatChunk.bitsPerSample, 1, 2, file);
    return formatChunk;
}

static ChunkInfo readDataInfo(FILE* file)
{
    constexpr char expectedId[4] = {'d', 'a', 't', 'a'};
    constexpr char endOfFile[1] = {'\0'};
    
    ChunkInfo chunkInfo = readChunkInfo(file);
    while( memcmp(chunkInfo.chunkId, expectedId, 4) != 0 )
    {
        assert( memcmp(chunkInfo.chunkId, endOfFile, 1) != 0 );

        fseek(file, chunkInfo.chunkSize, SEEK_CUR);
        chunkInfo = readChunkInfo(file);
    }
    return chunkInfo;
}

template <typename T>
static void readDataChunk(T* target, int count, FILE* file)
{
    fread(target, sizeof(T), count, file);
}

#endif /* WAV_READER_H_ */
