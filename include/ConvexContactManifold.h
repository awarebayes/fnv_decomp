#pragma once

#include <cstddef>

class ConvexContactManifold
{
public:
    struct AlignedPairData
    {

        class CommandArgument;

        unsigned char m_ucDimensionA;
        unsigned char m_ucDimensionB;
        unsigned short m_iOwnerObjectOffset;
        union
        {
            unsigned char m_vert[4];
            unsigned int m_uiVertexIndices;
        };

        void zeroVerts() { m_uiVertexIndices = 0; }
    };

    typedef unsigned short VertexId;
    enum { MAX_POINTS = 4 };
    enum { MAX_VERTICES = MAX_POINTS * 4 };

    const VertexId* getVertexIds() const;

    unsigned char m_ucNumVerticesA;
    unsigned char m_ucNumVerticesB;
    unsigned char m_usNumContactPoints;
    AlignedPairData m_akContactPoints[MAX_POINTS];
};
