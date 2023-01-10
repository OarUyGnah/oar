#include "StringBuffer.h"
#include "Endian.h"
#include "Mutex.h"
#include "ThisThread.h"
#include <cstdint>
#include <string.h>
namespace oar {
StringBuffer::StringBuffer()
    : _rIndex(0)
    , _wIndex(0)
{
    _buf.resize(initial_size);
}

StringBuffer::~StringBuffer() { }

void StringBuffer::append(const char* str, size_t size)
{
    if (writeableBytes() < size) {
    }
    std::copy(str, str + size, wBegin());
    _wIndex += size;
}

void StringBuffer::append(const void* data, size_t size)
{
    append(static_cast<const char*>(data), size);
}

void StringBuffer::append(std::string s)
{
    if (writeableBytes() < s.size()) {
    }
    std::copy(&*s.begin(), &*s.end(), wBegin());
    _wIndex += s.size();
}

void StringBuffer::appendInt8(int8_t val)
{
    append(&val, sizeof val);
}

void StringBuffer::appendInt16(int16_t val)
{
    int16_t netval = oar::hton(val);
    // printf("netval %d\n", netval);
    append(&netval, sizeof netval);
}

void StringBuffer::appendInt32(int32_t val)
{
    int32_t netval = oar::hton(val);
    append(&netval, sizeof netval);
}

void StringBuffer::appendInt64(int64_t val)
{
    int64_t netval = oar::hton(val);
    append(&netval, sizeof netval);
}

auto StringBuffer::readToPiece(size_t size) -> ContentPiece
{
    size_t real_size = readableBytes() < size ? readableBytes() : size;
    ContentPiece p(rBegin(), real_size);
    retrieve(real_size);
    return p;
}

int8_t StringBuffer::readInt8()
{
    int8_t val = previewInt8();
    retrieveInt8();
    return val;
}

int16_t StringBuffer::readInt16()
{
    int16_t val = previewInt16();
    retrieveInt16();
    return val;
}

int32_t StringBuffer::readInt32()
{
    int32_t val = previewInt32();
    retrieveInt32();
    return val;
}

int64_t StringBuffer::readInt64()
{
    int64_t val = previewInt64();
    retrieveInt64();
    return val;
}

auto StringBuffer::previewToPiece(size_t size) -> ContentPiece
{
    return ContentPiece(rBegin(), readableBytes() < size ? readableBytes() : size);
}

int8_t StringBuffer::previewInt8()
{
    int8_t val = 0;
    ::memcpy(&val, rBegin(), sizeof val);
    return val;
}
int16_t StringBuffer::previewInt16()
{
    int16_t val = 0;
    ::memcpy(&val, rBegin(), sizeof val);
    return oar::ntoh(val);
}
int32_t StringBuffer::previewInt32()
{
    int32_t val = 0;
    ::memcpy(&val, rBegin(), sizeof val);
    return oar::ntoh(val);
}
int64_t StringBuffer::previewInt64()
{
    int64_t val = 0;
    ::memcpy(&val, rBegin(), sizeof val);
    return oar::ntoh(val);
}

void StringBuffer::retrieve(size_t size)
{
    if (size < readableBytes()) {
        _rIndex += size;
    } else {
        retrieveAll();
    }
}

void StringBuffer::retrieveAll()
{
    _rIndex = 0;
    _wIndex = 0;
}

void StringBuffer::retrieveInt8()
{
    retrieve(sizeof(int8_t));
}

void StringBuffer::retrieveInt16()
{
    retrieve(sizeof(int16_t));
}

void StringBuffer::retrieveInt32()
{
    retrieve(sizeof(int32_t));
}

void StringBuffer::retrieveInt64()
{
    retrieve(sizeof(int64_t));
}
}