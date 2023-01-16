#ifndef __OAR_STRINGBUFFER_H__
#define __OAR_STRINGBUFFER_H__
#include "oar/Endian.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string.h>
#include <string>
#include <vector>

namespace oar {

class ContentPiece;

class StringBuffer {
public:
    static constexpr int initial_size = 1024;

    StringBuffer(int size = initial_size);
    ~StringBuffer();

    void append(const char* str, size_t size);
    void append(const void* data, size_t size);
    void append(std::string s);

    void appendInt8(int8_t val);
    void appendInt16(int16_t val);
    void appendInt32(int32_t val);
    void appendInt64(int64_t val);

    size_t size() const { return _buf.size(); }

    inline size_t readableBytes() { return _wIndex - _rIndex; }
    inline size_t writeableBytes() { return _buf.size() - _wIndex; }

    char* begin() { return &*_buf.begin(); }
    const char* begin() const { return &*_buf.begin(); }

    const char* peek() { return begin() + _rIndex; }

    char* wBegin() { return begin() + _wIndex; }
    const char* wBegin() const { return begin() + _wIndex; }

    char* rBegin() { return begin() + _rIndex; }
    const char* rBegin() const { return begin() + _rIndex; }

    void clear()
    {
        _buf.clear();
        _rIndex = 0;
        _wIndex = 0;
    }
    void getline();

    auto readToPiece(size_t size) -> ContentPiece;

    template <typename INT>
    INT readInt(size_t bits)
    {
        switch (bits) {
        case 8:
            return readInt8();
        case 16:
            return readInt16();
        case 32:
            return readInt32();
        case 64:
            return readInt64();
        default:
            throw std::exception();
        }
    }

    int8_t readInt8();
    int16_t readInt16();
    int32_t readInt32();
    int64_t readInt64();

    auto previewToPiece(size_t size) -> ContentPiece;
    int8_t previewInt8();
    int16_t previewInt16();
    int32_t previewInt32();
    int64_t previewInt64();

    void retrieve(size_t size);
    void retrieveAll();
    void retrieveInt8();
    void retrieveInt16();
    void retrieveInt32();
    void retrieveInt64();

private:
    std::vector<char> _buf;
    size_t _rIndex;
    size_t _wIndex;
};

class ContentPiece {
public:
    ContentPiece(const char* data, size_t size)
        : _data(new char[size])
        , _size(size)
    {
        memset(static_cast<void*>(_data), 0, size);
        std::copy(data, data + size, _data);
    }
    ~ContentPiece()
    {
        delete[] _data;
    }

    char* data() { return _data; }
    const char* data() const { return _data; }
    size_t size() const { return _size; }

private:
    char* _data;
    size_t _size;
};
}

#endif