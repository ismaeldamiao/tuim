/*
 * MIT License
 * 
 * Copyright (c) 2024 FennelFoxxo
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

static inline uint16_t convert_endian_16(uint16_t n, bool is_lsb) {
    uint8_t* byte_ptr = (uint8_t*)&n;
    
    // Get bytes in memory order
    uint8_t bl = byte_ptr[0];
    uint8_t bh = byte_ptr[1];
    
    if (is_lsb) {
        return bl | (uint16_t)bh<<8;
    }
    return bh | (uint16_t)bl<<8;
}

static inline uint32_t convert_endian_32(uint32_t n, bool is_lsb) {
    uint16_t* word_ptr = (uint16_t*)&n;
    
    // Get words in memory order
    uint16_t wl = word_ptr[0];
    uint16_t wh = word_ptr[1];
    
    if (is_lsb) {
        return convert_endian_16(wl, is_lsb) | (uint32_t)convert_endian_16(wh, is_lsb)<<16;
    }
    return convert_endian_16(wh, is_lsb) | (uint32_t)convert_endian_16(wl, is_lsb)<<16;
}

static inline uint64_t convert_endian_64(uint64_t n, bool is_lsb) {
    uint32_t* dword_ptr = (uint32_t*)&n;
    
    // Get doublewords in memory order
    uint32_t dl = dword_ptr[0];
    uint32_t dh = dword_ptr[1];
    
    if (is_lsb)   {
        return convert_endian_32(dl, is_lsb) | (uint64_t)convert_endian_32(dh, is_lsb)<<32;
    }
    return convert_endian_32(dh, is_lsb) | (uint64_t)convert_endian_32(dl, is_lsb)<<32;
}
