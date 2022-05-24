/**
 * @file AverageFilter.h
 * @author Jan Wielgus
 * @brief Average filter library
 * @date 2020-07-30
 */

#ifndef AVERAGEFILTER_H
#define AVERAGEFILTER_H

#include "IFilter.h"
#include <stdint.h>


namespace FL
{
    template <class T, uint16_t SmplsToAvg = 0>
    class AverageFilter;

    /**
     * @brief Average filter. There are two versions:
     * with static and dynamic internal array allocation.\n
     * For static allocation use: AverageFilter<TYPE, SMPLS_TO_AVG> name;\n
     * For dynamic allocation use: AverageFilter<TYPE> name(SMPLS_TO_AVG);
     * @tparam T Type of the averaged values (of source data).
     */
    template <class T>
    class AverageFilter<T, 0> : public IFilter<T>
    {
    private:
        const uint16_t SamplesToAverage;
        T* sampleArray;
        uint16_t arrayIndex = 0;
        T sum = 0;
        T average = 0;
        const bool deleteArray; // flag that indicates if destructor should delete sampleArray

    protected:
        /**
         * @brief Constructor for 
         * @param samplesToAverage 
         * @param sampleArray 
         */
        AverageFilter(T* sampleArray, uint16_t samplesToAverage):
            SamplesToAverage(samplesToAverage),
            sampleArray(sampleArray),
            deleteArray(false)
        {
            reset();
        }

    public:
        AverageFilter(uint16_t samplesToAverage):
            SamplesToAverage(samplesToAverage),
            deleteArray(true)
        {
            sampleArray = new T[SamplesToAverage];
            reset();
        }

        AverageFilter(AverageFilter&& toMove):
            SamplesToAverage(toMove.SamplesToAverage),
            deleteArray(toMove.deleteArray)
        {
            sampleArray = toMove.sampleArray;
            arrayIndex = toMove.arrayIndex;
            sum = toMove.sum;
            average = toMove.average;

            toMove.sampleArray = nullptr;
            toMove.arrayIndex = 0;
            toMove.sum = 0;
            toMove.average = 0;
        }

        AverageFilter(const AverageFilter&) = delete;
        AverageFilter& operator=(const AverageFilter&) = delete;
        AverageFilter& operator=(AverageFilter&& toMove) = delete;

        ~AverageFilter()
        {
            if (deleteArray)
                delete[] sampleArray;
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

            for (uint16_t i=0; i<SamplesToAverage; i++)
                sampleArray[i] = 0;
        }
    };


    /**
     * @brief Average filter. There are two versions:
     * with static and dynamic internal array allocation.\n
     * For static allocation use: AverageFilter<TYPE, SMPLS_TO_AVG> name;\n
     * For dynamic allocation use: AverageFilter<TYPE> name(SMPLS_TO_AVG);
     * @tparam T Type of the averaged values (of source data).
     * @tparam SamplesToAvg Amount of samples to average.
     */
    template <class T, uint16_t SamplesToAvg>
    class AverageFilter : public AverageFilter<T, 0>
    {
    public:
        T staticSamplesArray[SamplesToAvg];

    public:
        AverageFilter()
            : AverageFilter<T, 0>(staticSamplesArray, SamplesToAvg)
        {
        }

        AverageFilter(const AverageFilter&) = delete;
        AverageFilter& operator=(const AverageFilter&) = delete;
        AverageFilter(AverageFilter&&) = delete;
        AverageFilter& operator=(AverageFilter&&) = delete;
    };
}


#endif
