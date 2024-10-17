/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  wt_file.h
 * @brief WT File class definitions.
 *-----------------------------------------------------------------------------
 */
#ifndef _WT_FILE_H
#define _WT_FILE_H

#include <cmath>
#include <mutex>
#include "AudioFile.h"

// WT File class
class WtFile
{
public:
    // Helper functions
    static uint NumSamplesPerWave();

    // Constructor
    WtFile();

    // Destructor
    virtual ~WtFile();

    // Public functions
    bool load(std::string filename);
    void unload();
    std::vector<float> next_wave_samples();

private:
    // Private data
    std::mutex _mutex;
    AudioFile<float> _file;
    bool _loaded;
    uint _num_waves;
    uint _wave_index;
    bool _parse_fwd;
    const float *_samples;
    float _wave_time;
    float _wavetable_time;
};

#endif  // _WT_FILE_H
