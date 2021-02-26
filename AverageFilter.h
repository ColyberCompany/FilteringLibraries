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


template <class T, class SumType>
class AverageFilter : public IFilter<T>
{
private:
    const uint16_t SamplesToAverage;
    T* sampleArray;
    uint16_t arrayIndex = 0;
    SumType sum = 0;
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

    T update(SumType newValue) override
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

    T getFilteredValue() override
    {
        return average;
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
