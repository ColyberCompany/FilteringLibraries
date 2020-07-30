/**
 * @file EVAFilter.h
 * @author Jan Wielgus
 * @brief EVA (Exponentially Weighted Averages) filter library
 * @date 2020-07-30
 * 
 */

#ifndef EVAFILTER_H
#define EVAFILTER_H

#include <IFilter.h>


template <class T>
class EVAFilter : public IFilter<T>
{
private:
    static const float DefaultFilterBeta;

    T lastFilteredValue;
    float FilterBeta; // range: 0 <= fBeta < 1  ->  0.9999 - very smooth value; 0 - 100% original value
    float FilterBeta2; // 1-FilterBeta


public:
    EVAFilter(float filterBeta = DefaultFilterBeta)
    {
        setFilterBeta(filterBeta);
    
        reset();
    }

    void setFilterBeta(float filterBeta)
    {
        if (filterBeta >= 0 && filterBeta < 1)
            FilterBeta = filterBeta;
        else
            FilterBeta = DefaultFilterBeta;

        FilterBeta2 = 1 - FilterBeta;
    }

    float getFilterBeta()
    {
        return FilterBeta;
    }

    T update(T newValue) override
    {
        lastFilteredValue = lastFilteredValue * FilterBeta
                            + newValue * FilterBeta2;
    }

    T getFilteredValue() override
    {
        return lastFilteredValue;
    }

    void reset() override
    {
        lastFilteredValue = 0;
    }
};


#endif
