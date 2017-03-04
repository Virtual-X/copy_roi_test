//
//  copy_roi.c
//  copy_roi_test
//
//  Created by Igor Beati on 04.03.17.
//  Copyright © 2017 Igor Beati. All rights reserved.
//


#include "copy_roi.h"

#include <string.h>


void copy_roi(const char* s, char* d, v3 size_s, v3 size_d, v3 start_roi_s, v3 start_roi_d, v3 size_roi, int size_type)
{
    size_s.x *= size_type;
    size_d.x *= size_type;
    start_roi_s.x *= size_type;
    start_roi_d.x *= size_type;
    size_roi.x *= size_type;
    
    v3 off = start_roi_s - start_roi_d;
    v3 end_roi_d = min(start_roi_d + size_roi, size_d);
    start_roi_d = max(start_roi_d, { 0, 0, 0 });
    size_roi = end_roi_d - start_roi_d;
    if (size_roi.x <= 0 || size_roi.y <= 0 || size_roi.z <= 0) {
        return;
    }
    
    v3 start_overlap_d = max(start_roi_d, max(start_roi_s, { 0, 0, 0 }) - off);
    v3 end_overlap_d = min(end_roi_d, min(start_roi_s + size_roi, size_s) - off);
    v3 size_overlap = end_overlap_d - start_overlap_d;
    // 0, start_roi, start_overlap, end_overlap, end_roi, size
    
    char* dd = d + (start_roi_d.x + size_d.x * (start_roi_d.y + size_d.y * start_roi_d.z));
    
    if (size_overlap.x <= 0 || size_overlap.y <= 0 || size_overlap.z <= 0) {
        for (int z = start_roi_d.z; z < end_roi_d.z; z++) {
            for (int y = start_roi_d.y; y < end_roi_d.y; y++) {
                memset(dd, 0, size_roi.x);
                dd += size_d.x;
            }
            dd += size_d.x * (size_d.y - size_roi.y);
            
        }
        return;
    }
    
    int left_gap_x_d = start_overlap_d.x - start_roi_d.x;
    int right_gap_x_d = end_roi_d.x - end_overlap_d.x;
    int non_roi_size_x_d = start_roi_d.x + size_d.x - end_roi_d.x;
    
    v3 start_overlap_s = start_overlap_d + off;
    const char* ss = s + (start_overlap_s.x + size_s.x * (start_overlap_s.y + size_s.y * start_overlap_s.z));
    for (int z = start_roi_d.z; z < start_overlap_d.z; z++) {
        for (int y = start_roi_d.y; y < end_roi_d.y; y++) {
            memset(dd, 0, size_roi.x);
            dd += size_d.x;
        }
        dd += size_d.x * (size_d.y - size_roi.y);
        
    }
    for (int z = start_overlap_d.z; z < end_overlap_d.z; z++) {
        
        for (int y = start_roi_d.y; y < start_overlap_d.y; y++) {
            memset(dd, 0, size_roi.x);
            dd += size_d.x;
        }
        for (int y = start_overlap_d.y; y < end_overlap_d.y; y++) {
            
            if (left_gap_x_d > 0) {
                memset(dd, 0, left_gap_x_d);
                dd += left_gap_x_d;
            }
            memcpy(dd, ss, size_overlap.x);
            dd += size_overlap.x;
            ss += size_s.x;
            if (right_gap_x_d > 0) {
                memset(dd, 0, right_gap_x_d);
                dd += right_gap_x_d;
            }
            dd += non_roi_size_x_d;
            
        }
        for (int y = end_overlap_d.y; y < end_roi_d.y; y++) {
            memset(dd, 0, size_roi.x);
            dd += size_d.x;
        }
        ss += size_s.x * (size_s.y - size_overlap.y);
        
    }
    for (int z = end_overlap_d.z; z < end_roi_d.z; z++) {
        for (int y = start_roi_d.y; y < end_roi_d.y; y++) {
            memset(dd, 0, size_roi.x);
            dd += size_d.x;
        }
        dd += size_d.x * (size_d.y - size_roi.y);
    }
}

