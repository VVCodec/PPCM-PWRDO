
#ifndef global_h
#define global_h


//extern size_t      wwidth;
//extern size_t      hhight;
//extern size_t      rresolution;
//extern size_t      pprecision;
//extern size_t      fframenum;
//extern std::string ocname;
//extern bool        rdoflag;
////extern std::vector<uint32_t> ocMap;
extern double gradweight;

void g_getBasicImformation( int& picHeight, int& picWidth );
void AllocateMen( int& HeightOrg, int& WidthOrg );
void DestroyMen( int& HeightOrg, int& WidthOrg );
#endif /* global_h */
