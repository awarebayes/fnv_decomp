// bscore/bsrandom_singleton.h: declares MersenneTwister
#ifndef FNV_BSRANDOM_SINGLETON_H
#define FNV_BSRANDOM_SINGLETON_H

namespace bsrandom_singleton {
class MersenneTwister
{
  public:
    MersenneTwister();
    static MersenneTwister* GetRandomGenerator();

  private:
    unsigned char state[40];
};
extern MersenneTwister* DAT_011c57f8;
}
#endif
