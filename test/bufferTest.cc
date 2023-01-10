#include "../oar/StringBuffer.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string.h>
using namespace oar;

int main()
{
    // const char* str = "123";
    // StringBuffer buf;
    // std::cout << buf.size() << std::endl;
    // for (int i = 0; i < 10; i++) {
    //     buf.append(str, strlen(str));
    // }
    // std::cout << "12" << std::endl;
    // for (int i = 0; i < 10; i++) {
    //     buf.append(std::string("oarrrr"));
    // }

    // ContentPiece piece = buf.preview((size_t)100);
    // std::cout << piece.data() << std::endl;
    // std::cout << strlen(piece.data()) << std::endl;
    // std::cout << "piece size" << piece.size() << std::endl;
    StringBuffer buf;
    std::cout << buf.size() << std::endl;
    buf.appendInt8(15);
    buf.appendInt16(255);
    buf.appendInt32(1312);
    buf.appendInt64(599902);

    // auto i1 = buf.previewInt8();
    // printf("i1 : %d\n", i1); // cout不显示
    // auto i2 = buf.previewInt16();
    // printf("i2 : %d\n", i2); // cout不显示

    // auto i1 = buf.readInt<uint8_t>(8);
    // printf("i1 : %d\n", i1);
    // auto i2 = buf.readInt<uint16_t>(16);
    // printf("i2 : %d\n", i2);

    // printf("hton(i2) = %d\n", oar::hton(i2));
    // auto piece = buf.previewToPiece(15);
    // int8_t i1 = 0;
    // memcpy(&i1, piece.data(), sizeof i1);
    // printf("i1 : %d\n", i1); // cout不显示

    // int16_t i2 = 0;
    // memcpy(&i2, piece.data() + 1, sizeof i2);
    // std::cout << "i2 : " << oar::ntoh(i2) << std::endl;

    // int32_t i3 = 0;
    // memcpy(&i3, piece.data() + 3, sizeof i3);
    // std::cout << "i3 : " << oar::ntoh(i3) << std::endl;

    // int64_t i4 = 0;
    // memcpy(&i4, piece.data() + 7, sizeof i4);
    // std::cout << "i4 : " << oar::ntoh(i4) << std::endl;

    // std::cout << "piece size" << piece.size() << std::endl;
    return 0;
}