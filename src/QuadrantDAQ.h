#ifndef QuadrantDAQ_h
#define QuadrantDAQ_h

#include <VL53L0X.h>

#include "QuadrantCommon.h"

class QuadrantOut;

class QuadrantDAQ {

  public:

    enum SamplingMode {
      SAMPLINGMODE_SINGLE_SEQUENTIAL, // on update(), single lidar measurements are started one at
                                      //    a time, waiting for completion before starting the next

      SAMPLINGMODE_SINGLE_PIPELINE,   // on update(), single lidar measuremets are started (nearly)
                                      //    simultaneously, then round-robin queried for completion

      SAMPLINGMODE_CONTINUOUS,        // new measurements auto-start as fast as possible (~43-67 Hz),
                                      //    but not necessarily periodically

      SAMPLINGMODE_PERIODIC           // new measurements auto-start after a fixed period (42.4 Hz) 
    }; 

    void begin(void);
    void update(QuadrantOut *debug_out=NULL);
    void pushToFifo(void);
    void waitUntilAllRanging(QuadrantOut *debug_out=NULL);

    // setters
    void setSamplingMode(enum SamplingMode mode);
    void setLidarEnabled(int index, bool enabled);

    // getters
    int getSamplingMode(void);
    uint32_t getLidarTimingBudget(int index);
    uint16_t getLidarDistance(int index);
    uint32_t getTimestamp(void);
    bool isLidarEnabled(int index);
    uint8_t getTimeoutMask(void);
    bool isLidarRanging(uint8_t index);

  private:

    const uint8_t _lidarPins[4] = {QUADRANT_LIDAR0_ENABLE, QUADRANT_LIDAR1_ENABLE, QUADRANT_LIDAR2_ENABLE, QUADRANT_LIDAR3_ENABLE};
    const uint8_t _lidarAddrs[4] = {QUADRANT_LIDAR0_ADDR, QUADRANT_LIDAR1_ADDR, QUADRANT_LIDAR2_ADDR, QUADRANT_LIDAR3_ADDR};

    VL53L0X* _lidars[4];

    uint16_t _distance[4];
    uint32_t _timestamp;
    bool _lidarEnabled[4];
    enum SamplingMode _smode = SAMPLINGMODE_SINGLE_PIPELINE; // default

    void _initLidar(int index);
    bool _isLidarReady(uint8_t index);
    uint16_t _readLidar(uint8_t index);

    void _update_single_sequential(void);
    void _update_continuous_sequential(QuadrantOut *debug_out=NULL);
    void _update_single_round_robin(QuadrantOut *debug_out=NULL);
    void _update_continuous_round_robin(QuadrantOut *debug_out=NULL);
    void _collect_range_round_robin(QuadrantOut *debug_out=NULL);

    uint8_t _timeout_mask = 0;

};


#endif
