//
//  copy_roi.h
//  copy_roi_test
//
//  Created by Igor Beati on 04.03.17.
//  Copyright © 2017 Igor Beati. All rights reserved.
//

#ifndef copy_roi_h
#define copy_roi_h


#include "v3.h"


void copy_roi(const char* s, char* d, v3 size_s, v3 size_d, v3 start_roi_s, v3 start_roi_d, v3 size_roi, int size_type);

template<typename T>
void copy_roi(const T* s, T* d, v3 size_s, v3 size_d, v3 start_roi_s, v3 start_roi_d, v3 size_roi)
{
    copy_roi(reinterpret_cast<const char*>(s), reinterpret_cast<char*>(d), size_s, size_d, start_roi_s, start_roi_d, size_roi, sizeof(T));
}


#endif /* copy_roi_h */
