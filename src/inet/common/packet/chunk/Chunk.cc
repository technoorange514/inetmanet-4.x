//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "inet/common/packet/chunk/SliceChunk.h"
#include "inet/common/packet/serializer/ChunkSerializer.h"
#include "inet/common/packet/serializer/ChunkSerializerRegistry.h"

namespace inet {

bool Chunk::enableImplicitChunkSerialization = false;
int Chunk::nextId = 0;

Chunk::Chunk() :
    id(nextId++),
    flags(0)
{
}

Chunk::Chunk(const Chunk& other) :
    id(nextId++),
    flags(other.flags & ~CF_IMMUTABLE)
{
}

void Chunk::handleChange()
{
    CHUNK_CHECK_USAGE(isMutable(), "chunk is immutable");
}

int Chunk::getBitsArraySize()
{
    return (bit(getChunkLength()).get() + 31) / 32;
}

int Chunk::getBytesArraySize()
{
    return ((bit(getChunkLength()).get() + 7) / 8 + 15) / 16;
}

static std::string asStringValue;

const char *Chunk::getBitsAsString(int index)
{
    asStringValue = "";
    try {
        int offset = index * 32;
        int length = std::min(32, (int)bit(getChunkLength()).get() - offset);
        MemoryOutputStream outputStream;
        serialize(outputStream, shared_from_this(), bit(offset), bit(length));
        std::vector<bool> bits;
        outputStream.copyData(bits);
        for (int i = 0; i < length; i++) {
            if (i != 0 && i % 4 == 0)
                asStringValue += " ";
            asStringValue += (bits[i] ? "1" : "0");
        }
    }
    catch (cRuntimeError& e) {
        asStringValue = e.what();
    }
    return asStringValue.c_str();
}

const char *Chunk::getBytesAsString(int index)
{
    asStringValue = "";
    try {
        int offset = index * 8 * 16;
        int length = std::min(8 * 16, (int)bit(getChunkLength()).get() - offset);
        MemoryOutputStream outputStream;
        serialize(outputStream, shared_from_this(), bit(offset), bit(length));
        ASSERT(outputStream.getLength() == bit(length));
        std::vector<uint8_t> bytes;
        outputStream.copyData(bytes);
        char tmp[3] = "  ";
        for (int i = 0; i < bytes.size(); i++) {
            if (i != 0)
                asStringValue += " ";
            sprintf(tmp, "%02X", bytes[i]);
            asStringValue += tmp;
        }
    }
    catch (cRuntimeError& e) {
        asStringValue = e.what();
    }
    return asStringValue.c_str();
}

Ptr<Chunk> Chunk::convertChunk(const std::type_info& typeInfo, const Ptr<Chunk>& chunk, bit offset, bit length, int flags)
{
    auto chunkType = chunk->getChunkType();
    if (!enableImplicitChunkSerialization && !(flags & PF_ALLOW_SERIALIZATION) && chunkType != CT_BITS && chunkType != CT_BYTES)
        throw cRuntimeError("Implicit chunk serialization is disabled to prevent unpredictable performance degradation (you may consider changing the Chunk::enableImplicitChunkSerialization flag or passing the PF_ALLOW_SERIALIZATION flag to peek)");
    MemoryOutputStream outputStream;
    serialize(outputStream, chunk, offset, length);
    MemoryInputStream inputStream(outputStream.getData());
    return deserialize(inputStream, typeInfo);
}

void Chunk::moveIterator(Iterator& iterator, bit length) const
{
    auto position = iterator.getPosition() + length;
    iterator.setPosition(position);
    iterator.setIndex(position == bit(0) ? 0 : -1);
}

void Chunk::seekIterator(Iterator& iterator, bit position) const
{
    iterator.setPosition(position);
    iterator.setIndex(position == bit(0) ? 0 : -1);
}

Ptr<Chunk> Chunk::peek(const Iterator& iterator, bit length, int flags) const
{
    const auto& chunk = peekUnchecked(nullptr, nullptr, iterator, length, flags);
    return checkPeekResult<Chunk>(chunk, flags);
}

std::string Chunk::str() const
{
    std::ostringstream os;
    os << getClassName() << ", length = " << getChunkLength();
    return os.str();
}

void Chunk::serialize(MemoryOutputStream& stream, const Ptr<const Chunk>& chunk, bit offset, bit length)
{
    const Chunk *chunkPointer = chunk.get();
    auto serializer = ChunkSerializerRegistry::globalRegistry.getSerializer(typeid(*chunkPointer));
#if CHUNK_CHECK_IMPLEMENTATION_ENABLED
    auto startPosition = stream.getLength();
#endif
    serializer->serialize(stream, chunk, offset, length);
#if CHUNK_CHECK_IMPLEMENTATION_ENABLED
    auto endPosition = stream.getLength();
    auto expectedChunkLength = length == bit(-1) ? chunk->getChunkLength() - offset : length;
    CHUNK_CHECK_IMPLEMENTATION(expectedChunkLength == endPosition - startPosition);
#endif
}

Ptr<Chunk> Chunk::deserialize(MemoryInputStream& stream, const std::type_info& typeInfo)
{
    auto serializer = ChunkSerializerRegistry::globalRegistry.getSerializer(typeInfo);
#if CHUNK_CHECK_IMPLEMENTATION_ENABLED
    auto startPosition = byte(stream.getPosition());
#endif
    auto chunk = serializer->deserialize(stream, typeInfo);
#if CHUNK_CHECK_IMPLEMENTATION_ENABLED
    auto endPosition = byte(stream.getPosition());
    CHUNK_CHECK_IMPLEMENTATION(chunk->getChunkLength() == endPosition - startPosition);
#endif
    if (stream.isReadBeyondEnd())
        chunk->markIncomplete();
    return chunk;
}

} // namespace
