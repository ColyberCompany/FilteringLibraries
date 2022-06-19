/**
 * @file LowPassFilter.h
 * @author Jan Wielgus
 * @brief Low-pass filter library based on https://en.wikipedia.org/wiki/Low-pass_filter#Discrete-time_realization.
 * @date 2020-07-31
 */

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <IFilter.h>

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
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
        T alpha = 0;
        T output = 0;

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
         * @param cutOffFreq_Hz Filter cut-off frequency [in Hz]
         * @param deltaTime_s Time between next update() executions in seconds
         */
        void reconfigureFilter(T cutOffFreq_Hz, T deltaTime_s)
        {           
            T temp = 2 * M_PI * deltaTime_s * cutOffFreq_Hz;
            alpha = temp / (temp + 1);
        }

        /**
         * @brief Generate new filtered value.
         * @param newValue New measurement
         * @return New filtered value
         */
        T update(T newValue) override
        {
            output += alpha * (newValue - output);
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
