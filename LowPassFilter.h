/**
 * @file LowPassFilter.h
 * @author Jan Wielgus
 * @brief Low-pass filter library based on overlord1123 version under GPL-2.0 license. https://github.com/overlord1123/LowPassFilter.
 * @date 2020-07-31
 */

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <IFilter.h>

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <cmath>
#endif


namespace FL
{
    /**
     * @brief Low-pass filter.
     * @tparam T float, double or long double.
     */
    template <class T>
    class LowPassFilter : public IFilter<T>
    {
        T output = 0;
        T ePow;

    public:
        /**
         * @brief Construct and configure a new Low Pass Filter object.
         * @param cutOffFrequency Filter cut-off frequency [in Hz]
         * @param deltaTime Time between next update() executions in seconds
         */
        LowPassFilter(T cutOffFrequency, T deltaTime)
        {
            reconfigureFilter(cutOffFrequency, deltaTime);
        }

        /**
         * @brief Reconfigure filter parameters (do not reset filter).
         * @param cutOffFrequency Filter cut-off frequency [in Hz]
         * @param deltaTime Time between next update() executions in seconds
         */
        void reconfigureFilter(T cutOffFrequency, T deltaTime)
        {           
            if (deltaTime <= 0 || cutOffFrequency <= 0)
                ePow = 0;
            else
                ePow = 1 - exp(-deltaTime * 2 * M_PI * cutOffFrequency);
        }

        /**
         * @brief Generate new filtered value.
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
}


#endif
