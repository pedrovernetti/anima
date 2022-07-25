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

#pragma once
#ifndef _ANIMA_DECODEDDATAQUEUE_INCLUDED
#define _ANIMA_DECODEDDATAQUEUE_INCLUDED

#include "global.hpp"

namespace anima
{
    class decodedDataQueue
    {
        protected:

        byte * data;
        uint16_t * chunkSampleCounts;
        size_t totalBytes;
        uint16_t maxSamplesPerChunk;
        uint16_t totalChunks;
        uint16_t filledChunksCount;
        uint16_t lastChunkPosition;
        uint16_t nextChunkPosition;
        uint8_t sampleSize;

        public:

        decodedDataQueue() = delete;

        decodedDataQueue( const decodedDataQueue & ) = delete;

        decodedDataQueue( uint16_t maxEnqueuedChunks, uint16_t maxBytesPerChunk );

        decodedDataQueue( uint16_t maxEnqueuedChunks,
                          uint16_t maxSamplesPerChunk,
                          uint8_t bitsPerSample );

        ~decodedDataQueue();

        inline uint16_t bitsPerSample() const
        {
            return (this->sampleSize * 8);
        }

        inline uint16_t bytesPerSample() const
        {
            return this->sampleSize;
        }

        inline uint16_t maximumSamplesPerChunk() const
        {
            return this->maxSamplesPerChunk;
        }

        inline uint16_t maximumEnqueuedChunks() const
        {
            return this->totalChunks;
        }

        inline uint16_t enqueuedChunksCount() const
        {
            return this->filledChunksCount;
        }

        inline bool isFull() const
        {
            return (this->filledChunksCount == this->totalChunks);
        }

        inline bool isEmpty() const
        {
            return (this->filledChunksCount == 0);
        }

        inline void reset()
        {
            for (uint16_t i = 0; i < this->totalChunks; this->chunkSampleCounts[i++] = 0);
            this->filledChunksCount = 0;
            this->lastChunkPosition = 0;
            this->nextChunkPosition = 0;
        }

        void reset( uint16_t maxSamplesPerChunk, uint8_t bitsPerSample );

        void reset( uint16_t maxEnqueuedChunks,
                    uint16_t maxSamplesPerChunk,
                    uint8_t bitsPerSample,
                    bool forceReallocation = false );

        byte * enqueueNewChunk( bool force = false );

        inline void specifyLastEnqueuedChunkSampleCount( uint16_t size )
        {
            this->chunkSampleCounts[this->lastChunkPosition] = size;
        }

        inline byte * nextChunk()
        {
            if (!(this->filledChunksCount)) return nullptr;
            return &(this->data[this->maxSamplesPerChunk * this->nextChunkPosition * this->sampleSize]);
        }

        inline uint16_t nextChunkSampleCount() const
        {
            if (!(this->filledChunksCount)) return 0;
            return this->chunkSampleCounts[this->nextChunkPosition];
        }

        inline void dequeueChunk()
        {
            if (!(this->filledChunksCount)) return;
            this->chunkSampleCounts[this->nextChunkPosition] = 0;
            this->filledChunksCount--;
            if (this->filledChunksCount)
                this->nextChunkPosition = (this->nextChunkPosition + 1) % this->totalChunks;
        }

        decodedDataQueue & operator = ( const decodedDataQueue & ) = delete;
    };
}

#endif // _ANIMA_DECODEDDATAQUEUE_INCLUDED
