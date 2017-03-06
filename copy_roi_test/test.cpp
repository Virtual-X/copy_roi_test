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
#include <string.h>
#include <assert.h>


static void compress_roi(const float* roi_s, float* roi_d, v3 size_roi_s, uint32_t* inout_size_roi_d, const void* p)
{
    const uint32_t size = size_roi_s.x * size_roi_s.y * size_roi_s.z;
    assert(*inout_size_roi_d >= size);
    memcpy(roi_d, roi_s, size * sizeof(float));
    *inout_size_roi_d = size;
}


static void compress_roi_x(const float* roi_s, float* roi_d, v3 size_roi_s, uint32_t* inout_size_roi_d, const void* p)
{
    const uint32_t size = size_roi_s.x * size_roi_s.y * size_roi_s.z;
    assert(*inout_size_roi_d >= size);
    static float x = 1;
    for (size_t i = 0; i < size; i++) {
        roi_d[i] = x;
    }
    x += 1;
    *inout_size_roi_d = size;
}


static void decompress_roi(const float* roi_s, float* roi_d, uint32_t size_rois_s, v3 size_roi_d, const void* p)
{
    const uint32_t size = size_roi_d.x * size_roi_d.y * size_roi_d.z;
    assert(size_rois_s == size);
    memcpy(roi_d, roi_s, size * sizeof(float));
}


void test()
{
    const v3 size_s{ 100, 64, 10 };
    const v3 size_roi{ 32, 32, 3 };
//    const v3 size_s{ 10, 6, 3 };
//    const v3 size_roi{ 4, 3, 2 };
    const uint32_t size = size_s.x * size_s.y * size_s.z;
    float s[size];
    for (size_t i = 0; i < size; i++) {
        s[i] = (float)(((i * 777) % size) / (double)size);
    }
    float d[size * 3];
    float r[size];
    float temp[size_roi.x * size_roi.y * size_roi.z];
    
    (void)&compress_roi;
    (void)&compress_roi_x;
    
    uint32_t size_d = size * 3;
    compress_rois<float, void>(s, d, temp, size_s, &size_d, size_roi, &compress_roi, nullptr);

    decompress_rois<float, void>(d, r, temp, size_d, size_s, size_roi, &decompress_roi, nullptr);
    size_t n = 10;
    for (size_t i = 0; i < n; i++) {
        std::cout << (int)(s[i] * 1000) << " ";
        if ((i + 1) % size_s.x == 0) {
            std::cout << std::endl;
            if (((i + 1) / size_s.x) % size_s.y == 0) {
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
    for (size_t i = 0; i < n; i++) {
        std::cout << (int)(r[i] * 1000) << " ";
        if ((i + 1) % size_s.x == 0) {
            std::cout << std::endl;
            if (((i + 1) / size_s.x) % size_s.y == 0) {
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;
    assert(memcmp(s, r, size * 4) == 0);
}
