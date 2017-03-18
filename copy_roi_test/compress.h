//
//  compress.h
//  copy_roi_test
//
//  Created by Igor Beati on 04.03.17.
//  Copyright © 2017 Igor Beati. All rights reserved.
//

#ifndef compress_h
#define compress_h


#include "copy_roi.h"

#include <stdint.h>


template<typename T, class Param>
void compress_rois(const T* s, char* d, T* roi_temp, v3 size_s, uint32_t* inout_size_d, v3 size_roi,
                   void(*compress_roi)(const T* roi_s, char* roi_d, v3 size_roi_s, uint32_t* inout_size_roi_d, const Param* p), const Param* p)
{
    v3 n{ (size_s.x + size_roi.x - 1) / size_roi.x, (size_s.y + size_roi.y - 1) / size_roi.y, (size_s.z + size_roi.z - 1) / size_roi.z };
    uint32_t* rs = (uint32_t*)d;
    char* dd = (char*)(rs + n.x * n.y * n.z);
    for (int z = 0; z < n.z; z++) {
        for (int y = 0; y < n.y; y++) {
            for (int x = 0; x < n.x; x++) {
                copy_roi(s, roi_temp, size_s, size_roi, { x * size_roi.x, y * size_roi.y , z * size_roi.z }, { 0, 0, 0 }, size_roi);
                uint32_t l = *inout_size_d - (uint32_t)(dd - d);
                compress_roi(roi_temp, dd, size_roi, &l, p);
                dd += l;
                *rs++ = l;
            }
        }
    }
    *inout_size_d = (uint32_t)(dd - d);
}


template<typename T, class Param>
void decompress_rois(const char* s, T* d, T* roi_temp, uint32_t size_s, v3 size_d, v3 size_roi,
                     void(*decompress_roi)(const char* roi_s, T* roi_d, uint32_t size_rois_s, v3 size_roi_d, const Param* p), const Param* p)
{
    v3 n{ (size_d.x + size_roi.x - 1) / size_roi.x, (size_d.y + size_roi.y - 1) / size_roi.y, (size_d.z + size_roi.z - 1) / size_roi.z };
    const uint32_t* rs = (const uint32_t*)s;
    char* ss = (char*)(rs + n.x * n.y * n.z);
    for (int z = 0; z < n.z; z++) {
        for (int y = 0; y < n.y; y++) {
            for (int x = 0; x < n.x; x++) {
                uint32_t l = *rs++;
                decompress_roi(ss, roi_temp, l, size_roi, p);
                copy_roi(roi_temp, d, size_roi, size_d, { 0, 0, 0 }, { x * size_roi.x, y * size_roi.y, z * size_roi.z }, size_roi);
                ss += l;
            }
        }
    }
}


#endif /* compress_h */
