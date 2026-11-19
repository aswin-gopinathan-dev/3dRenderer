#include "../inc/Triangle.h"

Face::Face(int aa, int bb, int cc, Texture aa_uv, Texture bb_uv, Texture cc_uv, uint32_t clr) : 
        a(aa), b(bb), c(cc), a_uv(aa_uv), b_uv(bb_uv), c_uv(cc_uv), color(clr)
{

}

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}
