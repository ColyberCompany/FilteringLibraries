/**
 * @file OneEuroFilter.h
 * @author Jan Wielgus
 * @brief 1-Euro Filter. Based on Jonathan Aceituno implementation.
 * For details, see http://www.lifl.fr/~casiez/1euro
 * @date 2022-05-10
 */

#ifndef ONEEUROFILTER_h
#define ONEEUROFILTER_h

#include "IFilter.h"
#include "LowPassFilter.h"
#include <math.h>


/**
 * @brief One Euro Filter.
 * @tparam T float, double or long double.
 */
template <class T>
class OneEuroFilter : public IFilter<T>
{
    T dt_s;
    T minCutoff_Hz;
    T beta;

    LowPassFilter<T> xFilter; // x is a new value
    LowPassFilter<T> xDerivFilter; // x derivative filter
    T lastX;

public:
    /**
     * @param deltaTime_s Interval between next filter updates.
     * @param minCutoffFreq_Hz Minimal cut-off frequency [in Hz].
     * @param beta Beta parameter (reduces lag in during high changes).
     * Start with 0.0001 and increase / decrease it appropriately.
     * @param derivCutoffFreq_Hz Internal new samples derivative low-pass filter cutoff frequency.
     */
    OneEuroFilter(T deltaTime_s, T minCutoffFreq_Hz, T beta, T derivCutoffFreq_Hz = 1)
        :xFilter(minCutoffFreq_Hz, deltaTime_s),
        xDerivFilter(derivCutoffFreq_Hz, deltaTime_s)
    {
        this->dt_s = deltaTime_s;
        this->minCutoff_Hz = minCutoffFreq_Hz;
        this->beta = beta;
        reset();
    }

    /**
     * @brief Change filter parameters.
     * @param deltaTime_s Interval between next filter updates.
     * @param minCutoffFreq_Hz Minimal cut-off frequency [in Hz].
     * @param beta Beta parameter (reduces lag in during high changes).
     * Start with 0.0001 and increase / decrease it appropriately.
     * @param derivCutoffFreq_Hz Internal new samples derivative low-pass filter cutoff frequency.
     */
    void setFilterParameters(T deltaTime_s, T minCutoffFreq_Hz, T beta, T derivCutoffFreq_Hz)
    {
        setFilterParameters(deltaTime_s, minCutoffFreq_Hz, beta);
        this->xDerivFilter.reconfigureFilter(derivCutoffFreq_Hz, deltaTime_s);
    }

    /**
     * @brief Set filter parameters (derivative beta remains unchanged).
     * @param minCutoffFreq_Hz Minimal cut-off frequency [in Hz].
     * @param beta Beta parameter (reduces lag in during high changes).
     * Start with 0.0001 and increase / decrease it appropriately.
     */
    void setFilterParameters(T deltaTime_s, T minCutoffFreq_Hz, T beta)
    {
        this->dt_s = deltaTime_s;
        this->minCutoff_Hz = minCutoffFreq_Hz;
        this->beta = beta;
        this->xFilter.reconfigureFilter(minCutoff_Hz, deltaTime_s);
    }

    /**
     * @brief Update filter.
     * @param newValue New sample to filter.
     * @return New filter ouptut (the same value can be accessed
     * by getFilteredValue() method).
     */
    T update(T newValue) override
    {
        T xDeriv = (newValue - lastX) / dt_s;
        T xDerivFiltered = xDerivFilter.update(xDeriv);
        T newCutoff = minCutoff_Hz + beta * abs(xDerivFiltered);
        lastX = newValue;

        xFilter.reconfigureFilter(newCutoff, dt_s);
        return xFilter.update(newValue);
    }

    /**
     * @brief Get last filtered value.
     */
    T getFilteredValue() override
    {
        return xFilter.getFilteredValue();
    }

    /**
     * @brief Start filtering from 0. (don't reset filter parameters).
     */
    void reset() override
    {
        xFilter.reset();
        xDerivFilter.reset();
        lastX = 0;
    }
};


#endif
