//
//  v3.h
//  copy_roi_test
//
//  Created by Igor Beati on 04.03.17.
//  Copyright © 2017 Igor Beati. All rights reserved.
//

#ifndef v3_h
#define v3_h


struct v3
{
  int x, y, z;
};

inline v3 min(const v3 va, const v3 vb)
{
    return{ vb.x < va.x ? vb.x : va.x, vb.y < va.y ? vb.y : va.y, vb.z < va.z ? vb.z : va.z };
}

inline v3 max(const v3 va, const v3 vb)
{
    return{ va.x < vb.x ? vb.x : va.x, va.y < vb.y ? vb.y : va.y, va.z < vb.z ? vb.z : va.z };
}

inline v3 operator - (const v3 va, const v3 vb)
{
    return{ va.x - vb.x, va.y - vb.y, va.z - vb.z };
}

inline v3 operator + (const v3 va, const v3 vb)
{
    return{ va.x + vb.x, va.y + vb.y, va.z + vb.z };
}

inline bool operator == (const v3 va, const v3 vb)
{
    return va.x == vb.x && va.y == vb.y && va.z == vb.z;
}


#endif /* v3_h */
