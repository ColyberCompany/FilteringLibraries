/**
 * @file MedianFilter.h
 * @author Jan Wielgus (rework)
 * @brief Rework of https://github.com/GyverLibs/GyverFilters.
 * @version 0.1
 * @date 2022-06-18
 */

#ifndef MEDIANFILTER_h
#define MEDIANFILTER_h

#include "IFilter.h"
#include <stdint.h>


namespace FL
{
    template <class T, uint16_t Size>
    class MedianFilter : public IFilter<T>
    {
        T buffer[Size];
        uint16_t count = 0;

    public:
        T update(T newValue) override
        {
            buffer[count] = newValue;

            if ((count < Size - 1) && (buffer[count] > buffer[count + 1]))
            {
                for (uint16_t i = count; i < Size - 1; ++i)
                {
                    if (buffer[i] > buffer[i + 1])
                    {
                        T buff = buffer[i];
                        buffer[i] = buffer[i + 1];
                        buffer[i + 1] = buff;
                    }
                }
            }
            else if ((count > 0) && (buffer[count - 1] > buffer[count]))
            {
                for (uint16_t i = count; i > 0; --i)
                {
                    if (buffer[i] < buffer[i - 1])
                    {
                        T buff = buffer[i];
                        buffer[i] = buffer[i - 1];
                        buffer[i - 1] = buff;
                    }
                }
            }

            if (++count >= Size)
                count = 0;

            return buffer[Size / 2];
        }

        T getFilteredValue() override
        {
            return buffer[Size / 2];
        }

        void reset() override
        {
            for (uint16_t i = 0; i < Size; ++i)
            {
                buffer[i] = 0;
            }
            count = 0;
        }
    };
}


#endif
