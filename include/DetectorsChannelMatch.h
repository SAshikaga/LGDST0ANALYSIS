#ifndef DetectorsChannelMatch_h
#define DetectorsChannelMatch_h


class DetectorsChannelMatch{
public:
    DetectorsChannelMatch();
    ~DetectorsChannelMatch();
    bool isMatchedGTRLG(int gtr_module_id, int gtr_channel_id, int lg_module_id,int lg_channel_id){
    return gtr_lg_match[gtr_module_id][gtr_channel_id][lg_module_id][lg_channel_id];
    
    }
    bool isMatchedHBDLG(int hbd_module_id, int hbd_channel_id, int lg_module_id,int lg_channel_id){
    return hbd_lg_match[hbd_module_id][hbd_channel_id][lg_module_id][lg_channel_id];
    
    }
    void InitializeAllMatch();
    void GTRLGMatch(int gm, int gc, int lm, int lc);
    void HBDLGMatch(int hm, int hc, int lm, int lc);

private:
    enum{
    gtr_n_module = 9,
    gtr_n_channel = 24,
    hbd_n_module = 9,
    hbd_n_channel = 56,
    lg_n_module = 9,
    lg_n_channel = 56 
   };
   static bool gtr_lg_match[gtr_n_module][gtr_n_channel][lg_n_module][lg_n_channel];
   static bool hbd_lg_match[hbd_n_module][hbd_n_channel][lg_n_module][lg_n_channel];




};


#endif // DetectorsChannelMatch_h
