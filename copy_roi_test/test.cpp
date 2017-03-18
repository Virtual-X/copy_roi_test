//
//  test.cpp
//  copy_roi_test
//
//  Created by Igor Beati on 04.03.17.
//  Copyright © 2017 Igor Beati. All rights reserved.
//

#include "test.hpp"

#include "compress.h"

#include <iostream>
#include <chrono>
#include <random>

#include <string.h>
#include <assert.h>


typedef float type;

static void compress_roi(const type* roi_s, char* roi_d, v3 size_roi_s, uint32_t* inout_size_roi_d, const void* p)
{
    const uint32_t size = size_roi_s.x * size_roi_s.y * size_roi_s.z * sizeof(type);
    assert(*inout_size_roi_d >= size);
    memcpy(roi_d, roi_s, size);
    *inout_size_roi_d = size;
}


static void compress_roi_x(const type* roi_s, char* roi_d, v3 size_roi_s, uint32_t* inout_size_roi_d, const void* p)
{
    const uint32_t size = size_roi_s.x * size_roi_s.y * size_roi_s.z * sizeof(type);
    assert(*inout_size_roi_d >= size);
//    static type x = 1;
//    type* d = reinterpret_cast<type*>(roi_d);
//    for (size_t i = 0; i < size / sizeof(type); i++) {
//        d[i] = x;
//    }
//    x += 1;
    memcpy(roi_d, roi_s, size);
    *inout_size_roi_d = size;
}


static void decompress_roi(const char* roi_s, type* roi_d, uint32_t size_rois_s, v3 size_roi_d, const void* p)
{
    const uint32_t size = size_roi_d.x * size_roi_d.y * size_roi_d.z * sizeof(type);
    assert(size_rois_s == size);
    memcpy(roi_d, roi_s, size);
}


template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type get_random()
{
    static std::mt19937 random_engine;
    static std::uniform_int_distribution<T> uniform_dist;
    return uniform_dist(random_engine);
}


template<typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type get_random()
{
    static std::mt19937 random_engine;
    static std::uniform_real_distribution<T> uniform_dist;
    return uniform_dist(random_engine);
}


void test()
{
    // todo: loop over source and send to dest for compression
    
    // all you need is a distortion of reality
    const v3 size_s{ 512, 512, 200 };
    const v3 size_roi{ 32, 64, 10 };
//    const v3 size_s{ 10, 30, 10 };
//    const v3 size_roi{ 32, 32, 3 };
//    const v3 size_s{ 10, 6, 3 };
 //   const v3 size_roi{ 4, 3, 2 };
    const uint32_t size = size_s.x * size_s.y * size_s.z;
    
    typedef std::chrono::high_resolution_clock clock;
    auto start = clock::now();

    type* s = new type[size];
    for (size_t i = 0; i < size; i++) {
        s[i] = get_random<type>();
    }

    std::cout << "Init (s): " << std::chrono::duration_cast<std::chrono::seconds>(clock::now() - start).count() << std::endl;

    const int size_o = size * sizeof(type);
    const int size_b = size_o * 2;
    char* d = new char[size_b];
    type* r = new type[size];
    type temp_roi[size_roi.x * size_roi.y * size_roi.z];
    
    (void)&compress_roi;
    (void)&compress_roi_x;
    
    uint32_t size_d = size_b;
    //compress_roi(s, d, size_s, &size_d, nullptr);
    
    const int runs = 1;

    start = clock::now();
    for(int t = 0; t < runs; ++t)
        compress_rois<type, void>(s, d, temp_roi, size_s, &size_d, size_roi, &compress_roi, nullptr);
    std::cout << "Compress MB/s: " << (size_o + size_d) * runs / std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count() << std::endl;

    std::cout << "size_s " << size_o << std::endl;
    std::cout << "size_d " << size_d << std::endl;
    
    start = std::chrono::high_resolution_clock::now();
    for(int t = 0; t < runs; ++t)
        decompress_rois<type, void>(d, r, temp_roi, size_d, size_s, size_roi, &decompress_roi, nullptr);
    std::cout << "Decompress MB/s: " << (size_o + size_d) * runs / std::chrono::duration_cast<std::chrono::microseconds>(clock::now() - start).count() << std::endl;
    
    size_t n = 10;
    for (size_t i = 0; i < n; i++) {
        std::cout << s[i] << " ";
        if ((i + 1) % size_s.x == 0) {
            std::cout << std::endl;
            if (((i + 1) / size_s.x) % size_s.y == 0) {
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
    for (size_t i = 0; i < n; i++) {
        std::cout << r[i] << " ";
        if ((i + 1) % size_s.x == 0) {
            std::cout << std::endl;
            if (((i + 1) / size_s.x) % size_s.y == 0) {
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
    assert(memcmp(s, r, size * sizeof(type)) == 0);
}
