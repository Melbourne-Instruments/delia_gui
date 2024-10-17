/**
 *-----------------------------------------------------------------------------
 * Copyright (c) 2023-2024 Melbourne Instruments, Australia
 *-----------------------------------------------------------------------------
 * @file  wt_file.cpp
 * @brief WT File implementation.
 *-----------------------------------------------------------------------------
 */

#include <stdint.h>
#include "wt_file.h"
#include "gui_common.h"

// Constants
constexpr char MONIQ_WT_DIR[]         = "/udata/delia/wavetables/";
constexpr char WT_FILE_EXT[]          = ".wav";
constexpr uint MAX_NUM_WAVES          = 256;
constexpr uint WAVE_LENGTH            = (1024 * 2);
constexpr uint WAVE_DOWNSAMPLING_RATE = 8;
constexpr float WT_DISPLAY_TIME       = std::chrono::milliseconds(2000).count();

//----------------------------------------------------------------------------
// NumSamples
//----------------------------------------------------------------------------
uint WtFile::NumSamplesPerWave()
{
    // Return the number of samples per wave
    return WAVE_LENGTH / WAVE_DOWNSAMPLING_RATE;
}

//----------------------------------------------------------------------------
// WtFile
//----------------------------------------------------------------------------
WtFile::WtFile()
{
    // Initialise the private data
    _loaded = false;
    _num_waves = 0;
    _wave_index = 0;
    _parse_fwd = true;
    _wave_time = 0;
    _wavetable_time = 0;
}

//----------------------------------------------------------------------------
// ~WtFile
//----------------------------------------------------------------------------
WtFile::~WtFile()
{
    // Nothing specific to do
}

//----------------------------------------------------------------------------
// load
//----------------------------------------------------------------------------
bool WtFile::load(std::string filename)
{
    // Get the mutex lock
	std::unique_lock<std::mutex> lk(_mutex);

    // Try and load the WT
    auto filename_path = MONIQ_WT_DIR + filename + WT_FILE_EXT;
    _loaded = false;
    if (!_file.load(filename_path)) {
        MSG("Could not open the wavetable file: " << filename_path);
        return false;
    }

    // Check the number of samples is valid
    if ((_file.getNumChannels() == 0) || (_file.samples[0].size() % WAVE_LENGTH)) {
        MSG("Wavetable number of channels/samples is invalid: " << filename_path);
        return false;
    }

    // Get the number of waves and check it is valid
    auto num_waves = _file.samples[0].size() / WAVE_LENGTH;
    if (num_waves > MAX_NUM_WAVES) {
        MSG("Wavetable number of channels/samples is invalid: " << filename_path);
        return false;
    }

    // WT loaded
    _loaded = true;
    _num_waves = num_waves;
    _wave_index = 0;
    _samples = _file.samples[0].data();
    _wave_time = WT_DISPLAY_TIME / _num_waves;
    _wavetable_time = 0.0f;
    _parse_fwd = true;
    return true;
}

//----------------------------------------------------------------------------
// unload
//----------------------------------------------------------------------------
void WtFile::unload()
{
    // Get the mutex lock
	std::unique_lock<std::mutex> lk(_mutex);
    
    // This just indicates the WT is no longer loaded
    _loaded = false;
}

//----------------------------------------------------------------------------
// next_wave_samples
//----------------------------------------------------------------------------
std::vector<float> WtFile::next_wave_samples()
{
    std::vector<float> samples;
    
    // Get the mutex lock
	std::unique_lock<std::mutex> lk(_mutex);

    // If a wavetable is loaded
    if (_loaded) {
        // Calculate the wave index increment based on the cumulative wavtable time
        uint inc = std::round(_wavetable_time / _wave_time) - _wave_index;

        // Increment the total wavetable time by the refresh rate
        _wavetable_time += WT_CHART_REFRESH_RATE;

        // If the increment value is zero, skip this processing and return
        // no sample data
        if (inc) {
            // Get the next wave samples - down sampled
            for (uint i=0; i<(WAVE_LENGTH / WAVE_DOWNSAMPLING_RATE); i++) {
                samples.push_back(*_samples);
                _samples += WAVE_DOWNSAMPLING_RATE;
            }

            // Are we parsing the wavetable in a forward direction?
            if (_parse_fwd) {
                // Increment the wave index
                _wave_index += inc;

                // Have we reached the end of the wavetable?
                if (_wave_index >= _num_waves) {
                    // Reached the end of the waves, switch to reverse parsing
                    _wave_index = (_num_waves - 1);
                    _samples = _file.samples[0].data() + _file.samples[0].size() - WAVE_LENGTH;
                    _parse_fwd = false;
                }
                else {
                    // Increment the samples pointer if needed - if the increment
                    // is greater than 1
                    if (inc > 1) {
                        _samples += (WAVE_LENGTH * (inc - 1));
                    }
                }
            }
            else {
                // Decrement the wave index
                _wave_index += inc;

                // Have we reached the start of the wavetable?
                if (_wave_index >= (_num_waves << 1)) {
                    // Reached the start of the waves, switch to forward parsing
                    _wave_index = 0;
                    _samples = _file.samples[0].data();
                    _parse_fwd = true;
                    _wavetable_time = 0;              
                }
                else {
                    // Decrement the samples pointer (also check for underflow)
                    _samples -= ((inc + 1) * WAVE_LENGTH);
                    if (_samples < _file.samples[0].data()) {
                        _samples = _file.samples[0].data();
                    }
                }
            }
        }
        
    }
    return samples;
}
