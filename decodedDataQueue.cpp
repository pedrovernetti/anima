////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                ANIMA                                   //
//           Copyright (C) 2022-2022 Pedro Vernetti Gonçalves             //
//                                                                        //
//  This program is free software: you can redistribute it and/or modify  //
//  it under the terms of the GNU General Public License as published by  //
//   the Free Software Foundation, either version 3 of the License, or    //
//                 (at your option) any later version.                    //
//                                                                        //
//     This program is distributed in the hope that it will be useful,    //
//     but WITHOUT ANY WARRANTY; without even the implied warranty of     //
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      //
//             GNU General Public License for more details.               //
//                                                                        //
//   You should have received a copy of the GNU General Public License    //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#include "decodedDataQueue.hpp"

namespace anima
{
    decodedDataQueue::decodedDataQueue( uint16_t maxEnqueuedChunks, uint16_t maxBytesPerChunk ) :
        data(new byte[maxEnqueuedChunks * maxBytesPerChunk]),
        chunkSampleCounts(new uint16_t[maxEnqueuedChunks]()),
        totalBytes(maxEnqueuedChunks * maxBytesPerChunk),
        maxSamplesPerChunk(maxBytesPerChunk), totalChunks(maxEnqueuedChunks),
        filledChunksCount(0), lastChunkPosition(0), nextChunkPosition(0),
        sampleSize(1)
    {
    }

    decodedDataQueue::decodedDataQueue( uint16_t maxEnqueuedChunks,
                                        uint16_t maxSamplesPerChunk,
                                        uint8_t bitsPerSample ) :
        data(new byte[maxEnqueuedChunks * maxSamplesPerChunk * (bitsPerSample / 8)]),
        chunkSampleCounts(new uint16_t[maxEnqueuedChunks]()),
        totalBytes(maxEnqueuedChunks * maxSamplesPerChunk * (bitsPerSample / 8)),
        maxSamplesPerChunk(maxSamplesPerChunk), totalChunks(maxEnqueuedChunks),
        filledChunksCount(0), lastChunkPosition(0), nextChunkPosition(0),
        sampleSize(bitsPerSample / 8)
    {
    }

    decodedDataQueue::~decodedDataQueue()
    {
        delete[] this->data;
        delete[] this->chunkSampleCounts;
    }

    void decodedDataQueue::reset( uint16_t maxSamplesPerChunk, uint8_t bitsPerSample )
    {
        this->sampleSize = bitsPerSample / 8;
        this->maxSamplesPerChunk = maxSamplesPerChunk;
        this->totalChunks = this->totalBytes / (maxSamplesPerChunk * this->sampleSize);
        delete[] this->chunkSampleCounts;
        this->chunkSampleCounts = new uint16_t[this->totalChunks]();
        this->filledChunksCount = 0;
        this->lastChunkPosition = 0;
        this->nextChunkPosition = 0;
    }

    void decodedDataQueue::reset( uint16_t maxEnqueuedChunks,
                                  uint16_t maxSamplesPerChunk,
                                  uint8_t bitsPerSample,
                                  bool forceReallocation )
    {
        const size_t newTotalBytes = maxEnqueuedChunks * maxSamplesPerChunk * (bitsPerSample / 8);
        if ((newTotalBytes > this->totalBytes) || forceReallocation)
        {
            this->totalBytes = newTotalBytes;
            delete[] this->data;
            this->data = new byte[this->totalBytes];
        }
        if (maxEnqueuedChunks != this->maxSamplesPerChunk)
        {
            this->totalChunks = maxEnqueuedChunks;
            delete[] this->chunkSampleCounts;
            this->chunkSampleCounts = new uint16_t[maxEnqueuedChunks]();
        }
        else
        {
            for (uint16_t i = 0; i < maxEnqueuedChunks; this->chunkSampleCounts[i++] = 0);
        }
        this->sampleSize = bitsPerSample / 8;
        this->maxSamplesPerChunk = maxSamplesPerChunk;
        this->filledChunksCount = 0;
        this->lastChunkPosition = 0;
        this->nextChunkPosition = 0;
    }

    byte * decodedDataQueue::enqueueNewChunk( bool force )
    {
        if (force)
        {
            if (this->filledChunksCount == this->totalChunks)
            {
                this->nextChunkPosition = (this->nextChunkPosition + 1) % this->totalChunks;
                this->filledChunksCount--;
            }
        }
        else
        {
            // execution will stay hanged until another thread dequeues something, in case it is full
            while (this->filledChunksCount == this->totalChunks);
        }
        if (this->filledChunksCount)
            this->lastChunkPosition = (this->lastChunkPosition + 1) % this->totalChunks;
        this->filledChunksCount++;
        byte * chunk = &(this->data[this->maxSamplesPerChunk * this->lastChunkPosition * this->sampleSize]);
        return chunk;
    }
}
