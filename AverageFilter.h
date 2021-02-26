/**
 * @file AverageFilter.h
 * @author Jan Wielgus
 * @brief Average filter library
 * @date 2020-07-30
 * 
 */

#ifndef AVERAGEFILTER_H
#define AVERAGEFILTER_H

#include <IFilter.h>


template <class T>
class AverageFilter : public IFilter<T>
{
private:
    const uint16_t SamplesToAverage;
    T* sampleArray;
    uint16_t arrayIndex = 0;
    T sum = 0;
    T average = 0;


public:
    AverageFilter(uint16_t samplesToAverage)
        : SamplesToAverage(samplesToAverage)
    {
        if (SamplesToAverage > 0)
            sampleArray = new T[SamplesToAverage];
        
        reset();
    }

    ~AverageFilter()
    {
        if (SamplesToAverage > 0)
            delete [] sampleArray;
    }

    T update(T newValue) override
    {
        // Move arrayIndex to the next position in array loop
        arrayIndex++;
        if (arrayIndex >= SamplesToAverage)
            arrayIndex = 0;
        
        // Subtract oldest value form the sum and add the new one
        sum -= sampleArray[arrayIndex];
        sum += newValue;
        sampleArray[arrayIndex] = newValue;

        average = sum / (T)SamplesToAverage;
        return average;
    }

    /**
     * @brief Last filtered value getter of type T.
     * If type T is not a floating point and you want to have
     * floating point average, use getFilteredValueFloat() or
     * getFilteredValueDouble() instead.
     */
    T getFilteredValue() override
    {
        return average;
    }

    /**
     * @brief Last filtered value getter of type float.
     */
    float getFilteredValueFloat()
    {
        return (float)sum / (float)SamplesToAverage;
    }

    /**
     * @brief Last filtered value getter of type double.
     */
    double getFilteredValueDouble()
    {
        return (double)sum / (double)SamplesToAverage;
    }

    void reset()
    {
        arrayIndex = 0;
        sum = 0;
        average = 0;

        for (int i=0; i<SamplesToAverage; i++)
            sampleArray[i] = 0;
    }
};


#endif
