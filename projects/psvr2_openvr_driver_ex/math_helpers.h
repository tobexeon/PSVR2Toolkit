#pragma once

#include <cstdint>
#include <corecrt_math.h>
#include <corecrt_math_defines.h>
#include <cmath>
#include <set>
#include <stdexcept>

#include <openvr_driver.h>

namespace psvr2_toolkit {
    static double Clamp(double value, double min, double max)
    {
        return value < min ? min : value > max ? max : value;
    }

    static int8_t ClampedAdd(int8_t a, int8_t b)
    {
        // The result is clamped to the range of int8_t
        int16_t result = static_cast<int16_t>(a) + static_cast<int16_t>(b);
        if (result > INT8_MAX)
            return INT8_MAX;
        else if (result < INT8_MIN)
            return INT8_MIN;
        else
            return static_cast<int8_t>(result);
    }

    static int8_t ApplyExponentialGain(int8_t sample, float exponent)
    {
        if (std::abs(exponent - 1.0f) < 0.001f || sample == 0) {
            return sample;
        }

        float normalized = std::abs(static_cast<float>(sample)) / 127.0f;
        
        if (normalized > 1.0f) normalized = 1.0f;

        float curved = std::pow(normalized, exponent);

        float result = curved * 127.0f;
        
        if (sample < 0) {
            result = -result;
        }
        
        return static_cast<int8_t>(Clamp(result, -128.0, 127.0));
    }

    static int8_t CosineToByte(uint32_t position, double max, double amp, double overdrive)
    {
        double cosResult = Clamp(cos((position / max) * 2 * M_PI) * overdrive, -1.0, 1.0) * amp;
        
        int8_t out = static_cast<int8_t>(cosResult);

        return out;
    }
}
