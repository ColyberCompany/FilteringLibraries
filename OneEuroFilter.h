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


template <class T>
class OneEuroFilter : public IFilter<T>
{
    T deltaTime_s;
    T minCutoff_Hz;
    T beta;

    LowPassFilter<T> xFilter; // x is a new value
    LowPassFilter<T> xDerivFilter; // x derivative filter
    T lastX;

public:
    OneEuroFilter(T deltaTime_s, T minCutoffFreq_Hz, T beta, T derivCutoffFreq_Hz = 1)
    {
        setFilterParameters(deltaTime_s, minCutoffFreq_Hz, beta, derivCutoffFreq_Hz);
        reset();
    }

    void setFilterParameters(T deltaTime_s, T minCutoffFreq_Hz, T beta, T derivCutoffFreq_Hz)
    {
        this->deltaTime_s = deltaTime_s;
        this->minCutoff_Hz = minCutoffFreq_Hz;
        this->beta = beta;
        this->xFilter.reconfigureFilter(minCutoff_Hz, deltaTime_s);
        this->xDerivFilter.reconfigureFilter(derivCutoffFreq_Hz, deltaTime_s);
        reset();
    }

    /**
     * @brief Set filter parameters (derivative beta remains unchanged).
     * @param minCutoffFreq_Hz Minimum cut-off frequency [in Hz].
     * @param beta Beta parameter (reduces lag in during high changes).
     * Start with 0.0001 and increase / decrease it appropriately.
     */
    void setFilterParameters(T deltaTime_s, T minCutoffFreq_Hz, T beta)
    {
        this->deltaTime_s = deltaTime_s;
        this->minCutoff_Hz = minCutoffFreq_Hz;
        this->beta = beta;
        this->xFilter.reconfigureFilter(minCutoff_Hz, deltaTime_s);
        reset();
    }

    T update(T newValue) override
    {
        T xDeriv = (newValue - lastX) / deltaTime_s;
        T xDerivFiltered = xDerivFilter.update(xDeriv);
        T newCutoff = minCutoff_Hz + beta * abs(xDerivFiltered);
        lastX = newValue;

        xFilter.reconfigureFilter(newCutoff, deltaTime_s);
        return xFilter.update(newValue);
    }

    T getFilteredValue() override
    {
        return xFilter.getFilteredValue();
    }

    /**
     * @brief Don't reset filter parameters. Just start filtering from 0.
     */
    void reset() override
    {
        xFilter.reset();
        xDerivFilter.reset();
        lastX = 0;
    }
};


#endif
