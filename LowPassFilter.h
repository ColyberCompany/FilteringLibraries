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
    /**
     * @brief Using this constructor, filter always return 0 in update() method.
     * Use reconfigureFilter() method later.
     */
    LowPassFilter()
    {
        reconfigureFilter(0, 0);
        reset();
    }


    /**
     * @brief Construct and configure a new Low Pass Filter object.
     * 
     * @param cutOffFrequency Filter cut-off frequency [in Hz]
     * @param deltaTime Time between next update() executions in seconds
     */
    LowPassFilter(float cutOffFrequency, float deltaTime)
    {
        reconfigureFilter(cutOffFrequency, deltaTime);
        reset();
    }


    /**
     * @brief Reconfigure filter parameters
     * 
     * @param cutOffFrequency Filter cut-off frequency [in Hz]
     * @param deltaTime Time between next update() executions in seconds
     */
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


    /**
     * @brief Generate new filtered value
     * 
     * @param newValue New measurement
     * @return New filtered value
     */
    T update(T newValue) override
    {
        output += (newValue - output) * ePow;
        return output;
    }


    /**
     * @return Last filtered value
     */
    T getFilteredValue() override
    {
        return output;
    }


    /**
     * @brief Reset filter
     */
    void reset() override
    {
        output = 0;
    }
};


typedef LowPassFilter<float> LowPassFilterFloat;


#endif
