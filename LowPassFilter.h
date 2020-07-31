/**
 * @file LowPassFilter.h
 * @author overlord1123, edited by Jan Wielgus
 * @brief Low-pass filter library by overlord1123 under GPL-2.0 license. https://github.com/overlord1123/LowPassFilter
 * @date 2020-07-31
 * 
 */

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <IFilter.h>


template <class T>
class LowPassFilter : public IFilter<T>
{
private:
    T output;
    float ePow;


public:
    LowPassFilter(float cutOffFrequency, float deltaTime)
    {
        reconfigureFilter(cutOffFrequency, deltaTime);

        reset();
    }

    void reconfigureFilter(float cutOffFrequency, float deltaTime)
    {           
        if (deltaTime <= 0 || cutOffFrequency <= 0)
            ePow = 0;
        else
        {
            //FIXME: if exp is not declared in the scope, include arduino.h
            // same with M_PI
            ePow = 1 - exp(-deltaTime * 2 * M_PI * cutOffFrequency);
        }
    }

    T update(T newValue) override
    {
        output += (newValue - output) * ePow;
        return output;
    }

    T getFilteredValue() override
    {
        return output;
    }

    void reset() override
    {
        output = 0;
    }
};


typedef LowPassFilter<float> LowPassFilterFloat;


#endif
